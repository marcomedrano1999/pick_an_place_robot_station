/*
Robot Programming interface  

Author: Marco Medrano
Date: 27/03/22
*/

#include <WiFi.h>
#include <Servo.h>


// Number of servos to use
static const int NUM_SERVOS=2;

// Create array of servos
Servo servo[NUM_SERVOS];

// Create array of pins to attach servos
static const int servoPin[] = {13,12};

// Network credentials
const char* ssid = "Totalplay-2021_2.4Gnormal";
const char* password = "Totalplay11";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Decode HTTP GET value
String valueString[] = {String(5), String(5)};
String jointValueString = String(5);
int pos1 = 0;
int pos2 = 0;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);

  for(int n=0; n < NUM_SERVOS; n++){
    servo[n].attach(servoPin[n]);  // attaches the servo on the servoPin to the servo object
  }

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>body { text-align: center; font-family: \"Trebuchet MS\", Arial; margin-left:auto; margin-right:auto;}");
            client.println(".slider { width: 300px; }</style>");
            client.println("<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js\"></script>");
                     
            // Web Page
            client.println("</head><body><h1>ESP32 with Servo</h1>");
            client.println("<p>Position: <span id=\"servo1Pos\"></span></p>");          
            client.println("<input type=\"range\" min=\"0\" max=\"180\" class=\"slider\" id=\"servo1Slider\" onchange=\"servo(this.value,1)\" value=\""+valueString[0]+"\"/>");
            
            client.println("<script>var slider1 = document.getElementById(\"servo1Slider\");");
            client.println("var servo1P = document.getElementById(\"servo1Pos\"); servo1P.innerHTML = slider1.value;");
            client.println("slider1.oninput = function() { slider1.value = this.value; servo1P.innerHTML = this.value; }");
            client.println("$.ajaxSetup({timeout:1000}); function servo(pos,joint) { ");
            client.println("$.get(\"/?joint=\"+joint+\"&value=\" + pos + \"&\"); {Connection: close};}</script>");

            client.println("<p>Position: <span id=\"servo2Pos\"></span></p>");          
            client.println("<input type=\"range\" min=\"0\" max=\"180\" class=\"slider\" id=\"servo2Slider\" onchange=\"servo(this.value,2)\" value=\""+valueString[1]+"\"/>");
            
            client.println("<script>var slider2 = document.getElementById(\"servo2Slider\");");
            client.println("var servo2P = document.getElementById(\"servo2Pos\"); servo2P.innerHTML = slider2.value;");
            client.println("slider2.oninput = function() { slider2.value = this.value; servo2P.innerHTML = this.value; }");
            client.println("$.ajaxSetup({timeout:1000}); function servo(pos,joint) { ");
            client.println("$.get(\"/?joint=\"+joint+\"&value=\" + pos + \"&\"); {Connection: close};}</script>");
           
            client.println("</body></html>");     
            
            //GET /?value=180& HTTP/1.1
            if(header.indexOf("GET /?joint=")>=0) {

              // Get joint number
              pos1 = header.indexOf('=');
              pos2 = header.indexOf('&');
              jointValueString = header.substring(pos1+1, pos2);
              int jointNum = jointValueString.toInt()-1;
    
              Serial.print("Servo: ");
              Serial.print(jointValueString);
          
              // Get pos
              pos1 = header.indexOf('=',pos2+1);
              pos2 = header.indexOf('&',pos2+1);
              valueString[jointNum] = header.substring(pos1+1, pos2);
              
              //Rotate the servo
              servo[jointNum].write(valueString[jointNum].toInt());
              Serial.print("  pos: ");
              Serial.println(valueString[jointNum]); 
            }         
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
