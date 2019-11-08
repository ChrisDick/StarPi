# StarPi
StarPi is a Raspberry Pi based telescope server with interface to Stellarium. It uses a triple axis accelerometer and compass with GPS to calculate the position in the sky the telescope is pointing to.

More details can be found at https://hackaday.io/project/10181-starpi

# Installation

 From a console do the following: 

    sudo raspi-config 

  Enable ssh,i2c and Serial with the shell disabled.
  Enter your WiFi details  
  disable the serial shell and enable the hardware serial.  
  set the domain name and boot to cli.  
  reduce the graphics memory.  
  reboot.  
 
 Get the most recent version. 

    sudo apt-get update
    sudo apt-get dist-upgrade
    sudo rpi-upgrade
    sudo reboot
    
 for raspbian lite versions install git:

    sudo apt-get -y install git-core
    
 Clone the Source:

    git clone https://github.com/ChrisDick/StarPi

  edit any options in  to match your choice of sensors and install.

    nano StarPi/Software/Src/config.h
    cd ~/StarPi
    chmod u+x install.sh
    ./install.sh

Then go and make a nice cup of tea. it'll take a little while.

# Use
  over ssh:  
  Any of the following depending on how your run it. ( WMM.COF must be in directory StarPi is started from )  
  start GPSD in it's own ssh session  

    gpsd -D 5 -N -n /dev/serial0 

  in another ssh session  

    cd ~/StarPi/Software
    ./Out/StarPi 10001

 
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

