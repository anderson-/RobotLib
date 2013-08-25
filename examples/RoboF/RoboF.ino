#include <SPI.h>
#include <RF24_config.h>
#include <Robot.h>
#include <Wire.h>
#include <HMC5883L.h>
#include <SerialConnection.h>
#include <HBridge.h>
#include <Compass.h>
#include <IRProximitySensor.h>
#include <GenericRobot.h>

/**
 * Sketch para ser usado no RoboF, com radio, dispositivos basicos,
 * sem serial, com funçoes complexas, e suporte a adiçao de 
 * novos dispositivos *dinamicamente*
 */

class RoboF : public GenericRobot {
public:
  RoboF() : radio(7,8,false),
            hbridge(5,6,9,10),
            compass()
            {
    addConnection(radio);  //connID = 0
    //adicionado por padrao: 
    //addDevice(clock)     //devID = 0
    addDevice(hbridge);    //devID = 1
    addDevice(compass);    //devID = 2
    //tem que adiciona o sensor de distancia enviando {6, 5, 1, 17}
    //<ADD> <SENSOR_SID> <TAMANHO1BYTE> <PINO>
  }
  
private:
  RadioConnection radio;
  
  HBridge hbridge;
  Compass compass;
};

bool andarAteh (Device ** deviceList, uint8_t deviceListSize, Connection & c, const uint8_t * data, uint8_t length){
  // variaveis estaticas nao sao redefinidas e guardam seus valores em chamadas consecutivas da mesma funçao
  static HBridge * hbridge = NULL;
  static IRProximitySensor * irpsensor = NULL;
  static uint8_t dist = 0;
  
  if (data != NULL){ //inicializa as variaveis estaticas
    hbridge = (HBridge *) deviceList[0]; // posiçao 0!
    irpsensor = (IRProximitySensor *) deviceList[1]; // posiçao 1!
    dist = data[0];
  }
  
  //fazem a açao
  if (hbridge && irpsensor){
    if (irpsensor->getDistance() <= dist){
      hbridge->setMotorState(1,0);
      hbridge->setMotorState(0,0);
      return true; //termina
    } else {
      hbridge->setMotorState(1,-50);//ARRUMAR!!
      hbridge->setMotorState(0,-50);
      return false; //repete
    }
  }
  return true;
}

//não funciona pq eu não consegui fazer xD
bool rotate (Device ** deviceList, uint8_t deviceListSize, Connection & c, const uint8_t * data, uint8_t length){
  static HBridge * hbridge = NULL;
  static Compass * compass = NULL;
  static int16_t angle = 0;
  static uint8_t error = 0;
  if (data != NULL){ //inicializa a funçao
    hbridge = (HBridge *) deviceList[0]; // posiçao 0!
    compass = (Compass *) deviceList[1]; // posiçao 1!
    angle = ((uint16_t *)data)[0];
    error = data[2];
  }
  if (hbridge && compass && angle <= 360){
    if (compass->getAngle() > (((angle-error) < 0)? (angle-error+360) : (angle-error)) && 
        compass->getAngle() < (((angle+error) >= 360)? (angle+error-360) : (angle+error))){
      hbridge->setMotorState(1,0);
      hbridge->setMotorState(0,0);
      return true; //termina
    } else if (compass->getAngle() > angle){
      hbridge->setMotorState(1,-60);
      hbridge->setMotorState(0,60);
      return false; //repete
    } else if (compass->getAngle() < angle){
      hbridge->setMotorState(1,60);
      hbridge->setMotorState(0,-60);
      return false; //repete
    }
  }
  return true; //termina
}

RoboF robot;

void setup(){
  //adicionando funçoes... 
  robot.addAction(rotate);     // id = 0
  robot.addAction(andarAteh);  // id = 1
  robot.begin();
}

void loop(){
  robot.step();
}
