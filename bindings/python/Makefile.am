if HAVE_EASY_BINDINGS
if HAVE_PYTHON

swigdatadir = @SWIG_DATA_DIR@/python
dist_swigdata_DATA = libiodefv2cpp-python.i

EXTRA_DIST = libiodefv2cpp-python.i iodefv2.py _iodefv2.cxx pystrings.swg

all-am: python-build

python-build: _iodefv2.cxx
if HAVE_PYTHON2
	CC="$(CXX)" $(PYTHON2) setup.py build
endif
if HAVE_PYTHON3
	CC="$(CXX)" $(PYTHON3) setup.py build
endif
install-exec-hook:
if HAVE_PYTHON2
	$(PYTHON2) setup.py install `test -n "$(DESTDIR)" && echo --root $(DESTDIR)`
endif
if HAVE_PYTHON3
	$(PYTHON3) setup.py install `test -n "$(DESTDIR)" && echo --root $(DESTDIR)`
endif

uninstall-hook:
if HAVE_PYTHON2
	$(PYTHON2) setup.py uninstall `test -n "$(DESTDIR)" && echo --root $(DESTDIR)`
endif
if HAVE_PYTHON3
	$(PYTHON3) setup.py uninstall `test -n "$(DESTDIR)" && echo --root $(DESTDIR)`
endif

clean-local:
if HAVE_PYTHON2
	$(PYTHON2) setup.py clean -a
endif
if HAVE_PYTHON3
	$(PYTHON3) setup.py clean -a
endif

_iodefv2.cxx iodefv2.py: $(top_srcdir)/bindings/c++/include/*.hxx $(top_srcdir)/bindings/libiodefv2cpp.i libiodefv2cpp-python.i 
if HAVE_SWIG
	$(SWIG) -c++ -DSWIG_COMPILE_LIBIODEFV2 -I$(top_srcdir)/bindings -I$(top_srcdir)/bindings/c++/include -I$(top_srcdir)/src/include -I$(top_srcdir)/src/libiodefv2-error -python -threads -builtin -O -o $@ $(top_srcdir)/bindings/libiodefv2cpp.i
endif

endif
endif

-include $(top_srcdir)/git.mk
