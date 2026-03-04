#include "key_scan_stuck_raw_demo.h" 

/*  4R*4C
行
| Row 1 | PA 31 | 
| Row 2 | PA 30 | 
| Row 3 | PA 29 | 
| Row 4 | PA 28 | 

列
Col 1 | Col 2 | Col 3 | Col 4
PB17  | PB18  | PB20  | PA14

*/
u32 key_row[4] = {_PA_31, _PA_30, _PA_29, _PA_28};
u32 key_col[4] = {_PB_17, _PB_18, _PB_20, _PA_14};



u32 keyscan_irq_handle(void *Data)
{
    (void)Data;
    KSCAN_TypeDef *keyscan = KEYSCAN_DEV;
	u32 intr_status;
	u32 temp;
	u32 tempi;
	u32 ksdata[16]; //FIFO dep MAX =16
	u32 row_status = 0;

	intr_status = KeyScan_GetINT(keyscan);

	if (intr_status & KS_BIT_SCAN_EVENT_INT_STATUS) 
    { 
        //auto cleared
		temp = KeyScan_GetDataNum(keyscan);
		KeyScan_Read(keyscan, ksdata, temp);

		RTK_LOGI(TAG,"SCAN_EVENT_INT FIFO Data:");
		for (tempi = 0; tempi < temp; tempi++) 
		{
			printf(" %lx ", ksdata[tempi]);
		}
		printf("\r\n");
	}

	if (intr_status & KS_BIT_ALL_RELEASE_INT_STATUS) 
	{	
		RTK_LOGI(TAG,"Release Intr\r\n", __FUNCTION__);
		/*clear intr*/
		KeyScan_ClearINT(keyscan, KS_BIT_ALL_RELEASE_INT_CLR);
		KeyScan_INTConfig(keyscan, KS_BIT_ALL_RELEASE_INT_MASK | KS_BIT_SCAN_EVENT_INT_MASK, DISABLE);
		KeyScan_INTConfig(keyscan, KS_BIT_ALL_DEFAULT_INT_MASK, ENABLE);
		KeyScan_SetStuckRow(keyscan, 0);
	}
	if (intr_status & KS_BIT_ALL_DEFAULT_INT_STATUS) {	
		RTK_LOGI(TAG,"Default Intr\r\n", __FUNCTION__);
		KeyScan_ClearINT(keyscan, KS_BIT_ALL_DEFAULT_INT_CLR);
		/*clear intr*/
		temp = KeyScan_GetDataNum(keyscan);
		if (temp > 0) {
			KeyScan_Read(keyscan, ksdata, temp);
			for (tempi = 0; tempi < temp; tempi++) {
				printf(" %lx ", ksdata[tempi]);
			}
			printf("\n");

			KeyScan_INTConfig(keyscan, KS_BIT_ALL_DEFAULT_INT_MASK, DISABLE);
			KeyScan_INTConfig(keyscan, KS_BIT_ALL_RELEASE_INT_MASK | KS_BIT_SCAN_EVENT_INT_MASK, ENABLE);
		}
	}

	if (intr_status & KS_BIT_STUCK_EVENT_INT_STATUS) 
	{	
		RTK_LOGI(TAG,"Stuck Intr\r\n", __FUNCTION__);
		/* set row default status */
		row_status = KeyScan_GetStuckRow(keyscan);
		KeyScan_SetStuckRow(keyscan, row_status);

		/*clear intr*/
		KeyScan_ClearINT(keyscan, KS_BIT_STUCK_INT_CLR);
	}
	return 0;
}

void kscan_stuck_raw_thread(void)
{
    KeyScan_InitTypeDef KeyScan_InitStruct;
    KSCAN_TypeDef *key_scan = KEYSCAN_DEV;  //KM4 APB4
    Pinmux_Swdoff();
    /* pinmux config */
    int i = 0;
    for(i = 0; i < 4; i++)
    {
        PAD_PullCtrl(key_col[i], GPIO_PuPd_NOPULL);
		Pinmux_Config(key_col[i], (PINMUX_FUNCTION_KEY_COL0 + i));
		PAD_PullCtrl(key_row[i], GPIO_PuPd_UP);
		Pinmux_Config(key_row[i], (PINMUX_FUNCTION_KEY_ROW0 + i));
    }
    /* enable function and clk 1: Enable kscan clock */ 
	RCC_PeriphClockCmd(APBPeriph_KSCAN, APBPeriph_KSCAN_CLOCK, ENABLE);

    /* Init keyscan */
	KeyScan_StructInit(&KeyScan_InitStruct);
	KeyScan_InitStruct.KS_ColSel = 0x0F;
	KeyScan_InitStruct.KS_RowSel = 0x0F;
    /* Event Trigger:——KS_EVENT_TRIGGER_MODE
                key press/release event is stored in the FIFO only once during each key press/release operation
    Regular Scan:——KS_REGULAR_SCAN_MODE
                At each full scan, any key press event is stored in the FIFO until it is released */
	KeyScan_InitStruct.KS_WorkMode = KS_EVENT_TRIGGER_MODE; //Event Trigger V.S. Regular Scan 
	KeyScan_InitStruct.KS_LimitLevel = 6;
	KeyScan_Init(key_scan, &KeyScan_InitStruct);
	KeyScan_StuckAutoCmd(key_scan, ENABLE);//使能 Key-Scan 按键卡滞自动检测功能
	KeyScan_SetStuckThreshold(key_scan, 300);
	KeyScan_StuckPeriodicalPull(key_scan, 2000, 4000);

	KeyScan_INTConfig(key_scan,  KS_BIT_ALL_DEFAULT_INT_MASK | KS_BIT_STUCK_EVENT_INT_MASK, ENABLE);
    //bool InterruptRegister(IRQ_FUN IrqFun, IRQn_Type IrqNum, u32 Data, u32 Priority);

	InterruptRegister((IRQ_FUN) keyscan_irq_handle, KEYSCAN_IRQ, (u32)NULL, 3);
	InterruptEn(KEYSCAN_IRQ, 3);//52
	KeyScan_Cmd(key_scan, ENABLE);
    while (1)
    {
        /* code */
    }
    rtos_task_delete(NULL);
}