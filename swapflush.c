/*
 * Small tool to flush swap devices.
 */

#include <sys/stat.h>
#include <sys/sysctl.h>
#include <vm/vm_param.h>

#include <err.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX_DEVS 16

#ifdef SWAPOFF_FORCE
#define swapoff(name) swapoff(name, 0)
#endif

static void
usage(void)
{
	fprintf(stderr, "usage: %s -a | FILE ...\n", getprogname());
	exit(1);
}

static int
list(char **devs)
{
	struct xswdev xsw;
	size_t mibsize, size;
	int mib[MAX_DEVS], n;

	mibsize = sizeof mib / sizeof mib[0];
	if (sysctlnametomib("vm.swap_info", mib, &mibsize) == -1)
		err(1, "sysctlnametomib()");
	for (n = 0; n < MAX_DEVS; ++n) {
		mib[mibsize] = n;
		size = sizeof xsw;
		if (sysctl(mib, mibsize + 1, &xsw, &size, NULL, 0) == -1)
			break;
		if (xsw.xsw_version != XSWDEV_VERSION)
			errx(1, "xswdev version mismatch");
		if (-1 == asprintf(devs + n, "/dev/%s", devname(xsw.xsw_dev, S_IFCHR)))
			err(1, "malloc");
	}
	if (errno != ENOENT)
		err(1, "sysctl()");

	return n;
}

static int
swapflush(char *name)
{
	int fd, rc = 0;

	if (-1 == (fd = open(name, O_RDONLY))) {
		warn("open");
		return 1;
	} else if (-1 == swapoff(name)) {
		rc = 1;
		warn("swapoff");
	} else if (-1 == swapon(name)) {
		rc = 1;
		warn("swapon");
	}
	close(fd);

	return rc;
}

static int
swapflusheach(int n, char **devs)
{
	int i, rc = 0;

	for (i = 0; i < n; i++) {
		if (swapflush(devs[i]))
			rc = 1;
	}

	return rc;
}

int
main(int argc, char **argv)
{
	if (argc < 2)
		usage();

	if (0 == strcmp(argv[1], "-a")) {
		if (argc > 2)
			usage();

		char *devs[MAX_DEVS];
		int n = list(devs);
		return swapflusheach(n, devs);
	}

	return swapflusheach(argc - 1, argv + 1);
}



