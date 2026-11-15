#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

// 138 for the input, 1 for start padding, 1 for newline, 1 for null 
#define line_len 141

int main(int argc, char** argv) {
	// sliding window of 3 lines of the input
	
	FILE *f = fopen("input.txt", "r");
	char* lines[3];
	for (int i = 0; i < 3; i++) {
		lines[i] = malloc(line_len);
	}

	// adding start padding (we only need start padding, since the end has newlines.
	for (int i = 0; i < 3; i++) {
		lines[i][0] = '.';
	}

	size_t n = line_len;
	size_t ret = 0;
	uint16_t res = 0;
	bool freed = false;

	while (true) {
		if (ret != -1) {
			char* lineptr = &lines[0][1];
			ret = getline(&lineptr, &n, f);
		} 
		if (ret == -1 && lines[0]) {
			free(lines[0]);
			lines[0] = NULL;
		}

		if (lines[1]) {
			//loop over line
			for (size_t i = 1; i < (line_len - 2) * sizeof(char); i++) {
				if (lines[1][i] != '@') continue;

				// checking the 8 neighbouring cells
				uint8_t count = 0;
				for (int8_t j = -1; j <= 1; j++) {
					if (lines[0] && lines[0][i + j] == '@') count++;	// making sure we don't use check the freed line
					if (j && lines[1][i + j] == '@') count++;				// making sure we don't the j = 0 cell
					if (lines[2][i + j] == '@') count++;
				}
				if (count < 4) res++;
			}
		}
		

		char* tmp_line;
		tmp_line = lines[2];
		lines[2] = lines[1];
		lines[1] = lines[0];
		lines[0] = tmp_line;

		if (lines[0] == NULL && lines[1] == NULL && lines[2] == NULL) break;

	}

	printf("%u\n", res);
	return 0;
}
