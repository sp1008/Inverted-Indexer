#ifndef SORTED_LIST_H
#define SORTED_LIST_H
/*
 * sorted-list.h
 */

#include <stdlib.h>
#include <stdio.h>


/*
 * When your sorted list is used to store objects of some type, since the
 * type is opaque to you, you will need a comparator function to order
 * the objects in your sorted list.
 *
 * You can expect a comparator function to return -1 if the 1st object is
 * smaller, 0 if the two objects are equal, and 1 if the 2nd object is
 * smaller.
 *
 * Note that you are not expected to implement any comparator or destruct
 * functions.  You will be given a comparator function and a destruct
 * function when a new sorted list is created.
 */

typedef int (*CompareFuncT)( void *, void *, int);
typedef void (*DestructFuncT)( void * );

struct Item{
	void *data;
	struct Item *next;
	int ptrRef;
};
typedef struct Item *ItemPtr;

/*
 * Sorted list type.  You need to fill in the type as part of your implementation.
 */
struct SortedList{
	ItemPtr start;
	CompareFuncT cf;
	CompareFuncT cf2;
	DestructFuncT df;
	int numOfNodes;
};
typedef struct SortedList* SortedListPtr;


/*
 * Iterator type for user to "walk" through the list item by item, from
 * beginning to end.  You need to fill in the type as part of your implementation.
 */
struct SortedListIterator{
	ItemPtr current;
	SortedListPtr head;
	void *worth;
};
typedef struct SortedListIterator* SortedListIteratorPtr;


/*
 * SLCreate creates a new, empty sorted list.  The caller must provide
 * a comparator function that can be used to order objects that will be
 * kept in the list, and a destruct function that gets rid of the objects
 * once they are no longer in the list or referred to in an iterator.
 * a
 * If the function succeeds, it returns a (non-NULL) SortedListT object,
 * otherwise, it returns NULL.
 *
 * You need to fill in this function as part of your implementation.
 */

SortedListPtr SLCreate(CompareFuncT cf, DestructFuncT df, CompareFuncT cf2);

/*
 * SLDestroy destroys a list, freeing all dynamically allocated memory.
 *
 * You need to fill in this function as part of your implementation.
 */
void SLDestroy(SortedListPtr list);


/*
 * SLInsert inserts a given object into a sorted list, maintaining sorted
 * order of all objects in the list.  If the new object is equal to a subset
 * of existing objects in the list, then the subset can be kept in any
 * order.
 *
 * If the function succeeds, it returns 1, othrewise it returns 0.
 *
 * I CHANGED THIS SLINSERT TO HOLD AN INT INDEX AND A FLAG
 */

int SLInsert(SortedListPtr list, void *newObj, int index);

/*	ADDED TWO FUNCTIONS
 * Same as SLInsert, but repetition is allowed.
 * Binary Search function if number of node exceeds 50
 */
 
int SLnewInsert(SortedListPtr list, void *newObj, int index);

ItemPtr SortingMethod(SortedListPtr list, ItemPtr brandNew, ItemPtr temp, int index, int start, int max);


/*
 * SLCreateIterator creates an iterator object that will allow the caller
 * to "walk" through the list from beginning to the end using SLNextItem.
 *
 * If the function succeeds, it returns a non-NULL pointer to a
 * SortedListIterT object, otherwise it returns NULL.  The SortedListT
 * object should point to the first item in the sorted list, if the sorted
 * list is not empty.  If the sorted list object is empty, then the iterator
 * should have a null pointer.
 *
 * You need to fill in this function as part of your implementation.
 */

SortedListIteratorPtr SLCreateIterator(SortedListPtr list);


/*
 * SLDestroyIterator destroys an iterator object that was created using
 * SLCreateIterator().  Note that this function should destroy the
 * iterator but should NOT affect the original list used to create
 * the iterator in any way.
 *
 * You need to fill in this function as part of your implementation.
 */

void SLDestroyIterator(SortedListIteratorPtr iter);


/*
 * SLGetItem returns the pointer to the data associated with the
 * SortedListIteratorPtr.  It should return 0 if the iterator
 * advances past the end of the sorted list.
 * 
 * You need to fill in this function as part of your implementation.
*/

void * SLGetItem( SortedListIteratorPtr iter );

/*
 * SLNextItem returns the pointer to the data associated with the
 * next object in the list encapsulated by the given iterator.
 * It should return a NULL when the end of the list has been reached.
 *
 * One complication you MUST consider/address is what happens if a
 * sorted list encapsulated within an iterator is modified while that
 * iterator is active.  For example, what if an iterator is "pointing"
 * to some object in the list as the next one to be returned but that
 * object is removed from the list using SLRemove() before SLNextItem()
 * is called.
 *
 * You need to fill in this function as part of your implementation.
 */

void * SLNextItem(SortedListIteratorPtr iter);

/*
 * Sorted Lister Iterator Helper Function reRailIter will track the sorted list 
 * from the start, stopping when the value of the current iterator data is equal 
 * or greater than the value of the data of the node. Then, the iterator will 
 * transfer over to that specific node, thus able to continue tracking the next 
 * item in the sorted list.
 */
 
void reRailIter(SortedListIteratorPtr iter);

#endif
