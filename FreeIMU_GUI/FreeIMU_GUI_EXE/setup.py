from distutils.core import setup
import py2exe
import numpy
import pyqtgraph
import xml.dom.minidom

setup(
    windows=["cal_gui.py"],
    options={
        "py2exe": {
            "includes": ["ctypes", "logging",
						"scipy.sparse.csgraph._validation",
						"scipy.special._ufuncs_cxx",
						"PyQt4.QtCore", "PyQt4.QtGui", "sip", 
						"pyqtgraph","numpy","atexit",
						"xml.dom.minidom"],
			"excludes": ["Tkconstants", "Tkinter","tcl"]
        }
    }
)