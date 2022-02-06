#! /bin/bash
#Star Pi install script, tested with a Zero W
# still a lot of work to make this a totally automated installation. see README.md for up to date instructions.
# please do this first:
# From a console do the following.
# if using serial gps, disable shell on serial before connecting the GPS to it.
# for lite install git
#    sudo apt-get -y install git-core
# 
#   sudo raspi-config
#-enable ssh and i2c
#-disable the serial shell and enable the hardware serial
#-set the domain name and boot to cli
#-reduce the graphics memory
#-configure wifi
#-reboot
#    git clone https://github.com/ChrisDick/StarPi
# edit any options in the config to match your choice of sensors.
#    nano StarPi\Software\Src\Config.h
#    cd ~/StarPi
#    chmod u+x install.sh
#    ./install.sh
# then go and make a nice cup of tea. it'll take a little while.
# usage over ssh:
# Any of the following depending on how your run it. ( WMM.COF must be in directory StarPi is started from )
# start GPSD
#     gpsd /dev/serial0
# then
#     cd ~/StarPi/Software
#     sudo ./Out/StarPi 10001

sudo apt-get update
sudo apt-get -y dist-upgrade

#Assume we have a raspian based install
sudo apt-get -y install libnova-dev libcfitsio-dev libusb-1.0-0-dev libjpeg-dev libgsl-dev libcurl4-gnutls-dev cmake gpsd libgps-dev build-essential zlib1g-dev libtiff-dev libfftw3-dev libftdi-dev libraw-dev libdc1394-22-dev libgphoto2-dev libboost-dev libboost-regex-dev librtlsdr-dev liblimesuite-dev libftdi1-dev cdbs dkms fxload libusb-dev libgsl0-dev libkrb5-dev 

#StarPi:
cd ./Software
wget http://www.airspayce.com/mikem/bcm2835/bcm2835-1.71.tar.gz
tar zxvf bcm2835-1.71.tar.gz
cd bcm2835-1.71
./configure
make
sudo make check
sudo make install
cd ..
make

#indi:
#mkdir -p indi
#cd indi
#git clone https://github.com/indilib/indi.git
#git clone https://github.com/indilib/indi-3rdparty
#mkdir -p build/indi-core
#cd build/indi-core
#cmake -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Debug ./../../indi/
#make -j4
#sudo make install
#cd ../..
#mkdir -p indidrivers
#cd indidrivers
#mkdir -p build/indi-starpi
#cd build/indi-starpi
#cmake -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Debug ./../../../../indi-starpi
#make
#sudo cp indi-starpi /usr/bin/indi_starpi
#cd ..
#mkdir indi-gpsd
#cd indi-gpsd
#cmake -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Debug ./../../../indi-3rdparty/indi-gpsd
#make
#sudo make install
#cd ../../../..
#sudo cp ./indi-starpi/indi_starpi_sk.xml /usr/share/indi/indi_starpi_sk.xml



#if you want to use the picam with indi, uncomment this line then add indi_v4l2_ccd when calling indiserver:
#sudo modprobe bcm2835-v4l2

# todo auto start gpsd
#sudo systemctl enable gpsd.socket
#sudo systemctl start gpsd.socket


