#!/bin/sh

echo "starting ap mode"

if [ ! $# == 2 ]
then
	echo "Please input: wifi_start_ap SSID Key(or none)"
	exit
fi

SSID=$1
key=$2
echo the input SSID is $SSID, key is $key

if [ ! $key = "none" ]
then
	uci set wireless.@wifi-iface[-2].encryption=psk2
	uci set wireless.@wifi-iface[-2].key=$key
else
	
	uci set wireless.@wifi-iface[-2].encryption=none
	uci delete wireless.@wifi-iface[-2].key
	echo starting ap without key
fi


uci set wireless.@wifi-iface[-2].ssid=$SSID
uci set wireless.@wifi-iface[-2].disabled=0

#disable the sta mode 
uci set wireless.@wifi-iface[-1].disabled=1

uci commit wireless
/etc/init.d/network restart

