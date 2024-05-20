/* Author: 陳羿閔 B081020008
 * Date: 2023-11-13
 * Purpose: A program that converts given strings to a binary search tree.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 1024

typedef struct Tree Tree;
typedef struct Node Node;

// Tree data structure
// ===============================
// - root: pointer to root node
// - insert: insert data into tree
// - delete: delete data from tree
// - search: search data in tree
// - print: print tree
struct Tree {
    Node *root;
    void (*insert)(Tree *self, const int data);
    void (*delete)(Tree *self, const int data);
    const Node *(*search)(const Tree *self, const int data);
    void (*print)(const Tree *self);
    void (*destroy)(Tree *self);
};

// Node data structure
// ===============================
// - data: integer data
// - p: pointer to parent node
// - left: pointer to left child node
// - right: pointer to right child node
// - create_node: create node
struct Node {
    int data;
    Node *p;
    Node *left;
    Node *right;
};

/* Create node */
// create node with data
// data: data to be stored in node
Node *create_node(const int data)
{
    Node *newnode = (Node *)malloc(sizeof(Node));
    if (!newnode)
        return NULL;

    newnode->data = data;
    newnode->p = NULL;
    newnode->left = NULL;
    newnode->right = NULL;
    return newnode;
}

/* Insert functions */
// insert data into tree
// self: pointer to tree (mock this pointer in C++)
// data: data to be inserted
void insert(Tree *self, const int data)
{
    // create node
    Node *newnode = create_node(data);
    if (!newnode) {
        perror("Failed to malloc new data");
        return;
    }

    // if root is empty, insert new data
    // otherwise, find empty space
    if (!self->root) {
        self->root = newnode;
        return;
    }

    // find empty space
    Node **tmp = &self->root;
    Node *parent = NULL;

    // if new data is smaller, go left; otherwise, go right
    while (*tmp) {
        parent = *tmp;
        if ((*tmp)->data > data)
            tmp = &(*tmp)->left;
        else
            tmp = &(*tmp)->right;
    }

    // insert new data
    *tmp = newnode;
    newnode->p = parent;
}

/* Delete functions */
void change_parent_child(Node *old_child, Node *new_child)
// change the deleted node's parent's child to new child
// old_child: pointer to old child node
// new_child: pointer to new child node
{
    if (!old_child->p)
        return;
    else if (old_child->p->right == old_child)
        old_child->p->right = new_child;
    else
        old_child->p->left = new_child;
}

int rm_leaf(Node *target)
// remove leaf node
// target: pointer to target node to be deleted
{
    int target_data = target->data;
    change_parent_child(target, NULL);
    free(target);
    return target_data;
}

int rm_one_child_node(Node *target)
// remove node with one child
// target: pointer to target node to be deleted
{
    Node *child = (target->right) ? target->right : target->left;
    change_parent_child(target, child);
    int target_data = target->data;
    free(target);
    return target_data;
}

void rm_two_child_node(Node *target)
// remove node with two children
// target: pointer to target node to be deleted
{
    Node *leftmost = target->right;
    while (leftmost->left)
        leftmost = leftmost->left;

    // remove leftmost node
    int leftmost_data = rm_one_child_node(leftmost);
    target->data = leftmost_data;
}

void delete(Tree *self, const int data)
// delete data from tree
// self: pointer to tree (mock this pointer in C++)
// data: data to be deleted
{
    Node *target = (Node *)self->search(self, data);
    if (!target) {
        puts("Data not found");
        return;
    }

    // case 1: leaf node
    if (!target->right && !target->left) {
        if (target == self->root)
            self->root = NULL;
        rm_leaf(target);

        // case 2: node with one child
    } else if (!target->left || !target->right) {
        if (target == self->root)
            self->root = (target->right) ? target->right : target->left;
        else
            rm_one_child_node(target);

        // case 3: node with two children
    } else {
        rm_two_child_node(target);
    }
}

const Node *search_helper(const Node *node, const int data)
/* Search functions */
// hlper function for searching node
// node: pointer to current node
// data: data to be searched
{
    if (!node)
        return NULL;
    if (node->data > data)
        return search_helper(node->left, data);
    if (node->data < data)
        return search_helper(node->right, data);
    return node;
}

const Node *search(const Tree *self, const int data)
// search data in tree
// self: pointer to tree (mock this pointer in C++)
// data: data to be searched
{
    return search_helper(self->root, data);
}

/* Print (infix traversal) */
// for recording the index of node in buffer array (for printing in infix order)
static int infix_i = 0;
void infix(Node *node, Node *buf[])
// infix traversal
// node: pointer to current node
// buf: buffer array for storing node for printing
{
    if (!node)
        return;

    infix(node->left, buf);
    buf[infix_i++] = node;
    infix(node->right, buf);
}

void print(const Tree *self)
// print tree
// self: pointer to tree (mock this pointer in C++)
{
    Node *buf[MAX];
    infix(self->root, buf);

    printf("node: ");
    for (int i = 0; i < infix_i; i++)
        printf("%d ", buf[i]->data);

    printf("\nleft: ");
    for (int i = 0; i < infix_i; i++) {
        Node *left = buf[i]->left;
        (left) ? printf("%d ", left->data) : printf("%d ", 0);
    }

    printf("\nright: ");
    for (int i = 0; i < infix_i; i++) {
        Node *right = buf[i]->right;
        (right) ? printf("%d ", right->data) : printf("%d ", 0);
    }
    infix_i = 0;
}

/* Destroy */
void destroy_helper(Node *node)
// helper function for destroying tree
// node: pointer to current node
{
    if (!node)
        return;
    destroy_helper(node->left);
    destroy_helper(node->right);
    free(node);
}

void destroy(Tree *self)
// destroy tree
// self: pointer to tree (mock this pointer in C++)
{
    destroy_helper(self->root);
    free(self);
}

/* Tree constructor*/
void init_tree(Tree **t)
// initialize tree
// t: pointer to the address of tree (for checking if the tree is malloced
// successfully)
{
    if (!(*t = (Tree *)malloc(sizeof(Tree)))) {
        perror("Failed to init binary tree");
        return;
    }

    (*t)->root = NULL;
    (*t)->search = &search;
    (*t)->print = &print;
    (*t)->delete = &delete;
    (*t)->insert = &insert;
    (*t)->destroy = &destroy;
}

void empty_array(int arr[], int len)
// clear array (set all elements to 0)
// arr: array to be emptied
// len: length of array
{
    for (int i = 0; i < len; i++)
        arr[i] = 0;
}

void convert_array_to_tree(Tree *t, int arr[], int len)
// convert array to tree
// t: pointer to tree
// arr: array to be converted
// len: length of array
{
    for (int i = 0, item = arr[0]; i < len; item = arr[++i])
        (!t->search(t, item)) ? t->insert(t, item) : t->delete (t, item);
}

int main(void)
{
    int digit = 0;
    int arr_len = 0;
    int arr[MAX] = {0};

    while (EOF != scanf("%d", &digit)) {
        if (-1 != digit) {
            arr[arr_len++] = digit;
        } else {
            // insert array into tree
            Tree *t;
            init_tree(&t);
            convert_array_to_tree(t, arr, arr_len);
            t->print(t);
            printf("\n\n");

            // clear buffer array
            arr_len = 0;
            empty_array(arr, MAX);
            t->destroy(t);
        }
    }
    return 0;
}

/*int main(void)*/
/*{*/
/*int arr[] = {*/
/*4, 11, 3, 2, 11, 6, 4, 5, -1,*/
/*};*/
/*int arr_len = sizeof(arr) / sizeof(arr[0]);*/

/*Tree *t;*/
/*init_tree(&t);*/
/*convert_array_to_tree(t, arr, arr_len);*/
/*t->print(t);*/
/*printf("\n\n");*/
/*t->destroy(t);*/
/*return 0;*/
/*}*/
