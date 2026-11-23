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

	// finding the amount of equations and numbers
	uint64_t n_cols = 0;
	uint64_t n_rows = 0;
	while (getline(&line, &size, f) != -1) {

		// calculating n_rows
		if (!n_cols) {
			char prev_char = ' ';
			for (char* line_it = line; *line_it != '\0'; line_it++) {
				if (*line_it == ' ') {
					prev_char = *line_it;
				}

				if ((*line_it >= '0') && (*line_it <= '9')) {
					if (prev_char == ' ') n_cols++;
					prev_char = *line_it;
				}
			}
		}
		if ((*line == '+') || (*line == '*')) break;
		n_rows++;
	}

	uint64_t line_len = strlen(line);

	// line array
	// would be better to use getline to alocate so we don't need to copy the strings, but I didn't realize that when implementing this, and it works so yeah :/
	char** lines = malloc(n_rows * sizeof(char*));
	for (size_t i = 0; i < n_rows; i++) {
		lines[i] = malloc((line_len + 1) * sizeof(char));
	}
	
	// operator array
	char* ops = malloc((line_len + 1) * sizeof(char));


	// loading operators
	strcpy(ops, line);

	rewind(f);

	// loading lines
	for (char** lines_it = lines; getline(&line, &size, f) != -1; lines_it++) {
		if ((*line == '+') || (*line == '*')) break;
		strcpy(*lines_it, line);
	}

	uint64_t res = 0;
	
	// looping from end of line to start, storing individual numbers and either adding or multiplying when we hit a '+' or '*'
	size_t longest_num = 5;

	uint64_t nums[longest_num];
	bool flag = true;
	size_t index = 0;
	
	for (size_t i = line_len - 2; i != SIZE_MAX; i--) {
		if (flag) {
			for (size_t j = 0; j < longest_num; j++) nums[j] = 0;
			index = 0;
			flag = false;
		}

		char temp[n_rows + 1];
		for (size_t n = 0; n < n_rows + 1; n++) temp[n] = '\0';

		for (size_t j = 0; j < n_rows; j++) {
			temp[j] = lines[j][i];	
		}

			// remove spaces or newlines
		for (size_t k = 0; k < n_rows; k++) {
			if ((temp[k] == ' ') || (temp[k] == '\n')) {
				for (size_t kk = k; kk < n_rows; kk++) {
					temp[kk] = temp[kk + 1];
				}
			}
		}

		// if temp contains any numbers
		if (strpbrk(temp, "0123456789")) {
			nums[index++] = atol(temp);
		}
			
		if (ops[i] == '+') {
			uint64_t temp = 0;
			for (size_t j = 0; j < index; j++) {
				temp += nums[j];
			}
			printf("[+] %lu\n", temp);
			res += temp;
			flag = true;
		}

		if (ops[i] == '*') {
			uint64_t temp = 1;
			for (size_t j = 0; j < index; j++) {
				temp *= nums[j];
			}
			printf("[*] %lu\n", temp);
			res += temp;
			flag = true;
		}
	}
	
	for (size_t i = 0; i < n_rows; i++) free(lines[i]);
	
	free(lines);
	free(line);
	free(ops);
	

	printf("\n%lu\n", res);
	return 0;
}
