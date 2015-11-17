// testing.c
// Testing trees

# include <assert.h>
# include <stdio.h>
# include <stdlib.h>
# include <time.h>
 
# include "tree.h"
 
static inline
char* build_fname() {
    char         *fname;
    fname = calloc(30, 1);
    time_t        t;
    t = time(NULL);
    strftime(fname, 30, "tree-%F-%H-%M-%S.dot", localtime(&t));
    return fname;
}

void tests(int depth) {
    struct tree          *t = NULL;
       
    // Empty cases
    assert(tree_size(t) == 0);
    assert(tree_height(t) == -1);
    assert(tree_find(t, 1) == NULL);
                
    // build a tree
    int                   key = 0;
    t = tree_build_regular(depth, &key);
    assert(tree_height(t) == depth);
    assert(tree_size(t) == tree_compute_size(depth));
    {
        // searching
        struct tree        *r;
        assert(tree_find(t, t->key) == t);
        assert( (r = tree_find(t, 1)) != NULL && r->key == 1);
        assert( (r = tree_find(t, key - 1)) != NULL && r->key == key - 1);
        assert( (r = tree_find(t, key + 1)) == NULL);
    }

    // Printing
    printf("Prefix:\n");
    tree_depth_prefix_print(t);
    printf("\n\nInfix:\n");
    tree_depth_infix_print(t);
    printf("\n\nSuffix\n");
    tree_depth_suffix_print(t);
    printf("\n");
               
    printf("\nBFS:\n");
    tree_breadth_print(t);

    //Dot output
    char                 *fname;
    fname = build_fname();
    printf("Dot file: %s\n", fname);
    tree_dot_out(t, fname);
}

int main(int argc, char *argv[]) {
    int           depth = 3;
    if (argc > 1)
        depth = strtol(argv[1], NULL, 10);
         
    tests(depth);
           
    return 0;
}
