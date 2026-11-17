#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

//#define TEST

#ifdef TEST
#define file "test_input.txt"
#else
#define file "input.txt"
#endif

typedef struct {
	uint64_t start;
	uint64_t end;
} range;


int main(int argc, char** argv) {
	FILE *f = fopen(file, "r");	
	if (f == NULL) return -1;

	uint16_t res = 0;
	char* line;
	size_t size = 0;

	// finding num_ranges
	uint16_t num_ranges = 0;
	while(true) {
		getline(&line, &size, f);
		if (line[0] == '\n') break;
		num_ranges++;
	}

	rewind(f);

	//init range_list
	range* range_list = malloc(num_ranges * sizeof(range));
	range* range_it = range_list;

	// loop ranges
	while(true) {
		getline(&line, &size, f);
		if (line[0] == '\n') break;

		// turn '-' into '\0' to have two seperate decimal strings
		char* it = line;
		while (*it != '-') {
			it += sizeof(char);
		}
		*it = '\0';
		it += sizeof(char);

		// potential combine ranges
		// not today

		// store ranges
		range_it->start = atol(line);
		range_it->end   = atol(it);
		range_it += sizeof(range);
	}


	while(getline(&line, &size, f) != -1) {
		//loop through ranges and increment result if in range
		uint64_t num = atol(line);
		
		range_it = range_list;
		while(range_it < range_list + num_ranges * sizeof(range)) {
			if (num >= range_it->start && num <= range_it->end) {
				res++; 
				break;
			}
			range_it += sizeof(range);
		}
	}

	free(range_list);
	free(line);
	
	printf("%u\n", res);
	return 0;
}
