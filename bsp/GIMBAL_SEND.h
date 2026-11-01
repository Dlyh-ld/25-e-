#ifndef GIMBAL_SEND_H_
#define GIMBAL_SEND_H_

#include <stdint.h>

#define GIMBAL_YAW_ADDR   0x02U
#define GIMBAL_PITCH_ADDR 0x01U

#define GIMBAL_MODE_SPEED         0x0000U     //速度控制
#define GIMBAL_MODE_MULTI_T       0x0001U     //多圈位置控制-T
#define GIMBAL_MODE_SINGLE_T      0x0002U     //单圈绝对位置控制
#define GIMBAL_MODE_MULTI_DIRECT  0x0003U     //多圈位置控制-直通
#define GIMBAL_MODE_SINGLE_DIRECT 0x0004U     //单圈绝对位置控制-直通

void Gimbal_Init(void);
void Gimbal_Enable(uint8_t addr);
void Gimbal_Disable(uint8_t addr);
void Gimbal_SetMode(uint8_t addr, uint16_t mode);
void Gimbal_SetMultiTurnAngle(uint8_t addr, int32_t angle_x10);
void Gimbal_SetAngle(int32_t yaw_x10, int32_t pitch_x10);
void Gimbal_Home(void);





#endif
