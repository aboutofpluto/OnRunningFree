# Makefile

# targets
CMD = omx2gpx
GUI = onmovefreely

# directories
DIST_DIR = dist
C_SRC_DIR = src/c
PY_SRC_DIR = src/python
MKDIR = mkdir

# c/gcc config
SRCS = main.c commandline.c load.c point.c elevation.c core_header.c core_data.c pywrap.c
OBJECTS = $(addprefix $(C_SRC_DIR)/,$(SRCS:.c=.o))

CFLAGS = -O3 -Wall -L/usr/lib -L. -s -DNDEBUG 
LIBS = 

CC = gcc
LINKER = gcc

# python/swig/pyinstall config
PYTHON = python3

SWIG = swig
SWIG_C = $(addprefix $(C_SRC_DIR)/,_$(CMD).c)
SWIG_I = $(addprefix $(C_SRC_DIR)/,$(CMD).i)
SWIG_PY = $(addprefix $(C_SRC_DIR)/,$(CMD).py)
SWIG_CLEAN = $(SWIG_C) $(SWIG_C)*so $(SWIG_PY) $(addprefix $(PY_SRC_DIR)/,_$(CMD).c*so) $(addprefix $(PY_SRC_DIR)/,_$(CMD).py) 

INSTALLER = pyinstaller
INSTALLER_CLEAN = build $(GUI).spec

# defaut rule is omx2gpx
$(CMD): $(OBJECTS)
	$(MKDIR) -p $(DIST_DIR)
	$(LINKER) $(CFLAGS) -o $(addprefix $(DIST_DIR)/,$@) $^ $(LIBS) 

# rule for GUI onmovefreely using pyinstaller
$(GUI): py_module
	$(INSTALLER) -F $(addprefix $(PY_SRC_DIR)/,$(GUI).py)

# all build all binaries
all: $(CMD) $(GUI)

# build py module using swig and python setup.py
py_module:
	$(SWIG) -python -o $(SWIG_C) $(SWIG_I)
	$(PYTHON) $(addprefix $(C_SRC_DIR)/,setup.py) build_ext --inplace
	cp $(SWIG_PY) $(SWIG_C)*so $(PY_SRC_DIR)

# clean all temporary files
clean:
	rm -f $(OBJECTS) 
	rm -f $(SWIG_CLEAN)
	rm -rf $(INSTALLER_CLEAN)
	rm -rf __pycache__/

# install (as root on linux)
install:
	cp $(DIST_DIR)/$(CMD) /usr/local/bin
	cp $(DIST_DIR)/$(GUI) /usr/local/bin
