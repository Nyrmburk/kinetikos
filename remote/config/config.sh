#!/bin/bash

# prompt for sudo if not already
if [ $EUID != 0 ]; then
	echo "use sudo"
    exit
fi

NAME=$1

if [ -z "$NAME" ]
then
	echo "Please give a robot name:"
	read NAME
fi
echo "Setting up $NAME..."

# add a new user with no full name/phone number/...
# prompts the user for a password
adduser --gecos "" kinetikos
usermod -a -G dialout,video kinetikos

echo "updating apt"
apt -y update

echo "config host"
./config_host.sh $NAME

echo "config webserver"
./config_webserver.sh

echo "config serial"
./config_serial.sh

echo "config AP"
./config_ap.sh $NAME $NAME

echo "config startup"
cp kinetikos /etc/init.d/kinetikos
update-rc.d kinetikos defaults
