/* 
 * File:   IRProximitySensor.h
 * Author: Anderson Antunes
 *
 * Created on July 18, 2013, 4:04 PM
 */

#ifndef IR_PROXIMITY_SENSOR_H
#define	IR_PROXIMITY_SENSOR_H

#include <stdint.h>
#include "Device.h"

#define ANALOG		1
#define NUM_SENSORS	5

class IRProximitySensor : public Device {
  
public:
  IRProximitySensor(uint8_t pin_out, const uint8_t pins_sel[], uint16_t thld);

  IRProximitySensor(const uint8_t pins[]);

  void begin();
  void stop();
  void reset();
  void update();
  bool isReady();
  uint8_t get(uint8_t * buffer, uint8_t size);
  void set (const uint8_t * data, uint8_t size = 1);

  private:
  uint8_t value;
  uint8_t pin_out;
  uint8_t pins_sel[3];
  uint16_t thld;
  uint8_t pins[NUM_SENSORS];
  uint8_t analog;

};

#endif	/* IR_PROXIMITY_SENSOR_H */

