
// Descomente a linha abaixo para utilizar a biblioteca RF_24
// #define LIBRARY_RF24

#include <SPI.h>

#if LIBRARY_RF24
  #include <RF24_config.h>
#else
  #include <Mirf.h>
  #include <nRF24L01.h>
  #include <MirfHardwareSpiDriver.h>
#endif

#include <Wire.h>
#include <HMC5883L.h>
#include <GenericRobot.h>
#include <SerialConnection.h>
#include <RadioConnection.h>
#include <HBridge.h>
#include <Compass.h>
#include <IRProximitySensor.h>
#include <ReflectanceSensorArray.h>

/**
 * Sketch para ser usado no RoboF, com radio, dispositivos basicos,
 * sem serial, com funçoes complexas, e suporte a adiçao de 
 * novos dispositivos *dinamicamente*
 */
const uint8_t pin_sel[] = {4, 3, 16};

class RoboF : public GenericRobot {
public:
  RoboF() : radio(7,8,false),
            hbridge(5,6,9,10),
            compass(),
            irsensor(17),
            reflectance(A0, pin_sel, 400)
            {
    addConnection(radio);  //connID = 0
    //adicionado por padrao: 
    //addDevice(clock)      //devID = 0
    addDevice(hbridge);     //devID = 1
    addDevice(compass);     //devID = 2
    addDevice(irsensor);    //devID = 3
    addDevice(reflectance); //devID = 4
    // para adicionar novo sensor
    // <ADD> <SENSOR_ID> <TAMANHO1BYTE> <PINO>
  }
  
private:
  RadioConnection radio;
  
  HBridge hbridge;
  Compass compass;
  IRProximitySensor irsensor;
  ReflectanceSensorArray reflectance;
};

/**
 * Posiciona o robo em um determinado angulo da bussola.
 *
 * Entrada:
 *  1. (int16) - angulo desejado
 *  2. (int8)  - erro maximo (threshold)
 **/
bool head (Device ** deviceList, uint8_t deviceListSize, Connection & c, const uint8_t * data, uint8_t length) {
  static HBridge *  hbridge     = NULL;
  static Compass *  compass     = NULL;
  static int16_t    angle;
  static int8_t     thld;
  static uint8_t    iterations;

  //inicializa a funçao
  if (data != NULL){
    hbridge = (HBridge *) deviceList[0]; // posiçao 0!
    compass = (Compass *) deviceList[1]; // posiçao 1!
    angle = ((int16_t *)data)[0];
    thld = data[2];
    iterations = 0;
  }

  if (hbridge && compass && angle < 360) {
    // erro entre o angulo desejado e o atual
    int16_t error = angle - compass->getAngle();

    // ajuste para o menor angulo
    if(error > 180)       error -= 360; // (180,360) -> (-180,0)
    else if(error < -180) error += 360; // (-360,-180) -> (0,180)

    // verifica se esta dentro do erro limite
    if ((error >= -thld) && (error <= thld )) {
      // se esta dentro do limite, para
      hbridge->setMotorState(1,0);
      hbridge->setMotorState(0,0);

      // conta 5 iteracoes no angulo desejado,
      // para evitar que ele desvie do angulo pela inercia
      if (iterations >= 5) {
        return true; //termina
      } else {
        iterations++;
        return false; //repete
      }

    } else {
      // senao, calcula uma velocidade e direcao de giro proporcional ao erro
      int8_t speed;
      if (error > thld) {
        speed = (int8_t) max(30, min(127, error*0.515));
      } else {
        speed = (int8_t) min(-30, max(-127, -error*0.515));
      }
      hbridge->setMotorState(1, speed);
      hbridge->setMotorState(0,-speed);
      iterations = 0;

      return false; //repete
    }
  }

  return true; //termina
}


/**
 * Rotaciona o robo em um determinado angulo.
 *
 * Entrada:
 *  1. (int16) - angulo desejado
 *  2. (int8)  - erro maximo (threshold)
 **/
bool turn (Device ** deviceList, uint8_t deviceListSize, Connection & c, const uint8_t * data, uint8_t length) {
  static HBridge *  hbridge       = NULL;
  static Compass *  compass       = NULL;
  static int8_t     thld;
  static int16_t    turnRemaining;
  static int16_t    lastAngle;
  static uint8_t    iterations;

  //inicializa a funçao
  if (data != NULL){
    hbridge = (HBridge *) deviceList[0]; // posiçao 0!
    compass = (Compass *) deviceList[1]; // posiçao 1!
    turnRemaining = ((int16_t *)data)[0];
    thld = data[2];
    iterations = 0;
    lastAngle = compass->getAngle();
  }

  if (hbridge && compass) {
    // mede a rotacao feita nessa iteracao e subtrai da rotacao total
    int16_t currAngle = compass->getAngle();
    int16_t currTurn = currAngle - lastAngle;
    // ajuste para o menor angulo
    if (currTurn > 180)       currTurn -= 360;
    else if (currTurn < -180) currTurn += 360;
    turnRemaining -= currTurn;
    lastAngle = currAngle;

    // verifica se esta dentro do erro limite
    if ((turnRemaining >= -thld) && (turnRemaining <= thld )) {
      // se esta dentro do limite, para
      hbridge->setMotorState(1,0);
      hbridge->setMotorState(0,0);
      
      // conta 5 iteracoes no angulo desejado,
      // para evitar que ele desvie do angulo pela inercia
      if (iterations >= 5) {
        return true; //termina
      } else {
        iterations++;
        return false; //repete
      }

    } else {
      // senao, calcula uma velocidade e direcao de giro proporcional ao erro
      int8_t speed;
      if (turnRemaining > thld) {
        speed = (int8_t) max(40, min(127, turnRemaining*0.71));
      } else {
        speed = (int8_t) min(-40, max(-127, -turnRemaining*0.71));
      }
      hbridge->setMotorState(1, speed);
      hbridge->setMotorState(0,-speed);
      iterations = 0;

      return false; //repete
    }
  }
  
  return true; //termina
}

RoboF robot;

void setup(){
  //adicionando funçoes... 
  robot.addAction(head);  // id = 0
  robot.addAction(turn);  // id = 1
  robot.begin();
}

void loop(){
  robot.step();
}
