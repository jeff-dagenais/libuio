/*
   rwuio.c
   UIO helper function: ... ... ...

   Copyright (C) 2009, Hans J. Koch <hjk@linutronix.de>
   Copyright (C) 2009, Stephan Linz <linz@li-pro.net>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License version 2 as
   published by the Free Software Foundation.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <getopt.h>

#include "system.h"
#include "uio_helper.h"

static void usage (int status);
static void version (int status);

/* The name the program was run with, stripped of any leading path. */
char *program_name;

/* Option flags and variables */
static struct option const long_options[] =
{
	{"help", no_argument, 0, 'h'},
	{"map", required_argument, 0, 'm'},
	{"read", required_argument, 0, 'r'},
	{"uiodev", required_argument, 0, 'u'},
	{"verbose", no_argument, 0, 'v'},
	{"version", no_argument, 0, 'V'},
	{"write", required_argument, 0, 'w'},
	{"byte", no_argument, 0, '1'},
	{"word", no_argument, 0, '2'},
	{"dword", no_argument, 0, '4'},
	{"lword", no_argument, 0, '8'},
	{NULL, 0, NULL, 0}
};

int opt_verbose;
int opt_read;
int opt_write;
int opt_uiodev;
int opt_help;
int opt_version;
int opt_accessbits;

int uio_filter;
int uio_map;
long uio_offset;
unsigned long long uio_value;

static int decode_switches (int argc, char **argv);
static void uio_read(struct uio_info_t *info);
static void uio_write(struct uio_info_t *info);

int main (int argc, char **argv)
{
	struct uio_info_t *info_list, *p;

	program_name = argv[0];

	decode_switches (argc, argv);
	if (uio_offset < 0) {
		fprintf(stderr, "Negative offsets are not supported.\n");
		usage (-EINVAL);
	}

	if (opt_help)
		usage(0);

	if (opt_version)
		version(0);

	info_list = uio_find_devices(uio_filter);
	if (!info_list)
		printf("No UIO devices found.\n");

	p = info_list;

	while (p) {
		char dev_name[16];
		int fd;
		uio_get_all_info(p);
		uio_get_device_attributes(p);
		snprintf(dev_name,sizeof(dev_name),"/dev/uio%d",p->uio_num);
		fd = open(dev_name,O_RDWR);
		if (fd<0) {
			close(fd);
			p = p->next;
			continue;
		}
		uio_single_mmap(p,uio_map,fd);
		if (opt_read) uio_read(p);
		if (opt_write) uio_write(p);
		close(fd);
		p = p->next;
	}

	uio_free_info(info_list);
	exit (0);
}

static int uio_is_mapped(struct uio_info_t *info)
{
	if (opt_verbose) printf("mmap(): ");
	switch (info->maps[uio_map].mmap_result) {
		case UIO_MMAP_NOT_DONE:
			if (opt_verbose) printf("N/A\n");
			return 0;
		case UIO_MMAP_OK:
			if (opt_verbose) printf("OK\n");
			return 1;
		default:
			if (opt_verbose) printf("FAILED\n");
			return 0;
	}
}

static void uio_read(struct uio_info_t *info)
{
	unsigned long addr = info->maps[uio_map].addr + uio_offset;
	void* vp = info->maps[uio_map].internal_addr + uio_offset;
	if (uio_is_mapped(info))
		switch (opt_accessbits) {
			case 8:
				printf("%08lx: %08llx\n", addr,
				       *(volatile unsigned long long*)(vp));
			break;
			case 4:
				printf("%08lx: %08x\n", addr,
				       *(volatile unsigned int*)(vp));
			break;
			case 2:
				printf("%08lx: %04x\n", addr,
				       *(volatile unsigned short*)(vp));
			break;
			case 1:
			default:
				printf("%08lx: %02x\n", addr,
				       *(volatile unsigned char*)(vp));
			break;
		}

	return;
}

static void uio_write(struct uio_info_t *info)
{
	unsigned long addr = info->maps[uio_map].addr + uio_offset;
	void* vp = info->maps[uio_map].internal_addr + uio_offset;
	if (uio_is_mapped(info))
		switch (opt_accessbits) {
			case 8:
				*(volatile unsigned long long*)(vp) = uio_value;
				printf("%08lx: %016llx\n", addr, uio_value);
				break;
			case 4:
				*(volatile unsigned int*)(vp) = uio_value;
				printf("%08lx: %08x\n", addr, (unsigned int)uio_value);
				break;
			case 2:
				*(volatile unsigned short*)(vp) = uio_value;
				printf("%08lx: %04x\n", addr, (unsigned short)uio_value);
				break;
			case 1:
			default:
				*(volatile unsigned char*)(vp) = uio_value;
				printf("%08lx: %02x\n", addr, (unsigned char)uio_value);
				break;
		}

	return;
}

/* Set all the option flags according to the switches specified.
   Return the index of the first non-option argument.  */
static int decode_switches (int argc, char **argv)
{
	int opt, opt_index = 0;
	char *pc;
	opt_help = 0;
	opt_read = 0;
	opt_write = 0;
	opt_accessbits = 1; /* one byte access */
	opt_uiodev = 0;
	opt_version = 0;
	opt_verbose = 0;
	uio_filter = 0;
	uio_map = 0;
	uio_offset = 0;
	uio_value = 0;

	while (1) {
		opt = getopt_long(argc,argv,"1248hm:r:u:vVw:",long_options,&opt_index);
		if (opt == EOF)
			break;
		switch (opt) {
			case '1' : opt_accessbits = 1;
			break;
			case '2' : opt_accessbits = 2;
			break;
			case '4' : opt_accessbits = 4;
			break;
			case '8' : opt_accessbits = 8;
			break;
			case 'm' : uio_map = atoi(optarg);
			break;
			case 'r' : opt_read = 1; opt_write = 0;
			uio_offset = strtol(optarg, NULL, 0);
			break;
			case 'w' : opt_read = 0; opt_write = 1;
			uio_offset = strtol(optarg, &pc, 0);
			if (*pc++==':') uio_value = strtoull(pc, NULL, 0);
			break;
			case 'u' : opt_uiodev = 1;
			uio_filter = atoi(optarg);
			break;
			case 'v' : opt_verbose = 1;
			break;
			case 'h' : opt_help = 1;
			break;
			case 'V' : opt_version = 1;
			break;
		}
	}

	return 0;
}

static void usage (int status)
{
	printf (_("%s - List UIO devices.\n"), program_name);
	printf (_("Usage: %s [OPTIONS]\n"), program_name);
	printf (_("\
Options:\n\
  -h, --help       display this help and exit\n\
  -u, --uiodev     use the specific uio device (default 0)\n\
  -m, --map        work with given map number (default 0)\n\
  -r, --read       read from given offset (default 0)\n\
  -w, --write      write to given offset the given value (default 0:0)\n\
  -1, --byte       makes 8 bit (byte) access (default)\n\
  -2, --word       makes 16 bit (word) access\n\
  -4, --dword      makes 32 bit (double word) access\n\
  -8, --lword      makes 64 bit (quadruple word) access (not yet implemented)\n\
  -v, --verbose    also display device attributes\n\
  -V, --version    output version information and exit\n\
"));
	exit (status);
}

static void version (int status)
{
	printf (VERSION"\n");
	exit (status);
}
