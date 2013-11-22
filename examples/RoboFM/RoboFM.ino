#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>
#include <RadioConnectionM.h>
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
            compass(),
            irsensor(17)
            {
    addConnection(radio);  //connID = 0
    //adicionado por padrao: 
    //addDevice(clock)     //devID = 0
    addDevice(hbridge);    //devID = 1
    addDevice(compass);    //devID = 2
    addDevice(irsensor);   //devID = 3
    //tem que adiciona o sensor de distancia enviando {6, 5, 1, 17}
    //<ADD> <SENSOR_SID> <TAMANHO1BYTE> <PINO>
  }
  
private:
  RadioConnectionM radio;
  
  HBridge hbridge;
  Compass compass;
  IRProximitySensor irsensor;
};


bool andar (Device ** deviceList, uint8_t deviceListSize, Connection & c, const uint8_t * data, uint8_t length){
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

//   Funcionando, mas ainda tem o problema de passar um pouco do angulo após encontra-lo.
//   Essa funcao nao e equivalente a girar(angulo), mas pode ser utilizada/modificada para isso. O que ela faz e
// posicionar o robo em um angulo determinado.
bool headTo (Device ** deviceList, uint8_t deviceListSize, Connection & c, const uint8_t * data, uint8_t length){
  static HBridge * hbridge = NULL;
  static Compass * compass = NULL;
  static int16_t angle = 0;
  static int8_t thld = 0;
  static int8_t iterations = 0;
  int16_t error;
  if (data != NULL){ //inicializa a funçao
    hbridge = (HBridge *) deviceList[0]; // posiçao 0!
    compass = (Compass *) deviceList[1]; // posiçao 1!
    angle = ((int16_t *)data)[0];
    thld = data[2];
  }
  if (hbridge && compass && angle <= 360){
    error = angle - compass->getAngle();
		// ajuste do para o menor angulo
    // exceto quando chamado pela funcao turn
    if (length < 4) {
			if(error > 180) {
				error = error - 360; // (180,360) -> (-180,0)
			} else if(error < -180) {
				error = error + 360; // (-360,-180) -> (0,180)
			}
    }
    if ((error >= -thld) && (error <= thld )){ // se ja esta dentro do erro limite
      hbridge->setMotorState(1,0);
      hbridge->setMotorState(0,0);
      if (iterations >= 5) {
        return true; //termina
      }
      else {
        iterations++;
        return false;
      }
    } else {
      if (error > thld){ // se esta a direita do objetivo
        int8_t speed = (int8_t) (30 + error*0.515); // velocidade proporcional ao erro, 0.71 = 128/180°
        hbridge->setMotorState(0,-speed);
        hbridge->setMotorState(1,speed);
        iterations = 0;
        return false; //repete
      } else { // se esta a esquerda do objetivo
        int8_t speed = (int8_t) (30 - error*0.515); // velocidade proporcional ao erro, 0.515 = (128-35)/180°
        hbridge->setMotorState(0,speed);
        hbridge->setMotorState(1,-speed);
        iterations = 0;
        return false; //repete
      }
    }
  }
  return true; //termina
}

bool turn (Device ** deviceList, uint8_t deviceListSize, Connection & c, const uint8_t * data, uint8_t length) {
  Compass * compass = NULL;
  static int16_t error = 0;
  static int16_t destAngle = 0;
  static uint8_t data2[4];
  
  if (data != NULL){ //inicializa a funçao
    compass = (Compass *) deviceList[1]; // posiçao 1!
    error = ((int16_t *)data)[0];
		if (compass) {
			memcpy(data2, data, 4);
			destAngle = error + compass->getAngle();
			if (destAngle < 0)   destAngle += 360;
			if (destAngle > 360) destAngle -= 360;
			((int16_t *)data)[0] = destAngle;
		} else {
			return true;
		}
  }

  return rotateAbs(deviceList, deviceListSize, c, (const uint8_t *)data2, 4)
}

RoboF robot;

void setup(){
  //adicionando funçoes... 
  robot.addAction(headTo);  	// id = 0
  robot.addAction(turn);    	// id = 1
  robot.addAction(andar);  		// id = 2
  robot.begin();
}

void loop(){
  robot.step();
}
