//*-- IoT Information
#define SSID "UPC1889041"  //TODO: wifi name 
#define PASS "CFHXAJYD"   //TODO: wifi password
String GET = "GET /update?key=8OC2G029OG74M1V9"; //TODO: 8OC2G029OG74M1V9 change to https://thingspeak.com api write key

#define IP "184.106.153.149" // ThingSpeak IP Address: 184.106.153.149


// https://thingspeak.com/channels/189974
// GET /update?key=[THINGSPEAK_KEY]&field1=[data 1]&field2=[data 2]...;
// String GET = "GET /update?key=THINGSPEAK_KEY";

#include <SoftwareSerial.h>
int sensor_0 = A0;
int value_0;
int sensor_1 = A1;
int value_1;
int sensor_2 = A2;
int value_2;


//#define Serial FALSE //comment out to remove Serial msgs

//*-- Hardware SerialESP
#define _baudrate 115200

//*-- Software SerialESP
//
#define _rxpin      2
#define _txpin      3
SoftwareSerial SerialESP( _rxpin, _txpin ); // RX, TX


void setup() {
  SerialESP.begin( _baudrate );
  Serial.begin( _baudrate );

  sendSerial("AT");
  delay(5000);
  if(SerialESP.find("OK"))
  {
    Serial.println("RECEIVED: OK\nData ready to sent!");
    connectWiFi();
  }

}

void loop() {
  value_0 = analogRead(sensor_0);
  value_1 = analogRead(sensor_1);
  value_2 = analogRead(sensor_2);
  String s_value_0 =String(value_0);// turn integer to string
  String s_value_1= String(value_1);// turn integer to string
  String s_value_2=String(value_2);// turn integer to string
  updateTS(s_value_0,s_value_1, s_value_2);
  delay(3000); //
}
//----- update the  Thingspeak string with 3 values
void updateTS( String s0, String s1 , String s2)
{
  // ESP8266 Client
  String cmd = "AT+CIPSTART=\"TCP\",\"";// Setup TCP connection
  cmd += IP;
  cmd += "\",80";
  sendSerial(cmd);
  delay(2000);
  if( SerialESP.find( "Error" ) )
  {
    Serial.print( "RECEIVED: Error\nExit1" );
    return;
  }

  cmd = GET + "&field1=" + s0 +"&field2="+ s1 + "&field3=" + s2 +"\r\n";
  SerialESP.print( "AT+CIPSEND=" );
  SerialESP.println( cmd.length() );
  if(SerialESP.find( ">" ) )
  {
    Serial.print(">");
    Serial.print(cmd);
    SerialESP.print(cmd);
  }
  else
  {
    sendSerial( "AT+CIPCLOSE" );//close TCP connection
  }
  if( SerialESP.find("OK") )
  {
    Serial.println( "RECEIVED: OK" );
  }
  else
  {
    Serial.println( "RECEIVED: Error\nExit2" );
  }
}

void sendSerial(String cmd)
{
  Serial.print("SEND: ");
  Serial.println(cmd);
  SerialESP.println(cmd);
}

boolean connectWiFi()
{
  SerialESP.println("AT+CWMODE=1");//WiFi STA mode - if '3' it is both client and AP
  delay(2000);
  //Connect to Router with AT+CWJAP="SSID","Password";
  // Check if connected with AT+CWJAP?
  String cmd="AT+CWJAP=\""; // Join accespoint
  cmd+=SSID;
  cmd+="\",\"";
  cmd+=PASS;
  cmd+="\"";
  sendSerial(cmd);
  delay(5000);
  if(SerialESP.find("OK"))
  {
    Serial.println("RECEIVED: OK");
    return true;
  }
  else
  {
    Serial.println("RECEIVED: Error");
    return false;
  }

  cmd = "AT+CIPMUX=0";// Set Single connection
  sendSerial( cmd );
  if( SerialESP.find( "Error") )
  {
    Serial.print( "RECEIVED: Error" );
    return false;
  }
}

