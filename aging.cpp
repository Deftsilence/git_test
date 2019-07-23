

#include "aging.h"
#include <AP_Common/AP_Common.h>
#include <AP_HAL/AP_HAL.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <nuttx/mtd.h>
#include <errno.h>

#include <drivers/drv_baro.h>
#include <drivers/drv_mag.h>
#include <drivers/drv_accel.h>

#include <uORB/topics/battery_status.h>

#define POW2(_x)		((_x) * (_x))

#define AGING_MAIN_LOOP_MICROS  2500

#define AGING_LIGHT_CHAN  5
#define AGING_LIGHT_ON()  hal.rcout->write(AGING_LIGHT_CHAN, 30)
#define AGING_LIGHT_OFF() hal.rcout->write(AGING_LIGHT_CHAN, 1)
#define AGING_LIGHT_ON_INTERVAL       (1000)
#define AGING_LIGHT_OFF_INTERVAL      (3000)

#define AGING_LIGHT_STATUS_ON           (1)
#define AGING_LIGHT_STATUS_OFF          (0)

#define AGING_MOTOR_PWM_RATE            (490)
#define AGING_MOTOR_STOP(chan) hal.rcout->write(chan, 1500)
#define AGING_MOTOR_FF(chan) hal.rcout->write(chan, 1600)
#define AGING_MOTOR_RF(chan) hal.rcout->write(chan, 1400)
#define AGING_MOTOR_STATUS_CHANGE_TIME    (3000)
#define AGING_MOTOR_STATUS_STOP           (0)
#define AGING_MOTOR_STATUS_FF             (1)
#define AGING_MOTOR_STATUS_RF             (2)


void Aging::light_motor_setup()
{
    printf("light motor setup ...\r\n");

    //hal.rcout->force_safety_off();

    //hal.rcout->set_esc_scaling(1000, 2000);

    for (uint8_t i = 0; i < 6; i++) {
        hal.rcout->enable_ch(i);
    }

    //hal.rcout->set_freq(0xf, AGING_MOTOR_PWM_RATE);
}

void Aging::light_test()
{
    uint32_t interval;

    if (_light_status == AGING_LIGHT_STATUS_ON)
    {
        interval = AGING_LIGHT_ON_INTERVAL;
    }
    else
    {
        interval = AGING_LIGHT_OFF_INTERVAL;
    }
    
    if (millis() - _light_on_timestamp > interval)
    {
        //printf("light test...\r\n");
        if (_light_status == AGING_LIGHT_STATUS_ON)
        {
            //printf("light turn off.\r\n");
            AGING_LIGHT_OFF();
            _light_status = AGING_LIGHT_STATUS_OFF;
        }
        else
        {
            //printf("light turn on.\r\n");
           AGING_LIGHT_ON();
            _light_status = AGING_LIGHT_STATUS_ON;
        }

        _light_on_timestamp = millis();
    }
}

void Aging::motor_test()
{
    int chan;
    int16_t v[8];
    
    v[0] = 1500;
    v[1] = 1600;
    v[2] = 1600;
    v[3] = 1500;

    hal.rcin->set_overrides(v, 4);
}

void Aging::aging_run()
{
    static uint32_t tested = 0;

    if (!tested)
    {
        light_motor_setup();
        tested = 1;
    }
    
    /* light */
    light_test();
    
    /* motors */
    motor_test();
}


Aging::Aging(void):
    _motor_status(AGING_MOTOR_STATUS_FF),
    _motor_running_time(0),
    _light_on_timestamp(0),
    _light_off_timestamp(0),
    _light_status(AGING_LIGHT_STATUS_OFF)
{
}


