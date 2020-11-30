import RPi.GPIO as GPIO

GPIO.setmode(GPIO.BCM)

GPIO.setup(25,GPIO.OUT)
GPIO.setup(20,GPIO.IN)

import time

while True:
	if (GPIO.input(20)):
		GPIO.output(25,GPIO.HIGH)
	else:	
		GPIO.output(25,GPIO.LOW)
		time.sleep(1)
