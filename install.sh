#! /bin/bash
#Star Pi
# please do this first:
#git clone https://github.com/ChrisDick/StarPi
#chmod u+x install.sh
#./install.sh
# then go and make a nice cup of tea.
#sudo sed -i 's/raspberrypi/spacecam/g' /etc/hosts
#sudo sed -i 's/raspberrypi/spacecam/g' /etc/hostname

#we have a jessie based install
sudo apt-get install scons 
sudo apt-get install libgps-dev

#sudo apt-get update
#sudo apt-get dist-upgrade
#sudo rpi-update
sudo apt-get -y install libncurses5-dev python-dev pps-tools git-core python-smbus i2c-tools

cd ./Software

#Gps deamon
wget http://git.savannah.gnu.org/cgit/gpsd.git/snapshot/gpsd-master.tar.gz
tar -zxf  gpsd-master.tar.gz

rm gpsd-master.tar.gz

cp -r gpsd-master/* ./Src/GPSD 
rm -r gpsd-master

cd ./Src/GPSD 
sudo scons && sudo scons check && sudo scons udev-install

cd ./../..
mkdir Obj
mkdir Out
make
# add to library path
sudo sed -i -e "/usr/local/bin" /etc/ld.so.conf
sudo ldconfig
#copy executables to somewhere in the PATH

sudo cp ./Out/StarPi /usr/local/bin/StarPi
sudo cp  websocketd /usr/local/bin/websocketd
sudo chmod u+x /usr/local/bin/websocketd
sudo cp WMM.COF /usr/local/bin/WMM.COF
cd ..
#rpicam interface
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
#line above will ask for reboot, this is the last thing we need to do.
