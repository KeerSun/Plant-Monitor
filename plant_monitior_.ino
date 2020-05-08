

//www.elegoo.com
//2018.10.25

#include <LED_Font_1.h>
#include <dht_nonblocking.h>
#define DHT_SENSOR_TYPE DHT_TYPE_11
#include <LiquidCrystal.h>


static const int DHT_SENSOR_PIN = 2;
DHT_nonblocking dht_sensor( DHT_SENSOR_PIN, DHT_SENSOR_TYPE );
int lightPin = A0;
int sensorValue = 0;
int val = 0; //value for storing moisture value 
int soilPin = A1;
int soilPower = 3;
LiquidCrystal lcd(6, 7, 11, 10 , 9, 8);


/*
 * Initialize the serial port.
 */
void setup( )
{
  Serial.begin( 9600);
  pinMode(lightPin, INPUT); 
  pinMode(soilPower, OUTPUT);//Set D3 as an OUTPUT
  digitalWrite(soilPower, LOW);//Set to LOW so no power is flowing through the sensor
  lcd.begin(16, 2);

}



/*
 * Poll for a measurement, keeping the state machine alive.  Returns
 * true if a measurement is available.
 */
static bool measure_environment( float *temperature, float *humidity )
{
  static unsigned long measurement_timestamp = millis( );

  /* Measure once every four seconds. */
  if( millis( ) - measurement_timestamp > 3000ul )
  {
    if( dht_sensor.measure( temperature, humidity ))
    {
      measurement_timestamp = millis( );
      return( true );
    }
  }

  return( false );
}

bool isHappy (float temperature, float moisture) {
  if (temperature > 15 and temperature < 32 and moisture < 90 and moisture > 30) {
    return(true);
  } else {
    return(false);
  }
}


int readSoil(){

    digitalWrite(soilPower, HIGH);//turn D7 "On"
    delay(10);//wait 10 milliseconds 
    val = analogRead(soilPin);//Read the SIG value form sensor 
    digitalWrite(soilPower, LOW);//turn D7 "Off"
    return val;//send current moisture value
}

String dTimeNtime(int brightness){
  if (brightness > 800){
    return "good!" ;
  } else {
    return "poor " ;
  }
}



/*
 * Main program loop.
 */
void loop( )
{
  float temperature;
  float humidity;
  int brightness = analogRead(lightPin);
  String sunWhere = dTimeNtime(brightness);
  int mappedMoisture = map(readSoil(), 0, 900, 0, 100);

  /* Measure temperature and humidity.  If the functions returns
     true, then a measurement is available. */
  if( measure_environment( &temperature, &humidity) == true )
  {

    if (isHappy(temperature,mappedMoisture)){
      lcd.setCursor(0, 0);
      lcd.print(" all is well");
      lcd.setCursor(0, 1);
      lcd.print(" all is well");
      delay(1000);
    } else {

      auto moisture = "SM = " + String(mappedMoisture) +"%      ";
      auto brightness = "Br = " + sunWhere + "        ";
      auto temp = "Temp = " + String(int(temperature)) + " deg. C      " ;
      auto humid = "Hum = " + String(int(humidity)) + "%        ";

      
      delay(1000);
      lcd.setCursor(0, 0);
      lcd.print(moisture);
      lcd.setCursor(0, 1);
      lcd.print(temp);
      delay(2000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(brightness);
      lcd.setCursor(0, 1);
      lcd.print(humid);

    }


  }
}
