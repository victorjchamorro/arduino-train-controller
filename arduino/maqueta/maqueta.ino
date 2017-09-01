
int pinRele=4;
int pinPWM=6; //pin 5/6 PWM 1KHz, 9/10/11/3 PWM 500Hz
int velocidad=0;
int serial=0;


void setup() {
  // put your setup code here, to run once:
  pinMode(pinRele,OUTPUT);
  pinMode(pinPWM,OUTPUT);

  setPwmFrequency(pinPWM, 1);

  Serial.begin(9600);

  
}

void loop() {
  // put your main code here, to run repeatedly:

  if (serial==0 && Serial.available() > 0) {
    Serial.println("bienvenido, teclea w/s - a/d");
    serial=1;
  }
  
  if (Serial.available() > 0) {
    
    int receivedChars = Serial.read();

    switch(receivedChars){
      case 'w':
        //marcha
        marcha();
        break;
      case 's':
        //contramarcha
        contraMarcha();
        break;
      case 'd':
        //acelerar
        acelera();        
        break;
      case 'a':
        //decelerar
        decelera();        
        break;
      case 'x':
        //Paro
        paro();
        break;
    }
  }
}

void marcha(){
  digitalWrite(pinRele,LOW);
  Serial.println("marcha");
}

void contraMarcha(){
  digitalWrite(pinRele,HIGH);
  Serial.println("contramarcha");
}

void paro(){
  velocidad=0;
  digitalWrite(pinPWM,velocidad);
  Serial.println("paro");
}

void acelera(){
  velocidad=velocidad+5;
  if (velocidad>255) velocidad = 255;
  analogWrite(pinPWM,velocidad);
  Serial.println("velocidad: "+String(velocidad));
}

void decelera(){
  velocidad=velocidad-5;
  if (velocidad<0) velocidad = 0;
  analogWrite(pinPWM,velocidad);
  Serial.println("velocidad: "+String(velocidad));
}

/**
 * Divides a given PWM pin frequency by a divisor.
 *
 * The resulting frequency is equal to the base frequency divided by
 * the given divisor:
 *   - Base frequencies:
 *      o The base frequency for pins 3, 9, 10, and 11 is 31250 Hz.
 *      o The base frequency for pins 5 and 6 is 62500 Hz.
 *   - Divisors:
 *      o The divisors available on pins 5, 6, 9 and 10 are: 1, 8, 64,
 *        256, and 1024.
 *      o The divisors available on pins 3 and 11 are: 1, 8, 32, 64,
 *        128, 256, and 1024.
 *
 * PWM frequencies are tied together in pairs of pins. If one in a
 * pair is changed, the other is also changed to match:
 *   - Pins 5 and 6 are paired on timer0
 *   - Pins 9 and 10 are paired on timer1
 *   - Pins 3 and 11 are paired on timer2
 *
 * Note that this function will have side effects on anything else
 * that uses timers:
 *   - Changes on pins 3, 5, 6, or 11 may cause the delay() and
 *     millis() functions to stop working. Other timing-related
 *     functions may also be affected.
 *   - Changes on pins 9 or 10 will cause the Servo library to function
 *     incorrectly.
 *
 * Thanks to macegr of the Arduino forums for his documentation of the
 * PWM frequency divisors. His post can be viewed at:
 *   http://forum.arduino.cc/index.php?topic=16612#msg121031
 */
void setPwmFrequency(int pin, int divisor) {
  byte mode;
  if(pin == 5 || pin == 6 || pin == 9 || pin == 10) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
    if(pin == 5 || pin == 6) {
      TCCR0B = TCCR0B & 0b11111000 | mode;
    } else {
      TCCR1B = TCCR1B & 0b11111000 | mode;
    }
  } else if(pin == 3 || pin == 11) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 32: mode = 0x03; break;
      case 64: mode = 0x04; break;
      case 128: mode = 0x05; break;
      case 256: mode = 0x06; break;
      case 1024: mode = 0x07; break;
      default: return;
    }
    TCCR2B = TCCR2B & 0b11111000 | mode;
  }
}
