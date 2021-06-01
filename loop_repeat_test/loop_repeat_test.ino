/* 
  Original: Michalis Vasilakis    // Date: 8/06/2015 // www.ardumotive.com 
  Added:  [Arduino Fade Sample Code - public domain] // https://www.arduino.cc/en/Tutorial/BuiltInExamples/Fade
  Edited: Juan Banchs             // Date: 5/16/2021 // http://juanbanchs.webflow.io/
*/

//Constants
const int pResistor = A0; // Photoresistor at Arduino analog pin A0
const int ledPin = 9; // Led pin at Arduino pin 9
const int PHOTORESISTOR_THRESHOLD = 700; // Change this until its sensitive enough (0-1023)
const int TIME_DELAY = 50; // In miliseconds
const int NUMBER_OF_LOOPS = 10; // Number of repeats to do

//Variables
int pResistor_value; // Store value from photoresistor (0-1023)
int brightness = 0;  // how bright the LED is (0-255)
int fadeAmount = 5;  // how many points to fade the LED by
int loop_count = 0;  // Loop counter


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
  Serial.println("LABEL, Real Time, Time since Start, Photoresistor Measurement, Brightness (Digital Output)");
  // Serial.println(", Other Measurement"); // If you need any other measurements 
  Serial.println("RESETTIMER"); // Resets timer to 0
}

void loop() {
  
  // Set the brightness of LED:
  analogWrite(ledPin, brightness);
  
  // Delay so that the photoresistor has time to change:
  delay(TIME_DELAY);

  // Send the value
  send_value();

  // Prepare next loop:
  brightness = brightness + fadeAmount; // Change the brightness for next time through the loop
//  if (brightness <= 0 || brightness >= 255) // Reverse fading direction at the end of the fades
//  {fadeAmount = -fadeAmount;}


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


void send_value()
{
//  for (int i = 0; i <= 1000; i++) {
    // Read new photoresistor value:
    pResistor_value = analogRead(pResistor);
  
    // Write to PLX-Daq (Excel)
    Serial.print("DATA,TIME,TIMER,"); // Current Time, Time since start
    Serial.print(pResistor_value); Serial.print(",");
    Serial.print(brightness); Serial.print(",");
    // int Adata = 3; Serial.print(Adata); // If you need any other measurements 
    Serial.println(); // Move to next row
//  }
}
