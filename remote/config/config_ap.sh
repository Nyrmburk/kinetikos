#!/bin/bash

SSID=$1
PSK=$2

WLAN="wlan0"
# set up simultaneous AP so internet and AP mode are active
# doesn't work because dhcpcd craps out with the message:
# "carrier lost" right as it gets an ip address. without an
# ip address, hostpd can't give dhcp ranges to connections
#iw dev wlan0 interface add wlan1 type __ap
#WLAN="wlan1"

# install required software
apt -y install dnsmasq hostapd

# configure dhcpcd
DHCPCD=$"/etc/dhcpcd.conf"
mv -n $DHCPCD $DHCPCD.bak
cp -a $DHCPCD.bak $DHCPCD
echo "
#noarp
interface $WLAN
static ip_address=192.168.4.1/24
nohook wpa_supplicant
" >> $DHCPCD

# configure network interfaces
INTERFACES=$"/etc/network/interfaces"
mv -n $INTERFACES $INTERFACES.bak
cp -a $INTERFACES.bak $INTERFACES
echo "
#allow-hotplug $WLAN
" >> $INTERFACES

# restart the dhcpcd service
service dhcpcd restart

# configure hostapd
HOSTAPD=$"/etc/hostapd/hostapd.conf"
echo "
# This is the name of the WiFi interface we configured above
interface=$WLAN

# Use the nl80211 driver with the brcmfmac driver
driver=nl80211

# This is the name of the network
ssid=$SSID

# Use the 2.4GHz band
hw_mode=g

# Use channel 6
channel=6

# Enable 802.11n
ieee80211n=1

# Enable WMM
wmm_enabled=1

# Enable 40MHz channels with 20ns guard interval
ht_capab=[HT40][SHORT-GI-20][DSSS_CCK-40]

# Accept all MAC addresses
macaddr_acl=0

# Use WPA authentication
auth_algs=1

# Require clients to know the network name
ignore_broadcast_ssid=0

# Use WPA2
wpa=2

# Use a pre-shared key
wpa_key_mgmt=WPA-PSK

# The network passphrase
wpa_passphrase=$PSK

# Use AES, instead of TKIP
rsn_pairwise=CCMP
" > $HOSTAPD

HOSTAPDDAEMON=$"/etc/default/hostapd"
mv -n $HOSTAPDDAEMON $HOSTAPDDAEMON.bak
cp -a $HOSTAPDDAEMON.bak $HOSTAPDDAEMON
echo "
DAEMON_CONF='/etc/hostapd/hostapd.conf'

# disable wifi power management
#iwconfig $WLAN power off
" >> $HOSTAPDDAEMON

# configure dnsmasq
DNSMASQ=$"/etc/dnsmasq.conf"
mv -n $DNSMASQ $DNSMASQ.bak
echo "
interface=$WLAN      # Use interface $WLAN
dhcp-range=192.168.4.2,192.168.4.20,255.255.255.0,24h
" >> $DNSMASQ

#systemctl disable wpa_supplicant

service dnsmasq start
service hostapd start
