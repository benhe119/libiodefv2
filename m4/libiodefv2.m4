dnl Autoconf macros for libiodefv2
dnl $id$

# Modified for LIBIODEFV2 -- Yoann Vandoorselaere
# Modified for LIBGNUTLS -- nmav
# Configure paths for LIBGCRYPT
# Shamelessly stolen from the one of XDELTA by Owen Taylor
# Werner Koch   99-12-09

dnl AM_PATH_LIBIODEFV2([MINIMUM-VERSION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND ]]], THREAD_SUPPORT)
dnl Test for libiodefv2, and define LIBIODEFV2_PREFIX, LIBIODEFV2_CFLAGS, LIBIODEFV2_PTHREAD_CFLAGS,
dnl LIBIODEFV2_LDFLAGS, and LIBIODEFV2_LIBS
dnl
AC_DEFUN([AM_PATH_LIBIODEFV2],
[dnl
dnl Get the cflags and libraries from the libiodefv2-config script
dnl
AC_ARG_WITH(libiodefv2-prefix, AC_HELP_STRING(--with-libiodefv2-prefix=PFX,
            Prefix where libiodefv2 is installed (optional)),
            libiodefv2_config_prefix="$withval", libiodefv2_config_prefix="")

  if test x$libiodefv2_config_prefix != x ; then
     if test x${LIBIODEFV2_CONFIG+set} != xset ; then
        LIBIODEFV2_CONFIG=$libiodefv2_config_prefix/bin/libiodefv2-config
     fi
  fi

  AC_PATH_PROG(LIBIODEFV2_CONFIG, libiodefv2-config, no)
  if test "$LIBIODEFV2_CONFIG" != "no"; then
  	if $($LIBIODEFV2_CONFIG --thread > /dev/null 2>&1); then
        	LIBIODEFV2_PTHREAD_CFLAGS=`$LIBIODEFV2_CONFIG --thread --cflags`

        	if test x$4 = xtrue || test x$4 = xyes; then
                	libiodefv2_config_args="--thread"
        	else
                	libiodefv2_config_args="--no-thread"
        	fi
  	else
        	LIBIODEFV2_PTHREAD_CFLAGS=`$LIBIODEFV2_CONFIG --pthread-cflags`
  	fi
  fi

  min_libiodefv2_version=ifelse([$1], ,0.1.0,$1)
  AC_MSG_CHECKING(for libiodefv2 - version >= $min_libiodefv2_version)
  no_libiodefv2=""
  if test "$LIBIODEFV2_CONFIG" = "no" ; then
    no_libiodefv2=yes
  else
    LIBIODEFV2_CFLAGS=`$LIBIODEFV2_CONFIG $libiodefv2_config_args --cflags`
    LIBIODEFV2_LDFLAGS=`$LIBIODEFV2_CONFIG $libiodefv2_config_args --ldflags`
    LIBIODEFV2_LIBS=`$LIBIODEFV2_CONFIG $libiodefv2_config_args --libs`
    LIBIODEFV2_PREFIX=`$LIBIODEFV2_CONFIG $libiodefv2_config_args --prefix`
    LIBIODEFV2_CONFIG_PREFIX=`$LIBIODEFV2_CONFIG $libiodefv2_config_args --config-prefix`
    libiodefv2_config_version=`$LIBIODEFV2_CONFIG $libiodefv2_config_args --version`


      ac_save_CFLAGS="$CFLAGS"
      ac_save_LDFLAGS="$LDFLAGS"
      ac_save_LIBS="$LIBS"
      CFLAGS="$CFLAGS $LIBIODEFV2_CFLAGS"
      LDFLAGS="$LDFLAGS $LIBIODEFV2_LDFLAGS"
      LIBS="$LIBS $LIBIODEFV2_LIBS"
dnl
dnl Now check if the installed libiodefv2 is sufficiently new. Also sanity
dnl checks the results of libiodefv2-config to some extent
dnl
      rm -f conf.libiodefv2test
      AC_TRY_RUN([
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libiodefv2/libiodefv2.h>

int
main ()
{
    system ("touch conf.libiodefv2test");

    if( strcmp( libiodefv2_check_version(NULL), "$libiodefv2_config_version" ) )
    {
      printf("\n*** 'libiodefv2-config --version' returned %s, but LIBIODEFV2 (%s)\n",
             "$libiodefv2_config_version", libiodefv2_check_version(NULL) );
      printf("*** was found! If libiodefv2-config was correct, then it is best\n");
      printf("*** to remove the old version of LIBIODEFV2. You may also be able to fix the error\n");
      printf("*** by modifying your LD_LIBRARY_PATH enviroment variable, or by editing\n");
      printf("*** /etc/ld.so.conf. Make sure you have run ldconfig if that is\n");
      printf("*** required on your system.\n");
      printf("*** If libiodefv2-config was wrong, set the environment variable LIBIODEFV2_CONFIG\n");
      printf("*** to point to the correct copy of libiodefv2-config, and remove the file config.cache\n");
      printf("*** before re-running configure\n");
    }
    else if ( strcmp(libiodefv2_check_version(NULL), LIBIODEFV2_VERSION ) ) {
        printf("\n*** LIBIODEFV2 header file (version %s) does not match\n", LIBIODEFV2_VERSION);
        printf("*** library (version %s)\n", libiodefv2_check_version(NULL) );
    }
    else {
      if ( libiodefv2_check_version( "$min_libiodefv2_version" ) )
        return 0;
      else {
        printf("no\n*** An old version of LIBIODEFV2 (%s) was found.\n",
                libiodefv2_check_version(NULL) );
        printf("*** You need a version of LIBIODEFV2 newer than %s. The latest version of\n",
               "$min_libiodefv2_version" );
        printf("*** LIBIODEFV2 is always available from https://www.libiodefv2-siem.org/project/libiodefv2/files\n");
        printf("*** \n");
        printf("*** If you have already installed a sufficiently new version, this error\n");
        printf("*** probably means that the wrong copy of the libiodefv2-config shell script is\n");
        printf("*** being found. The easiest way to fix this is to remove the old version\n");
        printf("*** of LIBIODEFV2, but you can also set the LIBIODEFV2_CONFIG environment to point to the\n");
        printf("*** correct copy of libiodefv2-config. (In this case, you will have to\n");
        printf("*** modify your LD_LIBRARY_PATH enviroment variable, or edit /etc/ld.so.conf\n");
        printf("*** so that the correct libraries are found at run-time))\n");
      }
    }
    return 1;
}
],, no_libiodefv2=yes,[echo $ac_n "cross compiling; assumed OK... $ac_c"])
       CFLAGS="$ac_save_CFLAGS"
       LIBS="$ac_save_LIBS"
       LDFLAGS="$ac_save_LDFLAGS"
  fi

  if test "x$no_libiodefv2" = x ; then
     AC_MSG_RESULT(yes)
     ifelse([$2], , :, [$2])
  else
     if test -f conf.libiodefv2test ; then
        :
     else
        AC_MSG_RESULT(no)
     fi
     if test "$LIBIODEFV2_CONFIG" = "no" ; then
       echo "*** The libiodefv2-config script installed by LIBIODEFV2 could not be found"
       echo "*** If LIBIODEFV2 was installed in PREFIX, make sure PREFIX/bin is in"
       echo "*** your path, or set the LIBIODEFV2_CONFIG environment variable to the"
       echo "*** full path to libiodefv2-config."
     else
       if test -f conf.libiodefv2test ; then
        :
       else
          echo "*** Could not run libiodefv2 test program, checking why..."
          CFLAGS="$CFLAGS $LIBIODEFV2_CFLAGS"
          LDFLAGS="$LDFLAGS $LIBIODEFV2_LDFLAGS"
          LIBS="$LIBS $LIBIODEFV2_LIBS"
          AC_TRY_LINK([
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libiodefv2/libiodefv2.h>
],      [ return !!libiodefv2_check_version(NULL); ],
        [ echo "*** The test program compiled, but did not run. This usually means"
          echo "*** that the run-time linker is not finding LIBIODEFV2 or finding the wrong"
          echo "*** version of LIBIODEFV2. If it is not finding LIBIODEFV2, you'll need to set your"
          echo "*** LD_LIBRARY_PATH environment variable, or edit /etc/ld.so.conf to point"
          echo "*** to the installed location  Also, make sure you have run ldconfig if that"
          echo "*** is required on your system"
          echo "***"
          echo "*** If you have an old version installed, it is best to remove it, although"
          echo "*** you may also be able to get things to work by modifying LD_LIBRARY_PATH"
          echo "***" ],
        [ echo "*** The test program failed to compile or link. See the file config.log for the"
          echo "*** exact error that occured. This usually means LIBIODEFV2 was incorrectly installed"
          echo "*** or that you have moved LIBIODEFV2 since it was installed. In the latter case, you"
          echo "*** may want to edit the libiodefv2-config script: $LIBIODEFV2_CONFIG" ])
          CFLAGS="$ac_save_CFLAGS"
          LDFLAGS="$ac_save_LDFLAGS"
          LIBS="$ac_save_LIBS"
       fi
     fi
     LIBIODEFV2_CFLAGS=""
     LIBIODEFV2_LDFLAGS=""
     LIBIODEFV2_LIBS=""
     ifelse([$3], , :, [$3])
  fi
  rm -f conf.libiodefv2test
  AC_SUBST(LIBIODEFV2_CFLAGS)
  AC_SUBST(LIBIODEFV2_PTHREAD_CFLAGS)
  AC_SUBST(LIBIODEFV2_LDFLAGS)
  AC_SUBST(LIBIODEFV2_LIBS)
  AC_SUBST(LIBIODEFV2_PREFIX)
  AC_SUBST(LIBIODEFV2_CONFIG_PREFIX)

  m4_ifdef([LT_INIT],
           [AC_DEFINE([LIBIODEFV2_APPLICATION_USE_LIBTOOL2], [], [Define whether application use libtool >= 2.0])],
           [])

])

dnl *-*wedit:notab*-*  Please keep this as the last line.
