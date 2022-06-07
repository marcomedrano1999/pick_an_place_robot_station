/*
Web server and robot controller scketch

Author: Michelle Andrade
Modified by: Marco Medrano
Date: 23/05/2022
*/

// Libraries should be installed following the instructions in the repository
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#include <Servo.h>

class jointVector{
    private:
        int joints[5]={0,0,0,0,0};
    
    public:
    jointVector(int* jointsVec){
        for(int i=0;i<5;i++)
            this->joints[i] = jointsVec[i];
    }
    jointVector(){ }

    jointVector(int a1, int a2, int a3, int a4, int a5){
        this->joints[0]=a1;
        this->joints[1]=a2;
        this->joints[2]=a3;
        this->joints[3]=a4;
        this->joints[4]=a5;
    }
    
    int* getJoints(){
        return joints;
    }

    jointVector operator+(jointVector x){
        int* otherJoints = x.getJoints();
        int* thisJoints = this->getJoints();
        int jointRes[]={thisJoints[0]+otherJoints[0],
                        thisJoints[1]+otherJoints[1],
                        thisJoints[2]+otherJoints[2],
                        thisJoints[3]+otherJoints[3],
                        thisJoints[4]+otherJoints[4]};
        return jointVector(jointRes);
    }

    bool operator==(jointVector x){
        int* otherJoints = x.getJoints();
        int* thisJoints = this->getJoints();
        return thisJoints[0]==otherJoints[0] && thisJoints[1]==otherJoints[1] &&
                 thisJoints[2]==otherJoints[2] && thisJoints[3]==otherJoints[3] &&
                 thisJoints[4]==otherJoints[4]; 
    }

    jointVector operator-(jointVector x){
        int* otherJoints = x.getJoints();
        int* thisJoints = this->getJoints();
        int jointRes[]={thisJoints[0]-otherJoints[0],
                        thisJoints[1]-otherJoints[1],
                        thisJoints[2]-otherJoints[2],
                        thisJoints[3]-otherJoints[3],
                        thisJoints[4]-otherJoints[4]};
        return jointVector(jointRes);
    }

    jointVector& operator=(jointVector x){
        if(this==&x)
          return *this;
        
        int* otherJoints = x.getJoints();
        
        for(int i=0;i<5;i++)
          this->joints[i]=otherJoints[i];

        return *this;
    }

    jointVector operator*(int a){
        int* thisJoints = this->getJoints();
        int newJoints[]={0,0,0,0,0};
        for(int i=0;i<5;i++)
            newJoints[i] = a*thisJoints[i];
        
        return jointVector(newJoints);
    }

    jointVector operator/(int a){
        int* thisJoints = this->getJoints();
        int newJoints[]={0,0,0,0,0};
        for(int i=0;i<5;i++)
            newJoints[i] = thisJoints[i]/a;
        
        return jointVector(newJoints);
    }

    float norm(){
      float sum=0;
      for(int i=0;i<5;i++)
        sum += pow(this->joints[i],2);
      if(sum!=0);
        return sqrt(sum);
      return 0;
    }
};




// Wifi data 
const char* ssid = "";     // Introduce network name
const char* password = ""; // Introduce password

// Set static IP
IPAddress ip(192,168, 0, 10);
IPAddress gateway(193, 169, 0, 1);
IPAddress subnet(255, 255, 255, 0);

AsyncWebServer server(80); //Puerto TCP


// Variables to control the robot
bool IN_OPERATION = false;                                         // Start flag
static const int NUM_SERVOS=6;                                     // Number of servos to use
Servo servo[NUM_SERVOS];                                           // Create array of servos
static const int servoPin[] = {13,12,14,27,26,25};                 // Create array of pins to attach servos
const int objectDetectedPin1 = 34;                                 // Flag for object presence in conveyor belt
const int objectDetectedPin2 = 35;                                 // Flag for object presence in conveyor belt

const int PinEstado = 2; //Pin donde estará la señal del relé prendido o apagado
String Pin_Estado; //Pin para mandar el estado a la página web

int num_rojo=0;
int num_azul=0;
int num_verde=0;

String processor(const String& var){
  Serial.println(var);

  if( var == "PIN_ESTADO"){
    if(digitalRead(PinEstado))
    Pin_Estado = "Encendido";
   else
    Pin_Estado = "Apagado";
   return Pin_Estado;
  }
  else if(var=="Angulo_Del_Brazo")
    return "90°";
  else if(var=="Cantidad_Rojo")
    return String(num_rojo);
  else if(var=="Cantidad_Azul")
    return String(num_azul);
  else if(var=="Cantidad_Verde")
    return String(num_verde);
  return String();
}



jointVector interpolation(jointVector& init_joint, jointVector& end_joint,int num_steps,int step){
    return init_joint + (end_joint-init_joint)*(step-1)/(num_steps-1);
}

void moveServos(int* jointAngles){
  // move servos
  for(int n=0; n < NUM_SERVOS-1; n++){
    servo[n].write(jointAngles[n]);
  }
}



// Set robot velocity
float velocity=10;

// Declare fix joint positions  ****** POSITIONS NEED TO BE SET UP *****
jointVector homePosition = jointVector(90,48,25,150,0);         // Robot home position
jointVector conveyorBelt = jointVector(99,65,18,146,0);         // Gripper up to the conveyor belt
jointVector redCan = jointVector(6,56,41,152,0);               // Gripper up to the red can
jointVector blueCan = jointVector(27,56,37,152,0);              // Gripper up to the blue can
jointVector greenCan = jointVector(20,60,30,137,0);             // Gripper up to the green can
jointVector currentPosition = jointVector(90,48,25,150,0);      // Current position

void gripper(bool action){
  /*
  Function to control the gripper.
  If the input is true, the gripper will
  close, and it will open otherwise
  The gripper is in the sixth element of
  the servo array.
  */

  if(action){
    // Close gripper
    servo[5].write(163);
  }
  else{
    // Open it otherwise
    servo[5].write(120);
  }

  delay(500);
  return;
}

void moveRobot(jointVector& init_joint, jointVector& end_joint, float velocity){
  // Get distance between points
  float distance=(end_joint-init_joint).norm();
  Serial.print("Distance: ");
  Serial.println(distance);
  // Compute steps
  int steps = int(distance/velocity);
  Serial.print("Steps: ");
  Serial.println(steps);

  if(steps==1){
    moveServos(end_joint.getJoints());  
    delay(100);
  }
  else{
    for(int i = 1; i <= steps; i++){
          currentPosition = interpolation(init_joint,end_joint,steps,i);
          moveServos(currentPosition.getJoints());
          delay(100);
    }
  }
  Serial.println("Finish moving");
}

void setup(){
  // Start serial communication
  Serial.begin (115200);
  pinMode (PinEstado, OUTPUT);

  pinMode(objectDetectedPin1, INPUT);
  pinMode(objectDetectedPin2, INPUT);


  // Start flash system
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Conectamos al WiFi Modo estación con dirección IP estática
  //WiFi.mode(WIFI_STA);
  //WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, password);
  
  //Mient
  while (WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.println("Conecting to WiFi...");
  }

  // Print IP in serial monitor
  Serial.println(WiFi.localIP());
  
  // Requests from clients. Each one defines a 
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  server.on("/style.css", HTTP_GET, [] (AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });

  // Set start flag to true
  server.on("/ENCENDIDO", HTTP_GET, [] (AsyncWebServerRequest *request){
    IN_OPERATION = true;
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  // Stop process
  server.on("/APAGADO", HTTP_GET, [](AsyncWebServerRequest *request){
    IN_OPERATION = false;
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  // Emergency button stops the robot
  server.on("/EMERGENCIA", HTTP_GET, [](AsyncWebServerRequest *request){
    IN_OPERATION = false;
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  // Emergency button stops the robot
  server.on("/RETURN", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  // In free movement, display angles to modify them
  server.on("/ANGULO", HTTP_GET, [](AsyncWebServerRequest *request){
    
    request->send(SPIFFS, "/servoController.html", String(), false, processor);
  }); 

  // Start the server
  server.begin();

  // Initialize servos
  for(int n=0; n < NUM_SERVOS; n++){
    servo[n].attach(servoPin[n]);  // attaches the servo on the servoPin to the servo object
  }

  //  Start robot from home position
  moveServos(homePosition.getJoints());

}

void loop(){

  /*
  Code to control the robot
  
  The robot will wait until the conveyor belt's controller send the data about the
   presence and type of the object. Then, the robot will take it and place it in
   the color's container. Finally, it will return to its home position

   The presence and  type of the object are coded; when both inputs are set to 0,
   there is no object. If objectDetectionPin1=0 and objectDetectionPin2=1, the 
   object is red. If objectDetectionPin1=1 and objectDetectionPin2=0, the object
   is green. Finally, if objectDetectionPin1=1 and objectDetectionPin2=1, the 
   object is blue. 
  */
  // Execute only if the operator pushed the start button
  if(IN_OPERATION){

    Serial.println("En operacion...");
    int pin_1 = digitalRead(objectDetectedPin1);
    int pin_2 = digitalRead(objectDetectedPin2);
    if(pin_1 or pin_2){

      servo[5].write(120);

      Serial.println("Mover a banda");
      // Move robot to take object
      moveRobot(homePosition,conveyorBelt,velocity);

      delay(1000);

      Serial.println("Cerrar pinza");
      // Take object
      servo[5].write(163);

      delay(1000);

      Serial.println("Mover a home");
      // Go back to home position
      moveRobot(conveyorBelt,homePosition,velocity);

      delay(1000);

      // Red object
      if(pin_1==0 and pin_2){
        Serial.println("Mover a rojo");
        moveRobot(homePosition,redCan,velocity);

        num_rojo++;

        delay(1000);
        // drop object
        Serial.println("Soltar objeto");
        servo[5].write(120);

        delay(1000);
        
        Serial.println("Mover a home");
        moveRobot(redCan,homePosition,velocity);
      }
      // Green object
      else if(pin_1 and pin_2==0){
        Serial.println("Mover a verde");
        moveRobot(homePosition,greenCan,velocity);
        num_verde++;

        delay(1000);
        
        // drop object
        Serial.println("Soltar objeto");
        servo[5].write(120);

        delay(1000);
        Serial.println("Mover a home");
        moveRobot(greenCan,homePosition,velocity);
      }
      // Blue object
      else{
        Serial.println("Mover a azul");
        moveRobot(homePosition,blueCan,velocity);

        num_azul++;

        delay(1000);
        // drop object
        Serial.println("Soltar objeto");
        servo[5].write(120);

        delay(1000);
        Serial.println("Mover a home");
        moveRobot(blueCan,homePosition,velocity);
      }
      delay(2000);
    }
    
  }

  
}
