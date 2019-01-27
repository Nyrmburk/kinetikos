#!/bin/bash

# The rpi3 has only 2 serial modems. Mini UART, the default for gpio serial,
# baudrate is dependent on vpu clockspeed. The solution is to disable bluetooth
# and configure gpio to use the hardware modem normally defaulted to bluetooth.

# diable bluetooth
systemctl disable hciuart

# configure the device tree to use gpio instead of bluetooth for hardware UART
DEVICE_TREE=$"/boot/config.txt"
mv -n $DEVICE_TREE $DEVICE_TREE.bak
cp -a $DEVICE_TREE.bak $DEVICE_TREE
echo "
# disable bluetooth to enable hardware UART on gpio
dtoverlay=pi3-disable-bt
" >> $DEVICE_TREE

