#include <stdio.h>
#include <wiringPi.h>
#define LED 3
#define BUTTON 0

int main(void)
{
	wiringPiSetup ();
	pinMode (LED, OUTPUT)
	pinMode (BUTTON, INPUT)

	for(;;)
		{
			if (digitalRead(BUTTON) == HIGH)
			{
				digitalWrite(LED,HIGH);
				delay(10);
			}
			else
			{
				digitalWrite(LED,LOW);
				delay(10);
			}
		}
	return 0
}
