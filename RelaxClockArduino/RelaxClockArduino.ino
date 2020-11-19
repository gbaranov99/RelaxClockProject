// Greg Baranov and Shawn John

// Sources and tutorial on how to play music with piezoelectric speaker:
// https://create.arduino.cc/projecthub/nathan_ramanathan/play-polyphonic-tones-40e51b
// https://github.com/nathanRamaNoodles/Noodle-Synth/blob/master/src/MusicWithoutDelay.cpp


#include <LiquidCrystal.h>  // Include LCD library
#include <Time.h>           // Include Time library
#include <SoftwareSerial.h> // Include SoftwareSerial library, need for Bluetooth connection
#include "pitches.h"        // Header file that defines piezo speaker pitches into piano notes

// Initialize Bluetooth and LCD based on pin configuration
SoftwareSerial Bluetooth(3, 4); // RX, TX
LiquidCrystal lcd(12, 11, 10, 9, 8, 7);
String Data = ""; // The data received

// Declare the pins used
int LED = 13;
int lightSens = 14;
int lightData;
int pressSens = 15;
int pressData;
int buzzer = 5;


// Declare notes and note durations for first included song: He's A Pirate / Pirates the Carribean Theme Song
int noteCount1 = 83;
int melody1[] = {
   A4, A4, A4, A4, A4, A4, A4, A4, A4, A4, A4, A4, A4, A4, A4, A4,
   A4, C5, D5, D5, D5, E5, F5, F5,    F5, G5, E5, E5, D5, C5, C5, D5, 0,    A4, C5, D5, D5, D5, E5, F5, F5,   F5, G5, E5, E5, D5, C5, D5, 0,    A4, C5, D5, D5, D5, F5, G5, G5,   
   G5, A5, AS5, AS5, A5, G5, A5, D5, 0,    D5, E5, F5, F5, G5, A5, D5, 0,   D5, F5, E5, E5, F5, D5, E5, 0
};
int durations1[] = {
  4, 8, 8, 8, 8, 4, 8, 8, 8, 8, 4, 8, 8, 8, 8, 2,
  8, 8, 4, 4, 8, 8, 4, 4,   8, 8, 4, 4, 8, 8, 8, 4, 8,    8, 8, 4, 4, 8, 8, 4, 4,   8, 8, 4, 4, 8, 8, 4, 4,   8, 8, 4, 4, 8, 8, 4, 4,   8, 8, 4, 4, 8, 8, 8, 4, 8,    
  8, 8, 4, 4, 4, 8, 4, 4,   8, 8, 4, 4, 8, 8, 4, 4
};

// Declare variables used to play music
int playMusic = 0; // Boolean used to determine if music should be playing
int curNote = 0;   // Current note being played
// Set Pirates of the Carribean as the default song
int noteCount = noteCount1;
int *melody = melody1;
int *noteDurations = durations1;


// Declare alarm variables
time_t sleepAlarm = 0;
time_t wakeUpAlarm = 0;

int prevSecond = 0; // Variable to allow updates of LCD once a second, despite more frequent refreshing

// Function that reads in data from bluetooth, and sets global variables based on data read
void readBluetooth() {
  int readData = 0; // Variable to keep track of whether new Bluetooth data was received

  // If we received Bluetooth data, read it (one character at a time) into Data
  while(Bluetooth.available()) {
    delay(3);
    char c = Bluetooth.read();
    Data += c;
    readData = 1;
  }
  
  // Execute Bluetooth commands
  // Each command is sent from Android app with format X: Y, where X is a code between 0 and 2 and Y is payload data
  // For first 3 commands, payload data is a time of day, with structure HH:MM:SS
  //
  // Code definitions:
  // 0: set system time, 1: set sleep alarm time, 2: set wake up alarm time, 
  // 3: disable sleep alarm, 4: disable wake up alarm
  // 5: snooze alarm 5 minutes, 6: stop alarm, set it to same time again next day
  //
  // Example: "0: 10:20:30" sets Arduino's time to 10:20 AM, 30 seconds
  if (readData) {

    Serial.println(Data);
    
    // Convert characters to integers, only if character is an ASCII number
    for (int i = 0; i < 11; i ++) {
      if (Data[i] > 47 && Data[i] < 58) {
        Data[i] = Data[i] - '0';
        
      }
    }

    int hours = Data[3] * 10 + Data[4];
    int minutes = Data[6] * 10 + Data[7];
    int seconds = Data[9] * 10 + Data[10];

    // Set system time
    if (Data[0] == 0) {
      setTime(hours, minutes, seconds,1,1,2020);
    }

    // Set sleep alarm time
    else if(Data[0] == 1){
      time_t tmp = now();
      setTime(hours, minutes, seconds,day(),month(),year());
      if (tmp > now()) {
        setTime(hours, minutes, seconds,day() + 1,month(),year());
      }
      sleepAlarm = now();
      setTime(tmp);
    }

    // Set wake up alarm time
    else if (Data[0] == 2) {
      
    }

    // Disable sleep alarm
    else if (Data[0] == 3) {
      sleepAlarm = 0;
      playMusic = 0;
    }
    
    // Disable wake up alarm
    else if (Data[0] == 4) {
      wakeUpAlarm = 0;
    }
    
    // Snooze alarm 5 minutes
    else if (Data[0] == 5) {
      adjustAlarm(300);
    }
    
    // Stop sounding alarm, automatically sets to same time next day
    else if (Data[0] == 6) {
      adjustAlarm(86400);
    }
  
//    Serial.println("time: ");
//    Serial.println(now());
//    Serial.print("alarm: ");
//    Serial.println(sleepAlarm);
  }
  Data = "";
}

// Simple function to p
void leadingZero(int number) {
  if (number / 10 == 0) {
    lcd.print("0");
    lcd.print(number);
  }
  else {
    lcd.print(number);
  }
}

void updateLCD() {
  if (second() != prevSecond) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Time:   ");
    leadingZero(hour());
    lcd.print(":");
    leadingZero(minute());
    lcd.print(":");
    leadingZero(second());
    lcd.setCursor(0,1);
    if (sleepAlarm == 0) {
      lcd.print("Alarm:   None :)");
    }
    else {
      lcd.print("Alarm:  ");
      leadingZero(hour(sleepAlarm));
      lcd.print(":");
      leadingZero(minute(sleepAlarm));
      lcd.print(":");
      leadingZero(second(sleepAlarm));
    }
    prevSecond = second();
  }
}

void adjustAlarm(unsigned long adjustment) {
  time_t tmp = now();
  setTime(sleepAlarm);
  adjustTime(adjustment);
  sleepAlarm = now();
  setTime(tmp);
  playMusic = 0;
}

void setup() {
  Bluetooth.begin(9600);
  Serial.begin(9600);
  lcd.begin(16, 2); // Start LCD display and serial monitor
  
  Serial.println("Waiting for command...");
  // Bluetooth.println("Send 1 to turn on the LED. Send 0 to turn Off");
  pinMode(LED,OUTPUT);
  pinMode(3, INPUT);
  pinMode(4, OUTPUT);
  pinMode(lightSens, INPUT);
  pinMode(pressSens, INPUT);
  pinMode(buzzer, OUTPUT);

  analogWrite(6, 90); // Hardcode brightness of LCD, as potentiometer component was inconsistent

  setTime(0,0,0,1,1,2020);
  
  // Show starting messages
  lcd.setCursor(0,0);
  lcd.print("Time:   00:00:00");
  lcd.setCursor(0,1);
  lcd.print("Alarm:   None :)");
}
 
void loop() {
  // Because our alarms only care about time of day, not number of days since alarm started running, we do (milliseconds running) % (milliseconds in a day)
  // startTime lets us set the current time of the 
  // Allows us to only use millis() for time keeping, as we do not need the advanced functionality of the Time library
  
  readBluetooth();
//  Serial.println(Data);
  
  lightData = analogRead(lightSens);
  pressData = analogRead(pressSens);
//  Serial.print("Light Sensor reading: ");
//  Serial.println(lightData);
//  Serial.print("Pressure Sensor reading: ");
//  Serial.println(pressData);

//  Serial.print("time: ");
//  Serial.println(now());
//  Serial.print("alarm: ");
//  Serial.println(sleepAlarm);

  // Check if phone is on pressure pad and lights are off if it is past sleep alarm time
  if (sleepAlarm != 0 && sleepAlarm < now()) {
    // If the room is lit or phone is not on the pressure pad, sound sleep alarm and send notifications to user's phone
    if (lightData > 500 || pressData < 200) {
      // TODO: Send notifications to user's phone
      // TODO: Possibly set pressData to be calibrated based on how much user's phone weighs
      digitalWrite(LED, HIGH);
      curNote = 0;
      playMusic = 1;
    }
    // If room is dark and phone is on pressure pad, set sleep alarm to same time the next day
    else {
      adjustAlarm(86400);
    }
  }
  // Check if it is past wake up alarm time
  else if (wakeUpAlarm != 0 && wakeUpAlarm < now()) {
    digitalWrite(LED, HIGH);
    curNote = 0;
    playMusic = 0;
  }
  // If not yet time for alarm, don't do anything
  else {
    digitalWrite(LED,  LOW);
    curNote = 0;
    playMusic = 0;
  }

  while ((curNote < noteCount) && (playMusic)) {
    // Serial.println(playMusic);
    int noteDuration = 1000 / noteDurations[curNote];
    int pauseBetweenNotes = noteDuration * 1.30;
    tone(buzzer, melody[curNote], noteDuration);
    delay(pauseBetweenNotes);
    noTone(buzzer);
    curNote += 1;
    readBluetooth(); // Check if interrupt was received to stop playing current song
    updateLCD();
  }

  updateLCD();
  delay(200);
}
