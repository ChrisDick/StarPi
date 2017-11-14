all:
#APPLICATION = lsm303test

#substitutions

INCLUDE_PATHS := 	-I./../../../../../I2Cdevlib/Pi/I2Cdev/ \
					-I./../../../../../I2Cdevlib/Pi/LSM303DLHC/ \
					-I./../../../../../Hal/ \
					-I./../../../../../TelescopeManager/ \
					-I./../../../../../  

CPPFILES := 		./../../LSM303DLHC.cpp \
					./../../../I2Cdev/I2Cdev.cpp \
					./Main.cpp 

CFILES :=			

OUTDIR := Out
OBJDIR := Obj
OUT_DIR = ./Out/
CFLAGS =  -c -pedantic -Wall -W -O 
LIBRARIES = -lwiringPi 
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

#Use master makefile to do the work
#include ../../../../../../master.mk

#Recipes
all : $(OUT_DIR)lsm303test

#$(OUT_DIR)$(APPLICATION): 	${obj.cpp} ${obj.c} ${OUTDIR} 
#	echo $(CPPFILES)
#	$(CC) -Wall $(LIBRARIES) $(OUTPUT) ${obj.cpp} ${obj.c} >> log.txt 2>&1

%.o : 
	@echo compiling $@
	echo $(obj.cpp)
	$(CC) -c $(CFLAGS) $(INCLUDE_PATHS) $(DEFINES) $< $(OUTPUT)  >> log.txt 2>&1

$(OUT_DIR)lsm303test: 	${obj.cpp} ${obj.c} ${OUTDIR} 
	$(CC) -Wall -lwiringPi -lgps $(OUTPUT) ${obj.cpp} ${obj.c} >> log.txt 2>&1

.PHONY: clean
clean:
		rm $(OBJDIR)/*.o 

${OUTDIR} :
	mkdir $@

${OBJDIR} :
	mkdir $@

.PHONY: all

