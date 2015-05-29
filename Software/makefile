all:		Main Sample_Main

INCLUDE_PATHS := 	-I./Src/I2Cdevlib/Pi/I2Cdev/ \
					-I./Src/I2Cdevlib/Pi/HMC5883L/ \
					-I./Src/I2Cdevlib/Pi/ADXL345/ \
					-I./Src/I2Cdevlib/Pi/AK8975/ \
					-I./Src/I2Cdevlib/Pi/BMA150/ \
					-I./Src/I2Cdevlib/Pi/HMC5843/ \
					-I./Src/I2Cdevlib/Pi/MPU6050/ \
					-I./Src/I2Cdevlib/Pi/MPU9150/ \
					-I./Src/Hal/ \
					-I./Src/TelescopeManager/ \
					-I./Src/Scheduler/ \
					-I./Src/StellariumServer/ \
					-I./Src/

OBJ_DIR = ./Obj/

DEVICE_OBJS := $(addprefix $(OBJ_DIR), \
					ADXL345.o \
					AK8975.o \
					BMA150.o \
					HMC5843.o \
					HMC5883L.o \
					MPU6050.o \
					MPU9150.o \
					I2Cdev.o \
					Connection.o \
					Listener.o \
					Server.o \
					Socket.o \
					HalCompass.o \
					HalAccelerometer.o \
					TTC_Sched.o \
					Runnable.o \
					ServerPi.o \
					TelescopeManager.o \
					CelestrialConverter.o \
					TTC_Sched_Pi_Impl.o \
					)

PI_DIR = ../Src/I2Cdevlib/Pi/
CFLAGS =  -c -Wall -lwiringPi 
CC = g++
OUTPUT = -o $@
#todo change output to out directory
MPU6050:	$(OBJ_DIR)MPU6050_raw.o $(OBJ_DIR)MPU6050.o  $(OBJ_DIR)I2Cdev.o | $(DEVICE_OBJS)
		$(CC) -Wall -lwiringPi $(OUTPUT) $(OBJ_DIR)MPU6050_raw.o $(DEVICE_OBJS)

HMC5883L_heading:	$(OBJ_DIR)HMC5883L_raw.o $(DEVICE_OBJS)
		$(CC) -Wall -lwiringPi $(OUTPUT) $(OBJ_DIR)HMC5883L_raw.o $(DEVICE_OBJS)
		
Sample_Main:	$(OBJ_DIR)Sample_Main.o $(OBJ_DIR)TTC_Sched.o  $(OBJ_DIR)Runnable.o $(OBJ_DIR)TTC_Sched_Pi_Impl.o
		$(CC) -Wall -lwiringPi $(OUTPUT) $(OBJ_DIR)Sample_Main.o $(DEVICE_OBJS)

Main:	$(OBJ_DIR)Main.o $(DEVICE_OBJS)
		$(CC) -Wall -lwiringPi $(OUTPUT) $(OBJ_DIR)Main.o $(DEVICE_OBJS)

#todo create obj dir if it doesn't exist
#$(DEVICE_OBJS): | $(OBJDIR)
    

#$(OBJ_DIR):
#		mkdir $(OBJ_DIR)


.PHONY: clean
clean:
		rm $(OBJ_DIR)*.o 
		
$(OBJ_DIR)Main.o:		./Src/Main.cpp
		$(CC) $(CFLAGS) $(INCLUDE_PATHS) $(OUTPUT) ./Src/Main.cpp

$(OBJ_DIR)TelescopeManager.o:		./Src/TelescopeManager/TelescopeManager.cpp
		$(CC) $(CFLAGS) $(INCLUDE_PATHS) $(OUTPUT) ./Src/TelescopeManager/TelescopeManager.cpp

$(OBJ_DIR)Connection.o:		./Src/StellariumServer/Connection.cpp
		$(CC) $(CFLAGS) $(INCLUDE_PATHS) $(OUTPUT) ./Src/StellariumServer/Connection.cpp
		
$(OBJ_DIR)Listener.o:		./Src/StellariumServer/Listener.cpp
		$(CC) $(CFLAGS) $(INCLUDE_PATHS) $(OUTPUT) ./Src/StellariumServer/Listener.cpp
		
$(OBJ_DIR)LogFile.o:		./Src/StellariumServer/LogFile.cpp
		$(CC) $(CFLAGS) $(INCLUDE_PATHS) $(OUTPUT) ./Src/StellariumServer/LogFile.cpp
		
$(OBJ_DIR)Server.o:		./Src/StellariumServer/Server.cpp
		$(CC) $(CFLAGS) $(INCLUDE_PATHS) $(OUTPUT) ./Src/StellariumServer/Server.cpp
		
$(OBJ_DIR)ServerPi.o:		./Src/StellariumServer/ServerPi.cpp
		$(CC) $(CFLAGS) $(INCLUDE_PATHS) $(OUTPUT) ./Src/StellariumServer/ServerPi.cpp
		
$(OBJ_DIR)Socket.o:		./Src/StellariumServer/Socket.cpp
		$(CC) $(CFLAGS) $(INCLUDE_PATHS) $(OUTPUT) ./Src/StellariumServer/Socket.cpp
		
$(OBJ_DIR)CelestrialConverter.o:		./Src/TelescopeManager/CelestrialConverter.cpp
		$(CC) $(CFLAGS) $(INCLUDE_PATHS) $(OUTPUT) ./Src/TelescopeManager/CelestrialConverter.cpp
		
$(OBJ_DIR)Sample_Main.o:		./Src/Scheduler/Sample_Main.cpp
		$(CC) $(CFLAGS) $(INCLUDE_PATHS) $(OUTPUT) ./Src/Scheduler/Sample_Main.cpp

$(OBJ_DIR)TTC_Sched.o:		./Src/Scheduler/TTC_Sched.cpp
		$(CC) $(CFLAGS) $(INCLUDE_PATHS) $(OUTPUT) ./Src/Scheduler/TTC_Sched.cpp
		
$(OBJ_DIR)TTC_Sched_Pi_Impl.o:	./Src/Scheduler/TTC_Sched_Pi_Impl.cpp
		$(CC) $(CFLAGS) $(INCLUDE_PATHS) $(OUTPUT) ./Src/Scheduler/TTC_Sched_Pi_Impl.cpp

$(OBJ_DIR)Runnable.o:		./Src/Scheduler/Runnable.cpp
		$(CC) $(CFLAGS) $(INCLUDE_PATHS) $(OUTPUT) ./Src/Scheduler/Runnable.cpp
		
$(OBJ_DIR)HalAccelerometer.o:	./Src/Hal/HalAccelerometer.cpp
		$(CC) $(CFLAGS) $(INCLUDE_PATHS) $(OUTPUT) ./Src/Hal/HalAccelerometer.cpp 
		
$(OBJ_DIR)HalCompass.o:	./Src/Hal/HalCompass.cpp
		$(CC) $(CFLAGS) $(INCLUDE_PATHS) $(OUTPUT) ./Src/Hal/HalCompass.cpp 
		
$(OBJ_DIR)HMC5883L_raw.o:		./Src/I2Cdevlib/Pi/HMC5883L/Examples/HMC5883L_raw/HMC5883L_raw.cpp
		$(CC) $(CFLAGS) $(INCLUDE_PATHS) $(OUTPUT) ./Src/I2Cdevlib/Pi/HMC5883L/Examples/HMC5883L_raw/HMC5883L_raw.cpp 
		
$(OBJ_DIR)MPU6050_raw.o:		./Src/I2Cdevlib/Pi/MPU6050/Examples/MPU6050_raw/MPU6050_raw.cpp
		$(CC) $(CFLAGS) $(INCLUDE_PATHS) $(OUTPUT) ./Src/I2Cdevlib/Pi/MPU6050/Examples/MPU6050_raw/MPU6050_raw.cpp
		
$(OBJ_DIR)I2Cdev.o:	./Src/I2Cdevlib/Pi/I2Cdev/I2Cdev.cpp 
		$(CC) $(CFLAGS) $(INCLUDE_PATHS) $(OUTPUT) ./Src/I2Cdevlib/Pi/I2Cdev/I2Cdev.cpp 
		
$(OBJ_DIR)HMC5883L.o:	./Src/I2Cdevlib/Pi/HMC5883L/HMC5883L.cpp 
		$(CC) $(CFLAGS) $(INCLUDE_PATHS) $(OUTPUT) ./Src/I2Cdevlib/Pi/HMC5883L/HMC5883L.cpp 
		
$(OBJ_DIR)AD7746.o:	./Src/I2Cdevlib/Pi/AD7746/AD7746.cpp  
		$(CC) $(CFLAGS) $(INCLUDE_PATHS) $(OUTPUT) ./Src/I2Cdevlib/Pi/AD7746/AD7746.cpp  
		
$(OBJ_DIR)ADS1115.o:	./Src/I2Cdevlib/Pi/ADS1115/ADS1115.cpp 
		$(CC) $(CFLAGS) $(INCLUDE_PATHS) $(OUTPUT) ./Src/I2Cdevlib/Pi/ADS1115/ADS1115.cpp 
		
$(OBJ_DIR)ADXL345.o:	./Src/I2Cdevlib/Pi/ADXL345/ADXL345.cpp
		$(CC) $(CFLAGS) $(INCLUDE_PATHS) $(OUTPUT) ./Src/I2Cdevlib/Pi/ADXL345/ADXL345.cpp 
		
$(OBJ_DIR)AK8975.o:	./Src/I2Cdevlib/Pi/AK8975/AK8975.cpp
		$(CC) $(CFLAGS) $(INCLUDE_PATHS) $(OUTPUT) ./Src/I2Cdevlib/Pi/AK8975/AK8975.cpp 
		
$(OBJ_DIR)BMA150.o:	./Src/I2Cdevlib/Pi/BMA150/BMA150.cpp
		$(CC) $(CFLAGS) $(INCLUDE_PATHS) $(OUTPUT) ./Src/I2Cdevlib/Pi/BMA150/BMA150.cpp 
		
$(OBJ_DIR)BMP085.o:	./Src/I2Cdevlib/Pi/BMP085/BMP085.cpp
		$(CC) $(CFLAGS) $(INCLUDE_PATHS) $(OUTPUT) ./Src/I2Cdevlib/Pi/BMP085/BMP085.cpp 
		
$(OBJ_DIR)MC5843.o:	./Src/I2Cdevlib/Pi/HMC5843/HMC5843.cpp 
		$(CC) $(CFLAGS) $(INCLUDE_PATHS) $(OUTPUT) ./Src/I2Cdevlib/Pi/HMC5843/HMC5843.cpp 
		
$(OBJ_DIR)ITG3200.o: ./Src/I2Cdevlib/Pi/ITG3200/ITG3200.cpp 
		$(CC) $(CFLAGS) $(INCLUDE_PATHS) $(OUTPUT) ./Src/I2Cdevlib/Pi/ITG3200/ITG3200.cpp
		
$(OBJ_DIR)L3G4200D.o:	./Src/I2Cdevlib/Pi/L3G4200D/L3G4200D.cpp 
		$(CC) $(CFLAGS) $(INCLUDE_PATHS) $(OUTPUT) ./Src/I2Cdevlib/Pi/L3G4200D/L3G4200D.cpp 

$(OBJ_DIR)LM73.o:		./Src/I2Cdevlib/Pi/LM73/LM73.cpp 
		$(CC) $(CFLAGS) $(INCLUDE_PATHS) $(OUTPUT) ./Src/I2Cdevlib/Pi/LM73/LM73.cpp 
		
$(OBJ_DIR)MPR121.o:	./Src/I2Cdevlib/Pi/MPR121/MPR121.cpp 
		$(CC) $(CFLAGS) $(INCLUDE_PATHS) $(OUTPUT) ./Src/I2Cdevlib/Pi/MPR121/MPR121.cpp 

$(OBJ_DIR)MPU6050.o:	./Src/I2Cdevlib/Pi/MPU6050/MPU6050.cpp
		$(CC) $(CFLAGS) $(INCLUDE_PATHS) $(OUTPUT) ./Src/I2Cdevlib/Pi/MPU6050/MPU6050.cpp 
		
$(OBJ_DIR)MPU9150.o:	./Src/I2Cdevlib/Pi/MPU9150/MPU9150.cpp
		$(CC) $(CFLAGS) $(INCLUDE_PATHS) $(OUTPUT) ./Src/I2Cdevlib/Pi/MPU9150/MPU9150.cpp 
		
$(OBJ_DIR)TCA6424A.o:	./Src/I2Cdevlib/Pi/TCA6424A/TCA6424A.cpp
		$(CC) $(CFLAGS) $(INCLUDE_PATHS) $(OUTPUT) ./Src/I2Cdevlib/Pi/TCA6424A/TCA6424A.cpp
		
$(OBJ_DIR)DS1307.o:	./Src/I2Cdevlib/Pi/DS1307/DS1307.cpp 
		$(CC) $(CFLAGS) $(INCLUDE_PATHS) $(OUTPUT) ./Src/I2Cdevlib/Pi/DS1307/DS1307.cpp 
		
$(OBJ_DIR)SSD1308.o:	./Src/I2Cdevlib/Pi/SSD1308/SSD1308.cpp
		$(CC) $(CFLAGS) $(INCLUDE_PATHS) $(OUTPUT) ./Src/I2Cdevlib/Pi/SSD1308/SSD1308.cpp
		
$(OBJ_DIR)IAQ2000.o:	./Src/I2Cdevlib/Pi/IAQ2000/IAQ2000.cpp
		$(CC) $(CFLAGS) $(INCLUDE_PATHS) $(OUTPUT) ./Src/I2Cdevlib/Pi/IAQ2000/IAQ2000.cpp 
		
$(OBJ_DIR)HMC5843.o:	./Src/I2Cdevlib/Pi/HMC5843/HMC5843.cpp
		$(CC) $(CFLAGS) $(INCLUDE_PATHS) $(OUTPUT) ./Src/I2Cdevlib/Pi/HMC5843/HMC5843.cpp
		
