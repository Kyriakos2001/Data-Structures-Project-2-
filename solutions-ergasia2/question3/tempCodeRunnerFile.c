#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <Item.h>

BinarySearchTree *BSTinit()
{
    BinarySearchTree *bst = malloc(sizeof(BinarySearchTree));
    bst->root = NULL;
    return bst;
}

TreeNode *BSTinsert(TreeNode *node, Item item)
{
    if (node == NULL)
    {
        TreeNode *new_node = malloc(sizeof(TreeNode));
        new_node->item = item;
        new_node->left = new_node->right = NULL;
        return new_node;
    }
    int cmp = strcmp(item.isbn, node->item.isbn);
    if (cmp < 0)
        node->left = BSTinsert(node->left, item);
    else if (cmp > 0)
        node->right = BSTinsert(node->right, item);
    return node;
}

Item *BSTsearch(TreeNode *node, char *isbn)
{
    if (node == NULL)
        return NULL;
    int cmp = strcmp(isbn, node->item.isbn);
    if (cmp == 0)
        return &node->item;
    else if (cmp < 0)
        return BSTsearch(node->left, isbn);
    else
        return BSTsearch(node->right, isbn);
}

int BSTheight(TreeNode *node)
{
    if (node == NULL)
        return -1; // base case: empty tree
    int left_height = BSTheight(node->left);
    int right_height = BSTheight(node->right);
    return 1 + (left_height > right_height ? left_height : right_height);
}

void BSTfree(TreeNode *root)
{
    if (root == NULL)
    {
        return; // Base case: if the node is NULL, return immediately.
    }

    // Recursively free the left and right subtrees
    BSTfree(root->left);  // Free left subtree
    BSTfree(root->right); // Free right subtree

    // Free the node itself
    free(root); // Free the memory for the current node
}

int main()
{
    BinarySearchTree *bst = BSTinit(); // Initialize the binary search tree
    Item book1 = {"1234567890123", "Author One", "Book One", 2021};
    Item book2 = {"1234567890124", "Author Two", "Book Two", 2020};

    bst->root = BSTinsert(bst->root, book1);
    bst->root = BSTinsert(bst->root, book2);

    Item *found = BSTsearch(bst->root, "1234567890123");
    if (found != NULL)
    {
        printf("Found: %s by %s\n", found->title, found->author);
    }
    else
    {
        printf("Book not found.\n");
    }

    printf("Height of BST: %d\n", BSTheight(bst->root));

    BSTfree(bst->root);
    free(bst); // Free the entire tree structure

    return 0;
}
