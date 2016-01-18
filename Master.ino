/*
  Master module for control of Scorbot axis with Arduino slaves
  
  Autor: Enrique Arrabal
  Date of creation: 3/8/2014
  Date of last modification: 14/9/2014
  copyright (c) 2014 Enrique Arrabal
  
  GNU General Public License
  
  references:
  - http://arduino.cc/en/Tutorial/AnalogInOutSerial
  - http://playground.arduino.cc/Main/RotaryEncoders
  - http://playground.arduino.cc/Code/Timer1
  
*/

/*
------------------------------------------------------------------------------------------------
0         1         2         3         4         5         6         7         8         9
012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345
------------------------------------------------------------------------------------------------ 
*/
#include <MSerial.h>
#include <Wire.h>
#include "Configuration.h"
#include "Master.h"

void setup(){  
  Serial.begin(9600);  
  Serial.flush();  
  Wire.begin();  
}

void loop(){  
  // check if are any data to recieve
  nUSBData = MySerial.GetSerialNum(fUSBData);
  if(nUSBData > 0){  
    // Opcion_nunmber_Who are together on the same number  
    Who = int(fUSBData[0])%10;   
    // Separate
    Order.fValue = int(fUSBData[0]/10);
    Value.fValue = fUSBData[1];
    // if control is going to be enabled, prepare to ask for slaves
    if( (Order.fValue == ControlEnabled  && Value.fValue == 1)   || (Order.fValue == Homing  && Value.fValue == 1) )      RequestEnabled = true;    
    if( (Order.fValue == ControlEnabled  && Value.fValue == 0)   || (Order.fValue == Homing  && Value.fValue == 0) )      RequestEnabled = false;
    // if Data has to send to all slaves
    if(Who == 0){
      for(i=1;i<=numSlaves;i++){
        send_Package(i,Order,Value);  
      }
    } 
    // if Data has to send to one slaves on particular
    else{
      send_Package(Who,Order,Value);
    }
  };
  if(RequestEnabled){
    // Ask about the Manipuled variable state to the Slaves
    for(i=1;i<=numSlaves;i++){
      Wire.requestFrom(i,4);
      j=0;
      while(Wire.available()){
        MVariable.bArray[j] = Wire.read();
        j++;
      } 
      // Send Data to Interface  
      Serial.print(MVariable.fValue);
      Serial.print(" ");
    }
    Serial.print("\n");
  };
  delay(PlotResolution);
}


//--------------------------------------------
// Sub-functions to support the loop function
//--------------------------------------------
void send_Package(int Who,ufloat Order, ufloat Value){
  Wire.beginTransmission(Who);          
  Wire.write(Order.bArray,4);    // Order is shipped
  Wire.endTransmission();            
  delay(1);
  Wire.beginTransmission(Who);          
  Wire.write(Value.bArray,4);    // Value of the order is shipped
  Wire.endTransmission(); 
}

