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
 
 for raspbian lite versions install git:

    sudo apt-get -y install git
    
 Clone the Source:

    git clone https://github.com/ChrisDick/StarPi

  edit any options in to match your choice of sensors and install.

    nano StarPi/Software/Src/Config.h
    cd ~/StarPi
    chmod u+x install.sh
    ./install.sh

Then go and make a nice cup of tea. it'll take a little while.

# Use
  Intended on being run remotely over ssh:  
  WMM.COF must be in directory StarPi is started from, in this case ~/StarPi/Software

    cd ~/StarPi/Software
    ./Out/StarPi 10001

  The only current option is the port number for connection with Stellarium. The Stellarium default of 10001 is used in this example.
  
  To connect with Stellarium, open the telescope move dialog and configure a telescope. Add a new telescope and chose external software on a remote computer. Name the telescope, add the IP address of StarPi and set the port number to match the option given.
  
  To use indi to connect to StarPi, once StarPi is started, start the indiserver.
  
    indiserver -v indi_starpi
  
  Since StarPi uses a gpsd to get GPS data, it is possible to run indi_gpsd along side StarPi
  
    indiserver -v indi_starpi indi_gpsd
  
  