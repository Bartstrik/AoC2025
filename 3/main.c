#include <stdio.h>
#include <stdint.h>

int main(int argc, char** argv) {
	
	#define line_size 100 
	#define buff_size line_size + 2 // keeping space for newline and null terminator
	char buff[buff_size];
	long unsigned res = 0;

	FILE* f = fopen("./input.txt", "r");
	while (fgets(buff, buff_size, f)) {
		// 1)iterate over string and find highest number. (if multiple, take lowest index one)
		uint8_t highest_num = 0;
		uint8_t highest_num_idx = -1;
		for (uint8_t i = 0; i < line_size; i++) {
			uint8_t num = *(buff + i * sizeof(char)) - 48;
			if (num > highest_num) {
				highest_num = num;
				highest_num_idx = i;
			}
		}
		// if index is final char:
		uint8_t second_num = 0;
		if (highest_num_idx == line_size - 1) {
			// iterate string and find the highest number, combine that with the one found in step 1
			for (int i = 0; i < line_size - 1; i++) {
				uint8_t num = *(buff + i * sizeof(char)) - 48;
				if (num > second_num) {
					second_num = num;
				}
			}
			res += 10 * second_num + highest_num;
			
		} else {
			// iterate partial string to the right and combine highest number with the one found in step 1
			for (int i = highest_num_idx + 1; i < line_size; i++) {
				uint8_t num = *(buff + i * sizeof(char)) - 48;
				if (num > second_num) {
					second_num = num;
				}
			}
			res += 10 * highest_num + second_num;
		}
	}

	printf("%lu\n", res);
	return 0;
}
