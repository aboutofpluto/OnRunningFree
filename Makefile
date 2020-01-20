# Makefile

TARGET = omx2gpx
OBJECTS = main.o commandline.o load.o point.o elevation.o core_header.o core_data.o pywrap.o

CFLAGS = -O3 -Wall -L/usr/lib -L. -s -DNDEBUG 
LIBS = 

CC = gcc
LINKER = gcc
PYTHON = python3

SWIG = swig
SWIG_C = _omx2gpx.c
SWIG_OUT = $(SWIG_C) omx2gpx.py

INSTALLER = pyinstaller
INSTALLER_OUT = build dist onmovefreely.spec

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(LINKER) $(CFLAGS) -o $@ $^ $(LIBS) 

.cc.o:
	$(CC) $(CFLAGS) -o $< 

clean:
	rm -f $(OBJECTS)
	rm -f $(SWIG_OUT)
	rm -rf $(INSTALLER_OUT)

py_module:
	$(SWIG) -python -o $(SWIG_C) omx2gpx.i
	$(PYTHON) setup.py build_ext --inplace

gui: py_module
	$(INSTALLER) -F onmovefreely.py
