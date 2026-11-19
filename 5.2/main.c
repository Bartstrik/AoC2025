#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

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
	range* range_end = range_list + num_ranges * sizeof(range);
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
		uint64_t start = atol(line);
		uint64_t end = atol(it);

		if (end < start) {
			uint64_t temp = start;
			start = end;
			end = temp;
		}
		
		// store ranges in a new range
		range_it->start = start;
		range_it->end   = end;
		range_it += sizeof(range);
	}

	//combine ranges
	while(true) {
		bool found = false;

		for (range_it = range_list; range_it < range_end; range_it += sizeof(range)) {
			uint64_t start = range_it->start;
			uint64_t end = range_it->end;
			
			// skipping entries that have been zeroed
			if ((start == 0) && (end == 0)) continue;

			// inner loop
			for (range* inner_it = range_list; inner_it < range_end; inner_it += sizeof(range)) {
				// skipping entries that have been zeroed
				if ((inner_it->start == 0) && (inner_it->end == 0)) continue;

				//skiping itself
				if (range_it == inner_it) continue;

				if ((inner_it->start <= start && inner_it->end >= end) || (inner_it->start <= end && inner_it->end >= end)) {
					if (inner_it->start <= start) {
						range_it->start = inner_it->start;
					}
					if (inner_it->end >= end) {
						range_it->end = inner_it->end;
					}
					inner_it->start = 0;
					inner_it->end = 0;
					found = true;
					break;
				}
			}
		}
		if (found == false) break;
	}

	uint64_t res = 0;
	for (range_it = range_list; range_it < range_end; range_it += sizeof(range)) {
		if ((range_it->start == 0) && (range_it->end == 0)) continue;
		printf("%lu - %lu + 1 = %lu\n", range_it->end, range_it->start, range_it->end - range_it->start + 1);
		res += range_it->end - range_it->start + 1;
	}

	free(range_list);
	free(line);
	
	printf("\n%lu\n", res);
	return 0;
}
