#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

// will simply store the entire file into heap memory so I don't need to worry about saving to the file etc.



// 138 for the input, 1 for start padding, 1 for newline, 1 for null 
#define line_len 138
#define line_len_padded line_len + 3

// 138 lines and 2 padding lines
#define num_lines 138
#define num_lines_padded num_lines + 2

int main(int argc, char** argv) {
	FILE *f = fopen("input.txt", "r");
	if (f == NULL) return -1;

	char* lines[num_lines_padded];
	for (int i = 0; i < num_lines_padded; i++) {
		lines[i] = malloc(line_len_padded);
	}

	//padding first and last lines
	for (size_t i = 0; i < line_len_padded - 1; i++) {
		lines[0][i] = '.';
		lines[num_lines_padded - 1][i] = '.';
	}

	// adding start padding (we only need start padding, since the end has newlines.
	for (int i = 1; i < num_lines_padded - 1; i++) {
		lines[i][0] = '.';
	}

	size_t i = 1;
	// loading the file into lines
	while(true) {
		size_t n = line_len_padded;
		char* lineptr = &lines[i++][1];
		if (getline(&lineptr, &n, f) == -1) break;
	}


	uint16_t res = 0;
	while (true) {
		// computing which rolls can be removed, and replacing them with 'x'
		// loop over file
		for (size_t i = 1; i < num_lines_padded - 1; i++) {
			//loop over line
			for (size_t j = 1; j < (line_len_padded - 2) * sizeof(char); j++) {
				if (lines[i][j] != '@') continue;

				// checking the 8 neighbouring cells (could make an extra loop, but i think this is more tangible
				uint8_t count = 0;
				for (int8_t k = -1; k <= 1; k++) {
					if (lines[i - 1][j + k] == '@' || lines[i - 1][j + k] == 'x') count++;	
					if (k && (lines[i][j + k] == '@' || lines[i][j + k] == 'x')) count++;								// making sure we don't the k = 0 cell
					if (lines[i + 1][j + k] == '@' || (lines[i + 1][j + k]) == 'x') count++;
				}
				if (count < 4) {
					lines[i][j] = 'x';
				}
			}
		}
			
		// replace all x with .
		uint16_t amount = 0;
		for (size_t i = 1; i < num_lines_padded - 1; i++) {
			for (size_t j = 1; j < (line_len_padded - 2) * sizeof(char); j++) {
				if (lines[i][j] == 'x') {
					lines[i][j] = '.';
					amount++;
				}
			}
		}

		// stop when we didn't replace any
		if (!amount) break;
		res += amount;
	}

	for (size_t i = 0; i < num_lines_padded; i++) {
		free(lines[i]);
	}

	printf("%u\n", res);
	return 0;
}
