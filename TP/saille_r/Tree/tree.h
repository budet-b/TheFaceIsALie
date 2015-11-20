// Simple binary trees

# ifndef S3_PRACTICAL_20151109_TREE_H_
# define S3_PRACTICAL_20151109_TREE_H_

# include <stdlib.h>
# include <stdio.h>
# include <err.h>

struct tree {
    int           key;
    struct tree  *left, *right;
};
 
/*
tree_compute_size(depth) compute the size of a perfect tree.
*/
static inline
size_t tree_compute_size(int depth) {
    size_t n = 1;
    for(; depth >= 0; depth--)
        n*=2;
    return n-1;
}
 
/*
 tree_size(t) compute the size of the tree t
*/
size_t tree_size(struct tree *t);
 
/*
tree_height(t) compute the height of the tree t
Note: height of an empty tree is -1
 */
int tree_height(struct tree *t);
 
/*
tree_find(t, key) search for key in t
returns the pointer to the node holding key
returns NULL if not found.
*/
struct tree* tree_find(struct tree *t, int key);

/*
tree_build_regular(depth, &keys)
Build a perfect tree of height depth.
keys are set (and incremented) in infix order.
We first build recursively the left child (with its subtree)
Then we increment keys and use it to set the key of the current node
Finally we build recursively the right child.
*/
struct tree* tree_build_regular(int depth, int *keys);
 
/*
Basic printing: print the keys with a single space between each keys.
*/
 
/*
Depth first printing in prefix, infix or suffix order.
*/
 
void tree_depth_prefix_print(struct tree *t);
void tree_depth_infix_print(struct tree *t);
void tree_depth_suffix_print(struct tree *t);
 
/*
Printing using a breadth first traversal
Keys are separated by space
Each level are separated by a new-line.
*/
void tree_breadth_print(struct tree *t);
 
/*
dot output (see the subject)
*/
void tree_dot_out(struct tree *t, char *fname);

struct queue {
    void         *val;
    struct queue *next;
};
 
static inline
struct queue* queue_empty() { return NULL; }
static inline
int queue_is_empty(struct queue *q) { return q == NULL; }
 
void queue_push(struct queue **q, void *p);
void* queue_pop(struct queue **q);

# endif /* S3_PRACTICAL_20151109_TREE_H_ */
