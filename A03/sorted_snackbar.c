//---------------------------------------------------------------------
// sorted_snackbar.c 
// CS223 - Spring 2022
// Ask the user for a list of snacks and store them in alphabetical order
// Name: Bailey Hirota
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct snack {
	char name[32];
	int quantity;
	float cost;
	struct snack* next;
};

// Insert a new node to a list (implemented as a linked list). 
// The new node should store the given properties
// Param snacks: the first item in the list (NULL if empty)
// Param name: the snack name (max length is 32 characters)
// Param quantity: the snack quantity
// Param cost: the snack cost
// Returns the first item in the list
struct snack* insert_sorted(struct snack* snacks, 
		const char* name, int quantity, float cost) {
	struct snack *insert;
	insert = malloc(sizeof(*insert));
	if (insert == NULL) {
		printf("Error with malloc");
		return NULL;
	}
	strcpy(insert->name, name);
	insert->quantity = quantity;
	insert->cost = cost;
	insert->next = NULL;
	if (snacks == NULL) {
		return insert;
	}
	struct snack* current = snacks;
	int isRightPlace = 0;
	while (isRightPlace != 1 && current->next != NULL) {
		if (strcmp(current->name, name) < 0) {
			current = current->next;
		} else {
			isRightPlace = 1;
		}	
	}
	current->next = insert;
	if (current->next != NULL) {
		insert->next = current->next->next;
	}
	//current->next = insert;
	return snacks;
}

// Delete (e.g. free) all nodes in the given list of snacks
// Param snacks: the first node in the list (NULL if empty)
void clear(struct snack* snacks) {
	struct snack* current = snacks;
	struct snack* delete = snacks;
	while (current != NULL) {
		current = current->next;
		free(delete);
		delete = current;
	}
}

void print(struct snack* snacks) {
	struct snack* current = snacks;
	while (current != NULL) {
		printf("Name: %s\nQuantity: %d\nCost:$%f\n", current->name, current->quantity, current->cost);
		current = current->next;
	}
}

int main() {
	int num_snacks;
	char name[32];
	int quantity;
	float cost;
	printf("Enter number of snacks: ");
	scanf("%d", &num_snacks);
	struct snack *head;
	head = malloc(sizeof(*head));
	head = NULL;
	for (int i = 0; i < num_snacks; i++) {
		printf("Enter a name: ");
		scanf(" %s", name);
		printf("Enter a quantity: ");
		scanf(" %d", &quantity);
		printf("Enter a cost: ");
		scanf(" %f", &cost);
		head = insert_sorted(head, name, quantity, cost);
		printf("end");
	}
	printf("Welcome to Sorted Sally's Snack Bar.\n");
	print(head);
	clear(head);
	return 0;
}

