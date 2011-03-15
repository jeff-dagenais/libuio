# use autogen.sh

AC_PREREQ(2.59)
AC_INIT([UIO Device Access Library],
	[0.2.1],
	[libuio-bug@li-pro.net],
	[libuio],
	[http://sourceforge.net/projects/libuio/])

AC_CANONICAL_BUILD
AC_CANONICAL_HOST

AC_CONFIG_SRCDIR([src/uio_helper.h])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([1.10 dist-bzip2 dist-zip])
AM_MAINTAINER_MODE

AC_LANG([C])
AC_REQUIRE_CPP

LT_PREREQ([2.2])
AC_PROG_LIBTOOL
AC_PROG_INSTALL

AC_CONFIG_FILES([
	Makefile
	libuio.pc
	libuio-uninstalled.pc
	src/Makefile
	tools/Makefile
])
AC_OUTPUT
