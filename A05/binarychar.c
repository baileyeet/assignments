#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int compute(int  n) {
	int dec = 0, i = 0, rem;
	while (n!=0) {
		rem = n % 10;
		n /= 10;
		dec += rem * pow(2, i);
		++i;
	}
	return dec;
}

int main() {
	//unsigned char bits[8];
	int num;
	printf("Enter 8 bits: ");
	scanf("%d", &num);
	num = compute(num);
	printf("Your character is: %c\n", num);
	return 0;
}
