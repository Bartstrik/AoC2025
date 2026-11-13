#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>

#define num_digits 12
#define line_size 100 
#define buff_size line_size + 2 // keeping space for newline and null terminator

int main(int argc, char** argv) {
	
	char buff[buff_size];
	uint64_t res = 0;

	FILE* f = fopen("./input.txt", "r");
	while (fgets(buff, buff_size, f)) {
		// for the first number, we take the highest number from the first 89 chars
		// for the second number we take the highest number from the first number index + 1 to the min(first number index + 12, 100)
		// und so weiter
		char nums[num_digits + 1] = {'\0' * num_digits + 1};
		uint8_t nums_idx = 0;
		uint8_t line_idx = 0;

		while (nums_idx < num_digits) {
			uint8_t end_idx = line_size - num_digits + nums_idx;	

			for (size_t i = line_idx; i <= end_idx; i++) {
				char num = *(buff + i * sizeof(char));
				if (num > nums[nums_idx]) {
					nums[nums_idx] = num;
					line_idx = i;
				}
			}
			nums_idx++;
			line_idx++;
		}
		res += atoll(nums);
	}

	printf("%lu\n", res);
	return 0;
}
