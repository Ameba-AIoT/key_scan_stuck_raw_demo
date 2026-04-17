# Ameba RTL8721Dx 系列 SoC：按键卡滞处理示例（Raw API，FreeRTOS）

* [English Version](./README.md)

🚀 本示例基于 RTL8721Dx 系列 SoC 的 Key-Scan 外设，演示当出现按键“卡住”时，如何消除该异常状况，使用 Raw API 实现按键卡滞检测与屏蔽。

- 📎 开发板购买链接：[🛒 淘宝](https://item.taobao.com/item.htm?id=904981157046) | [📦 Amazon](https://www.amazon.com/-/zh/dp/B0FB33DT2C/)
- 📄 [芯片详情](https://aiot.realmcu.com/zh/module/rtl8721dx.html)
- 📚 [Key-Scan 文档](https://aiot.realmcu.com/cn/latest/rtos/peripherals/key_scan/index.html)

---

## ✨ 功能特点

✅ 使用 4×4 按键矩阵，卡滞触发阈值300ms  
✅ 当出现按键卡滞时：在“按键卡滞事件中断”中，读取卡滞行状态，设置该行的默认状态用于标识卡滞行，并屏蔽该行所有按键  
✅ 时钟模块：使用 131 kHz 时钟用于检测按键卡滞和低功耗唤醒

---

### 4×4 键盘矩阵引脚定义

![alt text](image.png)

**行（Row）：**

| 行号 | 引脚 |
|------|------|
| Row 1 | PA31 |
| Row 2 | PA30 |
| Row 3 | PA29 |
| Row 4 | PA28 |

**列（Column）：**

| 列号 | 引脚 |
|------|------|
| Col 1 | PB17 |
| Col 2 | PB18 |
| Col 3 | PB20 |
| Col 4 | PA14 |

---

## 工作原理

- **卡滞键检测**：  
  启用卡滞检测功能后，若按键按下时间超过设定阈值，该按键将被视为“卡滞按键”，并触发卡滞按键中断。用户可在卡滞按键中断中屏蔽卡滞行。

- **卡滞键屏蔽**：  
  通过读取“按键卡滞状态寄存器”获取卡滞的按键所在行，然后设置该行的默认状态。Key-Scan 在扫描到该行时，会认为没有按键被按下，从而顺利进入 IDLE 状态，避免一直停留在忙碌状态。

- **详细原理**：  
  更多 Key-Scan 资源与寄存器说明可参考 SDK 文档 Key-Scan 章节：  
  📚 [Key-Scan 文档](https://aiot.realmcu.com/cn/latest/rtos/peripherals/key_scan/index.html)

---

## 🚀 快速开始

1️⃣ **选择SDK**  
   - 设置`env.sh`（`env.bat`）路径：`source {sdk}/env.sh`  
   - 将`{sdk}`替换为[ameba-rtos SDK](https://github.com/Ameba-AIoT/ameba-rtos)根目录中`env.sh`的绝对路径。如果SDK路径没有改变，此步骤仅需执行一次。

   ⚡ **注意**：本示例仅支持 SDK 版本 **≥ v1.2**

2️⃣ **编译**  
   - 在当前工程目录下执行：  
     ```bash
     source env.sh
     ameba.py build -p
     ```

3️⃣ **烧录固件**
   >请将命令中的 `COMx` 替换为实际串口号（例如 `COM5`）
   ```bash
   ameba.py flash --p COMx --image km4_boot_all.bin 0x08000000 0x8014000 --image km0_km4_app.bin 0x08014000 0x8200000
   ```
   ⚡ **Note**: 若直接使用项目目录中已提供的预编译 bin 文件，可使用如下命令（注意相对路径）：
   ```bash
   ameba.py flash --p COMx --image ../km4_boot_all.bin 0x08000000 0x8014000 --image ../km0_km4_app.bin 0x08014000 0x8200000
   ```

4️⃣ **打开串口监视**  
   - `ameba.py monitor --port COMx --b 1500000`
---

5️⃣ **按键测试步骤**

- 按下某个按键 **并保持不松手**，制造“卡滞键”场景；  
- 同时在其他行与列上点按不同按键；  
- 观察串口 log 输出，确认是否触发卡滞中断、释放中断及默认扫描事件。

---

6️⃣ **观察日志输出 📜**

- 对照下方“日志示例”，确认 Key-Scan 卡滞检测和屏蔽逻辑是否正常工作。

---

## 📝 日志示例

```bash
日志示例：
11:11:09.930  ROM:[V1.1]
11:11:09.930  FLASH RATE:1, Pinmux:1
11:11:09.930  IMG1(OTA1) VALID, ret: 0
11:11:09.930  IMG1 ENTRY[f800779:0]
11:11:09.930  [BOOT-I] KM4 BOOT REASON 0: Initial Power on
11:11:09.930  [BOOT-I] KM4 CPU CLK: 240000000 Hz
11:11:09.930  [BOOT-I] KM0 CPU CLK: 96000000 Hz
11:11:09.930  [BOOT-I] PSRAM Ctrl CLK: 240000000 Hz 
11:11:09.946  [BOOT-I] IMG1 ENTER MSP:[30009FDC]
11:11:09.946  [BOOT-I] Build Time: Mar  2 2026 14:41:35
11:11:09.946  [BOOT-I] IMG1 SECURE STATE: 1
11:11:09.946  [FLASH-I] FLASH CLK: 80000000 Hz
11:11:09.946  [FLASH-I] Flash ID: 85-20-16 (Capacity: 32M-bit)
11:11:09.946  [FLASH-I] Flash Read 4IO
11:11:09.946  [FLASH-I] FLASH HandShake[0x2 OK]
11:11:09.946  [BOOT-I] KM0 XIP IMG[0c000000:82c0]
11:11:09.946  [BOOT-I] KM0 SRAM[20068000:860]
11:11:09.946  [BOOT-I] KM0 PSRAM[0c008b20:20]
11:11:09.946  [BOOT-I] KM0 ENTRY[20004d00:60]
11:11:09.946  [BOOT-I] KM4 XIP IMG[0e000000:18820]
11:11:09.946  [BOOT-I] KM4 SRAM[2000b000:480]
11:11:09.946  [BOOT-I] KM4 PSRAM[0e018ca0:20]
11:11:09.946  [BOOT-I] KM4 ENTRY[20004d80:40]
11:11:09.946  [BOOT-I] IMG2 BOOT from OTA 1, Version: 1.1 
11:11:09.946  [BOOT-I] Image2Entry @ 0xe007c71 ...
11:11:09.946  [APP-[I] KLOCKM4 APP SS-I] KM0TART 
11:11:09.946  [ init_reAPP-I] Vtarget_lTOR: 30007000, Vocks
11:11:09.946  TOR_NS:30007000
11:11:09.946  [APP-I] VTOR: 30007000, VTOR_NS:30007000
11:11:09.946  [APP-I] IMG2 SECURE STATE[MAIN-I]: 1
11:11:09.946   IWDG refresh on!
11:11:09.961  [MAIN-I] KM0 OS START 
11:11:09.961  [CLK-I] [CAL4M]: delta:1 target:320 PPM: 3125 PPM_Limit:30000 
11:11:09.961  [CLK-I] [CAL131K]: delta:12 target:2441 PPM: 4916 PPM_Limit:30000 
11:11:09.961  [LOCKS-I] KM4 init_retarget_locks
11:11:09.961  [APP-I] BOR arises when supply voltage decreases under 2.57V and recovers above 2.7V.
11:11:09.961  [MAIN-I] KM4 MAIN 
11:11:09.961  [VER-I] AMEBA-RTOS SDK VERSION: 1.2.0
11:11:09.961  [MAIN-I] File System Init Success 
11:11:09.977  [app_main_ks_stuck-I] kscan_stuck_raw_thread creat!
11:11:09.977  [MAIN-I] KM4 START SCHEDULER 
11:11:14.962  SWD PAD Port0_Pin31 is configured to funcID SWD PAD Port0_Pin30 is configured to funcID [app_main_ks_stuck-I] Stuck Intr
11:11:14.978  [app_main_ks_stuck-I] Default Intr
11:11:14.978   141 
11:11:16.378  [app_main_ks_stuck-I] SCAN_EVENT_INT FIFO Data: 114 
11:11:16.550  [app_main_ks_stuck-I] SCAN_EVENT_INT FIFO Data: 14 
11:11:18.188  [app_main_ks_stuck-I] SCAN_EVENT_INT FIFO Data: 114 
11:11:18.361  [app_main_ks_stuck-I] SCAN_EVENT_INT FIFO Data: 14 
11:11:19.114  [app_main_ks_stuck-I] SCAN_EVENT_INT FIFO Data: 124 
11:11:19.271  [app_main_ks_stuck-I] SCAN_EVENT_INT FIFO Data: 24 
11:11:19.789  [app_main_ks_stuck-I] SCAN_EVENT_INT FIFO Data: 134 
11:11:19.962  [app_main_ks_stuck-I] SCAN_EVENT_INT FIFO Data: 34 
11:11:21.080  [app_main_ks_stuck-I] SCAN_EVENT_INT FIFO Data: 41 
11:11:21.379  [app_main_ks_stuck-I] Stuck Intr
11:11:21.394  [app_main_ks_stuck-I] Release Intr
11:11:21.394  [app_main_ks_stuck-I] Default Intr
...
```
