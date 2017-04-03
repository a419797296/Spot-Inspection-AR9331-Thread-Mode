
luci.sys.exec("/root/scan_ssid.sh")
m = Map("spotInspection", "Network Configeration", "Please choose the SSID from the ssid list and enter the wifi key, then press the save&apply button to commit")
m:chain("luci")

s = m:section(TypedSection, "wifi_info", translate("Wifi Information"))
s.anonymous = true                                                                   
s.addremove = false 

s:tab("wifi", translate("Config Wifi"))

o = s:taboption("wifi", ListValue, "ssid", translate("SSID:")) 
--o.default = 0 
o.datatype = "uinteger"


str0 = luci.sys.exec("uci get spotInspection.ssidList.ssid0")
str1 = luci.sys.exec("uci get spotInspection.ssidList.ssid1")
str2 = luci.sys.exec("uci get spotInspection.ssidList.ssid2")
str3 = luci.sys.exec("uci get spotInspection.ssidList.ssid3")
str4 = luci.sys.exec("uci get spotInspection.ssidList.ssid4")
str5 = luci.sys.exec("uci get spotInspection.ssidList.ssid5")
o:value(0, translate(str0)) 
o:value(1, translate(str1)) 
o:value(2, translate(str2))
o:value(3, translate(str3)) 
o:value(4, translate(str4)) 
o:value(5, translate(str5))

key = s:taboption("wifi", Value, "key", translate("Key:"),"input the wifi key") 

local apply = luci.http.formvalue("cbi.apply")
if apply then
	io.popen("/root/config_wifi.sh")
end

return m


