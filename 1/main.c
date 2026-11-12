#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int main(int argc, char** argv) {

	FILE *f = fopen("./input.txt", "r");
	if (f == NULL) return -1;

	char line[50];
	int16_t n = 50;
	int16_t sign;
	uint16_t counter = 0;
	while (fgets(line, 100, f)) {
		if (line[0] == 'L') {
			sign = -1;
		} else if (line[0] == 'R') {
			sign = 1;
		} else return -1;

		int16_t i = atoi(line + sizeof(char));
		n += sign * i;

		while (n < 0) {
			n = 100 + n;
		} 

		while (n >= 100) {
			n = n - 100;
		}

		if (n == 0) counter++;
	}

	printf("%u\n", counter);
	fclose(f);

	return 0;
}
