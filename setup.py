#!/usr/bin/env python

from distutils.core import setup, Extension

setup(
    name = "python-writev",
    version = "1.0",
    author = "Ryan Smith-Roberts",
    author_email = "rsr@ivi.tv",
    license = "BSD",
    url = "http://code.google.com/p/python-writev/",
    download_url = "http://code.google.com/p/python-writev/downloads/list",
    classifiers = ['Development Status :: 5 - Production/Stable'],

    ext_modules=[Extension("writev", ["writevmodule.c"])],
    data_files= [('share/doc/python-writev', ['README.txt'])],

    description = "python-writev is a module providing an interface to the unix writev call",
    long_description = "python-writev is a module providing an interface to the unix writev call",
)

