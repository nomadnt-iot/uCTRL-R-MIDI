#include <vSlider.h>

#define LED_PIN 13

// Create an instance of vSlider. The first parameter is the PIN used
// by sensor, the second is the sensitivity treshold and the third is
// the hold time to store the value.
vSlider slider = vSlider(A0);

void setup(){
	analogReference(EXTERNAL);

	Serial.begin(115200);
	Serial.println(F("Starting VirtualSlider on A0"));

	slider.attachLock(notify_lock);
	slider.attachUnlock(notify_unlock);

	pinMode(LED_PIN, OUTPUT);
	digitalWrite(LED_PIN, LOW);
}

void loop(){
	Serial.print(F("RAW: "));
	Serial.println(slider.read());
	delay(20);
}

void notify_lock(){
	digitalWrite(LED_PIN, HIGH);
	Serial.println(F("Virtual Slider LOCKED"));
}

void notify_unlock(){
	digitalWrite(LED_PIN, LOW);
	Serial.println(F("Virtual Slider UNLOCKED"));
}