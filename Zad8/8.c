#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef struct tree {
    int value;
    struct tree* left;
    struct tree* right;
}tree;

tree* insert(tree* root, int value);
tree* deleteNode(tree* root, int value);
tree* find(tree* root, int value);
tree* findMin(tree* root);
int inorder(tree* root);
int preorder(tree* root);
int postorder(tree* root);
int freeTree(tree* root);
int height(tree* root);
int printLevel(tree* root, int level);
int levelOrder(tree* root);

int main() {
    tree* root = NULL;
    int choice, value;
    do {
        printf("1:Unos\n");
        printf("2:Inorder\n");
        printf("3:Preorder\n");
        printf("4:Postorder\n");
        printf("5:Level order\n");
        printf("6:Pretrazivanje\n");
        printf("7:Brisanje\n");
        printf("0:Izlaz\n");
        scanf("%d", &choice);

        switch (choice) {

        case 1:
            printf("Unesi vrijednost: ");
            scanf("%d", &value);
            root = insert(root, value);
            break;

        case 2:
            inorder(root);
            printf("\n");
            break;

        case 3:
            preorder(root);
            printf("\n");
            break;

        case 4:
            postorder(root);
            printf("\n");
            break;

        case 5:
            levelOrder(root);
            printf("\n");
            break;

        case 6:
            printf("Unesi vrijednost za pretrazivanje: ");
            scanf("%d", &value);
            find(root, value);
            break;

        case 7:
            printf("Unesi vrijednost za brisanje: ");
            scanf("%d", &value);
            root = deleteNode(root, value);
            break;

        case 0:
            break;

        default:
            printf("Neispravan odabir!\n");
        }

    } while (choice != 0);

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

tree* deleteNode(tree* root, int value) {
    tree* temp;

    if (root == NULL)
        return NULL;

    if (value < root->value)
        root->left = deleteNode(root->left, value);
    else if (value > root->value)
        root->right = deleteNode(root->right, value);
    else {
        if (root->left != NULL && root->right != NULL) {
            temp = findMin(root->right);
            root->value = temp->value;
            root->right = deleteNode(root->right, temp->value);
        }
        else {
            temp = root;
            if (root->left == NULL)
                root = root->right;
            else
                root = root->left;
            free(temp);
        }
    }
    return root;
}

tree* find(tree* root, int value) {
    if (root == NULL)
        return NULL;

    if (value < root->value)
        return find(root->left, value);
    else if (value > root->value)
        return find(root->right, value);
    else
        return root;
}

tree* findMin(tree* root) {
    if (root == NULL)
        return NULL;

    while (root->left != NULL)
        root = root->left;

    return root;
}

int inorder(tree* root) {
    if (root != NULL) {
        inorder(root->left);
        printf("%d ", root->value);
        inorder(root->right);
    }
    return 0;
}

int preorder(tree* root) {
    if (root != NULL) {
        printf("%d ", root->value);
        preorder(root->left);
        preorder(root->right);
    }
    return 0;
}

int postorder(tree* root) {
    if (root != NULL) {
        postorder(root->left);
        postorder(root->right);
        printf("%d ", root->value);
    }
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

int height(tree* root) {
    int l, r;

    if (root == NULL)
        return 0;

    l = height(root->left);
    r = height(root->right);

    if (l > r)
        return l + 1;
    else
        return r + 1;
}

int printLevel(tree* root, int level) {
    if (root == NULL)
        return;

    if (level == 1)
        printf("%d ", root->value);
    else {
        printLevel(root->left, level - 1);
        printLevel(root->right, level - 1);
    }
    return 0;
}

int levelOrder(tree* root) {
    int h = height(root);
    for (int i = 1; i <= h; i++)
        printLevel(root, i);
    return 0;
}