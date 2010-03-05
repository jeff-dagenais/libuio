ACLOCAL_AMFLAGS = -I m4
AM_CFLAGS = -DUIO_LIB_NAME=\"libuio\" -DUIO_LIB_CUR=`grep -v '^\#' \
	$(srcdir)/libtool-version | cut -d: -f1` -DUIO_LIB_REV=`grep -v '^\#' $(srcdir)/libtool-version | \
	cut -d: -f2` -DUIO_LIB_AGE=`grep -v '^\#' $(srcdir)/libtool-version | cut \
	-d: -f3`
SUBDIRS = src tools
