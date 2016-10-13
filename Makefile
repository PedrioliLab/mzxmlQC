CPPFLAGS=-D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -std=c++11 #-stdlib=libc++

LDLIBS=-lz -static
SRCS=src/mzxmlqc.cpp src/mzxml.cpp src/ramp/ramp.cpp src/ramp/ramp_base64.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

all: CPPFLAGS += -O3
all: mzxmlqc

debug: CPPFLAGS += -DDEBUG -g
debug: mzxmlqc

mzxmlqc: $(OBJS)
	$(CXX) $(LDFLAGS) -o bin/mzxmlqc $(OBJS) $(LDLIBS)

clean:
	rm $(OBJS)

mzxmlqc.o: src/mzxmlqc.cpp src/mzxml.hpp
mzxml.o: src/mzxml.cpp src/mzxml.hpp
ramp.o: src/ramp/ramp.cpp src/ramp/ramp.h 
ramp_base64.o: src/ramp/ramp_base64.cpp src/ramp/ramp_base64.h
