#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct {
    int64_t x;
	int64_t y;
} tile;

int main(int argc, char** argv) {
	if (argc != 2) {
		printf("usage: ./main \"filename\"\n");
		return -1;
	}

	FILE *f = fopen(argv[1], "r");	
	if (f == NULL) return -1;

	uint64_t num_tiles = 0;
	uint64_t width = 0, height = 0;
	char* line = NULL;
	size_t n;

	// calculating file metadata
	while (getline(&line, &n, f) != -1) {
		num_tiles++;
	}

	rewind(f);
		
	// load tiles
	tile *tiles = malloc(num_tiles * sizeof(tile));
	for (size_t i = 0; i < num_tiles; i++) {
		if (getline(&line, &n, f) == -1) return -1;
		char *line_it = line;
		char delim = ',';
		strsep(&line_it, &delim);
		tiles[i].x = (int64_t) atoll(line);
		tiles[i].y = (int64_t) atoll(line_it);
		if (tiles[i].x > width) width = tiles[i].x;
		if (tiles[i].y > height) height = tiles[i].y;
	}

	// printing tiles.
	width += 3;
	height += 2;
	uint64_t line_len = width + 2; // newline and null
	char* grid = malloc(line_len * height * sizeof(char));
	if (!strcmp(argv[1], "test_input.txt")) {

		for (size_t i = 0; i < height; i++) {
			for (size_t j = 0; j < width; j++) grid[i * line_len + j] = '.';
			grid[i * line_len + line_len - 2] = '\n';
			grid[i * line_len + line_len - 1] = '\0';
		}

		//updating red tiles
		for (size_t i = 0; i < num_tiles; i++) {
			uint64_t x = tiles[i].x;
			uint64_t y = tiles[i].y;
			grid[y * line_len + x] = '#';
		}

		//printing tiles
		for (size_t i = 0; i < height; i++) {
			printf("%s", &grid[i * line_len]);
		}
		
	}
	
	//computing recs and storing tiles
	tile *first, *second;
	uint64_t max_size = 0;	
	for (size_t i = 0; i < num_tiles; i++) {
		for (size_t j = i + 1; j < num_tiles; j++) {
			int64_t x_diff = tiles[i].x - tiles[j].x;
			int64_t y_diff = tiles[i].y - tiles[j].y;
	  		if (x_diff < 0) x_diff *= -1;
	  		if (y_diff < 0) y_diff *= -1;
			int64_t size = (x_diff + 1) * (y_diff + 1);
			if (size > max_size) {
				max_size = size;
				first = &tiles[i];
				second = &tiles[j];
			}
		}
	}

	// printing tiles with largest rec
	uint64_t min_x = (first->x < second->x) ? first->x : second->x;
	uint64_t max_x = (first->x > second->x) ? first->x : second->x;
	uint64_t min_y = (first->y < second->y) ? first->y : second->y;
	uint64_t max_y = (first->y > second->y) ? first->y : second->y;

	if (!strcmp(argv[1], "test_input.txt")) {
		for (size_t i = min_y; i <= max_y; i++) {
			for (size_t j = min_x; j <= max_x; j++) {
				grid[i * line_len + j] = 'O';
			}
		}

		printf("\n");
		//printing tiles
		for (size_t i = 0; i < height; i++) {
			printf("%s", &grid[i * line_len]);
		}
	}

	uint64_t res = (max_x - min_x + 1) * (max_y - min_y + 1);

	free(grid);
	free(tiles);

	printf("\n%lu\n", res);
	return 0;
}
