#include <stdio.h>
#include <stdlib.h>

#define BUF_SIZE 2048

char *cmd = NULL;
char buf[BUF_SIZE];

void usage();

int main(int argc, char* argv[argc + 1]) {
	cmd = argv[0];
}

void usage() {
	fprintf(stdout, "usage:\n%s [<file>]\n", cmd);
	fprintf(stdout, "file is optional: if file is not provided\nthen %s will read from STDIN\n", cmd);
	fflush(stdout);
}
