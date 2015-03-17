# AstroPi
AstroPi is a Raspberry Pi based telescope server with interface to Stellarium.

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


# Installation
Wiringpi can be installed following.
http://wiringpi.com/download-and-install/

current Pi camera control can be installed following:
http://www.raspberrypi.org/forums/viewtopic.php?t=63276


ToDo: install script

# Design

The design for this system has been done with the community edition of Visual Paradigm and can be found in the Design folder.