#!/usr/bin/env python

from distutils.core import setup, Extension

setup(
    name = "python-writev",
    version = "1.1",
    author = "Ryan Smith-Roberts",
    author_email = "rsr@ivi.tv",
    license = "BSD",
    url = "http://code.google.com/p/python-writev/",
    download_url = "http://code.google.com/p/python-writev/downloads/list",
    classifiers = [
            'Development Status :: 4 - Beta',
            'Intended Audience :: Developers',
            'License :: OSI Approved :: BSD License',
            'Operating System :: POSIX',
            'Programming Language :: Python :: 2',
        ],

    ext_modules=[Extension("writev", ["writevmodule.c"])],
    data_files= [('share/doc/python-writev', ['README.txt'])],

    description = "python-writev is a module providing an interface to the unix writev call",
    long_description = "python-writev is a module providing an interface to the unix writev call",
)

