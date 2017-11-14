#! /bin/bash
#Star Pi install script, tested with a Zero W
# still a lot of work to make this a totally automated installation.
# please do this first:
# From a console do the following.
# Note: do not use the gui raspi-config it doesn't have the option to enable hardware serial with console disabled and, if using serial gps, disable shell on serial before connecting the GPS to it.
# configure wifi
#    sudo nano /etc/wpa_supplicant/wpa_supplicant.conf
# add the following at the end
# network={
#         ssid="yourssid"
#         psk="yourpassword"
#}
# save the config
#    sudo apt-get update
#    sudo apt-get -y dist-upgrade
#    sudo rpi-update
# for lite install git and wiringpi
#    sudo apt-get -y install git-core
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
# edit any options in the config to match your choice of sensors.
#    nano StarPi/Software/Src/Config.h
#    cd ~/StarPi
#    chmod u+x install.sh
#    ./install.sh
# then go and make a nice cup of tea. it'll take a little while.

# usage over ssh:
# Any of the following depending on how your run it. ( WMM.COF must be in directory StarPi is started from )
# start GPSD in it's own ssh session
#     gpsd -D 5 -N -n /dev/serial0
# in another ssh session
# on it's own
#     cd ~/StarPi/Software
#     ./Out/StarPi 10001
# with website
#     cd ~/StarPi/Software
#     ./websocketd port=1234 Out/StarPi 10001

#Assume we have a jessie based install
sudo apt-get -y install scons libncurses5-dev python-dev pps-tools git-core python-smbus i2c-tools
cd ./Software
chmod u+x websocketd

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
cd ..

#rpicam interface:
git clone https://github.com/silvanmelchior/RPi_Cam_Web_Interface.git
sudo ./RPi_Cam_Web_Interface/install.sh
#sudo ./RPi_Cam_Web_Interface/install.sh q
#sudo mkdir /var/www/RPiCam
#sudo mv /var/www/html/* /var/www/RPiCam
#sudo mkdir /var/www/StarPi
#sudo cp -ar /var/www/RPiCam/* /var/www/StarPi
#sudo cp -ar ./Website/* /var/www/StarPi
sudo cp -ar ./Website/* /var/www/html

# ready after reboot
sudo reboot

#copy executables to somewhere in the PATH - may noy use due to WMM.COF
#sudo cp ./Out/StarPi /usr/local/bin/StarPi
#sudo cp  websocketd /usr/local/bin/websocketd
#sudo chmod u+x /usr/local/bin/websocketd
#sudo cp WMM.COF /usr/local/bin/WMM.COF
