# include "tree.h"

size_t tree_size(struct tree *t) {
    if( t == NULL)
        return 0;
    else
        return (1 + tree_size(t->left) + tree_size(t->right));
}

int max(int a, int b) {
    return (a>b?a:b);
}

int tree_height(struct tree *t) {
    if( t == NULL )
        return (-1);
    else
        return (1 + max(tree_height(t->left),tree_height(t->right)));
}

struct tree* tree_find(struct tree *t, int key) {
        if( t == NULL || t->key == key)
            return t;
        if( key < t->key)
            return tree_find(t->left, key);
        else 
            return tree_find(t->right, key);
}

struct tree* tree_build_regular(int depth, int *keys) {
    if(depth > -1) {
        struct tree *t = malloc(sizeof(struct tree));
        t->left = tree_build_regular(depth-1, keys);
        t->key = *keys;
        *keys = *keys + 1;
        t->right = tree_build_regular(depth-1, keys);
        return t;
    }
    else
        return NULL;
}

void tree_depth_prefix_print(struct tree *t) {
    if(t != NULL){
        printf(" %d", t->key);
        tree_depth_prefix_print(t->left);
        tree_depth_prefix_print(t->right);
    }
}

void tree_depth_infix_print(struct tree *t) {
    if(t != NULL) {
        tree_depth_infix_print(t->left);
        printf(" %d", t->key);
        tree_depth_infix_print(t->right);
    }
}

void tree_depth_suffix_print(struct tree *t) {
    if(t != NULL) {
        tree_depth_suffix_print(t->left);
        tree_depth_suffix_print(t->right);
        printf(" %d", t->key);
    }
}


void queue_push(struct queue **q, void *p) {
    struct queue *tmp = malloc(sizeof(struct queue));
    tmp->val = p;

    if (*q) {
        tmp->next = (*q)->next;
        (*q)->next = tmp;
    } else {
        tmp->next = tmp;
    }
    *q = tmp;
}
 
void* queue_pop(struct queue **q) {
    struct queue *tmp = (*q)->next;
    void *x = tmp->val;
    
    if (tmp == tmp->next )
        *q = NULL;
    else
        (*q)->next = tmp->next;
    
    free(tmp);
    return x;
}

void tree_breadth_print(struct tree *t) {
    struct queue *q = queue_empty();
    queue_push(&q, t);
    while(!queue_is_empty(q))
    {
        t = queue_pop(&q);
        printf("%d ", t->key);
        if(t->left)
            queue_push(&q, t->left);
        if(t->right)
            queue_push(&q, t->right);
    }
}

void tree_dot_out(struct tree *t, char *fname) {
    if (t) {
        FILE *file;
        if ( (file = fopen(fname,"w")) == NULL) 
            err(3, "Error while creating %s", fname);

        fprintf(file, "digraph Tree {\n");

        struct queue *q = queue_empty();
        queue_push(&q, t);
        while(!queue_is_empty(q))
        {
            t = queue_pop(&q);
            if(t->left) {
                fprintf(file, "%d -> %d\n", t->key, t->left->key);
                queue_push(&q, t->left);
            }
            if(t->right) {
                fprintf(file, "%d -> %d\n", t->key, t->right->key);
                queue_push(&q, t->right);
            }
        }

        fprintf(file,"}\n");
        fclose(file);
    }
}



















