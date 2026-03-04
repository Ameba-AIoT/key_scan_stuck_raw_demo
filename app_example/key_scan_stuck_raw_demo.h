#ifndef _KSCAN_STUCK_H_
#define _KSCAN_STUCK_H_

#include <stdio.h>
#include "ameba_soc.h"
#include "platform_stdlib.h"
#include "basic_types.h"
#include "os_wrapper.h"
#include "log.h"
#include "ameba_kscan.h"

#define TAG     "app_main_ks_stuck" 

void kscan_stuck_raw_thread(void);
u32 keyscan_irq_handle(void *Data);

#endif