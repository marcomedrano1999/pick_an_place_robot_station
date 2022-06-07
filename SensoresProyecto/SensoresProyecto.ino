#define LED (13u)

#include <Arduino_APDS9960.h>
#include <mbed.h>
#include <rtos.h>
#include <Servo.h>

using namespace rtos;

Servo myservo;  // create servo object to control a servo
bool avanzar=true;
Thread dispen;

void dispensador(){
  while(true){
    if(avanzar){
      myservo.write(180);                  // sets the servo position according to the scaled value
      delay(1000);     
      myservo.write(0);                  // sets the servo position according to the scaled value
      delay(1000);   // waits for the servo to get there
    }
    else{
     delay(250); 
    }
  }
}



void setup() {
  myservo.attach(7);  // attaches the servo on pin 9 to the servo object
  Serial.begin(9600);
  while (!Serial);

  // declarar pin como salida
  pinMode(LED,OUTPUT);
  digitalWrite(LED, LOW);

  pinMode(2,OUTPUT);
  //digitalWrite(2, LOW);
  pinMode(3,OUTPUT);
  //digitalWrite(3, LOW);
  pinMode(4,OUTPUT);
  //digitalWrite(4, LOW);
  pinMode(5,OUTPUT);
  //digitalWrite(5, LOW);
  pinMode(6,OUTPUT);
  //digitalWrite(5, LOW);
  
  if (!APDS.begin()) {
    Serial.println("Error initializing APDS-9960 sensor!");
  }

  // Iniciar hilo
  dispen.start(mbed::callback(dispensador));
}




void loop() {
  
  // check if a proximity reading is available
  if (APDS.proximityAvailable()) {
    // read the proximity
    // - 0   => close
    // - 255 => far
    // - -1  => error

    
    int proximity = APDS.readProximity();
    if (proximity < 70){
      delay(90);
      // Prendemos led
      digitalWrite(LED,HIGH);
      // Paramos la banda
      digitalWrite(4,HIGH);
      // Prendemos leds
      digitalWrite(5,HIGH);
      //Detiene el dispensador
      digitalWrite(6,HIGH);

      // variable para detener dispensador
      avanzar = false;
      
      // check if a color reading is available
      while (! APDS.colorAvailable()) {
        delay(5);
      }
      int r, g, b;
      
      // read the color
      APDS.readColor(r, g, b);
      
      // print the values
      Serial.print("r = ");
      Serial.println(r);
      Serial.print("g = ");
      Serial.println(g);
      Serial.print("b = ");
      Serial.println(b);
      Serial.println();
      
      // wait a bit before reading again
      delay(500);
      
      //Rojo pin 2
      if (r>g && r>b){
        Serial.println("Caja roja detectatada");
        digitalWrite(2,HIGH);
        digitalWrite(3,LOW);
      }
      
      //Verde pin 3
      else if (g>r && g>b){
        Serial.println("Caja verde detectatada");
        digitalWrite(2,LOW);
        digitalWrite(3,HIGH);
      }
      
      //Azul pin 4
      else if (b>g && b>r){
        Serial.println("Caja azul detectatada");
        digitalWrite(2,HIGH);
        digitalWrite(3,HIGH);
      }
      
      // Marca objeto no detectado
      
      
      //Serial.println("No se detecta ningun color");
      
      // Apaga leds
      
    }
  else{
      // variable para avanzar el dispensador
      avanzar = true;
      
      digitalWrite(2,LOW);
      digitalWrite(3,LOW);
      digitalWrite(LED, LOW);
      digitalWrite(4,LOW);
      digitalWrite(5,LOW);
      digitalWrite(6,LOW);
      }

    // print value to the Serial Monitor
    Serial.println(proximity);
  
  }
  // wait a bit before reading again
  
}
