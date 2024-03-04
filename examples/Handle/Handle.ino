#include <Arduino.h>

#include <JSON.h>

void setup() {

	// Initialize Serial
	Serial.begin(115200);

	// Create JSON Buffer
	char _Buffer[91] = "{\"Request\": {      \"Event\": 900,                                           \"FW_ID\": 1222}}";

	// Create JSON Object
	JSON _JSON(_Buffer, sizeof(_Buffer));

	// Print JSON Buffer
//	Serial.print("JSON Buffer      : "); Serial.println(_JSON.Buffer());

	// Print JSON Buffer Size
//	Serial.print("JSON Buffer Size : "); Serial.println(sizeof(_Buffer));

	// Print JSON Size
//	Serial.print("JSON Size        : "); Serial.println(_JSON.Size());

	// Print JSON Valid
	Serial.print("JSON Valid       : "); Serial.println(_JSON.isValid());

	// Print Key Find
	Serial.print("Key Find         : "); Serial.println(_JSON.Key_Find(F("FW_ID")));

	// Print Key Value
	Serial.print("Key Value        : "); Serial.println(_JSON[F("FW_ID")]);

}

void loop() {

}
