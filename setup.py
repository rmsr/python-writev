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
            'Topic :: Software Development :: Libraries :: Python Modules',
            'Topic :: Communications',
            'Topic :: Internet',
            'Programming Language :: Python',
            'Programming Language :: Python :: 2',
            'Environment :: Other Environment',
        ],

    ext_modules=[Extension("writev", ["writevmodule.c"])],
    data_files= [('share/doc/python-writev', ['README.txt'])],

    description = "python-writev is a module providing an interface to the unix writev system call",
    long_description = "python-writev is a module providing an interface to the unix writev system call.  It includes both a simple wrapper around writev, as well as an enhanced function that returns a list containing the unsent data.",
)

