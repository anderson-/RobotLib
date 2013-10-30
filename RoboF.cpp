#include <SPI.h>
#include <Wire.h>
#include <RF24_config.h>
#include <HMC5883L.h>
#include "Robot.h"
#include "SerialConnection.h"
#include "HBridge.h"
#include "Compass.h"
#include "IRProximitySensor.h"
#include "ReflectanceSensorArray.h"
#include "GenericRobot.h"
#include "RoboF.h"


bool andar(Device ** deviceList, uint8_t deviceListSize, Connection & c, const uint8_t * data, uint8_t length);
bool rotate(Device ** deviceList, uint8_t deviceListSize, Connection & c, const uint8_t * data, uint8_t length);


RoboF::RoboF() :
    radio(PIN_RADIO_CE, PIN_RADIO_CSN, RADIO_SLAVE),
    hbridge(PIN_MTR_L1, PIN_MTR_L2, PIN_MTR_R1, PIN_MTR_R2),
    compass(),
    irpsensor(PIN_IRP),
    reflectance(PIN_REFLET_IN, (const uint8_t[] )PINS_REFLET_SEL, REFLET_THLD),
    t(0) {
  addConnection(radio);     //connID = 0
  //adicionado por padrao:
  //addDevice(clock)        //devID = 0
  addDevice(hbridge);       //devID = 1
  addDevice(compass);       //devID = 2
  addDevice(irpsensor);     //devID = 3
  addDevice(reflectance);     //devID = 4
  // para adicionar outros sensores enviar:
  // <ADD> <SENSOR_SID> <TAMANHO1BYTE> <PINO>
  clock.add(t);

  addAction(andar);
  addAction(rotate);
}

bool andar(Device ** deviceList, uint8_t deviceListSize, Connection & c, const uint8_t * data, uint8_t length) {
  // variaveis estaticas nao sao redefinidas e guardam seus valores em chamadas consecutivas da mesma funçao
  static HBridge * hbridge = NULL;
  static IRProximitySensor * irpsensor = NULL;
  static uint8_t dist = 0;

  if (data != NULL) { //inicializa as variaveis estaticas
    hbridge = (HBridge *) deviceList[0]; // posiçao 0!
    irpsensor = (IRProximitySensor *) deviceList[1]; // posiçao 1!
    dist = data[0];
  }

  //fazem a açao
  if (hbridge && irpsensor) {
    if (irpsensor->getDistance() <= dist) {
      hbridge->setMotorState(1, 0);
      hbridge->setMotorState(0, 0);
      return true; //termina
    } else {
      hbridge->setMotorState(1, -50); //ARRUMAR!!
      hbridge->setMotorState(0, -50);
      return false; //repete
    }
  }
  return true;
}

//   Funcionando, mas ainda tem o problema de passar um pouco do angulo após encontra-lo.
//   Essa funcao nao e equivalente a girar(angulo), mas pode ser utilizada/modificada para isso. O que ela faz e
// posicionar o robo em um angulo determinado.
bool rotate(Device ** deviceList, uint8_t deviceListSize, Connection & c, const uint8_t * data, uint8_t length) {
  static HBridge * hbridge = NULL;
  static Compass * compass = NULL;
  static int16_t angle = 0;
  static int16_t last_error = 0;
  static int8_t thld = 0;
  static int8_t iterations = 0;
  static float P = 0;
  static float D = 0;
  const float Kp = 0.52;
  const float Kd = 8.73;
  int16_t error;

  if (data != NULL) { //inicializa a funçao
    hbridge = (HBridge *) deviceList[0]; // posiçao 0!
    compass = (Compass *) deviceList[1]; // posiçao 1!
    angle = ((int16_t *) data)[0];
    thld = data[2];
  }
  if (angle <= 360) {
    error = angle - compass->getAngle();
    // ajuste do para o menor angulo
    if (error > 180) {
      error = error - 360; // (180,360) -> (-180,0)
    } else if (error < -180) {
      error = error + 360; // (-360,-180) -> (0,180)
    }
    if ((error >= -thld) && (error <= thld)) { // se ja esta dentro do erro limite
      hbridge->setMotorState(1, 0);
      hbridge->setMotorState(0, 0);
      if (iterations >= 5) {
        return true; //termina
      } else {
        iterations++;
        return false;
      }
    } else {
      int8_t speed;
      P = error * Kp;
      D = (error - last_error) * Kd;
      if (error > thld) // se esta a direita do objetivo
        speed = (int8_t) max(30, P + D);
      else // se esta a esquerda do objetivo
        speed = (int8_t) min(-30, P + D);
      hbridge->setMotorState(1, -speed);
      hbridge->setMotorState(0, speed);
      iterations = 0;
      return false; //repete
    }
    last_error = error;
  }
  return true; //termina
}

