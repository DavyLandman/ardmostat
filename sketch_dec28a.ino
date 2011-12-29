#include <math.h>
#include <EtherCard.h>
static byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x32 };
static byte tempserverip[] = { 192, 168, 1, 12 };
char servername[] PROGMEM = "server-download2";


double Thermister(int RawADC) {
  double Temp;
  Temp = log(((10240000/RawADC) - 10000));
  Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp ))* Temp );
  Temp = Temp - 273.15;            // Convert Kelvin to Celcius
  return Temp;
}


byte Ethernet::buffer[700];

void setup() {
  Serial.begin(57600);
  Serial.println("\n[Starting temp logger]");
  
  if (ether.begin(sizeof Ethernet::buffer, mymac) == 0) 
    Serial.println( "Failed to access Ethernet controller");

  if (!ether.dhcpSetup())
    Serial.println("DHCP failed");
    
  ether.printIp("IP: ", ether.myip);
   if (!ether.dnsLookup(servername))
    Serial.println("DNS failed");
  ether.printIp("Server: ", ether.hisip);
  ether.hisport = 5555;

}


static word fillTemperatureData(byte fd) {
  Serial.println("got fill request");
  BufferFiller bfill = ether.tcpOffset();
  double temp = Thermister(analogRead(0));
  bfill.emit_raw(reinterpret_cast<char*>(&temp), sizeof temp); 
  return bfill.position();
}

static int done = 1;
static byte resultFromTemperatureSend(byte fd, byte statuscode, word datapos, word len_of_data) {
 Serial.println("got result request: ");
 Serial.println((int) statuscode);
  Serial.println((int) len_of_data);
  Serial.println((int)(((byte*) Ethernet::buffer + datapos)[1]));
 done = 1;
  return 0; 
}

void loop() {
  if (ether.dhcpExpired() && !ether.dhcpSetup())
    Serial.println("DHCP failed");  
  word len = ether.packetReceive();
  word pos = ether.packetLoop(len);
  // check if valid tcp data is received
 /* if (pos) {  
    Serial.println("got data:" + len);
     BufferFiller bfill = ether.tcpOffset();
     char* data = (char *) Ethernet::buffer + pos; 
     if (*data == 0x11) {
      Serial.println("correct welcome");     
       // correct welcome
       ether.clientTcpReq(resultFromTemperatureSend, fillTemperatureData, 5050); 
     }
  } else {
      Serial.println("got no data:");
  }*/

//  Serial.println(int(Thermister(analogRead(0))));  // display Fahrenheit
  if ((millis() % 30000) == 0) {
//      delay(5000);
//      done = 0;
      ether.clientTcpReq(resultFromTemperatureSend, fillTemperatureData, 5555); 
      delay(1);
  }
}
