/* 
  Original: Michalis Vasilakis    // Date: 8/06/2015 // www.ardumotive.com 
  Edited: Juan Banchs             // Date: 5/16/2021 // http://juanbanchs.webflow.io/
*/

//Constants
const int pResistor = A0; // Photoresistor at Arduino analog pin A0
const int ledPin = 9; // Led pin at Arduino pin 9
const int PHOTORESISTOR_THRESHOLD = 700; // Change for sensitivity (0-1023)
const int TIME_DELAY = 1000; // In miliseconds

//Variables
int pResistor_value; // Store value from photoresistor (0-1023)


// Code
void setup() {
  // Communication Setup:
  // analogReadResolution(12); // If we are using Arduino DUE => Use maximum bit rate.
  // Otherwise, max is automatically used
  Serial.begin(9600); // Acceptable value

  // Pin Setup:
  pinMode(ledPin, OUTPUT);  // Set lepPin - 9 pin as an output
  pinMode(pResistor, INPUT);// Set pResistor - A0 pin as an input (optional)

  // PLX-DAQ Setup:
  Serial.println("CLEARDATA"); // Clears up any data left from previous projects
  // Always write "LABEL," then "column headers"
  Serial.println("LABEL, Real Time, Time since Start, Photoresistor Measurement");
  // Serial.println(", Other Measurement"); // If you need any other measurements 
  Serial.println("RESETTIMER"); // Resets timer to 0
}

void loop() {
  // Read new photoresistor value:
  pResistor_value = analogRead(pResistors);

  // If dark enough, turn on the LED, otherwise, turn off;
  if (pResistor_value > PHOTORESISTOR_THRESHOLD)
  {digitalWrite(ledPin, LOW);}  // Turn led off
  else
  {digitalWrite(ledPin, HIGH);} // Turn led on

  // Write to PLX-Daq (Excel)
  Serial.print("DATA,TIME,TIMER,"); // Current Time, Time since start
  Serial.print(pResistor_value); Serial.print(",");
  // int Adata = 3; Serial.print(Adata); // If you need any other measurements 
  Serial.println(); // Move to next row

  // Complete measurement delay:
  delay(TIME_DELAY);
}
