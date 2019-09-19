/******************************************************************************
Copyright (c) 2019 released Swadha Energies Pvt. Ltd.  All rights reserved.
******************************************************************************/


#ifndef __timercfg_h
#define __timercfg_h

#include "stm32f0xx.h"

enum timer_id_tag
{
  thermal_ir_timer,
  thermal_ir_communication_timer,
  total_timers,
};

#endif