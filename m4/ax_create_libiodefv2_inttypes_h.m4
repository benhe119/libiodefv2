AC_DEFUN([AX_CREATE_LIBIODEFV2_INTTYPES_H],[
ac_libiodefv2_inttypes_h="$1"
ac_libiodefv2_inttypes_h_temp="$1_"

AC_SUBST(__LIBIODEFV2_HAVE_STDINT_H)
AC_SUBST(__LIBIODEFV2_HAVE_INTTYPES_H)
AC_SUBST(__LIBIODEFV2_HAVE_64BIT_LONG)
AC_SUBST(__LIBIODEFV2_STDINT_HAVE_UINT8)
AC_SUBST(__LIBIODEFV2_STDINT_HAVE_UINT16)
AC_SUBST(__LIBIODEFV2_STDINT_HAVE_UINT32)
AC_SUBST(__LIBIODEFV2_STDINT_HAVE_UINT64)
AC_SUBST(__LIBIODEFV2_64BIT_FORMAT_PREFIX)


ac_cv_have_stdint_h=
AC_CHECK_HEADER(stdint.h, ac_cv_have_stdint_h=yes, ac_cv_have_stdint_h=no)

if test "$ac_cv_have_stdint_h" = "yes" ; then
  __LIBIODEFV2_HAVE_STDINT_H="#define __LIBIODEFV2_HAVE_STDINT_H"
else
  __LIBIODEFV2_HAVE_STDINT_H="/* #define __LIBIODEFV2_HAVE_STDINT_H */"
fi

ac_cv_have_inttypes_h=
AC_CHECK_HEADER(inttypes.h, ac_cv_have_inttypes_h=yes, ac_cv_have_inttypes_h=no)

if test "$ac_cv_have_inttypes_h" = "yes" ; then
  __LIBIODEFV2_HAVE_INTTYPES_H="#define __LIBIODEFV2_HAVE_INTTYPES_H"
else
  __LIBIODEFV2_HAVE_INTTYPES_H="/* #define __LIBIODEFV2_HAVE_INTTYPES_H */"
fi

AC_CHECK_SIZEOF(long)

if test "$ac_cv_sizeof_long" = "8" ; then
  __LIBIODEFV2_HAVE_64BIT_LONG="#define __LIBIODEFV2_HAVE_64BIT_LONG"
else
  __LIBIODEFV2_HAVE_64BIT_LONG="/* #define __LIBIODEFV2_HAVE_64BIT_LONG */"
fi

ac_cv_have_uint8_t=
AC_CHECK_TYPE([uint8_t], ac_cv_have_uint8_t=yes, ac_cv_have_uint8_t=no)

if test "$ac_cv_have_uint8_t" = "yes" ; then
  __LIBIODEFV2_STDINT_HAVE_UINT8="#define __LIBIODEFV2_STDINT_HAVE_UINT8"
else
  __LIBIODEFV2_STDINT_HAVE_UINT8="/* #define __LIBIODEFV2_STDINT_HAVE_UINT8 */"
fi

ac_cv_have_uint16_t=
AC_CHECK_TYPE([uint16_t], ac_cv_have_uint16_t=yes, ac_cv_have_uint16_t=no)

if test "$ac_cv_have_uint16_t" = "yes" ; then
  __LIBIODEFV2_STDINT_HAVE_UINT16="#define __LIBIODEFV2_STDINT_HAVE_UINT16"
else
  __LIBIODEFV2_STDINT_HAVE_UINT16="/* #define __LIBIODEFV2_STDINT_HAVE_UINT16 */"
fi

ac_cv_have_uint32_t=
AC_CHECK_TYPE([uint32_t], ac_cv_have_uint32_t=yes, ac_cv_have_uint32_t=no)

if test "$ac_cv_have_uint32_t" = "yes" ; then
  __LIBIODEFV2_STDINT_HAVE_UINT32="#define __LIBIODEFV2_STDINT_HAVE_UINT32"
else
  __LIBIODEFV2_STDINT_HAVE_UINT32="/* #define __LIBIODEFV2_STDINT_HAVE_UINT32 */"
fi

ac_cv_have_uint64_t=
AC_CHECK_TYPE([uint64_t], ac_cv_have_uint64_t=yes, ac_cv_have_uint64_t=no)

if test "$ac_cv_have_uint64_t" = "yes" ; then
  __LIBIODEFV2_STDINT_HAVE_UINT64="#define __LIBIODEFV2_STDINT_HAVE_UINT64"
else
  __LIBIODEFV2_STDINT_HAVE_UINT64="/* #define __LIBIODEFV2_STDINT_HAVE_UINT64 */"
fi



])
