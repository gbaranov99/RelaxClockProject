# The Relax Clock



The Relax Clock seeks to help people build better sleeping habits through external reinforcement. Our project will remind the user when they should be going to sleep, as well as functioning as a regular alarm. If a user is not asleep by a certain time, Relax Clock will send phone reminders and play an alarm until the user turns off their phone and their lights. Through an accompanying phone app, the user will be able to set when they would like to fall asleep and wake up, what alarm sound to play, and if they are choosing to stay up.


## Materials

- HiLetgo HC-05 Wireless Bluetooth RF Transceiver purchased through Amazon.com.
(https://www.amazon.com/gp/product/B00B887DBC/ref=ppx_yo_dt_b_asin_title_o00_s00?ie=UTF8&psc=1)
	
- FORCE SENSING RESISTOR,1.5 INCH SQUARE purchased through Amazon.com.
(https://www.amazon.com/gp/product/B00B887DBC/ref=ppx_yo_dt_b_asin_title_o00_s00?ie=UTF8&psc=1)

- Photoresistor
- Piezo Buzzer
- 2x16 LCD
- Breadboard
- Arduino Uno
- LED
- Jumper wires 
- 220 ohm resistors 
- 1k ohm resistor
- 2k ohm resistor
- Android phone with android version oreo or higher





## Arduino Installation
#### Arduino to HC-05 bluetooth module
- Digital pin 4   -> 1K ohm resistor 
- GND ->  2K ohm resistor -> 1K ohm resistor -> wire1 -> TX
- Digital pin 3      ———->      RX
- 5V                ———->      VCC
- GND             ———->      GND
 
#### Arduino to LCD
- Digital pin 12 -> LCD RS
- Digital pin 11 -> LCD Enable
- Digital pin 10 -> LCD D4
- Digital pin 9 -> LCD D5
- Digital pin 8  -> LCD D6
- Digital pin 7 -> LCD D7
- Digital pin 6 -> LCD V0
- GND -> LCD R/w
- GND -> LCD VSS
- 5V -> LCD VDD
- 5V -> 220 ohm resistor -> LCD LED+
- GND -> LCD LED-
 
#### Arduino to pressure plate
- 5v -> wire 1 -> 1st Pressure plate pin
- Analog pin 1 -> 220 ohm resistor -> 2nd Pressure plate pin
- GND -> 220 ohm resistor -> wire 2
 
#### Arduino to photoresistor
- 5v -> wire 1 -> 1st Photoresistor Pin
- Analog pin 0 -> 220 ohm resistor -> 2nd Photoresistor Pin
- GND -> 220 ohm resistor -> wire 2
 
#### Arduino to speaker
- Digital pin 5 -> positive piezo speaker pin
- GND -> 220 ohm resistor -> negative piezo speaker pin
 
#### Arduino to LED
- Digital pin 13 -> long LED pin
- GND -> 220 ohm resistor -> short LED pin

## App Installation
Download the apk onto an Android phone with at least Android version 8.0 (Oreo). The location of the apk within the repository is:
https://github.com/gbaranov99/RelaxClockProject/blob/master/RelaxClockAndroid/Relax%20Clock%20APK/app-debug.apk
Install the apk onto the Android phone by “opening” it in a file browser on the phone.
After the application is installed, all that is left to do is pair your phone with the arduino, and begin using it as described in the following section, User Guide.

## User Guide
Once the circuit is constructed, the arduino code is loaded, and the phone app is installed, the project is ready to be used. To begin, the user powers on the arduino and pairs their phone with the arduino in their phone’s settings. Then, the user opens the Android app and selects the arduino as the bluetooth device to connect to, from a list of paired devices. Once the user selects their arduino, the app will switch to the main page. The main page allows the user to do many things. There will be a text field at the top that allows the user to put in a 24 hour time. Clicking the set time button will set the internal clock of the arduino to the entered time. Next, there are two parallel columns for each alarm (sleep alarm and wake alarm). The first text field for each column allows the user to set the time they want their respective alarms played. The set buttons set the alarms for the 24hr time inputted into the text box. The disable button disables the alarm respective to the column of the disable button. The alarms will be disabled until a new alarm has been set.  There is also a list of alarm themes/sounds in each column. Clicking on any of the themes sets the alarm theme for the respective alarm. Each alarm can be set to a different theme, allowing for a more relaxing sleep alarm and a more perturbing morning alarm.
At the time of night specified by the user, the sleep-alarm will sound. The user can stop the alarm by turning the lights off in their bedroom and putting the phone on the pressure plate. At the same time as the alarm is playing, the phone app will change to a new appearance, featuring only large snooze and stop buttons. If the user clicks snooze the alarm will turn off and play again in 5 minutes, and pressing stop will cause the alarm to go off at the same time tomorrow. If the user turns off the lights and places the phone on the pressure pad, the alarm will turn off for the night, and play at the same time the next day. Disabling the alarm through either a button press or satisfying the sensors will cause the appearance of the app to return to the main page (showing alarm configuration options instead of snooze and stop buttons). The user can disable either alarm without these steps with buttons on the main page of the phone app. However, disabling the sleep alarm will result in a pop-up warning about the dangers of abusing the bypass functionality. It is not a perfect way to deter staying up late, but simply making the user think about the negative effects of staying up can help them recognize bad habits. The morning alarm will sound as specified by the user, and can be disabled or snoozed by pressing either the snooze or stop buttons on the phone. The main difference between the morning and night alarm is that the morning alarm cannot be disabled with the sensors, and must be disabled manually. In order for the phone app to work it needs to be open either in the foreground or running in the background of the phone. 

