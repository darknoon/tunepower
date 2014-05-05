tunepower
=========

TunePower is a simple accessory for [Airport Express](https://www.apple.com/airport-express/) that automatically senses audio and turns on an accessory, namely an audio power amplifier. It is implemented with a [Digispark](http://digistump.com/products/1) microcontroller development board and a [PowerSwitch Tail II](http://www.powerswitchtail.com/Pages/default.aspx) relay to switch AC line voltage, as well as a small backpack board for the Digispark.

This system uses a simple LM741 op-amp circuit to amplify the audio for input into the [ATTiny85](http://www.atmel.com/devices/attiny85.aspx) microcontroller. The microcontroller manages turning on the relay circuit once an audio signal is detected. The software keeps the output on for a period of at least 10 minutes, to avoid rapid cycling of power amplifier.

Future enhancements:
One future feature that could increase reliability is to implement software such that when the circuit shuts off, a lockout period of 1 second is enforced. This will make sure any transients introduced in an AC line voltage transition do not reactivate the detector.


Pictures below:
1. Original prototype under development.
2. Digispark + backpack front view.
3. Digispark + backpack back view.


![alt tag](https://raw.github.com/darknoon/tunepower/master/readme-media/Breadboard_Prototype_Downscaled.jpg)
![alt tag](https://raw.github.com/darknoon/tunepower/master/readme-media/Digispark_front_Downscaled.jpg)
![alt tag](https://raw.github.com/darknoon/tunepower/master/readme-media/Digispark_back_Downscaled.jpg)
![alt tag](https://raw.github.com/darknoon/tunepower/master/readme-media/Installed_Downscaled.jpg)
![alt tag](https://raw.github.com/darknoon/tunepower/master/readme-media/Schematic.JPG)

