#include "oasis.h"

#define BPE 8

List* new_list() {
	List* list = malloc(sizeof(List));
	list->len = 0;
	list->cap = 5; // initial cap
	list->data = malloc(list->cap * BPE);
	return list;
}

void extend(List* list, int delta) {
	if (list->len + delta <= list->cap) {
		return;
	}

	list->cap *= 2;
	void* old_data = list->data;
	list->data = malloc(list->cap * BPE);
	if (list->len) {
		memcpy(list->data, old_data, list->len * BPE);
	}
	free(old_data);
}

void append(List* list, void* val) {
	extend(list, 1);
	list->data[list->len] = val;
	list->len++;
}
