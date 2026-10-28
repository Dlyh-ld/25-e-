#ifndef PID_H_
#define PID_H_


typedef struct{
    float KP;
    float KI;
    float KD;

    float Dbuf[3];
    float error[3];

    int IMAX;
    int OMAX;

    float set;
    float fbd;

    float OUT;
    float POUT;
    float IOUT;
    float DOUT;

} PID_t;



void PID_Init(PID_t *pid, float kp, float ki, float kd, float imax, float omax);
float PID_CALE(PID_t *pid, float set, float fbd);





#endif