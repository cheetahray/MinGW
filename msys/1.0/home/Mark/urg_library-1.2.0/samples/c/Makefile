TARGET = sensor_parameter get_distance get_distance_intensity get_multiecho get_multiecho_intensity sync_time_stamp calculate_xy find_port calculate_xy_3 calculate_xy_4 calculate_xy_5 calculate_xy_6 calculate_xy_7 calculate_xy_8

URG_LIB = ../../src/liburg_c.a
PTHREAD_LIB = ../../src/libpthreadGC-3.dll.a
OSC_LIB = ../../src/liblo.dll.a
#OSC_LIB = ../../src/liblo.7.dylib

include ../../build_rule.mk

CFLAGS = -O2 $(INCLUDES) -I../../include/c
LDLIBS = -lm `/bin/sh ld_wsock.sh` `/bin/sh ld_setupapi.sh`

all : $(TARGET)

clean :
	$(RM) *.o $(TARGET) *.exe

#$(TARGET) : open_urg_sensor.o kmeans.o rotate.o $(URG_LIB) $(OSC_LIB)
$(TARGET) : open_urg_sensor.o kmeans.o rotate.o $(URG_LIB) $(PTHREAD_LIB) $(OSC_LIB)

$(URG_LIB) :
	cd $(@D)/ && $(MAKE) $(@F)
