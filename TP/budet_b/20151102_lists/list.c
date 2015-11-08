//
//  list.c
//  
//
//  Created by Benjamin Budet on 03/11/2015.
//
//

#include <stdio.h>
#include "list.h"

struct list* list_empty(void){
    struct list *l;
    l = malloc(sizeof(struct list));
    l->next = NULL;
    return l;
}

size_t list_len(struct list *l){
    size_t i = 0;
    while (l != NULL) {
        i++;
        l = l->next;
    }
    return i;
}

void list_push_front(struct list *l, struct list *elm){
    elm->next = l->next;
    l->next=elm;
}

struct list* list_pop_front(struct list *l){
    if (l == NULL) {
        return NULL;
    }
    else {
        struct list* tmp;
        struct list* p;
        p = l;
        tmp = l->next;
        l = tmp;
        return p;
    }
}

struct list* list_find(struct list *l, int value){
    if (l == NULL) {
        return NULL;
    }
    else{
        if (l->value == value) {
            return l;
        }
        else{
            return list_find(l->next,value);
        }
    }
}

void list_insert(struct list *l, struct list *elm){
    while (l!=NULL) {
        if (l->next->value > elm->next->value) {
            l->next = elm->next;
            elm->next = l ->next;
            }
        l = l->next;
    }
}

void list_sort(struct list *l){
    struct list* head = l->next;
    l->next = NULL;
    while (head != NULL) {
        if (l->value > head->value){
            head->next=l->next;
        }
    }
    
}

/* Iterator */

typedef struct list    *iterator;

/*
 * list_begin(l) returns an iterator on the head of l
 */
iterator list_begin(struct list *l){
    return(l);
}
/*
 * list_is_end(it) true if it is the end of the list
 */
int list_is_end(iterator it){
    if(it->next != NULL)
        return 1; //liste vide
    else
        return 42; //liste non vide
}

/*
 * list_next(&it) move iterator it to the next position if possible
 */
void list_next(iterator *it){
    if(*it){
       it = it->next;
    }
}

/*
 * list_get(it) returns the current value
 */
int list_get(iterator it){
    return it->value;
}

/*
 * list_insert_at(it, elm) insert elm before the node pointed by it
 */
void list_insert_at(iterator it, struct list *elm);


//The following example demonstrate the use of iterator to traverse a list in order to print the list content:

void iterator_print(struct list *l) {
    int                           line;
    line = printf("l = [");
    
    // Iterate using iterators
    for (iterator it = list_begin(l); !list_is_end(it); list_next(&it)) {
        assert(it && it->next);
        if (line > 72) // break long lines
            line = printf("\n     ") - 1;
        line += printf(" %3d;", list_get(it));
    }
    
    printf("%s]\n", line>72 ? "\n" : " ");
}

int main(){
  /*  struct list *l = {1,3,5};
    struct list *elm = {2,8};
    list_insert(list *l, list *elm); */
    return 0;
}
