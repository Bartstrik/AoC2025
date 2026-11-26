#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

typedef struct {
	int64_t x;
	int64_t y;
	int64_t z;
	int16_t circuit;
} junc_box;

typedef struct {
	junc_box* from;
	junc_box* to;
	bool connected; //connected is true when the boxes have a direct connection and false when they are indirectly connected (but they have to be checked)
} connected_junc_box;

int main(int argc, char** argv) {
	if (argc != 2) {
		printf("usage: ./main \"filename\"\n");
		return -1;
	}

	FILE *f = fopen(argv[1], "r");	
	if (f == NULL) return -1;

	uint16_t num_conns = !strcmp(argv[1], "test_input.txt") ? 10 : 1000;
	char* line = NULL;
	size_t line_len = 0;
	uint64_t num_boxes = 0;
	size_t n;
	uint16_t circuit_idx = 0;
	size_t connected_junc_boxes_idx = 0;


	// calculating file metadata
	while (getline(&line, &n, f) != -1) {
		if (!line_len) line_len = strlen(line);
		num_boxes++;
	}

	junc_box *junc_boxes = malloc(num_boxes * sizeof(junc_box));
	connected_junc_box *connected_junc_boxes = malloc(num_conns * sizeof(connected_junc_box));

	rewind(f);
		
	//initializing junc_boxes array
	for (size_t i = 0; getline(&line, &n, f) != -1; i++) {
		char* line_it = line;	
		char delim = ',';

		char* temp = strsep(&line_it, &delim);
		junc_boxes[i].x = atol(temp);
		temp = strsep(&line_it, &delim);
		junc_boxes[i].y = atol(temp);
		temp = strsep(&line_it, &delim);
		junc_boxes[i].z = atol(temp);
		junc_boxes[i].circuit = -1;
	}

	// creating connections
	for (uint16_t _ = 0; _ < num_conns; _++) {
		// calculating shortest connection
		size_t idx_i, idx_j;
		uint64_t conn_len_sq = UINT64_MAX;
		for (size_t i = 0; i < num_boxes; i++) {
			for (size_t j = 0; j < num_boxes; j++) {
				if (i == j) continue;

				// skipping entries that are already directly connected
				if ((junc_boxes[i].circuit != -1) && (junc_boxes[i].circuit == junc_boxes[j].circuit)) {
					bool conn_flag = false;
					for (size_t k = 0; k < connected_junc_boxes_idx; k++) {
						if (((connected_junc_boxes[k].from == &junc_boxes[i]) && (connected_junc_boxes[k].to == &junc_boxes[j])) || 
						   ((connected_junc_boxes[k].to == &junc_boxes[i]) && (connected_junc_boxes[k].from == &junc_boxes[j]))) {
							conn_flag = true;
						}
					}
					if (conn_flag) continue;
				}

				uint64_t len = 0;
				len += pow(junc_boxes[i].x - junc_boxes[j].x, 2);
				len += pow(junc_boxes[i].y - junc_boxes[j].y, 2);
				len += pow(junc_boxes[i].z - junc_boxes[j].z, 2);

				if (len < conn_len_sq) {
					conn_len_sq = len;
					idx_i = i;
					idx_j = j;
				}
			}
		}
		
		// skipping entries that are already indirectly connected (and making sure they don't get checked again)
		if ((junc_boxes[idx_i].circuit == junc_boxes[idx_j].circuit) && (junc_boxes[idx_j].circuit != -1)) {
			connected_junc_boxes[connected_junc_boxes_idx].connected = false;
		} else {
			connected_junc_boxes[connected_junc_boxes_idx].connected = true;
		}
		
		// if part of circuit, add to circuit, else, create new circuit
		if (junc_boxes[idx_i].circuit != -1) { 
			int16_t circuit = junc_boxes[idx_j].circuit;
			for (size_t i = 0; i < connected_junc_boxes_idx; i++) {
				if (connected_junc_boxes[i].from->circuit == circuit) connected_junc_boxes[i].from->circuit = junc_boxes[idx_i].circuit;
				if (connected_junc_boxes[i].to->circuit == circuit) connected_junc_boxes[i].to->circuit = junc_boxes[idx_i].circuit;
			}
			junc_boxes[idx_j].circuit = junc_boxes[idx_i].circuit;

		} else if (junc_boxes[idx_j].circuit != -1) {
			int16_t circuit = junc_boxes[idx_i].circuit;
			for (size_t i = 0; i < connected_junc_boxes_idx; i++) {
				if (connected_junc_boxes[i].from->circuit == circuit) connected_junc_boxes[i].from->circuit = junc_boxes[idx_j].circuit;
				if (connected_junc_boxes[i].to->circuit == circuit) connected_junc_boxes[i].to->circuit = junc_boxes[idx_j].circuit;
			}
			junc_boxes[idx_i].circuit = junc_boxes[idx_j].circuit;

		} else {
			junc_boxes[idx_i].circuit = circuit_idx;
			junc_boxes[idx_j].circuit = circuit_idx;
			circuit_idx++;
		}
		connected_junc_boxes[connected_junc_boxes_idx].from = &junc_boxes[idx_i];
		connected_junc_boxes[connected_junc_boxes_idx++].to = &junc_boxes[idx_j];

	}

	// calculating number of boxes per circuit
	uint64_t *num_boxes_per_circuit = malloc(circuit_idx * sizeof(uint64_t));
	for (size_t i = 0; i < num_boxes; i++) {
		if (junc_boxes[i].circuit != -1 ) {
			num_boxes_per_circuit[junc_boxes[i].circuit]++;	
		}
	}

	// adding 3 biggest circuits
	uint64_t res = 1;
	for (size_t _ = 0; _ < 3; _++) {
		uint64_t max_idx = 0;
		for (size_t i = 0; i < circuit_idx; i++) {
			if (num_boxes_per_circuit[i] > num_boxes_per_circuit[max_idx]) max_idx = i;
		}
		res *= num_boxes_per_circuit[max_idx];
		num_boxes_per_circuit[max_idx] = 0;
	}

	free(num_boxes_per_circuit);
	free(junc_boxes);
	free(connected_junc_boxes);
	free(line);

	printf("\n%lu\n", res);
	return 0;
}
