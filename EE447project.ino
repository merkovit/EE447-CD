#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345.h>
 
/* Assign a unique ID to this sensor at the same time */
Adafruit_ADXL345 accel = Adafruit_ADXL345(12345);
 
 
float AccelMinX = 0;
float AccelMaxX = 0;
float AccelMinY = 0;
float AccelMaxY = 0;
float AccelMinZ = 0;
float AccelMaxZ = 0;
float prevAccelX = 0;
float prevAccelY = 0;
float prevAccelZ = 0;
float tolPert = 5;  //to be set by user ...MAY NEED TO EXPAND FOR X, Y, AND Z AXIS
float diffX = 0;
float diffY = 0;
float diffZ = 0;
int count = 0;  //MAY NEED TO GET RID OF THIS AND JUST HAVE AN IF STATEMENT CHECKING FOR ACCELERATION READINGS OF ZERO (GOLF SWING FINISHED)
int led = 13;
int pertFlag; //Flag to tell LED to blink after swing is over
 
void setup(void) 
{
  Serial.begin(9600);
  Serial.println("ADXL345 Accelerometer Calibration"); 
  Serial.println("");
  
  Serial.println("Setting up LED");
  pinMode (led, OUTPUT);
  
  /* Initialise the sensor */
  if(!accel.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("Wiring error!");
    while(1);
  }
}
 
void loop(void)
{
    Serial.println("Type key when ready..."); 
    while (!Serial.available()){}  // wait for a character
    
    while (count < 10) {
      /* Get a new sensor event */ 
      sensors_event_t accelEvent;  
      accel.getEvent(&accelEvent);
    
      /* Setting variables for comparison with tolerated perturbation variable */
      diffX = abs(prevAccelX) - abs(accelEvent.acceleration.x);  //WILL NEED TO ADJUST LOGIC FOR GOLF SWING
      diffY = abs(prevAccelY) - abs(accelEvent.acceleration.y);
      diffZ = abs(prevAccelZ) - abs(accelEvent.acceleration.z);
    
      /* Checking to see if there is a large perturbation in acceleration */
    
      if ((prevAccelX != 0) && (prevAccelY != 0) && (prevAccelZ != 0)) {
        if (abs(diffX) > tolPert) Serial.println("Hitch in swing, X-axis");
        if (abs(diffY) > tolPert) Serial.println("Hitch in swing, Y-axis");
        if (abs(diffZ) > tolPert) Serial.println("Hitch in swing, Z-axis");
        if ((abs(diffX) > tolPert) || (abs(diffY) > tolPert) || (abs(diffZ) > tolPert)) pertFlag = 1;
      }
    
    
      /* Logging current acceleration to compare to next acceleration */
      prevAccelX = accelEvent.acceleration.x;
      prevAccelY = accelEvent.acceleration.y;
      prevAccelZ = accelEvent.acceleration.z;
    
      /* Printing acceleration values to the Serial Monitor */
      Serial.print("Previous Accels: "); Serial.print(prevAccelX); Serial.print("  ");Serial.print(prevAccelY); Serial.print("  "); Serial.print(prevAccelZ); Serial.println();
      Serial.print("Accel differences: "); Serial.print(diffX); Serial.print("  ");Serial.print(diffY); Serial.print("  "); Serial.print(diffZ); Serial.println();
      delay(500);
      count = count++;
      Serial.println(count);
      Serial.println(pertFlag);
      }
     
     if (pertFlag == 1) {
       while (count > 5) {
         Serial.print("led if "); Serial.println(count);
         digitalWrite (led, HIGH);
         delay(1000);
         digitalWrite (led, LOW);
         delay(1000);
         count = count--;
       }
     }
      
      
      
 
      while (Serial.available())
      {
        Serial.read();  // clear the input buffer
      }
}
