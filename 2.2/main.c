#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define max_str_len 11 + 1 //10000034826 — 11 digits + '\0'

char split_arr[max_str_len][(max_str_len / 2) + 1];


int main(int argc, char **argv) {
	FILE *f = fopen("./input.txt", "r");
	if (f == NULL) return -1;
	char *line = malloc(100);
	unsigned long res = 0;

	size_t len = 100;
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

			size_t len = strlen(id);
			for (size_t split_size = 1; split_size <= len / 2; split_size++) {
				if (len % split_size) continue;
				if (len == split_size) continue;

				size_t split_amount = len / split_size;
				for (size_t j = 0; j < split_amount; j++) {
					 strncpy(split_arr[j], id + j * split_size * sizeof(char), split_size);
					split_arr[j][split_size] = '\0';
				}

				bool flag = true;
				for (size_t k = 0; k < split_amount - 1; k++) {
					if (strcmp(split_arr[k], split_arr[k + 1])) flag = false;
				}

				if (flag) {
					res += low_bound;
					break;
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
