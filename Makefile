# Makefile

TARGET = omx2gpx
OBJECTS = main.o commandline.o load.o point.o elevation.o core_header.o

CFLAGS = -O3 -Wall -L/usr/lib -L. -s -DNDEBUG 
LIBS = 

CC = gcc
LINKER = gcc

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(LINKER) $(CFLAGS) -o $@ $^ $(LIBS) 

.cc.o:
	$(CC) $(CFLAGS) -o $< 

clean:
	rm $(OBJECTS)


