#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


int main(int argc, char** argv) {
	if (argc != 2) {
		printf("usage: ./main \"filename\"\n");
		return -1;
	}

	FILE *f = fopen(argv[1], "r");	
	if (f == NULL) return -1;

	char* line;
	size_t n;
	uint64_t res = 0;
	size_t line_len;
	uint64_t* indices = NULL;

	while (getline(&line, &n, f) != -1) {
		// defining list that holds the indices of the beams
		if (!indices) {
			line_len = strlen(line);
			indices = malloc(line_len * sizeof(uint64_t));
			for (size_t i = 0; i < line_len; i++) indices[i] = 0;
			indices[strchr(line, 'S') - line] = 1;
		} else {
			// if splitter at index: split (and increment res if indices changes)
			// splitting means removing index and adding index + 1 and index - 1 if they dont already exist
			for (size_t i = 0; i < line_len; i++) {
				if (!indices[i]) continue;
				if (line[i] == '^') {
					indices[i - 1] += indices[i];
					indices[i + 1] += indices[i];
					indices[i] = 0;
				}
			}
			// replace '.' with '|' where indices
			for (size_t i = 0; i < line_len; i++) {
				if (!indices[i]) continue;
				if (line[i] == '.') line[i] = '|';
			}
		}

		printf("%s", line);
	}

	for (size_t i = 0; i < line_len; i++) {
		res += indices[i];
	}

	free(line);
	free(indices);

	printf("\n%lu\n", res);
	return 0;
}
