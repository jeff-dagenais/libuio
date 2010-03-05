INCLUDES =
METASOURCES = AUTO
lib_LTLIBRARIES = libuio.la
noinst_DATA = libtool-version
include_HEADERS = uio_helper.h
libuio_la_SOURCES = uio_find_devices.c uio_find_devices_byname.c uio_free.c \
	uio_get_all_info.c uio_get_device_attributes.c uio_get_event_count.c uio_get_mem_addr.c \
	uio_get_mem_size.c uio_get_name.c uio_get_version.c uio_helper.h uio_line_from_file.c \
	uio_mmap.c uio_munmap.c uio_num_from_filename.c uio_single_mmap.c \
	uio_single_munmap.c
libuio_la_LDFLAGS = -version-info `grep -v '^\#' $(srcdir)/libtool-version`
AM_CFLAGS = -DUIO_LIB_NAME=\"libuio\" -DUIO_LIB_CUR=`grep -v '^\#' \
	$(srcdir)/libtool-version | cut -d: -f1` -DUIO_LIB_REV=`grep -v '^\#' $(srcdir)/libtool-version | \
	cut -d: -f2` -DUIO_LIB_AGE=`grep -v '^\#' $(srcdir)/libtool-version | cut \
	-d: -f3`
