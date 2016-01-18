
#ifndef Master_h
#define Master_h
#include"Arduino.h"
#include"Configuration.h"
// Structures
union         ufloat{
  byte        bArray[4];
  float       fValue;
};
// Global Data
ufloat        Order;
ufloat        Value;
ufloat        MVariable;                
int           Who;
float         *fUSBData;
int           nUSBData;
boolean       RequestEnabled = false;
enum Orders{
  ControlEnabled = 0,
  Homing         = 13
};

Orders MyOrder;
const int     maxUSBData = numSlaves*2;
// Communication--------------------------------------------------------------------------------------------------
MSerial       MySerial = MSerial();
// Auxiliar variables--------------------------------------------------------------------------------------------
int           i,j;
#endif
