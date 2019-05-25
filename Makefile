
ESP_ROOT  = stuff/arduino-esp32
CHIP      = esp32
BUILD_DIR = build
SKETCH    = src/_main.cpp
PART_FILE = partition-table.csv
CUSTOM_LIBS  = libs
# LIBS = libs/HttpOTA
# VERBOSE=1

# UPLOAD_PORT = /dev/ttyUSB1
UPLOAD_SPEED = 921600
# BOARD = esp210

include stuff/makeEspArduino/makeEspArduino.mk

uploadwd: all
	./webdav_upload_fw_all.sh