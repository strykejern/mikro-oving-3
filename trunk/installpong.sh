echo Removing old files
rm -R ./pong/

echo Making new pong folder
mkdir pong

echo Downloading resources...
wget -O ./pong/pong.elf http://folk.ntnu.no/andereie/mikro/mikro-oving-3-read-only/pong/pong.elf
wget -O ./pong/driver_test.elf http://folk.ntnu.no/andereie/mikro/mikro-oving-3-read-only/pong/driver_test.elf
wget -O ./pong/installdriver.sh http://folk.ntnu.no/andereie/mikro/mikro-oving-3-read-only/installdriver.sh

chmod a+rwx ./pong/pong.elf
chmod a+rwx ./pong/driver_test.elf