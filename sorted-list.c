#include "sorted-list.h"

/* < represents least to greatest
 * > represents greatest to least
 */
#define SORTING <
#define NEWSORTING >

/* SORTED LIST FUNCTIONS */
SortedListPtr SLCreate(CompareFuncT cf, DestructFuncT df, CompareFuncT cf2){
	SortedListPtr list = malloc(sizeof(struct SortedList) + 1);
	
	list->start = NULL;
	list->cf = cf;
	list->df = df;
	list->cf2 = cf2;
	list->numOfNodes = 0;
	
	return list;
}

void SLDestroy(SortedListPtr list){
	ItemPtr temp = list->start;
	while(temp != NULL){
		ItemPtr temp2 = temp;
		temp = temp->next;
		free(temp2);
	}
	free(list);
}

int SLInsert(SortedListPtr list, void *newObj, int index){
	if(newObj == NULL){
		return 0;
	}
	/* create new item node with new Object */
	ItemPtr brandNew = malloc(sizeof(struct Item) + 1);
	brandNew->data = newObj;
	brandNew->ptrRef = 0;
	
	/* empty list */
	if(list->start == NULL){
		list->start = brandNew;
		brandNew->next = NULL;
		brandNew->ptrRef++;
		return 1;
	}	
		/* using comparator */
	ItemPtr temp = list->start;
	/* at the front */
	int x = list->cf(brandNew->data, temp->data, index);
	int y;
	if(x SORTING 0){
		list->start = brandNew;
		brandNew->next = temp;
		brandNew->ptrRef++;
		list->numOfNodes += 1;
		return 1;
	}else if(x == 0){
		return 0;
	}
	/* in the middle */
	if(list->numOfNodes > 50){
		temp = SortingMethod(list, brandNew, temp, index, 1, list->numOfNodes);
		if(temp == NULL){
			return 0;
		}
	}
	while(temp->next != NULL){
		y = list->cf(brandNew->data, temp->next->data, index);
		if(y SORTING 0){
			brandNew->next = temp->next;
			brandNew->ptrRef++;
			temp->next = brandNew;
			list->numOfNodes += 1;
			return 1;
		}else if(y == 0){
			return 0;
		}else{
			temp = temp->next;
		}
	}
	/* last item */
	temp->next = brandNew;
	brandNew->next = NULL;
	brandNew->ptrRef++;
	list->numOfNodes += 1;
	return 1;
}

int SLnewInsert(SortedListPtr list, void *newObj, int index){
	if(newObj == NULL){
		return 0;
	}
	/* create new item node with new Object */
	ItemPtr brandNew = malloc(sizeof(struct Item) + 1);
	brandNew->data = newObj;
	brandNew->ptrRef = 0;
	
	/* empty list */
	if(list->start == NULL){
		list->start = brandNew;
		brandNew->next = NULL;
		brandNew->ptrRef++;
		return 1;
	}
		
		/* using comparator */
	ItemPtr temp = list->start;
	/* at the front */
	int x = list->cf(brandNew->data, temp->data, index);
	int y;
	if(x NEWSORTING 0){
		list->start = brandNew;
		brandNew->next = temp;
		brandNew->ptrRef++;
		return 1;
	}else if(x == 0){
		if(list->cf2(brandNew->data, temp->data, index) SORTING 0){
			list->start = brandNew;
			brandNew->next = temp;
			brandNew->ptrRef++;
			return 1;
		}
	}
	/* in the middle */
	while(temp->next != NULL){
		y = list->cf(brandNew->data, temp->next->data, index);
		if(y NEWSORTING 0){
			brandNew->next = temp->next;
			brandNew->ptrRef++;
			temp->next = brandNew;
			return 1;
		}else if(y == 0){
			if(list->cf2(brandNew->data, temp->next->data, index) SORTING 0){
				brandNew->next = temp->next;
				temp->next = brandNew;
				brandNew->ptrRef++;
				return 1;
			}else{
				temp = temp->next;
			}
		}else{
			temp = temp->next;
		}
	}
	/* last item */
	temp->next = brandNew;
	brandNew->next = NULL;
	brandNew->ptrRef++;
	return 1;
}

ItemPtr SortingMethod(SortedListPtr list, ItemPtr brandNew, ItemPtr temp, int index, int start, int max){
		if(max - start <= 3){
			return temp;
		}
		ItemPtr current = temp;
		
		int counter;
		for(counter=start; counter<((start+max) / 2); counter++){
			current = current->next;
		}
		
		int z = list->cf(brandNew->data, current->data, index);
		
		if(z SORTING 0){
			max = (start+max) / 2;
			return SortingMethod(list, brandNew, temp, index, start, max);
		}else if(z NEWSORTING 0){
			start = (start+max) / 2;
			return SortingMethod(list, brandNew, current, index, start, max);
		}else{
			return NULL;
		}
}

/* ITERATOR FUNCTIONS */
SortedListIteratorPtr SLCreateIterator(SortedListPtr list){
	SortedListIteratorPtr iterator = malloc(sizeof(struct SortedListIterator) + 1);
	if(list->start != NULL){
		iterator->head = list;
		iterator->current = list->start;
		iterator->current->ptrRef++;
		iterator->worth = list->start->data;
		return iterator;
	}
	else{
		return NULL;
	}
}

void SLDestroyIterator(SortedListIteratorPtr iter){
	if((iter != NULL) && (iter->current != NULL)){
		iter->current->ptrRef--;
	}
	
	free(iter);
}

void *SLGetItem(SortedListIteratorPtr iter){
	if((iter == NULL) || (iter->current == NULL)){
		return 0;
	}else{
		return iter->current->data;
	}
}

void *SLNextItem(SortedListIteratorPtr iter){
	if((iter == NULL) || (iter->current == NULL)){
		return NULL;
	}
	
	void *data;
	/* make sure we don't send the deleted data back */
	
	data = iter->current->data;
	
	iter->current->ptrRef--;
	iter->current = iter->current->next;
	
	if(iter->current != NULL){
		iter->current->ptrRef++;
		iter->worth = iter->current->data;
	}
	
	return data;
}
