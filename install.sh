#! /bin/bash
#Star Pi install script, tested with a Zero W
# still a lot of work to make this a totally automated installation. see README.md for up to date instructions.
# please do this first:
# From a console do the following.
# if using serial gps, disable shell on serial before connecting the GPS to it.
# 
#   sudo raspi-config
#-enable ssh and i2c
#-disable the serial shell and enable the hardware serial
#-set the domain name and boot to cli
#-reduce the graphics memory
#-configure wifi
#-reboot
# for lite install git
#    sudo apt-get -y install git-core
# then clone the repository:
#    git clone https://github.com/ChrisDick/StarPi
# edit any options in the config to match your choice of sensors.
#    nano StarPi\Software\Src\Config.h
#    cd ~/StarPi
#    chmod u+x install.sh
#    ./install.sh
# then go and make a nice cup of tea. it'll take a little while.
# usage over ssh:
# Any of the following depending on how your run it. ( WMM.COF must be in directory StarPi is started from )
#     cd ~/StarPi/Software
#     sudo ./Out/StarPi 10001

now=$(date +"%T")
echo "Start time : $now"

sudo apt-get update
sudo apt-get -y dist-upgrade

#Assume we have a raspian based install
sudo apt-get -y install \
  build-essential \
  cdbs \
  cmake \
  dkms \
  fxload \
  gpsd \
  libboost-dev \
  libboost-regex-dev \
  libcfitsio-dev \
  libcurl4-gnutls-dev \
  libdc1394-dev \
  liberfa-dev \
  libev-dev \
  libevdev-dev \
  libfftw3-dev \
  libftdi-dev \
  libftdi1-dev \
  libgphoto2-dev \
  libgps-dev \
  libgsl-dev \
  libgsl0-dev \
  libjpeg-dev \
  libkrb5-dev \
  liblimesuite-dev \
  libnova-dev \
  libraw-dev \
  librtlsdr-dev \
  libtheora-dev \
  libtiff-dev \
  libusb-1.0-0-dev \
  libusb-dev \
  libxisf-dev \
  ninja-build \
  zlib1g-dev \
  libindi-dev \
  indi-bin \

#StarPi:
cd ./Software
wget http://www.airspayce.com/mikem/bcm2835/bcm2835-1.71.tar.gz
tar zxvf bcm2835-1.71.tar.gz
rm bcm2835-1.71.tar.gz
cd bcm2835-1.71
./configure
make
sudo make check
sudo make install
cd ..
make

#indi:
mkdir -p indi
cd indi
#match 3rd party version with package version.
VERSION=$(apt policy libindi-dev | awk '/Installed:/ {print $2}' | sed 's/^[0-9]://;s/-.*//')
MAJOR=$(echo "$VERSION" | cut -d. -f1)
MINOR=$(echo "$VERSION" | cut -d. -f2)
PATCH=$(echo "$VERSION" | cut -d. -f3)

wget https://github.com/indilib/indi-3rdparty/archive/refs/tags/v$MAJOR.$MINOR.$PATCH.tar.gz
tar -xzvf v$MAJOR.$MINOR.$PATCH.tar.gz   
rm v$MAJOR.$MINOR.$PATCH.tar.gz
mv indi-3rdparty-$MAJOR.$MINOR.$PATCH indi-3rdparty

mkdir -p indidrivers
cd indidrivers
mkdir -p build/indi-starpi
cd build/indi-starpi
cmake -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Debug ./../../../../indi-starpi
make

sudo cp indi-starpi /usr/bin/indi_starpi
cd ..
mkdir indi-gpsd
cd indi-gpsd
cmake -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Debug ./../../../indi-3rdparty/indi-gpsd
make
sudo make install
cd ../../../..
sudo cp ./indi-starpi/indi_starpi_sk.xml /usr/share/indi/indi_starpi_sk.xml

echo "Start time : $now"
now=$(date +"%T")
echo "Finish time : $now"

#if you want to use the picam with indi, uncomment this line then add indi_v4l2_ccd when calling indiserver:
#sudo modprobe bcm2835-v4l2



