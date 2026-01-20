#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct tree {
    int value;
    struct tree* left;
    struct tree* right;
}tree;

tree* insert(tree* root, int value);
int replace(tree* root);
int inorderToFile(tree* root, FILE* f);
int freeTree(tree* root);

int main(void) {
    tree* root = NULL;
    FILE* f;

    srand((unsigned)time(NULL));

    int n = 10;
    int x;

    for (int i = 0; i < n; i++) {
        x = 10 + rand() % 81;
        root = insert(root, x);
    }
   

    f = fopen("dat.txt", "w");
    if (!f) {
        return 1;
    } 

    fprintf(f, "Inorder nakon insert:\n");
    inorderToFile(root, f);
    fprintf(f, "\n");

    replace(root);

    fprintf(f, "Inorder nakon replace:\n");
    inorderToFile(root, f);
    fprintf(f, "\n");

    fclose(f);
    freeTree(root);

    return 0;
}

tree* insert(tree* root, int value) {
    if (root == NULL) {
        root = (tree*)malloc(sizeof(tree));
        if (!root) {
            printf("Error");
            return NULL;
        }
        root->value = value;
        root->left = NULL;
        root->right = NULL;
        return root;
    }

    if (value < root->value)
        root->left = insert(root->left, value);
    else if (value > root->value)
        root->right = insert(root->right, value);

    return root;
}


int replace(tree* root) {
    if (!root) {
        return 0;
    }
    int left = replace(root->left);
    int right = replace(root->right);
    int base = root->value;
    root->value = left + right;

    return base + left + right;
}


int inorderToFile(tree* root, FILE* f) {
    if (!root) {
        return 0;

    }
    inorderToFile(root->left, f);
    fprintf(f, "%d ", root->value);
    inorderToFile(root->right, f);

    return 0;
}

int freeTree(tree* root) {
    if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
    return 0;
}