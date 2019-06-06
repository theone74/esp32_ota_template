
ESP_ROOT  = stuff/arduino-esp32
CHIP      = esp32
BUILD_DIR = build
SKETCH    = src/_main.cpp
PART_FILE = partition-table.csv
# CUSTOM_LIBS  = libs
# VERBOSE=1

# LIBS = stuff/arduino-esp32/libraries/WiFi/src \
# 		stuff/arduino-esp32/libraries/HTTPClient/src \
# 		stuff/arduino-esp32/libraries/Update/src \
# 		stuff/arduino-esp32/libraries/WiFiClientSecure/src \
# 		stuff/arduino-esp32/libraries/Wire/src \
# 		stuff/arduino-esp32/libraries/Ticker/src \
# 		stuff/arduino-esp32/libraries/SPI/src \
# 		stuff/arduino-esp32/libraries/SPIFFS/src \
# 		stuff/arduino-esp32/libraries/FS/src \
#		libs/waveshare4_2

# BUILD_EXTRA_FLAGS= -Istuff/arduino-esp32/tools/sdk/include/freertos/freertos/

# UPLOAD_PORT = /dev/ttyUSB1
UPLOAD_PORT = /dev/tty.SLAB_USBtoUART
# UPLOAD_PORT = /dev/tty.wchusbserial1410
UPLOAD_SPEED = 921600
# BOARD = esp210

include stuff/makeEspArduino/makeEspArduino.mk

uploadwd: all
	./webdav_upload_fw_all.sh

monitor:
	idf_monitor.py --port $(UPLOAD_PORT) build/_main.elf