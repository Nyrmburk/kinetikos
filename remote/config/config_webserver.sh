#!/bin/bash

# install mdns for windows hosts
apt -y install samba

# install nginx
apt -y install nginx

# link webserver vhost path to user home directory
rm -r /var/www
ln -sf /home/kinetikos/remote/ /var/www

