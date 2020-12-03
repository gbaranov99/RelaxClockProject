// Relax Clock .ino Code
// By Greg Baranov and Shawn John

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

// Declare notes and note durations for second included song: Imperial March from Star Wars
int noteCount2 = 70;
int melody2[] = {
  A4, A4, A4, F4, C5, A4, F4, C5, A4, E5, E5, E5, F5, C5, GS4, F4, C5, A4,
  A5, A4, A4, A5, GS5, G5, FS5, F5, FS5, 0, 
  AS4, DS5, D5, CS5, C5, B4, C5, 0, F4, GS4, F4, GS4, C5, A4, C5, E5,
  A5, A4, A4, A5, GS5, G5, FS5, F5, FS5, 0, 
  AS4, DS5, D5, CS5, C5, B4, C5, 0, F4, GS4, F4, GS4, C5, A4, C5, E5
};
int durations2[] = {
  2, 2, 2, -4, 8, 2, -4, 8, 1,
  2, 2, 2, -4, 8, 2, -4, 8, 1,
  2, -4, 8, 2, -4, 8, 8, 8, 4, 4, 4, 2, -4, 8,
  8, 8, 8, 4, 4, 2, -4, 8, 2, -4, 8, 1,
  2, -4, 8, 2, -4, 8, 8, 8, 4, 4, 4, 2, -4, 8,
  8, 8, 8, 4, 4, 2, -4, 8, 2, -4, 8, 1
};


// Declare notes and note durations for third included song: Game of Thrones Main Title
int noteCount3 = 106;
int melody3[] = {
  G4, C4, DS4, F4, G4, C4, DS4, F4,G4, C4, DS4, F4, G4, C4, DS4, F4,G4, C4, E4, F4, G4, C4, E4, F4,G4, C4, E4, F4, G4, C4, E4, F4,
  G4, C4, DS4, F4, G4, C4, DS4, F4, D4, 0, F4, AS3, DS4, D4, F4, AS3,DS4, D4, C4, 0,
  G4, C4, DS4, F4, G4, C4, DS4, F4, D4, 0, F4, AS3, DS4, D4, F4, AS3,DS4, D4, C4, 0,
  G4, C4, DS4, F4, G4, C4, DS4, F4, D4, 0, F4, AS3, D4, DS4, D4, AS3 , C4, 0,
  
  C5, AS4, C4, G4, DS4, DS4, F4, G4, 0,
  C5, AS4, C4, G4, DS4, DS4, D4,
  
};

int durations3[] = {
  4,4,8,8,4,4,8,8,4,4,8,8,4,4,8,8,4,4,8,8,4,4,8,8,4,4,8,8,4,4,8,8,
  2,2,8,8,2,2,8,8,1, 1,2,2,8,8,2,2,8,8,1,1,
  2,2,8,8,2,2,8,8,1, 1,2,2,8,8,2,2,8,8,1,1,
  2,2,8,8,2,2,8,8,1, 1,2,2,2,4,2,4, 1,1,

  1, 1, 1, 1, 1, 2, 2, 1, 1, 
  1, 1, 1, 1, 1, 2, 2, 
};

// Declare variables used to play music
int curNote = 0;   // Current note being played

int wakeUpPlaying = 0;
int sleepPlaying = 0;

// Set Pirates of the Carribean as the default song for both sleep and wake up alarms
int sleepNoteCount = noteCount1;
int *sleepMelody = melody1;
int *sleepDurations = durations1;

int wakeUpNoteCount = noteCount1;
int *wakeUpMelody = melody1;
int *wakeUpDurations = durations1;


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

    // Get time data from bluetooth input
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
      time_t tmp = now();
      setTime(hours, minutes, seconds,day(),month(),year());
      if (tmp > now()) {
        setTime(hours, minutes, seconds,day() + 1,month(),year());
      }
      wakeUpAlarm = now();
      setTime(tmp);
    }

    // Disable sleep alarm
    else if (Data[0] == 3) {
      sleepAlarm = 0;
      sleepPlaying = 0;
    }
    
    // Disable wake up alarm
    else if (Data[0] == 4) {
      wakeUpAlarm = 0;
      wakeUpPlaying = 0;
    }
    
    // Snooze sleep alarm 5 minutes
    else if (Data[0] == 5) {
      sleepAlarm = adjustAlarm(300, sleepAlarm);
      sleepPlaying = 0;
    }
    
    // Stop sounding sleep alarm, automatically sets to same time next day
    else if (Data[0] == 6) {
      sleepAlarm = adjustAlarm(86400, sleepAlarm);
      sleepPlaying = 0;
    }

    // Snooze wake up alarm 5 minutes
    else if (Data[0] == 7) {
      wakeUpAlarm = adjustAlarm(300, wakeUpAlarm);
      wakeUpPlaying = 0;
    }
    
    // Stop sounding wake up alarm, automatically sets to same time next day
    else if (Data[0] == 8) {
      wakeUpAlarm = adjustAlarm(86400, wakeUpAlarm);
      wakeUpPlaying = 0;
    }

    // Set alarm sound from one of three presets
    else if (Data[0] == 9) {
      // Set wake up alarm song
      if (Data[3] == 0) {
        // 0 = Pirates of the Carribean, 1 = Imperial March, 2 = Game of Thrones Main Theme
        if (Data[5] == 0) {
          wakeUpNoteCount = noteCount1;
          wakeUpMelody = melody1;
          wakeUpDurations = durations1;
        }
        else if (Data[5] == 1) {
          wakeUpNoteCount = noteCount2;
          wakeUpMelody = melody2;
          wakeUpDurations = durations2;
        }
        else if (Data[5] == 2) {
          wakeUpNoteCount = noteCount3;
          wakeUpMelody = melody3;
          wakeUpDurations = durations3;
        }
      }
      // Set sleep alarm song
      else {
        if (Data[5] == 0) {
          // 0 = Pirates of the Carribean, 1 = Imperial March, 2 = Game of Thrones Main Theme
          sleepNoteCount = noteCount1;
          sleepMelody = melody1;
          sleepDurations = durations1;
        }
        else if (Data[5] == 1) {
          sleepNoteCount = noteCount2;  
          sleepMelody = melody2;
          sleepDurations = durations2;
        }
        else if (Data[5] == 2) {
          sleepNoteCount = noteCount3;
          sleepMelody = melody3;
          sleepDurations = durations3;
        }
      }
    }
  }
  Data = "";
}

// Simple function to pad a number with a leading 0 if it is a single digit
// Prints 01 to LCD if input is 1, and prints 31 if input is 31
void leadingZero(int number) {
  if (number / 10 == 0) {
    lcd.print("0");
    lcd.print(number);
  }
  else {
    lcd.print(number);
  }
}

// Updates LCD with current time and alarm information
void updateLCD() {
  if (second() != prevSecond) {
    // Display time info
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Time:   ");
    leadingZero(hour());
    lcd.print(":");
    leadingZero(minute());
    lcd.print(":");
    leadingZero(second());
    lcd.setCursor(0,1);

    int showSleep;
    
    // Display alarm info
    // If no alarms are set, display "None :)"
    if (!sleepAlarm && !wakeUpAlarm) {
      lcd.print("Alarm:   None :)");
    }
    // If both alarms are set, show the one that will occur sooner
    else if (sleepAlarm && wakeUpAlarm) {
      // Sleep alarm is sooner
      if ((sleepAlarm - now()) < (wakeUpAlarm - now())) {
        showSleep = 1;
      }
      // Wake up alarm is sooner
      else {
        showSleep = 0;
      }
    }
    // Only sleep alarm is set, show sleep alarm
    else if (sleepAlarm) {
      showSleep = 1;
    }
    // Only wake up alarm is set, show wake up alarm
    else {
      showSleep = 0;
    }

    // Prints sleep or wake up alarm to LCD depending on above logic
    // Print sleep alarm info if sleep alarm is sooner
    if (showSleep) {
      lcd.print("SAlarm: ");
      leadingZero(hour(sleepAlarm));
      lcd.print(":");
      leadingZero(minute(sleepAlarm));
      lcd.print(":");
      leadingZero(second(sleepAlarm));
    }
    // Print wake up alarm info if wake up alarm is sooner
    else {
      lcd.print("WAlarm: ");
      leadingZero(hour(wakeUpAlarm));
      lcd.print(":");
      leadingZero(minute(wakeUpAlarm));
      lcd.print(":");
      leadingZero(second(wakeUpAlarm));
    }
    prevSecond = second();
  }
}

// Adjust alarm time using now(), adjustTime() and setTime() functions from Time library
time_t adjustAlarm(unsigned long adjustment, time_t alarm) {
  time_t tmp = now();
  setTime(alarm);
  adjustTime(adjustment);
  alarm = now();
  setTime(tmp);
  sleepPlaying = 0;
  wakeUpPlaying = 0;
  return alarm;
}

// Setup function that initializes functioning of arduino
void setup() {
  // Start LCD display, Bluetooth and serial monitor
  Bluetooth.begin(9600);
  Serial.begin(9600); 
  lcd.begin(16, 2);

  // Initialize pins 
  pinMode(LED,OUTPUT);
  pinMode(3, INPUT);
  pinMode(4, OUTPUT);
  pinMode(lightSens, INPUT);
  pinMode(pressSens, INPUT);
  pinMode(buzzer, OUTPUT);

  // Hardcode brightness of LCD, as potentiometer component proved inconsistent
  analogWrite(6, 90); 

  // Set starting time to start of 2020
  // Date doesn't matter, as our alarms only concern themselves with the time of day
  setTime(0,0,0,1,1,2020);
  
  // Show starting messages
  lcd.setCursor(0,0);
  lcd.print("Time:   00:00:00");
  lcd.setCursor(0,1);
  lcd.print("Alarm:   None :)");
}

// Main arduino loop that runs our program
void loop() {

  // Every time loop() runs, check if there was any bluetooth input with the readBluetooth() function
  readBluetooth();

  // Get data from analog Light Sensor and Pressure Sensor
  lightData = analogRead(lightSens);
  pressData = analogRead(pressSens);

  // Check if phone is on pressure pad and lights are off if it is past sleep alarm time
  if (sleepAlarm != 0 && sleepAlarm < now()) {
    // If the room is lit or phone is not on the pressure pad, sound sleep alarm and send notifications to user's phone
    if (lightData > 500 || pressData < 200) {
      digitalWrite(LED, HIGH);
      curNote = 0;
      sleepPlaying = 1;
      Bluetooth.println(1);
    }
    // If room is dark and phone is on pressure pad, set sleep alarm to same time the next day
    else {
      sleepAlarm = adjustAlarm(86400, sleepAlarm);
      sleepPlaying = 0;
      Bluetooth.println(2);
    }
  }
  // Play wake up alarm if it is past that alarm's time
  else if (wakeUpAlarm != 0 && wakeUpAlarm < now()) {
    digitalWrite(LED, HIGH);
    curNote = 0;
    wakeUpPlaying = 1;
    Bluetooth.println(0);
  }
  // If not yet time for either alarm, don't do anything
  else {
    digitalWrite(LED,  LOW);
    curNote = 0;
    sleepPlaying = 0;
    wakeUpPlaying = 0;
  }

  // If it is time for wake up alarm to play, play the notes for the alarm within this loop
  // Much of this loop's logic is adapted from the github and tutorial at the start of this .ino file
  while ((curNote < wakeUpNoteCount) && (wakeUpPlaying)) {
    int noteDuration;       // How long the note should play for
    int pauseBetweenNotes;  // How long a pause between notes should be, to make sure notes don't blend together

    // If the duration in wakeUpDurations is > 0, it is a regular note
    if (wakeUpDurations[curNote] > 0) {
      noteDuration = 1000 / wakeUpDurations[curNote];
      pauseBetweenNotes = noteDuration * 1.30;
    }
    // If the duration is < 0, it is a dotted note, and should be played for 1.5 time the note's specified duration
    // Allows for more detailed and accurate music to be played
    else {
      noteDuration = 1000 / abs(wakeUpDurations[curNote]);
      noteDuration *= 1.5;
      pauseBetweenNotes = noteDuration * 1.30;
    }
    // Play the the current note in the melody for the specified duration
    tone(buzzer, wakeUpMelody[curNote], noteDuration);
    delay(pauseBetweenNotes);
    noTone(buzzer);
    curNote += 1;    // Move on to the next note in the arrays to play
    readBluetooth(); // Check if interrupt was received to stop playing current song
    updateLCD();
  }

  // Plays sleep alarm if it is time for the alarm to play
  // Functions identically to above wake up alarm except with sleep alarm variables
  while ((curNote < sleepNoteCount) && (sleepPlaying)) {
    int noteDuration;
    int pauseBetweenNotes;
    if (sleepDurations[curNote] > 0) {
      noteDuration = 1000 / sleepDurations[curNote];
      pauseBetweenNotes = noteDuration * 1.30;
    }
    else {
      noteDuration = 1000 / abs(sleepDurations[curNote]);
      noteDuration *= 1.5;
      pauseBetweenNotes = noteDuration * 1.30;
    }
    tone(buzzer, sleepMelody[curNote], noteDuration);
    delay(pauseBetweenNotes);
    noTone(buzzer);
    curNote += 1;
    readBluetooth();
    updateLCD();
  }

  // Update LCD and go on to next loop() call
  updateLCD();
  delay(200);
}
