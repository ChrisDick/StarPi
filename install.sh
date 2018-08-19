#! /bin/bash
#Star Pi install script, tested with a Zero W
# still a lot of work to make this a totally automated installation. see README.md for up to date instructions.
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
#-enable ssh and i2c
#-disable the serial shell and enable the hardware serial
#-set the domain name and boot to cli
#-reduce the graphics memory
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
# start GPSD in it's own ssh session
#     gpsd -D 5 -N -n /dev/serial0
# in another ssh session
#     cd ~/StarPi/Software
#     ./Out/StarPi 10001

#Assume we have a raspian based install
sudo apt-get -y install libnova-dev libcfitsio-dev libusb-1.0-0-dev libjpeg-dev libgsl-dev libcurl4-gnutls-dev cmake gpsd      
#sudo apt-get -y install libnova-dev libcfitsio-dev libusb-1.0-0-dev libjpeg-dev libgsl-dev libcurl4-gnutls-dev cmake zlib1g-dev build-essential  

#cdbs libusb-dev libtiff5-dev  fxload libkrb5-dev  dkmslibdc1394-22-dev

#all dependencies for thrid party
#libgps-dev libftdi1-dev libraw-dev libgphoto2-dev libboost-dev libboost-regex-dev librtlsdr-dev libfftw3-dev libftdi-dev      

#sudo apt-get install -y --reinstall libtheora0
#sudo apt-get --fix-broken install


#StarPi:
cd ./Software
make

#indi
cd ../..
git clone https://github.com/indilib/indi.git
cd indi
mkdir -p build/libindi
cd build/libindi
cmake -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Debug ../../libindi
make
sudo make install
cd ./../..
cp -r ./../StarPi/Software/indi-starpi ./3rdparty/indi-starpi
mkdir -p build/indi-starpi
cd build/indi-starpi
cmake -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Debug ./../../3rdparty/indi-starpi
make
sudo cp indi-starpi /usr/bin/indi_starpi
sudo cp ./../../3rdparty/indi-starpi/indi_starpi_sk.xml /usr/share/indi/indi_starpi_sk.xml
cd ..
mkdir indi-gpsd
cd indi-gpsd
cmake -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Debug ./../../3rdparty/indi-gpsd
make
sudo make install


# todo auto start gpsd
#sudo systemctl enable gpsd.socket
#sudo systemctl start gpsd.socket
