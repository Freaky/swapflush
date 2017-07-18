/*
 * Small tool to clear a swap device without removing it.
 */

#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

static void
usage(void)
{
	fprintf(stderr, "usage: %s FILE ...", getprogname());
	exit(1);
}

int
main(int argc, char **argv)
{
	if (argc < 1)
		usage();

	int i, fd, rc = 0;

	for (i = 1; i < argc; i++) {
		if (-1 == open(argv[i], O_RDONLY)) {
			rc = 1;
			warn("open");
			continue;
		} else if (-1 == swapoff(argv[i])) {
			rc = 1;
			warn("swapoff");
		} else if (-1 == swapon(argv[i])) {
			rc = 1;
			warn("swapon");
		}
		close(fd);
	}

	return rc;
}



