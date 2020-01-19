from distutils.core import setup, Extension

extension_mod = Extension("_omx2gpx", [
    "_omx2gpx.c", "pywrap.c", "load.c", "core_data.c",
    "core_header.c", "point.c", "elevation.c"])

setup(name = "omx2gpx", ext_modules=[extension_mod])
