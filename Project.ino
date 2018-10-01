#include<LiquidCrystal.h>

#define Rain_Sensor A2
#define motor 7
#define soil_sensor A0  // Analog input pin that the soil moisture sensor is attached to

LiquidCrystal lcd(0,1,8,9,10,11); // Creates an LC object. Parameters: (rs, enable, d4, d5, d6, d7) 

String state = "Rajasthan";
String crop = "Wheat";
float Kc = 0.25;
float ETo = 4;
float land_area = 0.065;   //In meter square
float soil_volume = 2.925  ; //volume of soil L
float minMoist = 0/*35*/,maxMoist = 50;
float motorTime = .446;    //To run for increase in soil moisture by 1% in 1 L land(Vol of Soil)
float volMotor = 1/60;     //Volume Of Water Coming Out Per Sec in L
float ET = Kc*ETo*0.001;  //It is in m/day
float ET_litres = ET*land_area*1000;    //L/day
float volPerPerc = volMotor*motorTime*soil_volume;   //Volume Req to increase 1% of moisture in total volume   
float moistValue;
int date,month,year;
int i=1;
void setup() 
{ 
     lcd.begin(16,2); // Initializes the interface to the LCD screen, and specifies the dimensions (width and height) of the display } 
     lcd.print("Water Irrigation");
     lcd.setCursor(4,1);
     lcd.print("Welcomes You");
     pinMode(motor, OUTPUT);
     delay(5000);
     lcd.clear();
     lcd.print("System Ready");
     delay(1000);
}
void loop() 
{
      moistValue = 100 - (analogRead(soil_sensor)/1023.00 * 100);
     //Check For Rain
     if(analogRead(Rain_Sensor)>100)        
     {
           lcd.clear();
           lcd.print("Raining...");
           while(analogRead(Rain_Sensor)>100);
           lcd.clear();
           lcd.print("Rain Stopped");           
           int currMoist = 100 - (analogRead(soil_sensor)/1023.00 * 100);
           lcd.clear();
           lcd.print("Rain Water: ");
           float rainWater = (currMoist - moistValue)*volPerPerc;
           lcd.print(rainWater);
           lcd.print("L");
           moistValue = currMoist;
           ET_litres = ET_litres - rainWater;
           lcd.setCursor(0,1);
           lcd.print("Remain Water: ");
           lcd.print(ET_litres);
           lcd.print("L");
     }
     //Rain Ends Here
     
     //If Crops Need Water
     if(moistValue < minMoist)
     {
          lcd.clear();
          lcd.print("Motor Started");
          lcd.setCursor(0,1);
          lcd.print("Avail.: ");
          lcd.print(ET_litres);
          lcd.print("L");
          digitalWrite(motor,HIGH);
          int run_time = motorTime*soil_volume*(maxMoist-moistValue)*1000;
          delay(run_time);
          digitalWrite(motor,LOW);
          lcd.clear();
          lcd.print("Motor Stopped");
          lcd.setCursor(0,1);
          lcd.print("Wait..");
          while(moistValue < minMoist)
          {
              moistValue = 100 - (analogRead(soil_sensor)/1023.00 * 100);    
          }
          ET_litres = ET_litres - (volMotor*run_time);
          lcd.clear();
          lcd.print("Done");
          lcd.setCursor(0,1);
          lcd.print("Given: ");
          lcd.print(ET_litres);
          lcd.print("L");
     }
     //Water Need Fullfilled
}
