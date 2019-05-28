#!/bin/bash

ESP_ROOT=${PWD}/stuff/arduino-esp32
OS=`uname -s`

if [ "$OS" == "Darwin" ]; then
	TOOLCHAIN="https://dl.espressif.com/dl/xtensa-esp32-elf-osx-1.22.0-80-g6c4433a-5.2.0.tar.gz"
else
	TOOLCHAIN="https://dl.espressif.com/dl/xtensa-esp32-elf-linux64-1.22.0-80-g6c4433a-5.2.0.tar.gz"
fi

[ ! -d stuff ] && mkdir stuff
cd stuff
git clone https://github.com/espressif/arduino-esp32.git
git clone https://github.com/plerup/makeEspArduino.git
cd -

cd ${ESP_ROOT}/tools/
wget ${TOOLCHAIN} -O xtensa-esp32-elf.tar.gz
tar -xzf xtensa-esp32-elf.tar.gz
rm -f xtensa-esp32-elf.tar.gz
cd -

cd ${ESP_ROOT}/tools/
git clone https://github.com/espressif/esptool.git esptool
cd -
cp ${ESP_ROOT}/tools/esptool/esptool.py ${ESP_ROOT}/tools/esptool/esptool

cd ${ESP_ROOT}/tools/
wget https://github.com/igrr/mkspiffs/releases/download/0.2.3/mkspiffs-0.2.3-arduino-esp32-osx.tar.gz
tar zxf mkspiffs-0.2.3-arduino-esp32-osx.tar.gz 
mv mkspiffs-0.2.3-arduino-esp32-osx mkspiffs
cd -

