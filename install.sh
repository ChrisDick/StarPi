#! /bin/bash
#Star Pi
# please do this first:
# remove console=serial0 ore similar from /boot/cmdline.txt
# sudo systemctl stop serial-getty@serial0.service 
# sudo systemctl disable serial-getty@serial0
#sudo apt-get update
#sudo apt-get dist-upgrade
#sudo rpi-update
#sudo raspi-config - serial - need to check this
#reboot
#git clone https://github.com/ChrisDick/StarPi
# cd StarPi
#chmod u+x install.sh
#./install.sh
# then go and make a nice cup of tea.
#sudo sed -i 's/raspberrypi/spacecam/g' /etc/hosts
#sudo sed -i 's/raspberrypi/spacecam/g' /etc/hostname

#sudo raspi-config nonint do_i2c %d


#we have a jessie based install
sudo apt-get -y install scons libncurses5-dev python-dev pps-tools git-core python-smbus i2c-tools

cd ./Software

#Gps deamon
wget http://git.savannah.gnu.org/cgit/gpsd.git/snapshot/gpsd-release-3.16.tar.gz
tar -zxf  gpsd-release-3.16.tar.gz
rm gpsd-release-3.16.tar.gz
cp -r gpsd-release-3.16/* ./Src/GPSD 
rm -r gpsd-release-3.16
cd ./Src/GPSD 
sudo scons && sudo scons check && sudo scons udev-install
sudo ldconfig

#StarPi
cd ./../..
make
# add to library path
#sudo sed -i -e "/usr/local/bin" /etc/ld.so.conf
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
#line above will ask for reboot, this is the last thing we need to do anyway.
#
# usage: start gdps- need to autostart..
#  gpsd -D 5 -N -n /dev/serial0
#  then any of the following depending on how your run it.
#  on it's own
#  ./~/StarPi/Software/Out/StarPi 10001
#  with website
#  ./websockets 1234 ~/StarPi/Software/Out/StarPi 10001
#  
