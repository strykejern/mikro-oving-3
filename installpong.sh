echo Uninitializing old drivers...
rmmod pong/stk1000_driver.ko

echo Downloading resources...
wget -O /pong/stk1000_driver.ko http://mikro-oving-3.googlecode.com/svn/trunk/driver/stk1000_driver.ko 
wget -O /pong/pong.elf http://mikro-oving-3.googlecode.com/svn/trunk/pong/pong.elf

echo Initializing custom STK1000 driver...
insmod /pong/stk1000_driver.ko

echo Listing loaded drivers...
lsmod

echo Removing previous device...
rm -f /dev/stk1000_driver

echo Making node...
mknod /dev/stk1000_driver c 254 0

echo Changing permissions...
chmod a+rwx /dev/stk1000_driver
chmod a+rwx /pong/pong.elf