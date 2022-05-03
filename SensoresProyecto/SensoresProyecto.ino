
#define LED (13u)

#include <Arduino_APDS9960.h>



void setup() {
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
  
  if (!APDS.begin()) {
    Serial.println("Error initializing APDS-9960 sensor!");
  }
}




void loop() {
  // check if a proximity reading is available
  if (APDS.proximityAvailable()) {
    // read the proximity
    // - 0   => close
    // - 255 => far
    // - -1  => error

    
    int proximity = APDS.readProximity();
    if (proximity < 50){
    
      digitalWrite(LED,HIGH);
      
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
  digitalWrite(4,LOW);
  }

  //Verde pin 3
  else if (g>r && g>b){
  Serial.println("Caja verde detectatada");
  digitalWrite(2,LOW);
  digitalWrite(3,HIGH);
  digitalWrite(4,LOW);
  }

  //Azul pin 4
  else if (b>g && b>r){
  Serial.println("Caja azul detectatada");
  digitalWrite(2,LOW);
  digitalWrite(3,LOW);
  digitalWrite(4,HIGH);
  }

  else {
  Serial.println("No se detecta ningun color");
  }


  
    }
    else{
      digitalWrite(LED, LOW);}

    // print value to the Serial Monitor
    Serial.println(proximity);
  
  }
  // wait a bit before reading again
  delay(100);
}
