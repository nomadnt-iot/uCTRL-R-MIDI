#include <vSlider.h>

#define LED_PIN 6

// Create an instance of vSlider. The first parameter is the PIN used
// by sensor, the second is the sensitivity treshold and the third is
// the hold time to store the value.
vSlider slider = vSlider(A0);

void setup(){
  analogReference(EXTERNAL);

  Serial.begin(115200);
  Serial.println(F("Starting VirtualSlider on A0"));

  pinMode(LED_PIN, OUTPUT);
}

void loop(){
	analogWrite(LED_PIN, slider.read());
	delay(20);
}
