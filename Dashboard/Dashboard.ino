x//X-sim3dashboard v1  (using TM1638 display and Arduino Nano v3)

// Made by TronicGr (Thanos) 4-26-2012 for X-sim3

// Shared as Public Domain



// Serial parser example: R~a01~~70~S~a02~G~a03~

// Where:

// ~a01~ is 16bit value for rpm

// ~70~ is data value parameter for RPM_MAX divided by 1000 to fit into a byte so actual value is 9500

// ~a02~ is 16bit value for speed

// ~a03~ is 8bit value for gear / neutral / reverse



// You can set the USO pause safely to 10ms for nice fast refresh rates!





#include <TM1638.h> //can be downloaded from http://code.google.com/p/tm1638-library/



// define a module on data pin 5, clock pin 4 and strobe pin 3

TM1638 module1(5, 4, 3);



//dÈfinition des broches du dÈcodeur 7 segments (vous pouvez changer

//les numÈros si bon vous semble)

const int bit_A= 6;

const int bit_B= 7;

const int bit_C= 8;

const int bit_D= 9;

//Definition des Pin pour Affichage Neutral et reverse

const int bit_E= 10;

const int bit_F= 11;



const int ledPin =  13;   // the number of the LED pin

int ledState = LOW;             // ledState used to set the LED

long previousMillis = 0;        // will store last time LED was updated



// the follow variables is a long because the time, measured in miliseconds,

// will quickly become a bigger number than can be stored in an int.

long interval = 40;           // interval at which to blink (milliseconds)



void setup() {

  //Create Serial Object

  Serial.begin(125000); 

  //Serial.begin(115200);

  pinMode(ledPin, OUTPUT);

//Les broches sont toutes des sorties

pinMode(bit_A, OUTPUT);

pinMode(bit_B, OUTPUT);

pinMode(bit_C, OUTPUT);

pinMode(bit_D, OUTPUT);

pinMode(bit_E, OUTPUT);

pinMode(bit_F, OUTPUT);



//Les broches sont toutes mises ‡ l'Ètat bas pour le decodeur BCD et haut pour piloter neutral et reverse

digitalWrite(bit_A, LOW);

digitalWrite(bit_B, LOW);

digitalWrite(bit_C, LOW);

digitalWrite(bit_D, LOW);

digitalWrite(bit_E, HIGH);

digitalWrite(bit_F, HIGH);



// initialize the screen:

module1.clearDisplay();              //clears the display from garbage if any

String name = "TronicGr";           //sets a custom logo start up banner

module1.setDisplayToString(name);    //prints the banner

name = "Thanos";           //sets a custom logo start up banner



module1.setLEDs(0b10000000 | 0b00000001<< 8 );

delay(50);

module1.setLEDs(0b11000000 | 0b00000011<< 8 );

delay(50);

module1.setLEDs(0b11100000 | 0b00000111<< 8 );

delay(50);

module1.setLEDs(0b11110000 | 0b00001111<< 8 );

delay(50);

module1.setLEDs(0b11111000 | 0b00011111<< 8 );

delay(50);

module1.setLEDs(0b11111100 | 0b00111111<< 8 );

delay(50);

module1.setLEDs(0b11111110 | 0b01111111<< 8 );

delay(50);

module1.setLEDs(0b11111111 | 0b11111111<< 8 );

delay(200);

module1.setLEDs(0b00000000 | 0b00000000<< 8 );

delay(50);

module1.setLEDs(0b11111111 | 0b11111111<< 8 );

delay(100);

module1.setLEDs(0b00000000 | 0b00000000<< 8 );

delay(50);

module1.setLEDs(0b11111111 | 0b11111111<< 8 );

delay(100);

module1.setLEDs(0b00000000 | 0b00000000<< 8 );

delay(50);

module1.setLEDs(0b11111111 | 0b11111111<< 8 );

delay(100);

module1.setLEDs(0b00000000 | 0b00000000<< 8 );

delay(1500);                        //small delay 1.5 sec 

module1.clearDisplay();              //clears the 1st display



}



void loop() {

unsigned long currentMillis = millis();

int i; 

char bufferArray[20];              // holds all serial data into a array

unsigned int rpm;                  //holds the rpm data (0-65535 size)

unsigned int rpmleds;              //holds the 8 leds values 

unsigned int rpmmax;               //retrieves from x-sim USO this value as parameter divided by 100

unsigned int carspeed;             //holds the speed data (0-65535 size)

byte gear;                         // holds gear value data

byte d1;                           // high byte temp variable

byte d2;                           // low byte temp variable

byte rpmdata = 0;                  // marker that new data are available

byte speeddata = 0;                // marker that new data are available

byte geardata = 0;                 // marker that new data are available

unsigned int blinkled = 0;

unsigned int centaine;

unsigned int dizaine;

unsigned int unite;



if (Serial.available() >= 9)  {    //if 6 bytes available in the Serial buffer...

       for (i=0; i<9; i++) {			// for each byte

       bufferArray[i] = Serial.read();     	// put into array

      } 

  }



digitalWrite(13, LOW);

delay(10);

digitalWrite(13, HIGH);      

Serial.flush();

 

if (bufferArray[0] == 'R' ){		// if new bytes have been recieved



    d1 = bufferArray[1];		// store high byte of rpm

    d2 = bufferArray[2];		// store low byte of rpm 

    rpm = ((d1<<8) + d2);               // concatonate bytes (shift 8 bits)

    rpmmax = bufferArray[3];            // retrieves the maxrpm value 

    rpmmax = (rpmmax * 100)+2000;       // multiplies the rpm data into thousants

    rpmdata=1;                          // we got new data!

}

if (bufferArray[4] == 'S' ){

    d1 = bufferArray[5];		// store high byte of speed

    d2 = bufferArray[6];		// store low byte of speed

    carspeed = ((d1<<8) + d2);          // concatonate bytes (shift 8 bits)

    centaine = carspeed / 100; 

    dizaine  = (carspeed - (centaine*100)) / 10;

    unite = carspeed - (centaine*100) - (dizaine*10);

    speeddata=1;                        // we got new data!

}

if (bufferArray[7] == 'G' ){

    gear = bufferArray[8];         // retrieves the single byte of gear (0-255 value)

    geardata=1;                    // we got new data!

}  

if (geardata == 1) {

    gear = gear - 127;                  // offset the 0 value in 8-bit 

    if (gear >= 1 and gear <10 ){

        afficher(gear);//appel de la fonction affichage avec envoi du nombre ‡ afficher

    }

    if (gear == 0){//Pour aficher neutral le caractere g de l`afficheur est bas et les autres haut

	digitalWrite(bit_A, HIGH);

	digitalWrite(bit_B, HIGH);

	digitalWrite(bit_C, HIGH);

	digitalWrite(bit_D, HIGH);

	digitalWrite(bit_E, LOW);

	digitalWrite(bit_F, HIGH);

    }

    if (gear == 255){                        // -1 that reprecents reverse rollover to 255 so...

        // displays the character for reverse

	digitalWrite(bit_A, HIGH);

	digitalWrite(bit_B, HIGH);

	digitalWrite(bit_C, HIGH);

	digitalWrite(bit_D, HIGH);

	digitalWrite(bit_E, LOW);

	digitalWrite(bit_F, LOW);

    }

    geardata=0;

}



if (speeddata == 1) {//affichage de la vitesse avec chaque digit pour centaine/dizaine/unite

      module1.clearDisplayDigit(0, false);

      module1.clearDisplayDigit(1, false);

      module1.clearDisplayDigit(2, false);

      module1.setDisplayDigit(centaine, 0, false);

      module1.setDisplayDigit(dizaine, 1, false);

      module1.setDisplayDigit(unite, 2, false);

      delay(10);

      speeddata=0;

}    

if (rpmdata == 1) {

    module1.setDisplayToDecNumber(rpm, 0, false);//displays numerical the speed

    rpmleds = map(rpm,0,rpmmax,0,9);    // distributes the rpm level to the 8 leds + 1 for shift change

    if (rpmleds==0){ 

      module1.setLEDs(0b00000000 | 0b00000000 << 8);

    }

    if (rpmleds==1){

      module1.setLEDs(0b00000000 | 0b00000001 << 8 );

    }

    if (rpmleds==2){

      module1.setLEDs(0b00000000 | 0b00000011 << 8 );

    }

    if (rpmleds==3){

      module1.setLEDs(0b00000000 | 0b00000111 << 8 );

    }

    if (rpmleds==4){

      module1.setLEDs(0b00000000 | 0b00001111 << 8);

    }

    if (rpmleds==5){

      module1.setLEDs(0b00000000 | 0b00011111 << 8);

    }

    if (rpmleds==6){

      module1.setLEDs(0b00100000 | 0b00011111 << 8 );

    }

    if (rpmleds==7){

      module1.setLEDs(0b01100000 | 0b00011111 <<8 );

    }

    if (rpmleds==8){

       //blinkled++;

      //module.setLEDs(0b11100000 | 0b000011111<<8 );}

      //if (blinkled > 100){

      module1.setLEDs(0b00000000 | 0b00000000 << 8);

      //delay(20);

      //}

      //if (blinkled > 200){

      if(currentMillis - previousMillis > interval) {

    // save the last time you blinked the LED 

       previousMillis = currentMillis;

       module1.setLEDs(0b11111111 | 0b11111111 << 8 );

      } 

    rpmdata=0;

      }

} 

}

//fonction Ècrivant sur un seul afficheur

void afficher(char chiffre)

{

  //Les broches sont toutes mises ‡ l'Ètat bas et haut neutral reverse

  digitalWrite(bit_A, LOW);

  digitalWrite(bit_B, LOW);

  digitalWrite(bit_C, LOW);

  digitalWrite(bit_D, LOW);

  digitalWrite(bit_E, HIGH);

  digitalWrite(bit_F, HIGH);

  //on allume les chiffres necessaires

  if(chiffre >= 8)

  {

    digitalWrite(bit_D, HIGH);

    chiffre= chiffre - 8;

    }

  if(chiffre>= 4)

  {

    digitalWrite(bit_C, HIGH);

  chiffre= chiffre - 4;

  }

  if(chiffre >= 2)

  {

    digitalWrite(bit_B, HIGH);

    chiffre= chiffre - 2;

  }

  if(chiffre >= 1)

  {

    digitalWrite(bit_A, HIGH);

    chiffre= chiffre - 1;

  }

}






