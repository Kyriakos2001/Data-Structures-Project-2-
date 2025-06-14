typedef struct Item
{
    char *isbn;   
    char *author; 
    char *title; 
    int year;     
} Item;

typedef struct TreeNode
{
    Item item;
    struct TreeNode *left, *right;
} TreeNode;

typedef struct BinarySearchTree
{
    TreeNode *root;
} BinarySearchTree;
