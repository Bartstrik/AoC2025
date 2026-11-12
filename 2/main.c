#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int main(int argc, char **argv) {
	FILE *f = fopen("./input.txt", "r");
	if (f == NULL) return -1;
	char *line = malloc(100);
	unsigned long res = 0;

	size_t len = 100;
	bool flag = true;
	while (getdelim(&line, &len, ',', f) != -1) {
		char *it = (char*)line;
		while (*it != '-') it += sizeof(char);
		*it = '\0';
		unsigned long low_bound = atol(line);
		it += sizeof(char);

		char *p2 = it;
		while (*it != '\n' && *it != ',') it += sizeof(char);
		*it = '\0';
		unsigned long up_bound = atol(p2);

		while (low_bound <= up_bound) {
			char id[100] = {'\0'};
			sprintf(id, "%lu", low_bound);

			uint8_t len = 0;
			while (*(id + len * sizeof(char)) != '\0') len++;

			if (!(len % 2)) {
				
				char *half = id + (len>>1) * sizeof(char);

				char second[100] = {'\0'};
				strcpy(second, half);

				*half = '\0';

				char first[100] = {'\0'};				
				strcpy(first, id);
				
				if (!strcmp(first, second)) {
					res += low_bound;
				}
			}
			low_bound++;
		}
	}

	printf("%lu\n", res);
	free(line);
	fclose(f);
	return 0;
}
