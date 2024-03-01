#include <Arduino.h>

#include <JSON.h>

void setup() {

	// Initialize Serial
	Serial.begin(115200);
  
	// Create JSON Buffer
	char _Buffer[60] = "{\"Request\": {      \"Event\": 900,        \"FW_ID\": 1222}}";

	// Create JSON Object
	JSON _JSON(_Buffer);

	// Print Buffer
	Serial.print(F("Buffer           : ")); Serial.println(_Buffer);

	// Print JSON Validity
	Serial.print(F("JSON Validity    : ")); Serial.println(_JSON.isValid() ? "Valid" : "Invalid");

	// Print JSON
	Serial.print(F("JSON             : ")); Serial.println(_JSON.Buffer());

	// Print JSON Size
	Serial.print(F("JSON Size        : ")); Serial.println(_JSON.Size());

	// Print Key Find
	Serial.print(F("Key Find (FW_ID) : ")); Serial.println(_JSON.Key_Find(F("FW_ID")) ? "Found" : "Not Found");
	Serial.print(F("Key Find (AW_ID) : ")); Serial.println(_JSON.Key_Find(F("AW_ID")) ? "Found" : "Not Found");

	// Print Key Value
	Serial.print(F("Key Value        : ")); Serial.println(_JSON[F("FW_ID")]);

	// Print Divider
	Serial.println(F("--------------------------------------------------"));

}

void loop() {

}
