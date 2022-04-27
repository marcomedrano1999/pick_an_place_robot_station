//Descargar las libreria del ESP32Wifi de algun repo
#include <ESP32Wifi.h>

//Información de WIFI
const char *ssid = "nombre";
const char *password = "password";

//Datos para un IP estática
IPAddress ip(192,168,0,10);
IPAddress gateway(192,168,0,1);
IPAddress subnet(255,255,255,0);

WiFiServer server(80);

Serial.println();
Serial.println();
Serial.print("Conectandose a ");  //Que nos vamos a conectar  a la red
Serial.println(nombre);

WiFi.mode(WIFI_STA); //No se por que no agarra
WiFi.config(ip, gateway, subnet);
WiFi.begin(nombre, password); 

while (WiFi.status () != WL_CONNECTED){
  dalay(500);
  Serial.print(".");
}

Serial.println("");
Serial.println("WiFi Conectado");

server.begin();
Serial.println("Servidor iniciado");

Serial.print("Usa esta URL para comunicarte al ESP: ");
Serial.print("http://");
Serial.print(WiFi.localIP());
Serial.println("/");

void loop() {
  //El servidor espera a que se conecte algun operador
  WiFiClient client = server.available();
  if (!client){
    return;
  }
  
}
