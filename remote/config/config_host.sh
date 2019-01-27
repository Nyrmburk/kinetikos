#!/bin/bash

NAME=$1

# set the hostname to the robot name
HOSTS=$"/etc/hosts"
mv -n $HOSTS $HOSTS.bak
cp -a $HOSTS.bak $HOSTS
echo "
127.0.1.1	$NAME
" >> $HOSTS

HOSTNAME=$"/etc/hostname"
mv -n $HOSTNAME $HOSTNAME.bak
cp -a $HOSTNAME.bak $HOSTNAME
echo $NAME > $HOSTNAME
hostname $NAME

