#include "task_bsp.h"

#include "CH57xBLE_LIB.h"
#include "bsp_config.h"
#include "bsp_mcu.h"

/* TMOS�¼����� */
#define HAL_RF_CALIBRATE_EVENT 0x01

tmosTaskID hal_task_id;

tmosEvents HAL_ProcessEvent(tmosTaskID task_id, tmosEvents events)
{
    if (events & HAL_RF_CALIBRATE_EVENT) {
        /* У׼BLE RF������У׼��ʱС��10ms */
        BLE_RegInit();
        /* ����ִ��У׼���� */
        tmos_start_task(hal_task_id, HAL_RF_CALIBRATE_EVENT, 
                        MS1_TO_SYSTEM_TIME(BLE_CALIBRATION_PERIOD));

        return events ^ HAL_RF_CALIBRATE_EVENT;
    }

    return 0;
}

void bsp_task_init()
{
    hal_task_id = TMOS_ProcessEventRegister(HAL_ProcessEvent);
    mcu_timer_init();
#if (defined HAL_SLEEP) && (HAL_SLEEP == TRUE)
    mcu_sleep_init();
#endif
    /* ���У׼���� */
    tmos_start_task(hal_task_id, HAL_RF_CALIBRATE_EVENT,
                    MS1_TO_SYSTEM_TIME(BLE_CALIBRATION_PERIOD));
}
