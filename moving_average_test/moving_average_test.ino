/* 
  Original: Michalis Vasilakis    // Date: 8/06/2015 // www.ardumotive.com 
  Added:  [Arduino Fade Sample Code - public domain] // https://www.arduino.cc/en/Tutorial/BuiltInExamples/Fade
  Added:  [Moving Avg Sample Code - public domain]   // https://www.arduino.cc/en/Tutorial/BuiltInExamples/Smoothing
  Edited: Juan Banchs             // Date: 5/16/2021 // http://juanbanchs.webflow.io/
*/

//Constants
const int pResistor = A0; // Photoresistor at Arduino analog pin A0
const int ledPin = 9; // Led pin at Arduino pin 9
const int PHOTORESISTOR_THRESHOLD = 700; // Change this until its sensitive enough (0-1023)
const int TIME_DELAY = 125; // In miliseconds
const int NUMBER_OF_LOOPS = 1; // Number of repeats to do

//Variables
int pResistor_value; // Store value from photoresistor (0-1023)
int brightness = 0;  // how bright the LED is (0-255)
int fadeAmount = 5;  // how many points to fade the LED by
int loop_count = 0;  // Loop counter

// For Moving average
const int numReadings = 10;

int readings[numReadings];      // The readings from the analog input
int readIndex = 0;              // The index of the current reading
int total = 0;                  // The running total
int average = 0;                // The average

// Code
void setup() {
  // Communication Setup:
  // analogReadResolution(12); // If we are using Arduino DUE => Use maximum bit rate. 
  // Otherwise, max is automatically used
  Serial.begin(9600); // Acceptable value

  // Pin Setup:
  pinMode(ledPin, OUTPUT);  // Set lepPin - 9 pin as an output
  pinMode(pResistor, INPUT);// Set pResistor - A0 pin as an input (optional)

  // Moving average Setup: Initialize all the readings to 0
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }

  // PLX-DAQ Setup:
  Serial.println("CLEARDATA"); // Clears up any data left from previous projects
  // Always write "LABEL," then "column headers"
  Serial.println("LABEL, Real Time, Time since Start, Photoresistor Measurement, Brightness (Digital Output)");
  // Serial.println(", Other Measurement"); // If you need any other measurements 
  Serial.println("RESETTIMER"); // Resets timer to 0
}

void loop() {
  
  // Set the brightness of LED:
  analogWrite(ledPin, brightness);
  
  // Delay so that the photoresistor has 
  // time to change:
  delay(TIME_DELAY);

  // Read new photoresistor value:
  pResistor_value = analogRead(pResistor);

  calculate_average();

  // Send the value to PLX-DAQ
//  send_value(pResistor_value);
  send_value(average);

  // Prepare next loop:
  // Change the brightness for next time 
  // through the loop
  brightness = brightness + fadeAmount; 

  // Check the loop number
  if (brightness == 260){
    loop_count = loop_count+1;
    brightness = 0;
    analogWrite(ledPin, brightness);
    delay(1000);

    // Exit after desired number of repeats
    if (loop_count == NUMBER_OF_LOOPS)
    {exit(0);}
  }    
}


void send_value(int value_to_be_sent)
{ 
    // Write to PLX-Daq (Excel)
    // Current Time, Time since start
    Serial.print("DATA,TIME,TIMER,"); 
    Serial.print(value_to_be_sent); Serial.print(",");
    Serial.print(brightness); Serial.print(",");
    // If you need any other measurements 
    // int Adata = 3; Serial.print(Adata);
    Serial.println(); // Move to next row
}

void calculate_average() {
  // subtract the last reading:
  total = total - readings[readIndex];
  // read from the sensor:
  readings[readIndex] = pResistor_value;
  // add the reading to the total:
  total = total + readings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }

  // calculate the average:
  average = total / numReadings;
}
