
#include "config.h"

/**
 * Sketch para ser usado no RoboF, com radio, dispositivos basicos,
 * sem serial, com funçoes complexas, e suporte a adiçao de 
 * novos dispositivos *dinamicamente*
 */
const uint8_t pin_sel[] = {4, 3, 16};

class RoboF : 
public GenericRobot {
public:
  RoboF() : 
  radio(7,8,ROBOT_ID,RADIO_ID,false),
  hbridge(5,6,9,10),
  irsensor(17),
  reflectance(A0, pin_sel, 200)
  {
  	if (EEPROM.read(0) == (byte)ROBOT_ID) {
  		// bussola calibrada
			EEPROM_readAnything(0,configuration);
			compass = Compass(configuration.xmin, configuration.xmax,
												configuration.ymin, configuration.ymax);
		} else {
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


RoboF robot;

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
    if (compass->available()) {

      int16_t newAngle = compass->getAngle();
      int16_t error = angle - newAngle;

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
        }
        else {
          iterations++;
        }

      } 
      else {
        // senao, calcula uma velocidade e direcao de giro proporcional ao erro
        int8_t speed;
        if (error > thld) {
          speed = (int8_t) max(30, min(127, error*0.515)); // error > thld -> error > 0
        }
        else {
          speed = (int8_t) min(-30, max(-127, error*0.515)); // error < -thld -> error > 0
        }
        hbridge->setMotorState(0, speed);
        hbridge->setMotorState(1,-speed);
        iterations = 0;
      }

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
  if (data != NULL) {
    hbridge = (HBridge *) deviceList[0]; // posiçao 0!
    compass = (Compass *) deviceList[1]; // posiçao 1!
    turnRemaining = ((int16_t *)data)[0];
    thld = data[2];
    iterations = 0;
    lastAngle = compass->getAngle();
  }

  if (hbridge && compass) {
    // mede a rotacao feita nessa iteracao e subtrai da rotacao total
    if (compass->available()) {

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
        if (iterations >= 10) {
          return true; //termina
        } 
        else {
          iterations++;
        }

      } 
      else {
        // senao, calcula uma velocidade e direcao de giro proporcional ao erro
        int8_t speed;
        if (turnRemaining > thld) {
          speed = (int8_t) max(30, min(127, turnRemaining*0.515)); // turnRemaining > thld -> turnRemaining > 0
        } 
        else {
          speed = (int8_t) min(-30, max(-127, turnRemaining*0.515)); // turnRemaining < -thld -> turnRemaining < 0
        }
        hbridge->setMotorState(0, speed);
        hbridge->setMotorState(1,-speed);
        iterations = 0;

      }
    }
    return false; //repete
  }

  return true; //termina
}


bool calibrateCompass (Device ** deviceList, uint8_t deviceListSize,
		Connection & c, const uint8_t * data, uint8_t length) {

  static HBridge *  hbridge    = NULL;
  static Compass *  compass    = NULL;
  static HMC5883L * hmc5883    = NULL;
  static Clock &    clock      = robot.getClock();
  static Timer 	    timer      = 0;

  // inicializa a funcao
  if (data != NULL) {
    hbridge = (HBridge *) deviceList[0]; // posiçao 0!
    compass = (Compass *) deviceList[1]; // posiçao 1!
    hmc5883 = compass->getCompass();
    clock = robot.getClock();
    clock.add(timer);
    timer = 10000;
    configuration.robotNumber = (byte)ROBOT_ID;
    configuration.xmin = FLT_MAX;
    configuration.xmax = FLT_MIN;
    configuration.ymin = FLT_MAX;
    configuration.ymax = FLT_MIN;
    hbridge->setMotorState(0, 40);
    hbridge->setMotorState(1,-40);
  }

  if (hbridge && compass && hmc5883 && !timer) {
    if (compass->available()) {
    	// le os valores XYZ da bussola
      MagnetometerScaled scaled = hmc5883->ReadScaledAxis();
      // salva os valores maximos e minimos de X e Y
      if (scaled.XAxis < configuration.xmin && scaled.XAxis > -400)
				configuration.xmin = scaled.XAxis;
			if (scaled.XAxis > configuration.xmax && scaled.XAxis < 400)
				configuration.xmax = scaled.XAxis;
			if (scaled.YAxis < configuration.ymin && scaled.YAxis > -400)
				configuration.ymin = scaled.YAxis;
			if (scaled.YAxis > configuration.ymax && scaled.YAxis < 400)
				configuration.ymax = scaled.YAxis;
    }
    return false;
  }

  hbridge->setMotorState(0,0);
  hbridge->setMotorState(1,0);
  // salva as configurações na EEPROM
  EEPROM_writeAnything(0,configuration);
  // calibra a bussola
  compass->calibrate(configuration.xmin, configuration.xmax,
										configuration.ymin, configuration.ymax);
  clock.remove(timer);
  return true;
}



void setup(){
  //adicionando funçoes... 
  robot.addAction(head);  // id = 0
  robot.addAction(turn);  // id = 1
  robot.addAction(calibrateCompass);  // id = 2
  robot.begin();
}

void loop(){
  robot.step();
}
