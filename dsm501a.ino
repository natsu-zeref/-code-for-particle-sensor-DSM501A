/**

 * DESCRIPTION
 * 
 * Dust Sensor for SamYoung DSM501
 *   connect the sensor as follows :
 *    Pin 2 of dust sensor PM1      -> Digital 3 (PMW)
 *    Pin 3 of dust sensor          -> +5V 
 *    Pin 4 of dust sensor PM2.5    -> Digital 6 (PWM) 
 *    Pin 5 of dust sensor          -> Ground
 * Datasheet: http://www.samyoungsnc.com/products/3-1%20Specification%20DSM501.pdf
* Contributor: epierre
**/


#define CHILD_ID_DUST_PM10            0
#define CHILD_ID_DUST_PM25            1
#define DUST_SENSOR_DIGITAL_PIN_PM10  6
#define DUST_SENSOR_DIGITAL_PIN_PM25  3

         // variable to store the value coming from the sensor
float valDUSTPM25 =0.0;
float lastDUSTPM25 =0.0;
float valDUSTPM10 =0.0;
float lastDUSTPM10 =0.0;
long lowpulseoccupancy = 0;
float ratio = 0;
long concentrationPM25 = 0;
long concentrationPM10 = 0;
int ppmv;
unsigned long sampletime_ms = 30000;
unsigned long starttime;
unsigned long endtime;
unsigned long duration;
 int temp=20; //external temperature, if you can replace this with a DHT11 or better
 const int COM_BAUD = 9600;
void setup() {
  // put your setup code here, to run once:
 Serial.begin(COM_BAUD);
  pinMode(DUST_SENSOR_DIGITAL_PIN_PM10,INPUT);
  pinMode(DUST_SENSOR_DIGITAL_PIN_PM25,INPUT);
 delay(1000);
}



void loop() {
  // put your main code here, to run repeatedly:
//get PM 2.5 density of particles over 2.5 μm
  Serial.print("PM25: ");
  Serial.println(concentrationPM25);
  Serial.print("\n");
  
  //get PM 1.0 - density of particles over 1 μm.
  
  Serial.print("PM10: ");
  Serial.println(concentrationPM10);
  Serial.print("\n");
  //ppmv=mg/m3 * (0.08205*Tmp)/Molecular_mass
  //0.08205   = Universal gas constant in atm·m3/(kmol·K)
   ppmv=(concentrationPM10*0.0283168/100/1000) *  (0.08205*temp)/0.01;
  Serial.print(ppmv);
 delay(9000);
}




long PM(int DUST_SENSOR_DIGITAL_PIN) {

 starttime = millis();
  while (1) {
  
    duration = pulseIn(DUST_SENSOR_DIGITAL_PIN, LOW);
    lowpulseoccupancy += duration;
    endtime = millis();
       if ((endtime-starttime) > sampletime_ms)
    {
    
 long    ratio = (lowpulseoccupancy-endtime+starttime)/(sampletime_ms*10.0);  // Integer percentage 0=>100
    long concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62; // using spec sheet curve
    Serial.print("lowpulseoccupancy:");
    Serial.print(lowpulseoccupancy);
    Serial.print("\n");
    Serial.print("ratio:");
    Serial.print(ratio);
    Serial.print("\n");
    Serial.print("DSM501A:");
    Serial.println(concentration);
    Serial.print("\n");
    
    lowpulseoccupancy = 0;    
     return(concentration);
    }
}
}
