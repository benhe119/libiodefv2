if HAVE_EASY_BINDINGS

EXTRA_DIST = libiodefv2cpp-lua.i libiodefv2.cxx

if HAVE_LUA

swigdatadir = @SWIG_DATA_DIR@/lua
dist_swigdata_DATA = libiodefv2cpp-lua.i

AM_CPPFLAGS = -I@top_srcdir@ -I@top_builddir@/src/include -I@top_srcdir@/src/include -I@top_builddir@/src/libiodefv2-error -I@top_srcdir@/bindings/c++/include @LUA_CFLAGS@ -I@top_srcdir@/libmissing -I@top_builddir@/libmissing

libiodefv2_la_LDFLAGS = -module -avoid-version @LUA_LIBS@
libiodefv2_la_LIBADD = $(top_builddir)/bindings/c++/.libs/libiodefv2cpp.la
libiodefv2_la_SOURCES = libiodefv2.cxx
libiodefv2dir = $(libdir)/lua/@LUA_VERSION@
libiodefv2_LTLIBRARIES = libiodefv2.la

libiodefv2.cxx: $(top_srcdir)/bindings/c++/include/*.hxx $(top_srcdir)/bindings/libiodefv2cpp.i libiodefv2cpp-lua.i 
if HAVE_SWIG
	$(SWIG) -c++ -DSWIG_COMPILE_LIBIODEFV2 -I$(top_srcdir)/bindings -I$(top_srcdir)/bindings/c++/include -I$(top_srcdir)/src/include -I$(top_srcdir)/src/libiodefv2-error -lua -o $@ $(top_srcdir)/bindings/libiodefv2cpp.i
endif


endif

endif

-include $(top_srcdir)/git.mk
