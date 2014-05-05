// Descomente a linha abaixo para utilizar a biblioteca RF_24
// #define LIBRARY_RF24

#include <float.h>
#include <SPI.h>

#ifdef LIBRARY_RF24
#include <RF24_config.h>
#else
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>
#endif

#include <Wire.h>
#include <HMC5883L.h>
#include <Robot.h>
#include <SerialConnection.h>
#include <RadioConnection.h>
#include <Clock.h>
#include <HBridge.h>
#include <Compass.h>
#include <IRProximitySensor.h>
#include <ReflectanceSensorArray.h>
#include <LED.h>
#include <Potentiometer.h>
#include <Buzzer.h>
#include <Button.h>
#include <EEPROM.h>
#include "EEPROMAnything.h"
#include "pitches.h"
#include "marioSongs.h"
#include "printf.h"

#define ROBOT_ID  1
#define RADIO_ID  (108+2*ROBOT_ID)


struct config_t {
  byte robotNumber;
  float xmin;
  float xmax;
  float ymin;
  float ymax;
} configuration;


/**
 * Sketch para ser usado no RoboF, com radio, dispositivos basicos,
 * sem serial, com funçoes complexas, e suporte a adiçao de 
 * novos dispositivos *dinamicamente*
 */
const uint8_t reflectance_addr[] = {0,1,2,3,4};

int state = 0;
char v, msg_recv;
int valor;
long time_sent, time_recv;
Timer timer = 0;


class RoboF : public Robot {

public:
  RadioConnection radio;
  Buzzer buzzer;
  LED led;
  Button button;
  Potentiometer potentiometer;
  Compass compass;
  HBridge hbridge;
  IRProximitySensor irsensor;
  ReflectanceSensorArray reflectance;
  
  RoboF() : 
  radio(7,8,ROBOT_ID,RADIO_ID,false),
  hbridge(5,6,9,10),
  irsensor(A3),
  buzzer(1),
  led(0),
  button(A0, 5),
  potentiometer(A0, 6),
  reflectance(A0, reflectance_addr, 200)
  {
    if (EEPROM.read(0) == (byte)ROBOT_ID) {
      // bussola calibrada
      EEPROM_readAnything(0,configuration);
      compass = Compass(configuration.xmin, configuration.xmax,
      configuration.ymin, configuration.ymax);
    } 
    else {
      // sem calibracao
      compass = Compass();
    }

    addConnection(radio);  //connID = 0
    //adicionado por padrao: 
    //addDevice(clock)      //devID = 0
    addDevice(hbridge);     //devID = 1
    addDevice(compass);     //devID = 2
    addDevice(irsensor);    //devID = 3
    addDevice(reflectance); //devID = 4
    addDevice(buzzer);      //devID = 5
    addDevice(led);         //devID = 6
    addDevice(button);      //devID = 7
    addDevice(potentiometer);//devID = 8
    // para adicionar novo sensor
    // <ADD> <SENSOR_ID> <TAMANHO1BYTE> <PINO>
  }

  void messageReceived(const uint8_t * data, uint8_t size, Connection & connection){
    if (&connection == &radio){
      // recebe de radio e envia para serial
      time_recv = millis();
      msg_recv = 1;
    }
  }

  void deviceReady (Device & d){

  }

  void think(){

  }

private:

};

RoboF robot;

void setup(){
  Serial.begin(9600);
  printf_begin();
  
  printf("######## ROBO F #########\n");
  printf("### PROGRAMA DE TESTE ###\n\n");
  printf("Inicializando...");
  
  robot.begin();
  robot.getClock().add(timer);
  
  printf("Pronto!\n\n");  
}

void loop(){
  
  robot.step();
  
  
  if(!timer) {
  
    switch(state) {

      // radio
      case 0:
        time_sent = millis();
        msg_recv = 0;
        robot.radio.sendMessage((const uint8_t *)&time_sent, sizeof(long));
        printf("1 - RADIO\n\tsent %lu...", time_sent);
        timer = 500;
        state++;
        break;

      case 1:
        if(msg_recv) {
          long delay = time_recv - time_sent;
          printf("ok...delay = %lu ms\n", delay);
        } else {
          printf("fail\n");
        }
        timer = 2500;
        state++;
        break;

      // motores
      case 2:
        robot.hbridge.setMotorState(1, 50);
        robot.hbridge.setMotorState(0,-50);
        printf("2 - MOTORES\n\tME = 50\n\tMD = -50\n");
        timer = 3000;
        state++;
        break;

      case 3:
        robot.hbridge.setMotorState(1,0);
        robot.hbridge.setMotorState(0,0);
        state++;
        break;

      // bussola
      case 4:
        valor = robot.compass.getAngle();
        printf("3 - BUSSOLA\n\tangulo = %d graus\n", valor);
        timer = 3000;
        state++;
        break;

      // sensor de distancia
      case 5:
        valor = robot.irsensor.getDistance();
        printf("4 - SENSOR DE DISTANCIA\n\tdistancia = %d cm\n", valor);
        timer = 3000;
        state++;
        break;

      // sensor de refletancia
      case 6:
        robot.reflectance.get((uint8_t *)&v, 1);
        valor = (v&1) + ((v>>1)&1)*10 + ((v>>2)&1)*100 + ((v>>3)&1)*1000 + ((v>>4)&1)*10000;
        printf("5 - SENSOR DE REFLETANCIA\n\trefletancia = %05d\n", valor);
        timer = 3000;
        state++;
        break;

      // buzzer
      case 7:
        printf("6 - BUZZER\n\ttocando: ");
        Serial.end();
        play();
        timer = 1000;
        state++;
        break;

      case 8:
        if(!robot.buzzer.isPlaying()) {
          Serial.begin(9600);
          state++;
        } else {
          timer = 1000;
        }
        break;

      // led
      case 9:
        printf("7 - LED\n\tperiodo = 500ms\n");
        Serial.end();
        robot.led.blink(500,3);
        timer = 3000;
        state++;
        break;

      // potenciometro
      case 10:
        Serial.begin(9600);
        robot.potentiometer.get((uint8_t *)&valor, 2);
        printf("8 - POTENCIOMETRO\n\tpotenciometro = %d\n", valor);
        timer = 3000;
        state++;
        break;

      // botao
      case 11:
        printf("9 - BOTAO\n\taperte para continuar...\n\n");
        timer = 200;
        state++;
        break;

      case 12:
        robot.button.get((uint8_t *)&v, 1);
        if(!v) {
          state++;
        } else {
          timer = 200;
        }
        break;

      default:
        printf("### PROGRAMA DE TESTE ###\n\n");
        state = 0;
    }
  } 
    
}

void play() {
  if(!robot.buzzer.isPlaying()) {
    printf("%s\n", melody_name);
    robot.buzzer.playMelody((sizeof(melody)/sizeof(int)), melody, noteDurations);
  }
}
