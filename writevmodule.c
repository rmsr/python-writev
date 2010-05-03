/* writev - Python extension module for writev(2) unix system call.*/
/*
Copyright (c) 2010, ivi, Inc.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
    * Neither the name of ivi, Inc. nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/

#include <Python.h>

#include <stdlib.h>
#include <sys/uio.h>
#include <limits.h>

PyDoc_STRVAR(writev_writev__doc__,
"writev(fd, seq) -> byteswritten\n\n\
Write a sequence of strings to a file descriptor.");

static PyObject *
writev_writev(PyObject *self, PyObject *args)
{
    PyObject *fdobj, *seq, *iter, *str;
    Py_ssize_t written;
    int fd, i, elements = 0;
    struct iovec iov[IOV_MAX];
    PyObject *strings[IOV_MAX];

    if (!PyArg_ParseTuple(args, "OO:writev", &fdobj, &seq))
        return NULL;

    if ((fd = PyObject_AsFileDescriptor(fdobj)) == -1)
        return NULL;

    iter = PyObject_GetIter(seq);
    if (iter == NULL)
        return NULL;

    while ((str = PyIter_Next(iter)) != NULL) {
        Py_ssize_t len;
        char *base;

        if (elements == IOV_MAX) {
            PyErr_SetString(PyExc_ValueError,
                    "writev() cannot write more than IOV_MAX strings");
            Py_DECREF(iter);
            for (i=0; i<elements; i++)
                Py_DECREF(strings[i]);
            Py_DECREF(str);
            return NULL;
        }

        if (PyString_AsStringAndSize(str, &base, &len) == -1) {
            Py_DECREF(iter);
            for (i=0; i<elements; i++)
                Py_DECREF(strings[i]);
            Py_DECREF(str);
            return NULL;
        }

        iov[elements].iov_base = base;
        iov[elements].iov_len = len;
        strings[elements] = str;
        elements++;
    }

    Py_DECREF(iter);

    Py_BEGIN_ALLOW_THREADS
    written = writev(fd, iov, elements);
    Py_END_ALLOW_THREADS

    for (i=0; i<elements; i++)
        Py_DECREF(strings[i]);

    if (written < 0)
        return PyErr_SetFromErrno(PyExc_OSError);
    return PyInt_FromSsize_t(written);
}

PyDoc_STRVAR(writev_lwritev__doc__,
"lwritev(fd, seq) -> (byteswritten, remaining)\n\n\
Write a sequence of strings to a file descriptor.\n\n\
Returns a tuple containing the bytes successfully written and a list \
composed of the strings or parts of strings that were not written.");

static PyObject *
writev_lwritev(PyObject *self, PyObject *args)
{
    PyObject *fdobj, *seq, *str, *new, *iter = NULL, *list = NULL;
    Py_ssize_t written, len, remaining;
    int fd, i, err, elements = 0, offset = 0, appending = 0;
    struct iovec iov[IOV_MAX];
    PyObject *strings[IOV_MAX];
    char *base;

    if (!PyArg_ParseTuple(args, "OO:lwritev", &fdobj, &seq))
        return NULL;

    if ((fd = PyObject_AsFileDescriptor(fdobj)) == -1)
        return NULL;

    if ((list = PyList_New(0)) == NULL)
        return NULL;

    if ((iter = PyObject_GetIter(seq)) == NULL)
        goto error;

    while ((str = PyIter_Next(iter)) != NULL) {
        if (elements == IOV_MAX) {
            PyErr_SetString(PyExc_ValueError,
                    "lwritev() cannot write more than IOV_MAX strings");
            Py_DECREF(str);
            goto error;
        }

        if (PyString_AsStringAndSize(str, &base, &len) == -1) {
            Py_DECREF(str);
            goto error;
        }

        iov[elements].iov_base = base;
        iov[elements].iov_len = len;
        strings[elements] = str;
        elements++;
    }

    Py_DECREF(iter);
    iter = NULL;

    Py_BEGIN_ALLOW_THREADS
    written = writev(fd, iov, elements);
    Py_END_ALLOW_THREADS

    if (written < 0) {
        PyErr_SetFromErrno(PyExc_OSError);
        goto error;
    }

    remaining = written;
    for (i=0; i<elements; i++) {
        offset++;
        str = strings[i];
        if (appending) {
            err = PyList_Append(list, str);
            Py_DECREF(str);
            if (err != 0)
                goto error;
            continue;
        } 
        remaining -= PyString_GET_SIZE(str);
        if (remaining < 0) {
            appending = 1;
            if (PyString_AsStringAndSize(str, &base, &len) == -1) {
                Py_DECREF(str);
                goto error;
            }
            new = PyString_FromStringAndSize(base + len + remaining,
                    -remaining);
            Py_DECREF(str);
            if (new == NULL)
                goto error;
            err = PyList_Append(list, new);
            Py_DECREF(new);
            if (err != 0)
                goto error;
            continue;
        }
        Py_DECREF(str);
    }

    return Py_BuildValue("nN", written, list);

error:
    Py_XDECREF(iter);
    Py_XDECREF(list);
    if (elements) {
        for (i=offset; i<elements; i++)
            Py_DECREF(strings[i]);
    }
    return NULL;
}

static PyMethodDef writev_methods[] = {
    {"writev",  writev_writev,  METH_VARARGS,   writev_writev__doc__},
    {"lwritev", writev_lwritev, METH_VARARGS,   writev_lwritev__doc__},
    {NULL,      NULL,           0,              NULL},  /* sentinel */
};

PyDoc_STRVAR(module_doc,
"This module implements the writev system call\n");

PyMODINIT_FUNC
initwritev(void)
{
    PyObject *m;
    m = Py_InitModule3("writev", writev_methods, module_doc);
    PyModule_AddIntConstant(m, "IOV_MAX", IOV_MAX);
};
