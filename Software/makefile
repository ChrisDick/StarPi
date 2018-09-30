all :

#substitutions

INCLUDE_PATHS := 	-I./Src/I2Cdevlib/Pi/I2Cdev/ \
					-I./Src/I2Cdevlib/Pi/BME280/ \
					-I./Src/I2Cdevlib/Pi/LSM303DLHC/ \
					-I./Src/Hal/ \
					-I./Src/TelescopeManager/ \
					-I./Src/Scheduler/ \
					-I./Src/StellariumServer/ \
					-I./Src/GPSD/ \
					-I./Src/MagModelCorrection/ \
					-I./Src/SOFA/ \
					-I./Src/ \
					-I./Src/Utils/ \
					-I./Src/Drivers/ 

CPPFILES := 		Src/Main.cpp \
					Src/I2Cdevlib/Pi/BME280/BME280.cpp \
					Src/I2Cdevlib/Pi/LSM303DLHC/LSM303DLHC.cpp \
					Src/I2Cdevlib/Pi/I2Cdev/I2Cdev.cpp \
					Src/StellariumServer/Connection.cpp \
					Src/StellariumServer/Listener.cpp \
					Src/StellariumServer/Server.cpp \
					Src/StellariumServer/Socket.cpp \
					Src/StellariumServer/ServerPi.cpp \
					Src/TelescopeManager/TelescopeOrientation.cpp \
					Src/TelescopeManager/TelescopeSocket.cpp \
					Src/Hal/HalGps.cpp \
					Src/Hal/HalAccelerometer.cpp \
					Src/Hal/HalMagnetometer.cpp \
					Src/Hal/HalSocket.cpp \
					Src/Hal/HalFilesystem.cpp \
					Src/Hal/HalWeather.cpp \
					Src/Drivers/GPIO.cpp \
					Src/Scheduler/TTC_Sched.cpp \
					Src/Scheduler/Runnable.cpp \
					Src/TelescopeManager/TelescopeManager.cpp \
					Src/TelescopeManager/erfa.cpp \
					Src/Scheduler/TTC_Sched_Pi_Impl.cpp \
					Src/MagModelCorrection/MagModel.cpp

CFILES :=			Src/MagModelCorrection/GeomagnetismLibrary.c

OUTDIR := Out
OBJDIR := Obj
OUT_DIR = ./Out/
CFLAGS =  -std=c++0x -c -pedantic -Wall -W -O -lwiringPi -lgps 
CC = g++
OUTPUT = -o $@

#Object list generation
target = ${OBJDIR}/$(patsubst %.c,%.o, $(patsubst %.cpp,%.o,$(notdir ${1})))
obj.c :=
obj.cpp :=

define obj
  $(call target,${1}) : ${1} | ${OBJDIR}
  obj$(suffix ${1}) += $(call target,${1})
endef

define SOURCES
  $(foreach src,${1},$(eval $(call obj,${src})))
endef

$(eval $(call SOURCES,${CFILES}))
$(eval $(call SOURCES,${CPPFILES}))

#Recipes
all : $(OUT_DIR)StarPi   

$(OUT_DIR)StarPi:	${obj.cpp} ${obj.c} ${OUTDIR}
	@echo link files..
	$(CC) -Wall -lwiringPi -lgps $(OUTPUT) ${obj.cpp} ${obj.c}  >> log.txt 2>&1

%.o : 
	@echo compiling $@
	$(CC) -c $(CFLAGS) $(INCLUDE_PATHS) $< $(OUTPUT)  >> log.txt 2>&1

.PHONY: clean
clean:
		rm $(OBJDIR)/*.o 

.PHONY: cleansched
cleansched:
		rm $(OBJDIR)/TTC_Sched.o 
		rm $(OBJDIR)/TTC_Sched_Pi_Impl.o 

.PHONY: cleandata
cleandata:
		rm $(OBJDIR)/TelescopeManager.o 
		rm $(OBJDIR)/TelescopeIO.o 
		rm $(OBJDIR)/CelestrialConverter.o 
		rm $(OBJDIR)/HalWebsocketd.o 

${OUTDIR} :
	mkdir $@

${OBJDIR} :
	mkdir $@

.PHONY: all
