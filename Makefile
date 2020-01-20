# Makefile

# targets
CMD = omx2gpx
GUI = onmovefreely

# directories
DIST_DIR = dist/
C_SRC_DIR = src/c/
PY_SRC_DIR = src/python/
ASSETS_DIR = assets/
MKDIR = mkdir

# c/gcc config
SRCS = main.c commandline.c load.c point.c elevation.c core_header.c core_data.c pywrap.c
OBJECTS = $(addprefix $(C_SRC_DIR),$(SRCS:.c=.o))

CFLAGS = -O3 -Wall -L/usr/lib -L. -s -DNDEBUG 
LIBS = 

CC = gcc
LINKER = gcc

# python/swig/pyinstall config
PYTHON = python3

SWIG = swig
SWIG_C = $(addprefix $(C_SRC_DIR),_$(CMD).c)
SWIG_I = $(addprefix $(C_SRC_DIR),$(CMD).i)
SWIG_PY = $(addprefix $(C_SRC_DIR),$(CMD).py)
SWIG_CLEAN = $(SWIG_C) $(SWIG_C)*so $(SWIG_PY) $(addprefix $(PY_SRC_DIR),_$(CMD).c*so) $(addprefix $(PY_SRC_DIR),_$(CMD).py) 

# pyinstaller config
BUILDER = pyinstaller
BUILDER_CLEAN = build $(GUI).spec

# extra config to build and install depending on OS (TO BE COMPLETED)
ifeq ($(OS),Windows_NT)
	BUILDER_OPTIONS = --icon=$(addprefix $(ASSETS_DIR),icon.ico)
	INSTALL_RULE = install_windows
else
	ifeq ($(shell uname), Darwin)
		BUILDER_OPTIONS = --icon=$(addprefix $(ASSETS_DIR),icon.icns)
		INSTALL_RULE = install_macosx
	endif
	ifeq ($(shell uname), Linux)
		BUILDER_OPTIONS = 
		INSTALL_RULE = install_linux
	endif
endif


# defaut rule is omx2gpx
$(CMD): $(OBJECTS)
	$(MKDIR) -p $(DIST_DIR)
	$(LINKER) $(CFLAGS) -o $(addprefix $(DIST_DIR),$@) $^ $(LIBS) 

# rule for GUI onmovefreely using pyinstaller
$(GUI): py_module
	$(BUILDER) $(BUILDER_OPTIONS) -w -F $(addprefix $(PY_SRC_DIR),$(GUI).py)

# all build all binaries
all: $(CMD) $(GUI)

# build py module using swig and python setup.py
py_module:
	$(SWIG) -python -o $(SWIG_C) $(SWIG_I)
	$(PYTHON) $(addprefix $(C_SRC_DIR),setup.py) build_ext --inplace
	cp $(SWIG_PY) $(SWIG_C)*so $(PY_SRC_DIR)

# clean all temporary files
clean:
	rm -f $(OBJECTS) 
	rm -f $(SWIG_CLEAN)
	rm -rf $(BUILDER_CLEAN)
	rm -rf __pycache__/

# install
install: $(INSTALL_RULE)

install_linux:
	cp $(addprefix $(DIST_DIR),$(CMD)) /usr/local/bin
	cp $(addprefix $(DIST_DIR),$(GUI)) /usr/local/bin
	cp $(addprefix $(ASSETS_DIR),$(GUI).desktop) /usr/share/applications/
	cp $(addprefix $(ASSETS_DIR),$(GUI).png) /usr/share/icons/hicolor/scalable/apps/

install_windows:

install_macosx:
