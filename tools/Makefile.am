AM_CPPFLAGS = -I$(top_srcdir)/src
METASOURCES = AUTO

EXTRA_DIST =
EXTRA_DIST +=				\
	system.h

noinst_HEADERS = system.h

bin_PROGRAMS = lsuio rwuio

lsuio_SOURCES = lsuio.c
lsuio_LDADD = $(top_builddir)/src/libuio.la

rwuio_SOURCES = rwuio.c
rwuio_LDADD = $(top_builddir)/src/libuio.la
