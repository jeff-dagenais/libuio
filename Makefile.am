ACLOCAL_AMFLAGS = -I m4

AM_CFLAGS = -DUIO_LIB_NAME=\"libuio\" -DUIO_LIB_CUR=`grep -v '^\#' \
	$(srcdir)/libtool-version | cut -d: -f1` -DUIO_LIB_REV=`grep -v '^\#' $(srcdir)/libtool-version | \
	cut -d: -f2` -DUIO_LIB_AGE=`grep -v '^\#' $(srcdir)/libtool-version | cut \
	-d: -f3`

SUBDIRS = src tools

EXTRA_DIST =
EXTRA_DIST +=				\
	libuio.pc.in			\
	libuio-uninstalled.pc.in

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libuio.pc

dist-hook: ChangeLog

ChangeLog: configure.ac
	git log --no-merges master | \
	  unexpand -a | sed -e 's/\s\s*$$//' > $@

# release with paranoia backup to avoid debauched git repository
rel__tar = ${AMTAR} cof - ".git"
.PHONY: release
release:
	$(rel__tar) | bzip2 -9 -c >.backup-gitmeta-$$(date +%s).tar.bz2 && \
	touch configure.ac && \
	$(MAKE)  $(AM_MAKEFLAGS) \
	  top_distdir="$(top_distdir)" distdir="$(distdir)" \
	  distcheck && \
	git commit -asm 'Prepare release $(PACKAGE_VERSION)' && \
	git branch --no-track release/v$(PACKAGE_VERSION) && \
	git tag -sm 'v$(PACKAGE_VERSION)' v$(PACKAGE_VERSION) && \
	touch configure.ac && \
	$(MAKE)  $(AM_MAKEFLAGS) \
	  top_distdir="$(top_distdir)" distdir="$(distdir)" \
	  distcheck && \
	git commit --amend -aC HEAD
