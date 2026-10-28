#define WHEEL_DIA 4.5

#define KEY_DEBOUNCE_MS 20U

#define MOTOR_KP 500.0f
#define MOTOR_KI 1.0f
#define MOTOR_KD 0.2f
#define MOTOR_MaxIout 500.0f
#define MOTOR_MaxOut 10000.0f
#define MOTOR_MinOut 1000.0f
#define MOTOR_TARGET_SPEED 6000.0f

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
    GIMBAL_YAW = 0,
    GIMBAL_PITCH
} GIMBAL_t;

typedef enum{
    MOTOR_LEFT = 0,
    MOTOR_RIGHT
} MOTOR_c;



#define Max(a, b) ((a) > (b) ? (a) : (b))
#define Min(a, b) ((a) < (b) ? (a) : (b))
