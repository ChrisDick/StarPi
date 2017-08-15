#! /bin/bash
#Star Pi install script, tested with a Zero W
# still a lot of work to make this a totally automated installation.
# please do this first:
# From a console do the following. 
# Note: do not use the gui raspi-config it doesn't have the option to enable hardware serial with console disabled.
# configure wifi
#    sudo nano /etc/wpa_supplicant/wpa_supplicant.conf
# add the following at the end
# network={
#         ssid="yourssid"
#         psk="yourpassword"
#}
# save the config
#    sudo apt-get update
#    sudo apt-get dist-upgrade
#    sudo rpi-update
# for lite install git and wiringpi
#    sudo apt-get install git-core
#    git clone git://git.drogon.net/wiringPi
#    cd ~/wiringPi
#    ./build
# for both jessie variants
#    sudo raspi-config 
#-turn enable the camera, ssh, i2c 
#-disable the serial shell and enable the hardware serial
#-set the domain name and boot to cli
#-reduce the graphics memory
#-reboot
#    git clone https://github.com/ChrisDick/StarPi
# edit any options in config.h to match your choice of sensors.
#    cd ~/StarPi
#    chmod u+x install.sh
#    ./install.sh
# then go and make a nice cup of tea. it'll take a little while.

# usage over ssh: 
# Any of the following depending on how your run it. ( WMM.COF must be in directory StarPi is started from )
# start GPSD in it's own ssh session
# B+
#     gpsd -D 5 -N -n /dev/ttyAMA0
# on the zero (W)
#     gpsd -D 5 -N -n /dev/serial0 
# in another ssh session
# on it's own 
#     cd ~/StarPi/Software
#     ./Out/StarPi 10001
# with website
#     cd ~/StarPi/Software
#     ./websockets 1234 /Out/StarPi 10001

#Assume we have a jessie based install
sudo apt-get -y install scons libncurses5-dev python-dev pps-tools git-core python-smbus i2c-tools
cd ./Software

#Gps deamon:
wget http://git.savannah.gnu.org/cgit/gpsd.git/snapshot/gpsd-release-3.16.tar.gz
tar -zxf  gpsd-release-3.16.tar.gz
rm gpsd-release-3.16.tar.gz
cp -r gpsd-release-3.16/* ./Src/GPSD 
rm -r gpsd-release-3.16
cd ./Src/GPSD 
sudo scons && sudo scons check && sudo scons udev-install
sudo ldconfig
# todo auto start gpsd
#sudo systemctl enable gpsd.socket
#sudo systemctl start gpsd.socket

#StarPi:
cd ./../..
make
#copy executables to somewhere in the PATH - may noy use due to WMM.COF
sudo cp ./Out/StarPi /usr/local/bin/StarPi
sudo cp  websocketd /usr/local/bin/websocketd
sudo chmod u+x /usr/local/bin/websocketd
sudo cp WMM.COF /usr/local/bin/WMM.COF
cd ..

#rpicam interface:
git clone https://github.com/silvanmelchior/RPi_Cam_Web_Interface.git
cp RPi_Cam_config.txt ./RPi_Cam_Web_Interface/config.txt
sudo mkdir /var/www
sudo mkdir /var/www/html
sudo mkdir /var/www/html/StarPi
sudo mkdir /var/www/html/RPiCam
sudo cp -ar ./Website/* /var/www/html/StarPi
cd RPi_Cam_Web_Interface
sudo cp -ar ./www/* /var/www/html/RPiCam
chmod u+x *.sh
sudo ./install.sh q
sudo reboot
