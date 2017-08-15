# StarPi
StarPi is a Raspberry Pi based telescope server with interface to Stellarium.

# Installation

 From a console do the following. 
 **Note:** do not use the gui raspi-config it doesn't have the option to enable hardware serial with console disabled.
 configure wifi
>sudo nano /etc/wpa_supplicant/wpa_supplicant.conf
 add the following at the end
>network={
>      ssid="yourssid"
>      psk="yourpassword"
>}
 save the config 
>sudo apt-get update
>sudo apt-get dist-upgrade
>sudo rpi-update
 for lite install git and wiringpi
>sudo apt-get install git-core
>git clone git://git.drogon.net/wiringPi
>cd ~/wiringPi
>./build
 for both jessie variants
>sudo raspi-config 
-turn enable the camera, ssh, i2c 
-disable the serial shell and enable the hardware serial
-set the domain name and boot to cli
-reduce the graphics memory
-reboot
>git clone https://github.com/ChrisDick/StarPi
 edit any options in config.h to match your choice of sensors.
>cd ~/StarPi
>chmod u+x install.sh
>./install.sh
 then go and make a nice cup of tea. it'll take a little while.

# Use
 over ssh: 
 Any of the following depending on how your run it. ( WMM.COF must be in directory StarPi is started from )
 start GPSD in it's own ssh session
 B+
>gpsd -D 5 -N -n /dev/ttyAMA0
 on the zero (W)
>gpsd -D 5 -N -n /dev/serial0 
 in another ssh session
 on it's own 
>cd ~/StarPi/Software

>./Out/StarPi 10001
 with website
>cd ~/StarPi/Software
>./websockets 1234 /Out/StarPi 10001
 
# Design

The design for this system has been done with the community edition of Visual Paradigm and can be found in the Design folder.

The system,

Shall calculate the position the telescope is pointing to on the sky map from measurements taken from Accelerometer and Magnetometer sensors.
Shall send position to Stellarium via TCP/IP.
Shall Receive GoTo commands via TCP/IP from Stellarium.
Shall Receive User inputs from a web interface
May receive user inputs from GPIO.
May Move the Telescope to change the position on the skymap
May adjust focus of The image.
Shall Control a Pi camera to take photos and videos.
May display current position, target position, mode and menus on an LCD display.
May act as a Wireless access point with DHCP server to allow access to the server from Stellarium or a web browser on another wireless device. This will allow the system to be used without any network present.

