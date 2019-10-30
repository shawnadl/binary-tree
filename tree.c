/*
*Sangmyung University
*Computer Science
*Data structure
*Kyungha Min Prof.
*Student: Luong Duc Anh
*Practice no.7: Tree
*/
/*========================================================================================================*/
/*========================================================================================================*/
#include<stdio.h>
#include<stdlib.h>

struct _treeNode {
	struct _treeNode *left;
	int data;
	struct _treeNode *right;
};

typedef struct _treeNode treeNode;
typedef treeNode *treeNodePtr;
/*========================================================================================================*/
/*========================================================================================================*/
//function prototypes
void insert(treeNodePtr *, treeNodePtr, int, int, int);
void inOrder(treeNodePtr);
int valid(treeNodePtr, treeNodePtr*, int, int, int);
int check_rules(treeNodePtr, treeNodePtr*, int, int, int);
void count_height(treeNodePtr, unsigned int *, unsigned int *);
unsigned int count_width(treeNodePtr);
unsigned int search(treeNodePtr, int);
/*========================================================================================================*/
/*========================================================================================================*/
int main() {
	FILE* fp = fopen("list.txt", "r");
	if (fp == NULL) {
		puts("can not open file!");
		return -1;
	}

	//declaration
	treeNodePtr root = NULL;
	treeNodePtr tmp = NULL;
	int cval, lval, rval, sval;
	unsigned int height = 0;
	unsigned int count = 0;

	//Reading data from file, print original and then insert to tree
	printf("Original data:\n\n");
	while (fscanf(fp, "%d %d %d", &cval, &lval, &rval) != EOF) {
		printf("%d %d %d\n", cval, lval, rval);
		//if inputs is satisfied rule 2,3 insert it.
		if (valid(root, &tmp, cval, lval, rval)) {
			insert(&root, tmp, cval, lval, rval);
		}
	}

	//test insert
	printf("\n\n1. Insert tree and print out the tree (in order):\n\n   ");
	inOrder(root);
	puts("");

	//test height
	count_height(root, &height, &count);
	printf("\n\n2. Tree height: %u\n", height);

	//test width
	printf("\n\n3. Tree width: %u\n", count_width(root));

	//test search
	puts("\n\n4. Enter a number to test the search function: ");
	while (scanf("%d", &sval) != EOF)
		printf("\n\nSearch for: %d\nResult: %u\n", sval, search(root, sval));

	return 0;
}
/*========================================================================================================*/
//This function inserts 2 valid numbers to the tree or creates a tree with 3 numbers.
/*========================================================================================================*/
void insert(treeNodePtr *root, treeNodePtr tmp, int cval, int lval, int rval) {
	//if tree is empty -> create tree
	if (*root == NULL) {
		*root = (treeNode*)malloc(sizeof(treeNode));							//Dynamic memory allocation
		(*root)->left = (treeNode*)malloc(sizeof(treeNode));					//for 3 pointers: root, left and right
		(*root)->right = (treeNode*)malloc(sizeof(treeNode));
		if (*root != NULL && (*root)->left != NULL && (*root)->right != NULL) {	//if allocation is success
			(*root)->data = cval;												//assigned value to root, left and right
			(*root)->left->data = lval;
			(*root)->right->data = rval;
			//the leafs of left node and right node must be initialized to 0, or else, their value is unpredictable
			(*root)->left->left = (*root)->left->right = (*root)->right->left = (*root)->right->right = NULL;
		}
		//if allocation is not success
		else {
			printf("%d not inserted, no memory available!\n", (*root)->data);
		}
	}//end if

	 //else if tree is not empty
	else {
		tmp->left = (treeNode*)malloc(sizeof(treeNode));						//Allocate the dynamic memory for 2 leafs of the tmp node.
		tmp->right = (treeNode*)malloc(sizeof(treeNode));						//tmp stores the address of the node that is gained after
																				//checking rule 2 and rule 3
		tmp->left->data = lval;													//assign corresponding values for the leafs of that node.
		tmp->right->data = rval;
		tmp->left->left = tmp->left->right = tmp->right->left = tmp->right->right = NULL;	//initializing the left and right of leaf to 0
	}
}
/*========================================================================================================*/
//This function uses check_rules function to determine wheter inputs are valid or not.
//This function will test all the members of tree by using check_rules function.
//If both rule 2 and 3 are not detected, valid return 1, else return 0
/*========================================================================================================*/
int valid(treeNodePtr root, treeNodePtr *tmp, int cval, int lval, int rval) {
	if (root == NULL)															//if root = null, no rules are detected
		return 1;
	//Using check_rules function for all members
	//PreOrder traversal is used
	else
		return check_rules(root, tmp, cval, lval, rval) ? (valid(root->left, tmp, cval, lval, rval) ? (valid(root->right, tmp, cval, lval, rval) ? 1 : 0) : 0) : 0;
}
/*========================================================================================================*/
//This function check rule 2 and 3, if input numbers are valid, tmp will store the address of the leaf that will become new node
//Function return 1 if it is OK and new node is determined, and 0 if rule 2 or rule 3 is detected.
/*========================================================================================================*/
int check_rules(treeNodePtr root, treeNodePtr *tmp, int cval, int lval, int rval) {
	//checking rule 2 and 3, if member of tree is valid, return 1, else return 0 as rule 2 or 3 is detected.
	int i = ((lval != root->data) && (rval != root->data)) ? ((cval != root->data) ? 1 : ((root->left == NULL) ? 1 : 0)) : 0;
	//store the address of the leaf that will become new node to tmp
	if ((cval == root->data) && (root->left == NULL))
		*tmp = root;
	return i;
}
/*========================================================================================================*/
//This funtion prints all members of tree by inOrder traversal
/*========================================================================================================*/
void inOrder(treeNodePtr root) {
	if (root != NULL) {
		inOrder(root->left);
		printf("%d  ", root->data);
		inOrder(root->right);
	}
}
/*========================================================================================================*/
//this function counts height of the tree, counter will count from the root, every time counter counts to the leaf, height will be compared with counter,
//if counter is greater than height, assign height = counter.
//when going up, counter counts +1, when going back down, counter counts -1
/*========================================================================================================*/
void count_height(treeNodePtr treePtr, unsigned int *height, unsigned int *count) {
	(*count)++;										//count 1 time
													//if it is not a leaf
	if (treePtr != NULL) {							//post order traversal
		count_height(treePtr->left, height, count);
		count_height(treePtr->right, height, count);
		(*count)--;									//count -1 when going down		
	}
	else {											//if it is a leaf
		(*count)--;									//go back and count -1
		if (*height <= *count)						//if count is greater than old height
			*height = *count;						//then height is the count
	}
}
/*========================================================================================================*/
//Simply, width is the sum of the heights of 2 trees, one is the left node tree which root is the left node of the original root 
//and one is the right node tree which root is the right node of the original root.
/*========================================================================================================*/
unsigned int count_width(treeNodePtr root) {
	unsigned int height1, height2, count;
	height1 = height2 = count = 0;

	count_height(root->left, &height1, &count);		//count the height of the left node tree
	count = 0;										//reset counter
	count_height(root->right, &height2, &count);	//counter the height of the right node tree
	return height1 + height2;
}
/*========================================================================================================*/
//This function searchs for the sval
/*========================================================================================================*/
unsigned int search(treeNodePtr root, int sval) {
	if (root != 0) {
		//if root->data = sval, return 1, else, search for the left node, and right node respectively
		return (root->data == sval) ? 1 : (search(root->left, sval) || search(root->right, sval));
	}
	//if root == 0, it means sval is not found.
	else
		return 0;
}









