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
/* thread using timer one for smoothin ad values*/
void thread_timer1(void)
{
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));

}

void setup()
{
  Serial.begin(9600);
  Serial.println(">0.1");
  // initialize GPIO
  init_gpio();
  // create thread using timer one
  Timer1.initialize(timer_update);
  Timer1.attachInterrupt(thread_timer1);
}
char inChar[4];
uint8_t index = 0;
void loop()
{
  if (Serial.available()) {
    // get the new byte:
    inChar[index] = (char)Serial.read();
    Serial.print(inChar[index]);
    index++;
  }
  if (index > 3) {
    if (inChar[3] == '\n') {
      Serial.println(">OK!");
      switch (inChar[0]) {

        case 'A':
          Serial.println("analog");
          break;

        case 'O':
          Serial.println("output");
          break;

        case 'I':
          Serial.println("input");
          break;

        default:
          Serial.println("nack");
          break;
      }
    }else
    {
      Serial.println("\nnack");
    }
    index = 0;
  }

}
