# OMx to GPX converters

This repository provides two binaries:

- omx2gpx, a command-line tool to convert an OMD/OMH pair of files to a GPX file.
- OnMoveFreely, a graphical interface built on omx2gpx for easy batch conversions

## omx2gpx

omx2gpx was originally written by Clément Corde - c1702@yahoo.com. See src/c/README.txt for further information.

## OnMoveFreely

On MoveFreely is a GUI built on omx2gpx using python, swig and pyinstaller.

## Installation

In brief:

   $ sudo apt-get install swig
   $ pip install pyinstaller
   $ make all

This should create two binaries in `dist` sub-directory. You can run `sudo make install` to finish the process

## How to use

Command-line:

	$ omx2gpx <input_file>

GUI:

	$ OnMoveFreely
	

## Further references

[Original c code](https://sourceforge.net/projects/omx2gpx/)
[OMD/OMH specifications](https://github.com/ylecuyer/OnMove200)

[SWIG documentation](http://www.swig.org/Doc4.0/Python.html)
[PyInstaller documentation](https://pyinstaller.readthedocs.io)
[FreeDesktop specifications](https://specifications.freedesktop.org/desktop-entry-spec/latest/index.html)
