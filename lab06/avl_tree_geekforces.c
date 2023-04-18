// C program to insert a node in AVL tree
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// An AVL tree node
struct Node
{
	int key;
	struct Node *left;
	struct Node *right;
	int height;
};

// A utility function to get the height of the tree
int height(struct Node *N)
{
	if (N == NULL)
		return 0;
	return N->height;
}

// A utility function to get maximum of two integers
int max(int a, int b)
{
	return (a > b)? a : b;
}

/* Helper function that allocates a new node with the given key and
	NULL left and right pointers. */
struct Node* newNode(int key)
{
	struct Node* node = (struct Node*)
						malloc(sizeof(struct Node));
	node->key = key;
	node->left = NULL;
	node->right = NULL;
	node->height = 1; // new node is initially added at leaf
	return(node);
}

// A utility function to right rotate subtree rooted with y
// See the diagram given above.
struct Node *rightRotate(struct Node *y)
{
	struct Node *x = y->left;
	struct Node *T2 = x->right;

	// Perform rotation
	x->right = y;
	y->left = T2;

	// Update heights
	y->height = max(height(y->left),
					height(y->right)) + 1;
	x->height = max(height(x->left),
					height(x->right)) + 1;

	// Return new root
	return x;
}

// A utility function to left rotate subtree rooted with x
// See the diagram given above.
struct Node *leftRotate(struct Node *x)
{
	struct Node *y = x->right;
	struct Node *T2 = y->left;

	// Perform rotation
	y->left = x;
	x->right = T2;

	// Update heights
	x->height = max(height(x->left),
					height(x->right)) + 1;
	y->height = max(height(y->left),
					height(y->right)) + 1;

	// Return new root
	return y;
}

// Get Balance factor of node N
int getBalance(struct Node *N)
{
	if (N == NULL)
		return 0;
	return height(N->left) - height(N->right);
}

// Recursive function to insert a key in the subtree rooted
// with node and returns the new root of the subtree.
struct Node* Insert(struct Node* node, int key)
{
	/* 1. Perform the normal BST insertion */
	if (node == NULL)
		return(newNode(key));

	if (key < node->key)
		node->left = Insert(node->left, key);
	else if (key > node->key)
		node->right = Insert(node->right, key);
	else // Equal keys are not allowed in BST
		return node;

	/* 2. Update height of this ancestor node */
	node->height = 1 + max(height(node->left),
						height(node->right));

	/* 3. Get the balance factor of this ancestor
		node to check whether this node became
		unbalanced */
	int balance = getBalance(node);

	// If this node becomes unbalanced, then
	// there are 4 cases

	// Left Left Case
	if (balance > 1 && key < node->left->key)
		return rightRotate(node);

	// Right Right Case
	if (balance < -1 && key > node->right->key)
		return leftRotate(node);

	// Left Right Case
	if (balance > 1 && key > node->left->key)
	{
		node->left = leftRotate(node->left);
		return rightRotate(node);
	}

	// Right Left Case
	if (balance < -1 && key < node->right->key)
	{
		node->right = rightRotate(node->right);
		return leftRotate(node);
	}

	/* return the (unchanged) node pointer */
	return node;
}


/* Given a non-empty binary search tree, return the
   node with minimum key value found in that tree.
   Note that the entire tree does not need to be
   searched. */
struct Node * minValueNode(struct Node* node)
{
    struct Node* current = node;
 
    /* loop down to find the leftmost leaf */
    while (current->left != NULL)
        current = current->left;
 
    return current;
}

// Recursive function to delete a node with given key
// from subtree with given root. It returns root of
// the modified subtree.
struct Node* Delete(struct Node* root, int key)
{
    // STEP 1: PERFORM STANDARD BST DELETE
 
    if (root == NULL)
        return root;
 
    // If the key to be deleted is smaller than the
    // root's key, then it lies in left subtree
    if ( key < root->key )
        root->left = Delete(root->left, key);
 
    // If the key to be deleted is greater than the
    // root's key, then it lies in right subtree
    else if( key > root->key )
        root->right = Delete(root->right, key);
 
    // if key is same as root's key, then This is
    // the node to be deleted
    else
    {
        // node with only one child or no child
        if( (root->left == NULL) || (root->right == NULL) )
        {
            struct Node *temp = root->left ? root->left :
                                             root->right;
 
            // No child case
            if (temp == NULL)
            {
                temp = root;
                root = NULL;
            }
            else // One child case
             *root = *temp; // Copy the contents of
                            // the non-empty child
            free(temp);
        }
        else
        {
            // node with two children: Get the inorder
            // successor (smallest in the right subtree)
            struct Node* temp = minValueNode(root->right);
 
            // Copy the inorder successor's data to this node
            root->key = temp->key;
 
            // Delete the inorder successor
            root->right = Delete(root->right, temp->key);
        }
    }
 
    // If the tree had only one node then return
    if (root == NULL)
      return root;
 
    // STEP 2: UPDATE HEIGHT OF THE CURRENT NODE
    root->height = 1 + max(height(root->left),
                           height(root->right));
 
    // STEP 3: GET THE BALANCE FACTOR OF THIS NODE (to
    // check whether this node became unbalanced)
    int balance = getBalance(root);
 
    // If this node becomes unbalanced, then there are 4 cases
 
    // Left Left Case
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);
 
    // Left Right Case
    if (balance > 1 && getBalance(root->left) < 0)
    {
        root->left =  leftRotate(root->left);
        return rightRotate(root);
    }
 
    // Right Right Case
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);
 
    // Right Left Case
    if (balance < -1 && getBalance(root->right) > 0)
    {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }
 
    return root;
}

// A utility function to print preorder traversal
// of the tree.
// The function also prints height of every node
void PrintPreorder(struct Node *root)
{
	if(root != NULL)
	{
		printf("%d ", root->key);
		PrintPreorder(root->left);
		PrintPreorder(root->right);
	}
}

void do_test1()
{
	struct Node *Tree = NULL;
	char	str[10];
	int		arr[10] = {30, 50, 40, 20, 60, 10, 80, 70, 90, 100};

	memset(str, 'i', sizeof(str));
	for (int i = 0; i < 10; i++)
	{
		if (str[i] == 'i')
			Tree = Insert(Tree, arr[i]);
		else if (str[i] == 'd')
			Tree = Delete(Tree, arr[i]);
		PrintPreorder(Tree);
		printf("\n");
	}
	printf("==================\n\n\n");
}

void do_test2()
{
	struct Node	*Tree = NULL;
	char	str[12];
	int		arr[12] = {40, 20, 60, 10, 30, 50, 70, 35, 65, 65, 50, 20};

	memset(str, 'i', sizeof(str));
	str[9] = 'd';
	str[10] = 'd';
	str[11] = 'd';
	for (int i = 0; i < 12; i++)
	{
		// printf("str[%d]: %c\n", i, str[i]);

		if (str[i] == 'i')
			Tree = Insert(Tree, arr[i]);
		else if (str[i] == 'd')
			Tree = Delete(Tree, arr[i]);
		PrintPreorder(Tree);
		printf("\n");
	}
	printf("==================\n\n\n");
}

void do_test3()
{
	struct Node	*Tree = NULL;
	char	str[13];
	int		arr[13] = {60, 40, 80, 20, 50, 70, 90, 10, 30, 100, 20, 100, 60};

	memset(str, 'i', sizeof(str));
	str[10] = 'd';
	str[11] = 'd';
	str[12] = 'd';
	for (int i = 0; i < 13; i++)
	{
		// printf("str[%d]: %c\n", i, str[i]);

		if (str[i] == 'i')
			Tree = Insert(Tree, arr[i]);
		else if (str[i] == 'd')
			Tree = Delete(Tree, arr[i]);
		PrintPreorder(Tree);
		printf("\n");
	}
	printf("==================\n\n\n");
}


void do_test4()
{
	struct Node	*Tree = NULL;
	char	str[17];
	int		arr[17] = {25, 15, 50, 10, 22, 35, 70, 4, 12, 18, 24, 31, 44, 66, 90, 30, 46};

	memset(str, 'i', sizeof(str));
	for (int i = 0; i < 17; i++)
	{
		// printf("str[%d]: %c\n", i, str[i]);

		if (str[i] == 'i')
			Tree = Insert(Tree, arr[i]);
		else if (str[i] == 'd')
			Tree = Delete(Tree, arr[i]);
		PrintPreorder(Tree);
		printf("\n");
	}
	printf("==================\n\n\n");
}

void do_test5()
{
	struct Node	*Tree = NULL;
	char	str[19];
	int		arr[19] = {50, 25, 75, 15, 35, 60, 90, 10, 30, 55, 70, 5, 80, 95, 40, 45, 5, 90, 25}; 

	memset(str, 'i', sizeof(str));
	str[16] = 'd';
	str[17] = 'd';
	str[18] = 'd';
	for (int i = 0; i < 19; i++)
	{
		// printf("str[%d]: %c\n", i, str[i]);

		if (str[i] == 'i')
			Tree = Insert(Tree, arr[i]);
		else if (str[i] == 'd')
			Tree = Delete(Tree, arr[i]);
		PrintPreorder(Tree);
		printf("\n");
	}
	printf("==================\n\n\n");
}

void do_test6()
{
	struct Node	*Tree = NULL;
	char	str[16];
	int		arr[16] = {60, 30, 90, 20, 40, 70, 100, 10, 50, 80, 110, 120, 10, 30, 50, 110};

	memset(str, 'i', sizeof(str));
	str[12] = 'd';
	str[13] = 'd';
	str[14] = 'd';
	str[15] = 'd';
	for (int i = 0; i < 16; i++)
	{
		// printf("str[%d]: %c\n", i, str[i]);

		if (str[i] == 'i')
			Tree = Insert(Tree, arr[i]);
		else if (str[i] == 'd')
			Tree = Delete(Tree, arr[i]);
		PrintPreorder(Tree);
		printf("\n");
	}
	printf("==================\n\n\n");
}



// // 이 테케는 ok
// int main()
// {
//   struct Node *root = NULL;

//   /* Constructing tree given in the above figure */
//     root = insert(root, 30);
//     root = insert(root, 50);
//     root = insert(root, 40);
//     root = insert(root, 20);
//     root = insert(root, 60);
//     root = insert(root, 10);
//     root = insert(root, 80);
//     root = insert(root, 80);
//     root = insert(root, 90);
//     root = insert(root, 100);
 
//     /* The constructed AVL Tree would be
//             9
//            /  \
//           1    10
//         /  \     \
//        0    5     11
//       /    /  \
//      -1   2    6
//     */
 
//     printf("Preorder traversal of the constructed AVL "
//            "tree is \n");
//     preOrder(root);
 
//     root = deleteNode(root, 10);
 
//     /* The AVL Tree after deletion of 10
//             1
//            /  \
//           0    9
//         /     /  \
//        -1    5     11
//            /  \
//           2    6
//     */
 
//     printf("\nPreorder traversal after deletion of 10 \n");
//     preOrder(root);
 
//     return 0;
// }

int	main(void)
{
	do_test1();
	do_test2();
	do_test3();
	do_test4();
	do_test5();
	do_test6();
	return (0);
}