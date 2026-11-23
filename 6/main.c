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
	size_t size = 0;
	char delim = ' ';

	// finding the amount of equations
	// and the amount of numbers per equation
	uint64_t n_nums  = 0;
	uint64_t n_eqs = 0;
	while (getline(&line, &size, f) != -1) {

		// calculating n_nums
		if (!n_eqs) {
			char prev_char = ' ';
			for (char* line_it = line; *line_it != '\0'; line_it++) {
				if (*line_it == ' ') {
					prev_char = *line_it;
				}

				if ((*line_it >= '0') && (*line_it <= '9')) {
					if (prev_char == ' ') n_eqs++;
					prev_char = *line_it;
				}
			}
		}
		if ((*line == '+') || (*line == '*')) break;
		n_nums++;
	}

	// 2D matrix 
	uint64_t* mat = malloc(n_eqs * n_nums * sizeof(uint64_t));
	// operator array
	char* ops = malloc(n_eqs * sizeof(char));

	uint64_t* mat_it = mat;
	char* ops_it = ops;

	for (char* line_it = line; *line_it != '\0'; line_it++) {
		if ((*line_it == '+') || (*line_it == '*')) {
			*ops_it = *line_it;
			ops_it++;
		}
	}

	rewind(f);

	// loading matrix
	while (getline(&line, &size, f) != -1) {
		if ((*line == '+') || (*line == '*')) break;

		char* line_it = line;
		while (*line_it != '\0') {
			if ((*line_it >= '0') && (*line_it <= '9')) {
				char* inner_line_it = line_it;
				while((*inner_line_it >= '0') && (*inner_line_it <= '9')) inner_line_it++;
				*inner_line_it = '\0';

				//storing value
				*mat_it = atol(line_it);
				mat_it++;
				line_it = inner_line_it;
			}
			line_it++;
		}
	}

	// loading operators
	ops_it = ops;
	for (char* line_it = line; *line_it != '\0'; line_it++) {
		if ((*line_it == '+') || (*line_it == '*')) {
			*ops_it = *line_it;
			ops_it++;
		}

	}

	uint64_t res = 0;
	for (uint64_t i = 0; i < n_eqs; i++) {
		if (ops[i] == '+') {
			for (uint64_t j = 0; j < n_nums; j++) {
				res += mat[i + n_eqs * j];
			}

		} else if (ops[i] == '*') {
			uint64_t temp = 1;
			for (uint64_t j = 0; j < n_nums; j++) {
				temp *= mat[i + n_eqs * j];
			}
			res += temp;
		}

	}

	free(mat);
	free(line);
	

	printf("\n%lu\n", res);
	return 0;
}
