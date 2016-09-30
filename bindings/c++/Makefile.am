if HAVE_EASY_BINDINGS

SUBDIRS = include

lib_LTLIBRARIES = libiodefv2cpp.la
libiodefv2cpp_ladir = ${prefix}/lib
libiodefv2cpp_la_SOURCES = \
	libiodefv2-error.cxx \
	iodefv2.cxx \
	iodefv2-class.cxx \
	iodefv2-criteria.cxx \
	iodefv2-path.cxx \
	iodefv2-time.cxx \
	iodefv2-value.cxx

libiodefv2cpp_la_LDFLAGS = -no-undefined -version-info @LIBIODEFV2CPP_SONAME@
libiodefv2cpp_la_LIBADD  = $(top_builddir)/src/.libs/libiodefv2.la
libiodefv2cpp_la_CPPFLAGS = -I$(top_builddir) -I$(top_builddir)/src/include -I$(top_srcdir)/src/include -I$(top_builddir)/src/libiodefv2-error -I$(top_builddir)/bindings/c++/include -I$(top_srcdir)/bindings/c++/include 

endif

-include $(top_srcdir)/git.mk
