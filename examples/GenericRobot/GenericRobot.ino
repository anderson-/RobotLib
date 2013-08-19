#include <SPI.h>
#include <RF24_config.h>
#include <Robot.h>
#include <Wire.h>
#include <HMC5883L.h>
#include <SerialConnection.h>
#include <HBridge.h>
#include <Compass.h>
#include <GenericRobot.h>

/**
 * Sketch usado para testar um robo generico, que posui seus
 * dispositivos alocados em tempo de execuçao. Para isso deve-se
 * enviar uma mensagem, via conexao serial, contendo:
 *
 * { 
 *   6  (add), 
 *   1  (id da classe do dispositivo¹ - LED),
 *   1  (tamanho da lista de argumentos - 1 byte), 
 *   13 (argumento especifico - pino do led)
 * };
 *
 * (¹) ver GenericRobot.h - enum DEVICE
 *
 * NOTA: Para a aplicaçao final deve-se incluir radio
 */

class NewRobot : public GenericRobot {
public:
  NewRobot() : serial(Serial,9600){
    addConnection(serial);
  }

private:
  SerialConnection serial;
};

NewRobot robot;

void setup(){
  robot.begin();
}

void loop(){
  robot.step();
}
