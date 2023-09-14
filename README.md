# CONTROL  MOTOR CUBEMEARS AK70_10

mod: ALan Moreno rodriguez

https://forum.arduino.cc/t/mcp-can-does-not-name-a-type-can-bus-problems/1056774

motor Ak70-10


libreria:


https://github.com/coryjfowler/MCP_CAN_lib


comando:

pack_cmd( float p_in, float v_in, float kp_in, float kd_in, float t_in)

p_in= posicion
v_in= velocidad
kp_in= ganancia kp
kd_in= ganancia kd
t_in = torque


ejemplo:

pack_cmd(0, 2, 0, 4.5, 0);
solo varia el segundo parametro "2".


