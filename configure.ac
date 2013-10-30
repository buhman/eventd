#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([eventd], [0], [zack@buhman.org])
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])

#

AM_INIT_AUTOMAKE([foreign -Wall -Werror])
AC_PROG_CC_C99

# Check for libraries.
#AC_CHECK_LIB([asound], [snd_mixer_open])

# Check for header files.
AC_CHECK_HEADERS([fcntl.h stdlib.h sys/ioctl.h unistd.h])

# Check for library functions.
AC_FUNC_MALLOC

PKG_CHECK_MODULES(ALSA, [alsa])
AC_SUBST(alsa_CFLAGS)
AC_SUBST(alsa_LIBS)

PKG_CHECK_MODULES(UDEV, [libudev])
AC_SUBST(udev_CFLAGS)
AC_SUBST(udev_LIBS)

AC_CONFIG_FILES([Makefile
		 src/Makefile])

#libtool
AC_PROG_LIBTOOL
LT_INIT
AC_CONFIG_MACRO_DIR([m4])

AC_OUTPUT
