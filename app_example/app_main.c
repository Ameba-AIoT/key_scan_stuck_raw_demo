#include "key_scan_stuck_raw_demo.h"
//how to use keyscan when there is key stuck

void app_example(void)
{     
    RTK_LOGI(TAG, "kscan_stuck_raw_thread creat!\r\n");   
    if (rtos_task_create(NULL, "kscan_stuck_raw_thread", (rtos_task_t)kscan_stuck_raw_thread, NULL, 1024 * 4, 1) != RTK_SUCCESS) 
    {
        RTK_LOGE(TAG, "\n\r%s rtos_task_create(init_thread) failed", __FUNCTION__);
    }
}
