#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct chunk {
	int size;
	int in_use;
	struct chunk *next;
};


struct chunk *flist = NULL;

void *malloc (size_t size) {
	if( size == 0 ){
		return NULL; 
	}
	struct chunk *next = flist; 
	struct chunk *prev = NULL;
	while (next != NULL) {
		if (next->size >= size) {
			if(prev != NULL) {
				prev->next = next->next;
			} else {
				flist = next->next;
			}
			next->in_use += size;
			return (void*)(next + 1); 
		} else {
			prev = next;
			next = next->next; 
		}
	}

	/* use sbrk to allocate new memory */
	void *memory = sbrk(size + sizeof(struct chunk));
	if (memory == (void *)-1) {
		return NULL;
	} else {
		struct chunk *cnk = (struct chunk*)memory; 
		cnk->size = size;
		cnk->in_use = size;
		return (void*)(cnk + 1);
	}


}

void free(void *memory) {
	if (memory != NULL) {
		struct chunk *cnk = (struct chunk*)((struct chunk*)memory - 1);
		cnk->next = flist;
		cnk->in_use = 0;
		flist = cnk;
	}
	return;
}

void fragstats(void* buffers[], int len) {
	int total = 0;
	int mem = 0;
	int least = flist->size;
	int most = flist->size;
	struct chunk* cnk = flist;
	while (cnk != NULL) {
		total ++;
		mem += cnk->size;
		if (cnk->size < least) {
                        least = cnk->size;
                }
		if (cnk->size > most) {
			most = cnk->size;
		}
		cnk = cnk->next;
	}
	int use = 0;
	int total_unused = 0;	
	int small,big;
       	int first = 0;	
	for (int i = 0; i < len; i++) {
		if (buffers[i] != NULL) { //
			struct chunk *pointer = (struct chunk*)((struct chunk*)buffers[i] - 1);
                        total_unused += pointer->size;
			use++;
			if (first == 0) {
				small = pointer->size - pointer->in_use;
				big = pointer->size - pointer->in_use;
				first = 1;
				continue;	
			}
			if (pointer->size - pointer->in_use < small) {
				small = pointer->size - pointer->in_use;	
			}
			if (pointer->size - pointer->in_use > big) {
				big = pointer->size - pointer->in_use;
			}
		}
	}
	printf("Total blocks: %d Free: %d In Use: %d\n", total+use, total, use); 
	printf("Internal unused: total: %d average: %f smallest: %d largest: %d\n", total_unused, (double)total_unused/use, small, big);
	printf("External unused: total: %d average: %f smallest: %d largest: %d\n", mem, (double)mem/total, least, most);
}

