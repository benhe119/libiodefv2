if HAVE_EASY_BINDINGS
if HAVE_RUBY

swigdatadir = @SWIG_DATA_DIR@/ruby
dist_swigdata_DATA = libiodefv2cpp-ruby.i

EXTRA_DIST = libiodefv2cpp-ruby.i LibIodef.cxx

rbexec_LTLIBRARIES = LibIodef.la
LibIodef_la_CPPFLAGS = $(RUBY_INCLUDES) $(RUBY_CCFLAGS) -I@top_builddir@ -I$(top_srcdir)/src/include -I$(top_builddir)/src/include -I@top_builddir@/src/libiodefv2-error -I$(top_srcdir)/bindings/c++/include
LibIodef_la_LDFLAGS = -module -avoid-version
LibIodef_la_LIBADD = $(top_builddir)/bindings/c++/.libs/libiodefv2cpp.la $(RUBY_LIBS)
nodist_LibIodef_la_SOURCES = LibIodef.cxx

LibIodef.cxx: $(top_srcdir)/bindings/c++/include/*.hxx $(top_srcdir)/bindings/libiodefv2cpp.i libiodefv2cpp-ruby.i 
if HAVE_SWIG
	$(SWIG) -c++ -DSWIG_COMPILE_LIBIODEFV2 -I$(top_srcdir)/bindings -I$(top_srcdir)/bindings/c++/include -I$(top_srcdir)/src/include -I$(top_srcdir)/src/libiodefv2-error -ruby -o $@ $(top_srcdir)/bindings/libiodefv2cpp.i
endif

endif
endif

-include $(top_srcdir)/git.mk
