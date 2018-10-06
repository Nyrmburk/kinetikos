#!/bin/bash

# set up a second interface as JUST ap
# iw dev wlan0 interface add wlan1 type __ap

# install required software
apt install dnsmasq hostapd

# configure dhcpcd
DHCPCD=$"/etc/dhcpcd.conf"
mv -n $DHCPCD $DHCPCD.bak
cp -a $DHCPCD.bak $DHCPCD
echo "
denyinterfaces wlan0
" >> $DHCPCD

# configure network interfaces
INTERFACES=$"/etc/network/interfaces"
mv -n $INTERFACES $INTERFACES.bak
cp -a $INTERFACES.bak $INTERFACES
echo "
allow-hotplug wlan0
iface wlan0 inet static
    address 172.24.1.1
    netmask 255.255.255.0
    network 172.24.1.0
    broadcast 172.24.1.255
#    wpa-conf /etc/wpa_supplicant/wpa_supplicant.conf
" >> $INTERFACES

# restart the dhcpcd service
service dhcpcd restart

# configure hostapd
HOSTAPD=$"/etc/hostapd/hostapd.conf"
echo "
# This is the name of the WiFi interface we configured above
interface=wlan0

# Use the nl80211 driver with the brcmfmac driver
driver=nl80211

# This is the name of the network
ssid=Pi3-AP

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
wpa_passphrase=raspberry

# Use AES, instead of TKIP
rsn_pairwise=CCMP
" > $HOSTAPD

HOSTAPDDAEMON=$"/etc/default/hostapd"
mv -n $HOSTAPDDAEMON $HOSTAPDDAEMON.bak
cp -a $HOSTAPDDAEMON.bak $HOSTAPDDAEMON
echo "
service wpa_supplicant stop
DAEMON_CONF='/etc/hostapd/hostapd.conf'
" >> $HOSTAPDDAEMON

# configure dnsmasq
DNSMASQ=$"/etc/dnsmasq.conf"
mv -n $DNSMASQ $DNSMASQ.bak
echo "
interface=wlan0      # Use interface wlan0
listen-address=172.24.1.1 # Explicitly specify the address to listen on
bind-interfaces      # Bind to the interface to make sure we aren't sending things elsewhere
server=8.8.8.8       # Forward DNS requests to Google DNS
domain-needed        # Don't forward short names
bogus-priv           # Never forward addresses in the non-routed address spaces.
dhcp-range=172.24.1.50,172.24.1.150,12h # Assign IP addresses between 172.24.1.50 and 172.24.1.150 with a 12 hour lease time
" > $DNSMASQ

service wpa_supplicant stop

service dnsmasq start
service hostapd start
