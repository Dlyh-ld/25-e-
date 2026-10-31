#include "ti_msp_dl_config.h"
#include "param.h"
#include "pid.h"

void PID_Init(PID_t *pid, float kp, float ki, float kd, float imax, float omax)
{
    pid->KP = kp;
    pid->KI = ki;
    pid->KD = kd;

    pid->IMAX = imax;
    pid->OMAX = omax;
    
    pid->Dbuf[0] = pid->Dbuf[1] = pid->Dbuf[2] = 0.0f;
    pid->error[0] = pid->error[1] = pid->error[2] = 0.0f;
    pid->OUT = pid->POUT = pid->IOUT = pid->DOUT = 0.0f;
}


float PID_CALE(PID_t *pid, float set, float fbd)
{
    if (pid == NULL) {
        return 0.0f;
    }
    pid->error[2] = pid->error[1];
    pid->error[1] = pid->error[0];
    pid->set = set;
    pid->fbd = fbd;
    pid->error[0] = set - fbd;
    pid->POUT = pid->KP * pid->error[0];
    pid->IOUT += pid->KI * pid->error[0];
    pid->Dbuf[2] = pid->Dbuf[1];
    pid->Dbuf[1] = pid->Dbuf[0];
    pid->Dbuf[0] = (pid->error[0] - 2.0f * pid->error[1] + pid->error[2]);
    pid->DOUT = pid->KD * pid->Dbuf[0];
    pid->IOUT = Min(Max(pid->IOUT, -(float)pid->IMAX), (float)pid->IMAX);
    pid->OUT = pid->POUT + pid->IOUT + pid->DOUT;
    pid->OUT = Min(Max(pid->OUT, -(float)pid->OMAX), (float)pid->OMAX);

    return pid->OUT;
}
