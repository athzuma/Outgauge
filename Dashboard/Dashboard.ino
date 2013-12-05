#include <Servo.h> 


int ON = 1;
int OFF = 0;
int cont;
int marcha;

int VMotorRPM = 11;
int VMotorVELO = 10;

int linha1 = 2;
int linha2 = 3;
int linha3 = 4;
int coluna1 = 5;
int coluna2 = 6;
int coluna3 = 7;

Servo motorRPM;
Servo motorVELO;

pinMode(VMotorRPM, OUTPUT);
pinMode(VMotorVELO, OUTPUT);

motorRPM.attach(VMotorRPM);
motorVELO.attach(VMotorVELO);

//motorRPM.write(GRAUS);


int ledState = LOW;             // ledState used to set the LED

long previousMillis = 0;        // will store last time LED was updated



// the follow variables is a long because the time, measured in miliseconds,

// will quickly become a bigger number than can be stored in an int.

long interval = 40;           // interval at which to blink (milliseconds)



void setup() {
  //Serial.begin(125000); 
  Serial.begin(115200);

  barraLED(1, ON);
  barraLED(8, ON);

  delay(50);

  barraLED(2, ON);
  barraLED(7, ON);

  delay(50);

  barraLED(3, ON);
  barraLED(6, ON);

  delay(50);

  barraLED(4, ON);
  barraLED(5, ON);

  delay(50);
  for (cont=1;cont<=8;cont++){
    barraLED(cont, OFF);
  }
  delay(200);
  for (cont=1;cont<=8;cont++){
    barraLED(cont, ON);
  }
  delay(200);
  for (cont=1;cont<=8;cont++){
    barraLED(cont, OFF);
  }
  delay(200);
  for (cont=1;cont<=8;cont++){
    barraLED(cont, ON);
  }
  delay(200);
  for (cont=1;cont<=8;cont++){
    barraLED(cont, OFF);
  }
  delay(200);
  for (cont=1;cont<=8;cont++){
    barraLED(cont, ON);
  }
  delay(200);
  for (cont=1;cont<=8;cont++){
    barraLED(cont, OFF);
  }
  delay(1500);
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

  if (Serial.available() >= 9)  
  {    //if 6 bytes available in the Serial buffer...
    for (i=0; i<9; i++) 
    {			// for each byte
      bufferArray[i] = Serial.read();     	// put into array
    } 
  }

  digitalWrite(13, LOW);
  delay(10);
  digitalWrite(13, HIGH);      

  Serial.flush();

  if (bufferArray[0] == 'R' )
  {		// if new bytes have been recieved
    d1 = bufferArray[1];		// store high byte of rpm
    d2 = bufferArray[2];		// store low byte of rpm 
    rpm = ((d1<<8) + d2);               // concatonate bytes (shift 8 bits)
    rpmmax = bufferArray[3];            // retrieves the maxrpm value 
    rpmmax = (rpmmax * 100)+2000;       // multiplies the rpm data into thousants
    rpmdata=1;                          // we got new data!
  }

  if (bufferArray[4] == 'S' )
  {
    d1 = bufferArray[5];		// store high byte of speed
    d2 = bufferArray[6];		// store low byte of speed
    carspeed = ((d1<<8) + d2);          // concatonate bytes (shift 8 bits)
    speeddata=1;                        // we got new data!
  }

  if (bufferArray[7] == 'G' )
  {
    gear = bufferArray[8];         // retrieves the single byte of gear (0-255 value)
    geardata=1;                    // we got new data!
  }  

  if (geardata == 1)
  {
    if (gear == 0) {
      displayRE();
      //Re
    }
    if (gear == 1) {
      displayMORTO();
      //ponto morto
    }
    if (gear > 1) {
      marcha = gear - 1;
      displayMARCHA(marcha);
    }
    geardata=0;
  }

  if (speeddata == 1) 
  {
    setVELO(carspeed);
    delay(10);
    speeddata=0;
  }    

  if (rpmdata == 1) 
  {
    setRPM(rpm);
    rpmleds = map(rpm,0,rpmmax,0,9);    // distributes the rpm level to the 8 leds + 1 for shift change

    if (rpmleds==0){ 
      for (cont=1;cont<=8;cont++){
        barraLED(cont, OFF);
      }
    }

    if (rpmleds==1){
      for (cont=1;cont<=8;cont++){
        barraLED(cont, OFF);
      }
      barraLED(1, ON);
    }

    if (rpmleds==2){
      for (cont=1;cont<=8;cont++){
        barraLED(cont, OFF);
      }
      barraLED(1, ON);
      barraLED(2, ON);
    }

    if (rpmleds==3){
      for (cont=1;cont<=8;cont++){
        barraLED(cont, OFF);
      }
      barraLED(1, ON);
      barraLED(2, ON);
      barraLED(3, ON);
    }

    if (rpmleds==4){
      for (cont=1;cont<=8;cont++){
        barraLED(cont, OFF);
      }
      barraLED(1, ON);
      barraLED(2, ON);
      barraLED(3, ON);
      barraLED(4, ON);
    }

    if (rpmleds==5){
      for (cont=1;cont<=8;cont++){
        barraLED(cont, OFF);
      }
      barraLED(1, ON);
      barraLED(2, ON);
      barraLED(3, ON);
      barraLED(4, ON);
      barraLED(5, ON);
    }

    if (rpmleds==6){
      for (cont=1;cont<=8;cont++){
        barraLED(cont, OFF);
      }
      barraLED(1, ON);
      barraLED(2, ON);
      barraLED(3, ON);
      barraLED(4, ON);
      barraLED(5, ON);
      barraLED(6, ON);
    }

    if (rpmleds==7){
      for (cont=1;cont<=8;cont++){
        barraLED(cont, OFF);
      }
      barraLED(1, ON);
      barraLED(2, ON);
      barraLED(3, ON);
      barraLED(4, ON);
      barraLED(5, ON);
      barraLED(6, ON);
      barraLED(7, ON);
    }

    if (rpmleds==8){
      for (cont=1;cont<=8;cont++){
        barraLED(cont, OFF);
      }
      if(currentMillis - previousMillis > interval) 
      {
        previousMillis = currentMillis;
        for (cont=1;cont<=8;cont++){
          barraLED(cont, ON);
        }
      } 
      rpmdata=0;
    }

  } 
}//Fim do loop


void display7int(int n, int valor){
  
  
}

void display7str(String text){
  
  
}

void limpaDisplay(){
  
}

void displayMARCHA(){
  
}

void barraLED(int led, int estado){
  
}


void setVELO(int velo){
  int r;
  r = (velo*180)/310;
  motorVELO.write(r);
}

void setRPM(int rpm){
  int r;
  r = (rpm*180)/310;
  motorRPM.write(r);
}
