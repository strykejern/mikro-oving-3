echo uninitializing old driver...
rmmod led_driver.ko

echo downloading driver...
wget -O led_driver.ko http://mikro-oving-3.googlecode.com/svn/trunk/driver/led_driver.ko 

echo initializing driver...
insmod led_driver.ko

echo listing driver...
lsmod

echo Removing previous device...
rm -f /dev/led_driver

echo Making node...
mknod /dev/led_driver c 254 0

echo Changing permissions
chmod a+rwx /dev/led_driver