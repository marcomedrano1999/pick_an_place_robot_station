//Descargar las libreria del ESP32Wifi de algun repo
#include <WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <FS.h>
#include <WebServer.h>

// Al compilar marca "error compilando para la tarjeta Arduino Uno"

//Información de WIFI
const char* ssid = "ssid";
const char* password = "contraseña";

//Datos para un IP estática
IPAddress ip(192,168, 0, 10)
IPAddress gateway(193, 169, 0, 1)
IPAddress subnet(255, 255, 255, 0)

const int Pin_Estado = 2; //Pin donde estará la señal del relé prendido o apagado

String Pin_Estado; //Pin para mandar el estado a la página web

AsyncWebServer server(80); //Puerto TCP

String processor(const String& var) //Función Processor para regresar valores a pagína
{
  if(var == "PIN_ESTADO")
  {
     if(digitalRead(Pin_Estado)==1)// Ver si Pin_Estado si es la variable que se usará
     {
      Pin_Estado = "Encendido";
     }
     else
     {
      Pin_Estado = "Apagado";
     }
     return Pin_Estado;
  }
}

void setup(){
  //Iniciamos comunicación serial y ponemos el pin de rele como salida
  Serial.begin (115200);
  pinMode (Pin_Estado, OUTPUT);

  //Iniciamos el sistema Flash
  if(!SPIFFS.begin()){ //Ver si el SPIFFS no funciona
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

// Conectamos al WiFi Modo estación con dirección IP estática
WiFi.mode(WIFI_STA);
WiFi.config(ip, gateway, subnet);
WiFi.begin(ssid, password);

//Mientras se conecta a la res que se imprima conectando a WiFi

while (WiFi.status() != WL_CONNECTED){
  delay(1000);
  Serial.println("Conectando a WiFi...");
}

//Imprmir IP en el monitor serial
Serial.println(WiFi.localIP());

//A partir de aquí vamos a resolver las solicitudes del cliente
//Cuano ingresen a nuestra página
server.on("/estilos.css", HTTP_GET, [] (AsyncWebServerRequest *request){ //Ver por qué no funciona
  request -> send(SPIFFS, "estilos.css", "text/css");
});
  server.on("/RELE=ON", HTTP_GET, [] (AsyncWebServerRequest *request){ //Ver por qué no funciona
  digitalWrite(Pin_Estado, HIGH);
  request->send(SPIFFS, "/index.html", String(), false, processor);
});

//Cuando pulsemos el botón apagado
server.on("/RELE=OFF", HTTP_GET, [](AsyncWebServerRequest *request){
digitalWrite(Pin_Estado, LOW);
request->(SPIFFS, "/index.html", String(), false, processor);
});

//Iniciamos servidor
server.begin();
}

void loop(){
  //No se usará debido a la libreria del servidor web asíncrono
}
