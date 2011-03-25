wget -O led.ko http://mikro-oving-3.googlecode.com/svn/trunk/driver/led.ko 
rmmod led.ko
insmod led.ko
lsmod