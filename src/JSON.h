/* *******************************************************************************
 *  Copyright (C) 2014-2022 Mehmet Gunce Akkoyun Can not be copied and/or
 *	distributed without the express permission of Mehmet Gunce Akkoyun.
 *
 *	Library				: JSON
 *	Code Developer		: Mehmet Gunce Akkoyun (akkoyun@me.com)
 *
 *********************************************************************************/

#ifndef __JSON__
#define __JSON__

	// Define Arduino Library
	#ifndef __Arduino__
		#include <Arduino.h>
	#endif

	// Include Definitions
	#include "Definitions.h"

	// JSON Library
	class JSON {

		// Private Context
		private:

			// Declare JSON Buffer
			char _JSON_Buffer[_JSON_BUFFER_SIZE_];

			// Declare JSON Size
			uint16_t _JSON_Size;

			// Declare Buffer Size
			uint16_t _Buffer_Size;

			// Declare JSON State
			bool _JSON_Valid = _JSON_INVALID_;

			// Variable JSON Function
			bool Validate(const char * _Buffer) {

				// Declare Variables
				int8_t _Braces_Count = 0;
				int8_t _Brackets_Count = 0;
				bool _in_String = false;

				// Control JSON Data
				for (uint16_t i = 0; i < this->_Buffer_Size; i++) {

					// Control for char
					switch (_Buffer[i]) {

						// Control for Braces Open
						case '{': {

							// Control for String
							if (!_in_String) _Braces_Count++;

							// End Case
							break;

						}

						// Control for Braces Close
						case '}': {

							// Control for String
							if (!_in_String) _Braces_Count--;

							// End Case
							break;

						}

						// Control for Brackets Open
						case '[': {

							// Control for String
							if (!_in_String) _Brackets_Count++;

							// End Case
							break;

						}

						// Control for Brackets Close
						case ']': {

							// Control for String
							if (!_in_String) _Brackets_Count--;

							// End Case
							break;

						}

						// Control for String
						case '"': {

							// Control for String
							_in_String = !_in_String;

							// End Case
							break;

						}

						// Control for Comma
						case ',': {

							// Control for String
							if (!_in_String && (_Braces_Count == 0 && _Brackets_Count == 0)) return(false);

							// End Case
							break;

						}

						// Control for Colon
						case ':': {

							// Control for String
							if (!_in_String && (_Braces_Count == 0 || _Brackets_Count != 0)) return(false);

							// End Case
							break;

						}

					}

					// Control for Braces and Brackets
					if (_Braces_Count < 0 || _Brackets_Count < 0) return(false);

				}

				// End Function
				return ((_Braces_Count == 0 && _Brackets_Count == 0) && !_in_String);

			}

			// Get JSON Data from Buffer
			bool Handle(const char * _Data, bool _Space = false) {

				// Declare Variables
				bool _JSON_Handle = false;
				uint16_t _Data_Order = 0;

				// Clear JSON Buffer
				memset(this->_JSON_Buffer, '\0', _JSON_BUFFER_SIZE_);

				// Handle JSON Data
				for (uint16_t i = 0; i < this->_Buffer_Size; i++) {

					// Control for JSON Data Start
					if (_Data[i] == '{') _JSON_Handle = true;

					// Handle JSON Data
					if (_Space) {

						// Handle With Space
						if (_JSON_Handle && _Data[i] != '\n' && _Data[i] != '\r') this->_JSON_Buffer[_Data_Order++] = _Data[i];

					} else {

						// Handle Without Space
						if (_JSON_Handle && _Data[i] != ' ' && _Data[i] != '\n' && _Data[i] != '\r') this->_JSON_Buffer[_Data_Order++] = _Data[i];

					}

					// Control for JSON Data End
					if (_Data_Order > 2 && _Data[i - 2] == '}' && _Data[i - 1] == '\r' && _Data[i] == '\n') _JSON_Handle = false;

				}

				// Set JSON Size
				this->_JSON_Size = _Data_Order;

				// End Function
				return (_Data_Order > 2);

			}

			// ASCII Find Function
			uint16_t Find_Char(const char _Char, const uint8_t _Count = 1) {

				// Declare Counter
				uint16_t _Counter = 0;

				// Find Char in Buffer
				for (uint16_t i = 0; this->_JSON_Buffer[i] != '\0'; ++i) {

					// Control for Char
					if (this->_JSON_Buffer[i] == _Char) {

						// Increase Counter
						_Counter++;

						// Control for Counter
						if (_Counter == _Count) return(i);

					}

				}

				// End Function
				return(0);

			}

		// Public Context
		public:

			// Constructor
			explicit JSON(const char * _Buffer, uint16_t _Size) : _Buffer_Size(_Size) {

				// Validate JSON Data
				this->_JSON_Valid = this->Validate(_Buffer);

				// Handle JSON Data from Buffer
				if (this->_JSON_Valid == _JSON_VALID_) this->Handle(_Buffer, false);

			}

			// JSON Valid Function
			bool isValid(void) {

				// Return JSON Valid
				return (_JSON_Valid);

			}

			// Get JSON Size
			uint16_t Size(void) {

				// Return JSON Size
				return (this->_JSON_Size);

			}

			// Get JSON Buffer
			const char * Buffer(void) {

				// Return JSON Buffer
				return (this->_JSON_Buffer);

			}

			// Find Key in Buffer
			bool Key_Find(const __FlashStringHelper * _Key) {

				// Declare Function Variables
				struct Variables_Struct {
					uint16_t Value = 0;
					uint16_t Char_Counter = 0;
					char Name[10];
					bool Found = false;
				} _Object;

				// Control for char
				for (uint16_t i = 0; i < this->_Buffer_Size; i++) {

					// Count Char
					if (this->_JSON_Buffer[i] == 34) _Object.Char_Counter++;
					
				}

				// Control for Char Count
				if (_Object.Char_Counter % 2 == 1) return(false);

				// Find Start of FW_ID
				for (uint16_t i = 1; i < _Object.Char_Counter; i++) {

					// Find Start of Key
					uint16_t _Key_Start = this->Find_Char('\"', i);
					uint16_t _Key_End = this->Find_Char('\"', i + 1);

					// Control for Root Key
					if (_Key_Start > 0 && _Key_End > 0) {

						// Clear Object
						memset(_Object.Name, '\0', sizeof(_Object.Name));

						// Copy Root Key
						memcpy(_Object.Name, &this->_JSON_Buffer[_Key_Start + 1], _Key_End - _Key_Start - 1);

						// Control for Root
						if (strcmp_P(_Object.Name, (const char PROGMEM *)_Key) == 0) _Object.Found = true;

					}

				}

				// Control for Object Found
				if (_Object.Found) return(true);

				// Return No Event
				return(false);

			}

			// Get Key Value Buffer
			uint16_t operator[](const __FlashStringHelper * _Key) {

				// Declare Function Variables
				struct Variables_Struct {
					uint16_t Value = 0;
					uint16_t Char_Counter = 0;
					char Name[10];
					bool Found = false;
					uint16_t Value_Start = 0;
				} _Object;

				// Control for char
				for (uint16_t i = 0; i < this->_JSON_Size; i++) if (this->_JSON_Buffer[i] == 34) _Object.Char_Counter++;

				// Control for Char Count
				if (_Object.Char_Counter % 2 == 1) return(0);

				// Find Start of FW_ID
				for (uint16_t i = 1; i < _Object.Char_Counter; i++) {

					// Find Start of Key
					uint16_t _Key_Start = this->Find_Char('\"', i);
					uint16_t _Key_End = this->Find_Char('\"', i + 1);

					// Control for Root Key
					if (_Key_Start > 0 && _Key_End > 0) {

						// Clear Object
						memset(_Object.Name, '\0', sizeof(_Object.Name));

						// Copy Root Key
						memcpy(_Object.Name, &this->_JSON_Buffer[_Key_Start + 1], _Key_End - _Key_Start - 1);

						// Control for Root
						if (strcmp_P(_Object.Name, (const char PROGMEM *)_Key) == 0) {
							
							// Set Object Found
							_Object.Found = true;

							// Set Object Value Start
							_Object.Value_Start = _Key_End + 2;	
							
						}

					}

				}
				
				// Handle Number
				if ((this->_JSON_Buffer[_Object.Value_Start] - 48) >= 0 && (this->_JSON_Buffer[_Object.Value_Start] - 48) <= 9) {

					// Calculate Event
					_Object.Value = (this->_JSON_Buffer[_Object.Value_Start] - 48);

					// Handle Number
					if ((this->_JSON_Buffer[_Object.Value_Start + 1] - 48) >= 0 && (this->_JSON_Buffer[_Object.Value_Start + 1] - 48) <= 9) {

						// Calculate Event
						_Object.Value = (_Object.Value * 10) + (this->_JSON_Buffer[_Object.Value_Start + 1] - 48);

						// Handle Number
						if ((this->_JSON_Buffer[_Object.Value_Start + 2] - 48) >= 0 && (this->_JSON_Buffer[_Object.Value_Start + 2] - 48) <= 9) {

							// Calculate Event
							_Object.Value = (_Object.Value * 10) + (this->_JSON_Buffer[_Object.Value_Start + 2] - 48);

							// Handle Number
							if ((this->_JSON_Buffer[_Object.Value_Start + 3] - 48) >= 0 && (this->_JSON_Buffer[_Object.Value_Start + 3] - 48) <= 9) {

								// Calculate Event
								_Object.Value = (_Object.Value * 10) + (this->_JSON_Buffer[_Object.Value_Start + 3] - 48);

								// Handle Number
								if ((this->_JSON_Buffer[_Object.Value_Start + 4] - 48) >= 0 && (this->_JSON_Buffer[_Object.Value_Start + 4] - 48) <= 9) {

									// Calculate Event
									_Object.Value = (_Object.Value * 10) + (this->_JSON_Buffer[_Object.Value_Start + 4] - 48);

								}

							}

						}

					}

				}

				// Control for Object Found
				if (_Object.Found) return(_Object.Value);

				// Return No Event
				return(0);

			}

	};

#endif /* defined(__JSON__) */
