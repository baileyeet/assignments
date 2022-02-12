//---------------------------------------------------------------------
// match_braces.c 
// CS223 - Spring 2022
// Identify matched braces from a given file
// Name:
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct node {
	char sym;
	int linenum;
	int colnum;
	struct node* next;
};

// Push a new node to a stack (implemented as a linked list). 
// The new node should store the given symbol, line number, and column number
// Param sym: a character symbol, '{' for this program
// Param line: the line number of the symbol
// Param line: the column number of the symbol
// Param top: the top node of the stack (NULL if empty)
// Returns the new top of the stack
struct node* push(char sym, int line, int col, struct node* top) {

	struct node* new_top = (struct node *) malloc(sizeof(struct node));
	if (new_top == NULL) {
		printf("Error with malloc");
		return NULL;
	}
	if (top == NULL) {
		new_top->sym = sym;
		new_top->linenum = line;
		new_top->colnum = col;
		new_top->next = NULL;
		return new_top;
	}
	//struct node* placer = top;
	//while (placer->next != NULL)  {
	//	placer = placer->next;
	//}
	new_top->sym = sym;
	new_top->linenum = line;
	new_top->colnum = col;
	new_top->next = top;
	//if(top != NULL) {
	//	placer->next = new_top;	
	//}
	return new_top;
}

// Pop the top node from a stack (implemented as a linked list) and frees it
// Param top: the top node of the current stack (NULL if empty)
// Returns the new top of the stack
struct node* pop(struct node* top) {
	struct node* placer = top;
	if (top == NULL) {
		return NULL;
	}
	placer = top;
	top = top->next;
	free(placer);
	return top;

}

// Delete (e.g. free) all nodes in the given stack
// Para top: the top node of the stack (NULL if empty)
void clear(struct node* top) {
	struct node* current = top;
	struct node* delete = top;
	while (current != NULL) {
		current = current->next;
		free(delete);
		delete = current;
	}
}

// Print all nodes in the given stack (from top to bottom)
// Param top: the top node of the stack (NULL if empty)
void print(struct node* top) {
	struct node* placer = top;
	if (top == NULL) {
		printf("Empty stack.");
		return; 
	}
	while (placer != NULL)  {
		printf("%c, %d, %d", placer->sym, placer->linenum, placer->colnum);
		placer = placer->next;
	}
	return;
}


int main(int argc, char* argv[]) {
	char *file_name = NULL;
	char current[32];
	int line = 1;
	int col = 1;
	char c = ' ';
	struct node* top = NULL;
	//if (argc < 1) {
	//	return 1;
	//}
	//file_name = argv[0];
	FILE *fp;	
	fp = fopen("prog1.c", "r");

	if(fp == NULL) { // check if successful
		perror("Error in opening file");
		return -1;
	}

	while(c = fgetc(fp)) {
		if( feof(fp) ) {
			break ;
		}	
		if (c == '{') {
			top = push(c, line, col, top);		
			//printf("(%d, %d)\n", line, col);
		} else if (c == '}') {
			// pop here and get last element	
			//		if (pushed->sym == pop(top)->sym) {
			//printf("%c %d %d", pop(top)->sym, pop(top)->linenum, pop(top)->colnum);
			printf("(%d, %d)\n", top->linenum, top->colnum);
			top = pop(top);
			printf("(%d, %d)\n", top->linenum, top->colnum);
			//printf("%c %d %d", pop(top)->sym, pop(top)->linenum, pop(top)->colnum);
			//printf("(%d, %d)\n", line, col);
			//			continue;
			//			}
			//printf("(%d, %d)\n", peek(top)->linenum, peek(top)->colnum);
			//printf("(%d, %d)\n", line, col);
			//pop(top);
		} else if (c == '\n') {
			line++;
			col = 1;
			continue;
		}
		col++;	
	}
	fclose(fp);
	//clear(top);

	return 0;
}
