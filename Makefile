# Makefile

CMD = omx2gpx
OBJECTS = main.o commandline.o load.o point.o elevation.o core_header.o core_data.o pywrap.o

CFLAGS = -O3 -Wall -L/usr/lib -L. -s -DNDEBUG 
LIBS = 

CC = gcc
LINKER = gcc
PYTHON = python3

SWIG = swig
SWIG_C = _$(CMD).c
SWIG_I = $(CMD).i
SWIG_OUT = $(SWIG_C)* $(CMD).py

INSTALLER = pyinstaller
GUI = onmovefreely
INSTALLER_OUT = build $(GUI).spec

$(CMD): $(OBJECTS)
	$(LINKER) $(CFLAGS) -o $@ $^ $(LIBS) 

$(GUI): py_module
	$(INSTALLER) -F $(GUI).py

all: $(CMD) $(GUI)

.cc.o:
	$(CC) $(CFLAGS) -o $< 

py_module:
	$(SWIG) -python -o $(SWIG_C) $(SWIG_I)
	$(PYTHON) setup.py build_ext --inplace

clean:
	rm -f $(CMD)
	rm -f $(OBJECTS) 
	rm -f $(SWIG_OUT)
	rm -rf $(INSTALLER_OUT)
	rm -rf __pycache__/

