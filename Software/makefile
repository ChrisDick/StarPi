all :

#substitutions

INCLUDE_PATHS := 	-I./Src/I2Cdevlib/Pi/I2Cdev/ \
					-I./Src/I2Cdevlib/Pi/HMC5883L/ \
					-I./Src/I2Cdevlib/Pi/ADXL345/ \
					-I./Src/I2Cdevlib/Pi/AK8975/ \
					-I./Src/I2Cdevlib/Pi/BMA150/ \
					-I./Src/I2Cdevlib/Pi/HMC5843/ \
					-I./Src/I2Cdevlib/Pi/MPU6050/ \
					-I./Src/I2Cdevlib/Pi/MPU9150/ \
					-I./Src/I2Cdevlib/Pi/LSM303DLHC/ \
					-I./Src/Hal/ \
					-I./Src/TelescopeManager/ \
					-I./Src/Scheduler/ \
					-I./Src/StellariumServer/ \
					-I./Src/GPSD/ \
					-I./Src/MagModelCorrection/ \
					-I./Src/SOFA \
					-I./Src/ \
					-I./Src/Utils \
					-I./Src/Drivers/ 

CPPFILES := 		Src/Main.cpp \
					Src/I2Cdevlib/Pi/ADXL345/ADXL345.cpp \
					Src/I2Cdevlib/Pi/AK8975/AK8975.cpp \
					Src/I2Cdevlib/Pi/BMA150/BMA150.cpp \
					Src/I2Cdevlib/Pi/HMC5843/HMC5843.cpp \
					Src/I2Cdevlib/Pi/HMC5883L/HMC5883L.cpp \
					Src/I2Cdevlib/Pi/MPU6050/MPU6050.cpp \
					Src/I2Cdevlib/Pi/MPU9150/MPU9150.cpp \
					Src/I2Cdevlib/Pi/I2Cdev/I2Cdev.cpp \
					Src/StellariumServer/Connection.cpp \
					Src/StellariumServer/Listener.cpp \
					Src/StellariumServer/Server.cpp \
					Src/StellariumServer/Socket.cpp \
					Src/StellariumServer/ServerPi.cpp \
					Src/TelescopeManager/TelescopeOrientation.cpp \
					Src/TelescopeManager/TelescopeIO.cpp \
					Src/Hal/HalGps.cpp \
					Src/Hal/HalAccelerometer.cpp \
					Src/Hal/HalMagnetometer.cpp \
					Src/Hal/HalWebsocketd.cpp \
					Src/Scheduler/TTC_Sched.cpp \
					Src/Scheduler/Runnable.cpp \
					Src/TelescopeManager/TelescopeManager.cpp \
					Src/TelescopeManager/CelestrialConverter.cpp \
					Src/Scheduler/TTC_Sched_Pi_Impl.cpp \
					Src/MagModelCorrection/MagModel.cpp \
					Src/MagModelCorrection/GeomagnetismLibrary.c \
					Src/GPSD/libgpsmm.cpp

CFILES :=	\
            Src/SOFA/a2af.c \
            Src/SOFA/a2tf.c \
            Src/SOFA/ab.c \
            Src/SOFA/af2a.c \
            Src/SOFA/anp.c \
            Src/SOFA/anpm.c \
            Src/SOFA/apcg.c \
            Src/SOFA/apcg13.c \
            Src/SOFA/apci.c \
            Src/SOFA/apci13.c \
            Src/SOFA/apco.c \
            Src/SOFA/apco13.c \
            Src/SOFA/apcs.c \
            Src/SOFA/apcs13.c \
            Src/SOFA/aper.c \
            Src/SOFA/aper13.c \
            Src/SOFA/apio.c \
            Src/SOFA/apio13.c \
            Src/SOFA/atci13.c \
            Src/SOFA/atciq.c \
            Src/SOFA/atciqn.c \
            Src/SOFA/atciqz.c \
            Src/SOFA/atco13.c \
            Src/SOFA/atic13.c \
            Src/SOFA/aticq.c \
            Src/SOFA/aticqn.c \
            Src/SOFA/atio13.c \
            Src/SOFA/atioq.c \
            Src/SOFA/atoc13.c \
            Src/SOFA/atoi13.c \
            Src/SOFA/atoiq.c \
            Src/SOFA/bi00.c \
            Src/SOFA/bp00.c \
            Src/SOFA/bp06.c \
            Src/SOFA/bpn2xy.c \
            Src/SOFA/c2i00a.c \
            Src/SOFA/c2i00b.c \
            Src/SOFA/c2i06a.c \
            Src/SOFA/c2ibpn.c \
            Src/SOFA/c2ixy.c \
            Src/SOFA/c2ixys.c \
            Src/SOFA/c2s.c \
            Src/SOFA/c2t00a.c \
            Src/SOFA/c2t00b.c \
            Src/SOFA/c2t06a.c \
            Src/SOFA/c2tcio.c \
            Src/SOFA/c2teqx.c \
            Src/SOFA/c2tpe.c \
            Src/SOFA/c2txy.c \
            Src/SOFA/cal2jd.c \
            Src/SOFA/cp.c \
            Src/SOFA/cpv.c \
            Src/SOFA/cr.c \
            Src/SOFA/d2dtf.c \
            Src/SOFA/d2tf.c \
            Src/SOFA/dat.c \
            Src/SOFA/dtdb.c \
            Src/SOFA/dtf2d.c \
            Src/SOFA/eceq06.c \
            Src/SOFA/ecm06.c \
            Src/SOFA/eqec06.c \
            Src/SOFA/ee00.c \
            Src/SOFA/ee00a.c \
            Src/SOFA/ee00b.c \
            Src/SOFA/ee06a.c \
            Src/SOFA/eect00.c \
            Src/SOFA/eform.c \
            Src/SOFA/eo06a.c \
            Src/SOFA/eors.c \
            Src/SOFA/epb.c \
            Src/SOFA/epb2jd.c \
            Src/SOFA/epj.c \
            Src/SOFA/epj2jd.c \
            Src/SOFA/epv00.c \
            Src/SOFA/eqeq94.c \
            Src/SOFA/era00.c \
            Src/SOFA/fad03.c \
            Src/SOFA/fae03.c \
            Src/SOFA/faf03.c \
            Src/SOFA/faju03.c \
            Src/SOFA/fal03.c \
            Src/SOFA/falp03.c \
            Src/SOFA/fama03.c \
            Src/SOFA/fame03.c \
            Src/SOFA/fane03.c \
            Src/SOFA/faom03.c \
            Src/SOFA/fapa03.c \
            Src/SOFA/fasa03.c \
            Src/SOFA/faur03.c \
            Src/SOFA/fave03.c \
            Src/SOFA/fk52h.c \
            Src/SOFA/fk5hip.c \
            Src/SOFA/fk5hz.c \
            Src/SOFA/fw2m.c \
            Src/SOFA/fw2xy.c \
            Src/SOFA/g2icrs.c \
            Src/SOFA/gc2gd.c \
            Src/SOFA/gc2gde.c \
            Src/SOFA/gd2gc.c \
            Src/SOFA/gd2gce.c \
            Src/SOFA/gmst00.c \
            Src/SOFA/gmst06.c \
            Src/SOFA/gmst82.c \
            Src/SOFA/gst00a.c \
            Src/SOFA/gst00b.c \
            Src/SOFA/gst06.c \
            Src/SOFA/gst06a.c \
            Src/SOFA/gst94.c \
            Src/SOFA/h2fk5.c \
            Src/SOFA/hfk5z.c \
            Src/SOFA/icrs2g.c \
            Src/SOFA/ir.c \
            Src/SOFA/jd2cal.c \
            Src/SOFA/jdcalf.c \
            Src/SOFA/ld.c \
            Src/SOFA/ldn.c \
            Src/SOFA/ldsun.c \
            Src/SOFA/lteceq.c \
            Src/SOFA/ltecm.c \
            Src/SOFA/lteqec.c \
            Src/SOFA/ltp.c \
            Src/SOFA/ltpb.c \
            Src/SOFA/ltpecl.c \
            Src/SOFA/ltpequ.c \
            Src/SOFA/num00a.c \
            Src/SOFA/num00b.c \
            Src/SOFA/num06a.c \
            Src/SOFA/numat.c \
            Src/SOFA/nut00a.c \
            Src/SOFA/nut00b.c \
            Src/SOFA/nut06a.c \
            Src/SOFA/nut80.c \
            Src/SOFA/nutm80.c \
            Src/SOFA/obl06.c \
            Src/SOFA/obl80.c \
            Src/SOFA/p06e.c \
            Src/SOFA/p2pv.c \
            Src/SOFA/p2s.c \
            Src/SOFA/pap.c \
            Src/SOFA/pas.c \
            Src/SOFA/pb06.c \
            Src/SOFA/pdp.c \
            Src/SOFA/pfw06.c \
            Src/SOFA/plan94.c \
            Src/SOFA/pm.c \
            Src/SOFA/pmat00.c \
            Src/SOFA/pmat06.c \
            Src/SOFA/pmat76.c \
            Src/SOFA/pmp.c \
            Src/SOFA/pmpx.c \
            Src/SOFA/pmsafe.c \
            Src/SOFA/pn.c \
            Src/SOFA/pn00.c \
            Src/SOFA/pn00a.c \
            Src/SOFA/pn00b.c \
            Src/SOFA/pn06.c \
            Src/SOFA/pn06a.c \
            Src/SOFA/pnm00a.c \
            Src/SOFA/pnm00b.c \
            Src/SOFA/pnm06a.c \
            Src/SOFA/pnm80.c \
            Src/SOFA/pom00.c \
            Src/SOFA/ppp.c \
            Src/SOFA/ppsp.c \
            Src/SOFA/pr00.c \
            Src/SOFA/prec76.c \
            Src/SOFA/pv2p.c \
            Src/SOFA/pv2s.c \
            Src/SOFA/pvdpv.c \
            Src/SOFA/pvm.c \
            Src/SOFA/pvmpv.c \
            Src/SOFA/pvppv.c \
            Src/SOFA/pvstar.c \
            Src/SOFA/pvtob.c \
            Src/SOFA/pvu.c \
            Src/SOFA/pvup.c \
            Src/SOFA/pvxpv.c \
            Src/SOFA/pxp.c \
            Src/SOFA/refco.c \
            Src/SOFA/rm2v.c \
            Src/SOFA/rv2m.c \
            Src/SOFA/rx.c \
            Src/SOFA/rxp.c \
            Src/SOFA/rxpv.c \
            Src/SOFA/rxr.c \
            Src/SOFA/ry.c \
            Src/SOFA/rz.c \
            Src/SOFA/s00.c \
            Src/SOFA/s00a.c \
            Src/SOFA/s00b.c \
            Src/SOFA/s06.c \
            Src/SOFA/s06a.c \
            Src/SOFA/s2c.c \
            Src/SOFA/s2p.c \
            Src/SOFA/s2pv.c \
            Src/SOFA/s2xpv.c \
            Src/SOFA/sepp.c \
            Src/SOFA/seps.c \
            Src/SOFA/sp00.c \
            Src/SOFA/starpm.c \
            Src/SOFA/starpv.c \
            Src/SOFA/sxp.c \
            Src/SOFA/sxpv.c \
            Src/SOFA/taitt.c \
            Src/SOFA/taiut1.c \
            Src/SOFA/taiutc.c \
            Src/SOFA/tcbtdb.c \
            Src/SOFA/tcgtt.c \
            Src/SOFA/tdbtcb.c \
            Src/SOFA/tdbtt.c \
            Src/SOFA/tf2a.c \
            Src/SOFA/tf2d.c \
            Src/SOFA/tr.c \
            Src/SOFA/trxp.c \
            Src/SOFA/trxpv.c \
            Src/SOFA/tttai.c \
            Src/SOFA/tttcg.c \
            Src/SOFA/tttdb.c \
            Src/SOFA/ttut1.c \
            Src/SOFA/ut1tai.c \
            Src/SOFA/ut1tt.c \
            Src/SOFA/ut1utc.c \
            Src/SOFA/utctai.c \
            Src/SOFA/utcut1.c \
            Src/SOFA/xy06.c \
            Src/SOFA/xys00a.c \
            Src/SOFA/xys00b.c \
            Src/SOFA/xys06a.c \
            Src/SOFA/zp.c \
            Src/SOFA/zpv.c \
            Src/SOFA/zr.c
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

$(OUT_DIR)StarPi:	${obj.cpp} ${obj.c}
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
