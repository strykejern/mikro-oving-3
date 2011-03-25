wget -O led_driver.ko http://mikro-oving-3.googlecode.com/svn/trunk/driver/led_driver.ko 
rmmod led_driver.ko
insmod led_driver.ko
lsmod