/*
  Marcelo Roberto Gadotti
  2018/02/19
  Serial report from AD and GPIO
*/
#include <Arduino.h>
#include <TimerOne.h>

// define outputs
#define O2    2
#define O3    3
#define O4    4
#define O5    5
#define O6    6
#define O7    7
#define O8    8
#define O9    9
#define O10   10

//define inputs
#define I11   11
#define I12   12

// time from thread
#define timer_update  100000

/* Initialize GPIO */
void init_gpio()
{
  // heart bit led
  pinMode(LED_BUILTIN, OUTPUT);
  // output pins
  pinMode(O2, OUTPUT);
  pinMode(O3, OUTPUT);
  pinMode(O4, OUTPUT);
  pinMode(O5, OUTPUT);
  pinMode(O6, OUTPUT);
  pinMode(O7, OUTPUT);
  pinMode(O8, OUTPUT);
  pinMode(O9, OUTPUT);
  pinMode(O10, OUTPUT);
  //input pins
  pinMode(I11, INPUT);
  pinMode(I12, INPUT);
}
/* set value from output */
void setOutput(uint8_t id, uint8_t val)
{
  // set value from output pin
  if (val > 0) {
    digitalWrite(id,HIGH);
  }else
  {
    digitalWrite(id,LOW);
  }
}
/* return value from analog pin */
void getAnalog(uint8_t id)
{
  Serial.print("AD -> ");
  Serial.println(id);
}
/* return value from input pin */
void getInput(uint8_t id)
{
  // get value from input pin
  Serial.println(digitalRead(id));
}

/* thread using timer one for smoothin ad values*/
void thread_timer1(void)
{
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}

void setup()
{
  Serial.begin(9600);
  // initialize GPIO
  init_gpio();
  // create thread using timer one
  Timer1.initialize(timer_update);
  Timer1.attachInterrupt(thread_timer1);
  Serial.println(">0.1");
}

char inChar[8];
char nBuffer[3];
uint8_t idGPIO = 0, setGPIO = 0, nChar = 0;
uint8_t index = 0;

void loop()
{
  if (Serial.available()) {
    // clear buffer
    memset(inChar,'0',7);
    // read char until \n
    nChar = Serial.readBytesUntil('\n',inChar,7);
    // Serial.println(inChar);
    // Serial.println(nChar);

    // is there a command?
    if (inChar[0] == '>' && inChar[nChar - 1] == '#') {
      // get id from GPIO and cast to int
      nBuffer[0] = inChar[2];
      nBuffer[1] = inChar[3];
      nBuffer[2] = '\0';
      // cast to int
      idGPIO = (uint8_t)atoi(nBuffer);
      // check received command
      switch (inChar[1]) {
        case 'A':
          // get value from analog pin
          getAnalog(idGPIO);
          break;
        case 'O':
          // get id from output
          nBuffer[0] = inChar[4];
          nBuffer[1] = '\0';
          setGPIO = (uint8_t)atoi(nBuffer);
          // set output value
          setOutput(idGPIO, setGPIO);
          break;
        case 'I':
          // get value from input
          getInput(idGPIO);
          break;
        default:
          Serial.println("nack");
          break;
      }

    }
  }
}
