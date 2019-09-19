#ifndef __THERMAL_IR_H
#define __THERMAL_IR_H

#include "stm32f0xx.h"

void thermal_ir_sensor_init(void);
uint8_t D6T_checkPEC(uint8_t buf[],uint8_t size);
uint8_t calc_crc(uint8_t data);
void thermal_ir_read(void);
void thermal_ir_communication(void);
#endif /* __THERMAL_IR_H */

