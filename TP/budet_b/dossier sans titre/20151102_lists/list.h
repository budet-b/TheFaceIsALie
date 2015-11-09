//
//  list.h
//  
//
//  Created by Benjamin Budet on 03/11/2015.
//
//

#ifndef list_h
#define list_h

# include <stdlib.h>

struct list {
    struct list          *next;
    int                   value;
};

static inline
int list_is_empty(struct list *l) { return l->next==NULL; }

/* Basic Operations */

/*
 * list_empty() create a new list (new sentinel)
 */
struct list* list_empty(void);

/*
 * list_len(l) return the number of element in the list
 */
size_t list_len(struct list *l);

/*
 * list_push_front(l, elm) push elm in front of l
 */
void list_push_front(struct list *l, struct list *elm);

/*
 * list_pop_front(l) extract and return first element
 * return NULL if the list is empty.
 */
struct list* list_pop_front(struct list *l);

/*
 * list_find(l,value) search for value in l
 * returns the pointer to the cell containing value
 * returns NULL if not found
 */
struct list* list_find(struct list *l, int value);

/* Sorting */

/*
 * list_insert(l, elm) inserts elm at the right place into a sorted list
 * (increasing order)
 */
void list_insert(struct list *l, struct list *elm);

/*
 * list_sort(l) sort l in increasing order.
 */
void list_sort(struct list *l);

/* Iterator */

typedef struct list    *iterator;

/*
 * list_begin(l) returns an iterator on the head of l
 */
iterator list_begin(struct list *l);

/*
 * list_is_end(it) true if it is the end of the list
 */
int list_is_end(iterator it);

/*
 * list_next(&it) move iterator it to the next position if possible
 */
void list_next(iterator *it);

/*
 * list_get(it) returns the current value
 */
int list_get(iterator it);

# endif
