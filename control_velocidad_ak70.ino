/*
mod: ALan Moreno rodriguez
https://forum.arduino.cc/t/mcp-can-does-not-name-a-type-can-bus-problems/1056774

motor Ak70-10
codigo controlado por velocidad    rad/s =  v*4*pi
*/
#include <mcp_can.h>
#include <SPI.h>

 #define P_MIN -12.5
 #define P_MAX 12.5 
 #define V_MIN -20.0f
 #define V_MAX 20.0f
 #define KP_MIN 0.0f
 #define KP_MAX 500.0f
 #define KD_MIN 0.0f
 #define KD_MAX 5.0f
 #define T_MIN -24.0f
 #define T_MAX 24.0f



const int SPI_CS_PIN = 10;

MCP_CAN CAN (SPI_CS_PIN);   

void setup() {



  Serial.begin(115200);
 //configuracion del mcp_scan
 delay(1000);
  while (CAN_OK != CAN.begin(MCP_ANY,CAN_1000KBPS, MCP_8MHZ))
  {
    Serial.println("CAN BUS Shield init fail");
    Serial.println(" Init CAN BUS Shield again");
    delay(100);
  }
CAN.setMode(MCP_NORMAL);
Serial.println("CAN BUS Shield is ok!");


// desabilitar motor
ExitMotorMode();

delay(100);
//habilitar el motor
 EnterMotorMode();
//
// pack_cmd(0, 0, 0, 0, 0);
// ExitMotorMode();

}


//maxima velocidad es 4.5 
void loop() {

EnterMotorMode();
pack_cmd(0, 2, 0, 4.5, 0);
delay(2000);
pack_cmd(0, -2, 0, 3.2, 0);
delay(2000);
ExitMotorMode();
delay(100); 


}

void EnterMotorMode(){
  //Enter Motor Mode(enable)
  byte buf[8];
  buf[0] = 0xFF;
  buf[1] = 0xFF;
  buf[2] = 0xFF;
  buf[3] = 0xFF;
  buf[4] = 0xFF;
  buf[5] = 0xFF;
  buf[6] = 0xFF;
  buf[7] = 0xFC;
  CAN.sendMsgBuf(0x01, 0, 8, buf);
}
void ExitMotorMode(){
  byte buf[8];
  buf[0] = 0xFF;
  buf[1] = 0xFF;
  buf[2] = 0xFF;
  buf[3] = 0xFF;
  buf[4] = 0xFF;
  buf[5] = 0xFF;
  buf[6] = 0xFF;
  buf[7] = 0xFD;
  CAN.sendMsgBuf(0x01, 0, 8, buf);
}
void Zero(){
  //
  byte buf[8];
  buf[0] = 0xFF;
  buf[1] = 0xFF;
  buf[2] = 0xFF;
  buf[3] = 0xFF;
  buf[4] = 0xFF;
  buf[5] = 0xFF;
  buf[6] = 0xFF;
  buf[7] = 0xFE;
  CAN.sendMsgBuf(0x01, 0, 8, buf);

}
void pack_cmd( float p_in, float v_in, float kp_in, float kd_in, float t_in){
  byte buf[8];
 

  float p_des = constrain(p_in, P_MIN, P_MAX); 
  float v_des = constrain(v_in, V_MIN, V_MAX); 
  float kp = constrain(kp_in, KP_MIN, KP_MAX); 
  float kd = constrain(kd_in, KP_MIN, KP_MAX);
  float t_ff = constrain(t_in, T_MIN, T_MAX); 


  unsigned int p_int = float_to_uint(p_des, P_MIN, P_MAX, 16);
  unsigned int v_int = float_to_uint(v_des, V_MIN, V_MAX, 12);
  unsigned int kp_int = float_to_uint(kp, KP_MIN, KP_MAX, 12);
  unsigned int kd_int = float_to_uint(kd, KD_MIN, KD_MAX, 12);
  unsigned int t_int = float_to_uint(t_ff, T_MIN, T_MAX, 12);


  buf[0] = p_int >> 8;
  buf[1] = p_int & 0xFF;
  buf[2] = v_int >> 4;
  buf[3] = ((v_int & 0xF) << 4) | (kp_int >> 8);
  buf[4] = kp_int & 0xFF;
  buf[5] = kd_int >> 4;
  buf[6] = ((kd_int & 0xF) << 4) | (t_int >> 8);
  buf[7] = t_int & 0xFF;

   
 CAN.sendMsgBuf(0x01, 0, 8, buf);
}


unsigned int float_to_uint(float x, float x_min, float x_max, int bits)
{
  
  float span = x_max-x_min;
  float offset = x_min;
  unsigned int pgg = 0;
  if(bits==12){
    pgg = (unsigned int) ((x-offset)*4095.0/span);
  }
  if(bits==16){
    pgg = (unsigned int) ((x-offset)*65535.0/span);
  }
  return pgg;
}

float uint_to_float(unsigned int x_int, float x_min, float x_max, int bits)
{
  
  float span = x_max-x_min;
  float offset = x_min;
  float pgg = 0;
  if (bits==12){
    pgg = ((float) x_int)*span/4095 + offset;
  }
  if (bits==16){
    pgg = ((float) x_int)*span/65535.0 + offset;
  }
  return pgg;

}