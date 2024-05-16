#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 2048

char *cmd = NULL;
char buf[BUF_SIZE];

void usage();
void print_hexdump(char *in, size_t size);

int main(int argc, char* argv[argc + 1]) {
	cmd = argv[0];

	if (argc == 2 && (!strcmp("--help", argv[1]) || !strcmp("-h", argv[1]))) {
		usage();	
		return EXIT_SUCCESS;
	}

	FILE *f = NULL;

	if (argc == 2) {
		f = fopen(argv[1], "r");
	} else {
		f = stdin;
	}

	if (f == NULL) {
		perror("failed to open file");
		return EXIT_FAILURE;
	}

	char *input = NULL;
	size_t input_size = 0;
	
	while(!feof(f)) {
		size_t bytes_read = fread(buf, 1, BUF_SIZE, f);

		if (bytes_read == 0 && !feof(f)) {
			perror("error: EOF");
			return EXIT_FAILURE;
		}
		if (!input) {
			input = malloc(bytes_read + 1);
			input_size += bytes_read;
			memcpy(input, buf, input_size);
			continue;
		}

		char *new = realloc(input, input_size + bytes_read + 1);
		if (!new) {
			free(input);
			perror("error: memory allocation fail");
			return EXIT_FAILURE;
		}
		input = new;
		memcpy(input + input_size, buf, bytes_read);
		input_size += bytes_read;
	}
	input[input_size] = '\0';
	
	print_hexdump(input, input_size);

	if (input) free(input);
	if (argc == 2) {
		fclose(f);
	}

	return EXIT_SUCCESS;
}

void print_hexdump(char *in, size_t size) {
	size_t offset = 0;

	for (size_t i = 0; i < size; i++) {
		if(i % 16 == 0) {
			fprintf(stdout, "%08zX: ", offset);
			offset += 16;
		}

		fprintf(stdout, "%02X", (unsigned char)in[i]);
		if (i % 2 != 0) fprintf(stdout, " ");

		if((i + 1) % 16 == 0 || i == size - 1)  {
			size_t spaces = 16 - (i % 16);
			for (size_t j = 0; j < spaces; j++) {
				fprintf(stdout, "   ");
			}

			for (size_t j = i - (i % 16); j <= i; j++) {
				if (' ' <= in[j] || in[j] >= '~') {
					fprintf(stdout, "%c", in[j]);
				} else {
					fprintf(stdout, ".");
				}
			}

			fprintf(stdout, "\n");
		}
	}
	fflush(stdout);
}

void usage() {
	fprintf(stdout, "usage:\n%s [<file>]\n", cmd);
	fprintf(stdout, "file is optional: if file is not provided\nthen %s will read from STDIN\n", cmd);
	fflush(stdout);
}
