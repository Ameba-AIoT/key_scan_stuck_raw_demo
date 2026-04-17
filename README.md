# Ameba RTL8721Dx SoC: Key Stuck Handling Example (Raw API, FreeRTOS)

* [中文版本](./README_CN.md)

🚀 This example is based on the Key-Scan peripheral of the RTL8721Dx series SoC.  
It demonstrates how to detect and handle “stuck keys” using the Raw API when a key is kept pressed abnormally.

- 📎 EVB Purchase Links: [🛒 Taobao](https://item.taobao.com/item.htm?id=904981157046) | [📦 Amazon](https://www.amazon.com/-/zh/dp/B0FB33DT2C/)
- 📄 [Chip Information](https://aiot.realmcu.com/zh/module/rtl8721dx.html)
- 📚 [Key-Scan Documentation](https://aiot.realmcu.com/cn/latest/rtos/peripherals/key_scan/index.html)

---

## ✨ Features

✅ 4×4 key matrix,stuck threshold 300ms 
✅ When a key is stuck: in the **stuck-key interrupt**, the firmware reads the row status, sets a default state for the stuck row as an indicator, and masks all keys on that row  
✅ Clock module: a 131 kHz clock is used for key-stuck detection and low-power wakeup

---

### 4×4 Key Matrix Pin Assignment

![alt text](image.png)

**Rows:**

| Row | Pin  |
|-----|------|
| Row 1 | PA31 |
| Row 2 | PA30 |
| Row 3 | PA29 |
| Row 4 | PA28 |

**Columns:**

| Column | Pin  |
|--------|------|
| Col 1 | PB17 |
| Col 2 | PB18 |
| Col 3 | PB20 |
| Col 4 | PA14 |

---

## Working Principle

- **Stuck-key detection**:  
  After enabling the stuck-key detection function, if a key is held longer than the configured threshold, it is treated as a “stuck key” and a stuck-key interrupt is generated. The user can then mask the entire row of the stuck key in the interrupt handler.

- **Stuck-key masking**:  
  The firmware reads the stuck-key status register to locate the row of the stuck key, then sets a default state for that row. When the Key-Scan engine scans this row, it will treat it as “no key pressed”, allowing the engine to return to IDLE instead of staying busy indefinitely.

- **More details**:  
  For detailed register description and timing diagrams, please refer to the SDK Key-Scan documentation:  
  📚 [Key-Scan Documentation](https://aiot.realmcu.com/cn/latest/rtos/peripherals/key_scan/index.html)

---

## 🚀 Quick Start

1️⃣ **Select SDK**  
   - Set the path for `env.sh` (`env.bat`): `source {sdk}/env.sh`  
   - Replace `{sdk}` with the absolute path to `env.sh` in the root directory of the [ameba-rtos SDK](https://github.com/Ameba-AIoT/ameba-rtos). This step only needs to be performed once if the SDK path remains unchanged.

2️⃣ **Build**  
   - Execute the following in the demo example directory:  
     ```bash
     source env.sh
     ameba.py build -p
     ```

3️⃣ **Burning the Firmware**  
   > Replace `COMx` with your actual serial port (for example, `COM5`).

```bash
ameba.py flash --p COMx --image km4_boot_all.bin 0x08000000 0x8014000 --image km0_km4_app.bin 0x08014000 0x8200000
```

 ⚡ **Note**: If you want to use the **prebuilt binaries** provided in the project directory (parent folder), run:

```bash
ameba.py flash --p COMx --image ../km4_boot_all.bin 0x08000000 0x8014000 --image ../km0_km4_app.bin 0x08014000 0x8200000
```

4️⃣ **Monitor**  
   - `ameba.py monitor --port COMx --b 1500000`
---

5️⃣ **Key Test Procedure**

- Press and hold one key and **do not release it**, to create a “stuck key” condition.  
- Meanwhile, press other keys on different rows and columns.  
- Observe the UART log to check whether the stuck interrupt, release interrupt and default scan events are triggered as expected.

---

6️⃣ **Check Log Output 📜**

- Compare with the following “Log Example” to verify that the key stuck handling logic is working correctly.

---

## 📝 Log Example

```bash
Log example:
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
