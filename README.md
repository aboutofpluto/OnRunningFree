# OMx to GPX converters

This repository provides two binaries:

- `omx2gpx`, a command-line tool to convert an OMD/OMH pair of files to a GPX file.
- `OnRunningFree`, a graphical interface built on omx2gpx for easy batch conversions

## omx2gpx

omx2gpx was originally written by Clément Corde - c1702@yahoo.com. See src/c/README.txt for further information.

## OnRunningFree

On RunningFree is a GUI built on omx2gpx using python, swig and pyinstaller.

## Installation

Some binary of OnRunningFree have already been compiled. Give them a try, you can find them in the `bin` directory.

## Requirements and manual installation

- python 3
- swig
- pyinstaller
- GNU Make

In brief:

      $ sudo apt-get install swig libfontconfig-dev
   	  $ pip install pyinstaller
   	  $ make all

This should create two binaries in `dist` sub-directory. You can run `sudo make install` to finish the process

## How to use

Command-line:

	$ omx2gpx <input_file>

GUI:

	$ onrunningfree
	

## Further references

- [Original c code](https://sourceforge.net/projects/omx2gpx/)
- [OMD/OMH specifications](https://github.com/ylecuyer/OnMove200)
- [SWIG documentation](http://www.swig.org/Doc4.0/Python.html)
- [PyInstaller documentation](https://pyinstaller.readthedocs.io)
- [FreeDesktop specifications](https://specifications.freedesktop.org/desktop-entry-spec/latest/index.html)
- [Running Free - Iron Maiden](https://www.youtube.com/watch?v=80Hyz4pOXtE)
