SUBDIRS = libiodefv2-error include

AM_CPPFLAGS = @PCFLAGS@ -I$(srcdir)/include -I$(top_builddir) -I$(top_builddir)/src/include -I$(srcdir)/libiodefv2-error \
-I$(top_builddir)/libmissing -I$(top_srcdir)/libmissing $(LTDLINCL) @LIBIODEFV2_CFLAGS@ @LIBGCRYPT_CFLAGS@ 

pkgconfigdir = $(libdir)/pkgconfig

libiodefv2includedir = $(includedir)/
libiodefv2_la_LIBADD = @LIBIODEFV2_LIBS@ $(LIBLTDL) $(top_builddir)/libmissing/libmissing.la libiodefv2-error/libiodefv2-error.la
libiodefv2_la_LDFLAGS = $(GETADDRINFO_LIB) $(GETHOSTNAME_LIB) $(HOSTENT_LIB) $(INET_NTOP_LIB) $(INET_PTON_LIB) $(LIBSOCKET) $(LIB_POLL) $(LIB_SELECT) $(LTLIBINTL) $(LTLIBTHREAD) $(SERVENT_LIB) -no-undefined -version-info @LIBIODEFV2_SONAME@ -export-symbols-regex "^(iodefv2|_?libiodefv2)_"
lib_LTLIBRARIES = libiodefv2.la
BUILT_SOURCES = $(LTDLDEPS) 

AM_YFLAGS = -d
LEX_OUTPUT_ROOT = lex.yy

libiodefv2_la_SOURCES = 		\
	common.c			\
	ntp.c				\
	variable.c			\
	libiodefv2-hash.c			\
	libiodefv2-ident.c			\
	libiodefv2-io.c			\
	libiodefv2-log.c			\
	libiodefv2-string.c		\
	iodefv2-additional-data.c		\
	iodefv2-class.c			\
	iodefv2-criteria.c		\
	iodefv2-criteria-string.yac.y	\
	iodefv2-criteria-string.lex.l	\
	iodefv2-criterion-value.c		\
	iodefv2-data.c			\
	iodefv2-document-helpers.c		\
	iodefv2-document-read-json.c		\
	iodefv2-document-print.c		\
	iodefv2-document-print-json.c		\
	iodefv2-document-write.c		\
	iodefv2-object.c			\
	iodefv2-path.c			\
	iodefv2-time.c			\
	iodefv2-tree-wrap.c		\
	iodefv2-value.c			\
	iodefv2-value-type.c		

-include $(top_srcdir)/git.mk
