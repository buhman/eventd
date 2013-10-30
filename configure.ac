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
AC_CHECK_LIB([asound], [snd_mixer_open])
AC_CHECK_LIB([udev], [udev_new])

# Check for header files.
AC_CHECK_HEADERS([fcntl.h stdlib.h sys/ioctl.h unistd.h])

# Check for library functions.
AC_FUNC_MALLOC

AC_CONFIG_FILES([Makefile
		 src/Makefile])
AC_OUTPUT
