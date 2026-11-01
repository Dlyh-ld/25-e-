#define WHEEL_DIA 4.5

#define TRACK_KP          0.03f
#define TRACK_KI          0.0f
#define TRACK_KD          0.10f

#define SPEED_KP       30.0f
#define SPEED_KI       1.0f
#define SPEED_KD       0.0f

#define MOTOR_MaxIout     400U
#define MOTOR_MaxOut      960U
#define MOTOR_MinOut      180U



#define GIMBAL_KP 250.0f
#define GIMBAL_KI 0.0f
#define GIMBAL_KD 0.4f

#define GIMBAL_MaxIout 0
#define GIMBAL_MaxOut 1000.0f


typedef enum{
    MODE_IDLE = 0,        /* 待机：电机停，云台回中 */
    MODE_GIMBAL_ONLY,     /* 只测试云台 */
    MODE_ALL,             /* 电机 + 云台同时运行 */
    MODE_MOTOR_ONLY       /* 只巡线电机 */
} MODE_t;

typedef enum{
    GIMBAL_YAW = 1,
    GIMBAL_PITCH
} GIMBAL_t;


#define Max(a, b) ((a) > (b) ? (a) : (b))
#define Min(a, b) ((a) < (b) ? (a) : (b))
