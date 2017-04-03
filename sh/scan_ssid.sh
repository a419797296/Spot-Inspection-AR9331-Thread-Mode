#!/bin/sh
n=0
for i in `iw dev wlan0 scan | grep SSID| awk -F'[:]' '{print $2}'`
do
	echo here is :----$i----
	uci set spotInspection.ssid."ssid$n"="$i"
	n=$(($n+1))
done
uci commit
