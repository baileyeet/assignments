#include <stdio.h>
#include <string.h>

struct cake {
	float cost;
	char flavor[16];
	struct cake* next;
};

struct cake makeCake(const char* flavor, float cost) {
	struct cake newCake;
	strncpy(newCake.flavor, flavor, 16);
	newCake.cost = cost;
	newCake.next = NULL;
	return newCake;
}

// todo: implement cheapestCake
struct cheapestCake(struct cake head) {
	struct cake temp = &head;
	//float least = head.cost;
	//char least_name* = head.name;
	while (temp.next != NULL) {
	
		//while (temp.next != NULL) {
		printf("cake: %S ($%f)", temp.flavor, temp.cost);
		if (temp.next.cost < temp.cost) {
			temp = temp.next;
			//least_name = temp.next.name;
		} 
		temp = temp.next;	
		//}
	}
	return temp;
}

int main() {
	struct cake cake1 = makeCake("red velvet", 2.00);
	struct cake cake2 = makeCake("chocolate", 1.75);
	struct cake cake3 = makeCake("mocha", 3.50);
	struct cake cake4 = makeCake("vanilla", 5.50);
	// todo: define 4th cake

	cake1.next = &cake2;
	cake2.next = &cake3;
	cake3.next = &cake4;
	// draw stack and heap here

	printf("%s ", cake1.flavor);
	// todo: call cheapestCak

	printf("The cheapest cake is %s", cheapestCake(cake1).name);
}
