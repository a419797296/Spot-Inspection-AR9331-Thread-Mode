#!/bin/sh

echo "starting sta mode"

if [ ! $# == 2 ]
then
	echo "Please input: wifi_start_sta SSID Key"
	exit
fi

ping www.baidu.com -c 1 > nul

if [ $? == 0 ]
then
	SSID=`uci get wireless.@wifi-iface[-1].ssid`
	key=`uci get wireless.@wifi-iface[-1].key`
	echo the network have already connected,which SSID is $SSID, key is $key
	exit
fi

SSID=$1
key=$2
echo the input SSID is $SSID, key is $key
uci set wireless.@wifi-iface[-1].ssid=$SSID
uci set wireless.@wifi-iface[-1].key=$key
uci set wireless.@wifi-iface[-1].disabled=0
uci commit wireless
/etc/init.d/network restart

