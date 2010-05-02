== Introduction ==

python-writev is a module providing an interface to the writev(2) unix system
call.

This software is released under the new BSD license.

== Status ==

Currently beta status.  The author is a newcomer to python C extensions
and may have gotten the reference counting wrong, especially in error
conditions.  Reviews are welcome.

== Usage ==

import writev
written = writev.writev(fd, [string1, string2, ...])
written, remaining = writev.lwritev(fd, [string1, string2, ...])

The writev function is a simple wrapper around writev.  It returns the
number of bytes written successfully by writev.

The lwritev function is an enhanced interface which does the work of
generating a list of unwritten strings for you.  It returns a tuple composed
of an integer containing the bytes written, and a list.  The list contains,
first, a new string composed of the unwritten bytes of any partially-written
string, and then the remaining whole unwritten strings.

== Contact ==

Submit issues and discuss at:
http://code.google.com/p/python-writev/
