/*Sangmyung university
 *Computer Science
 *Data structure
 *Kyungha Min Prof.
 *Student: Luong Duc Anh ·Éµæ¾ÆÀ× - 201685044
 *Practice 8: Tree
/*********************************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
/*********************************************************************************************/
struct s_info {
	char name[32];
	char sex;
	char city[32];
	char dept[32];
	float gpa;
	int height;
	int weight;
	struct s_info* left;
	struct s_info* right;
};
typedef struct s_info info;
typedef info* infoPtr;
/*********************************************************************************************/
//prototypes
void load(infoPtr*, info);						//load all data from file to the tree
void print(infoPtr);							//print out the tree
void insert(infoPtr, info);						//insert data to tree
void del(infoPtr*, info);						//delete 1 node of tree
void fix_tree(infoPtr, infoPtr, char*);			//this function is used in the del() function to help re-arrange the tree after deleting
info* find_left(infoPtr);						//find the most left node/leaf
info* search(infoPtr, info);					//search for data
/*********************************************************************************************/
int main(void) {
	FILE* fp = fopen("list.txt", "r");
	if (fp == NULL) {
		printf("Load fail! Can not open file!\n");
		return -1;
	}

	infoPtr root = NULL;
	info std;
	char command[32];
	char* tok[9];
	int i;
	//Reading the whole string from keyboard, examine the first word and do the following statements base on the first word.
	puts("Enter command:");
	while (scanf("%[^\n]%*c", command)) {
		tok[0] = strtok(command, " ");			//strtok to spilit the first word out of the string.
		if (strcmp(tok[0], "LOAD") == 0) {		//if the first word is LOAD then read file and load them
			while (fscanf(fp, "%s %c %s %s %f %d %d\n", std.name, &std.sex, std.city, std.dept, &std.gpa, &std.height, &std.weight) != EOF)
				load(&root, std);
			puts("done!\n===========================================================");
		}
		else if (strcmp(tok[0], "PRINT") == 0) {	//if the first word is PRINT then print out the tree
			print(root);
			puts("done!\n===========================================================");
		}
		else if (strcmp(tok[0], "INSERT") == 0) {	//if the first word is INSERT then read all the next words and insert them to the tree
			i = 0;
			while (tok[i] != NULL)
				tok[++i] = strtok(NULL, " ");
			std.gpa = atof(tok[5]);
			std.height = atoi(tok[6]);
			std.weight = atoi(tok[7]);
			strcpy(std.name, tok[1]);
			std.sex = tok[2][0];
			strcpy(std.city, tok[3]);
			strcpy(std.dept, tok[4]);
			insert(root, std);
			puts("done!\n===========================================================");
		}
		else if (strcmp(tok[0], "SEARCH") == 0) {	//if the first word is SEARCH, read 1 next word and search for it
			strcpy(std.name, strtok(NULL, " "));
			if (search(root, std) == NULL)
				puts("Student is not found!");
			puts("done!\n===========================================================");
		}
		else if (strcmp(tok[0], "DELETE") == 0) {	//if the first word is DELETE, read 1 next word, search for it and delete
			tok[1] = strtok(NULL, " ");
			strcpy(std.name, tok[1]);
			del(&root, std);
			puts("done!\n===========================================================");
		}
		else if (strcmp(tok[0], "EXIT") == 0)	//if it is EXIT, just break the loop
			break;
		else {
			puts("invalid command!");			//other word is all invalid command
			puts("===========================================================");
		}
	}
	return -1;
}
/*********************************************************************************************/
void load(infoPtr * root, info std) {
	//if tree is empty, allocate dynamic memory for the root pointer and then store data to it.
	if (NULL == *root) {
		*root = (info*)malloc(sizeof(info));
		if (NULL != *root) {
			strcpy((*root)->name, std.name);
			(*root)->sex = std.sex;
			strcpy((*root)->city, std.city);
			strcpy((*root)->dept, std.dept);
			(*root)->gpa = std.gpa;
			(*root)->height = std.height;
			(*root)->weight = std.weight;
			(*root)->left = NULL;
			(*root)->right = NULL;
		}//end if
		else
			puts("Can not load, No memory available!");
	}//end if
	//else if the tree is not empty, search for suitable leaf and add data to it
	else {
		if (strcmp(std.name, (*root)->name) < 0)
			load(&((*root)->left), std);
		else
			load(&((*root)->right), std);
	}//end else
}
/*********************************************************************************************/
//print out the tree by In order traversal, which will print out all members in ascending order
void print(infoPtr root) {
	if (NULL != root) {
		print(root->left);
		printf("%s\t%c\t%s\t%s\t%.1f\t%d\t%d\n",
			root->name, root->sex, root->city, root->dept, root->gpa, root->height, root->weight);
		print(root->right);
	}
}
/*********************************************************************************************/
//insert 1 node is simply adding 1 new data
void insert(infoPtr root, info std) {
	load(&root, std);
}
/*********************************************************************************************/
//1. if tree is empty, return 0 as student is not found.
//2. else if student is stored in root then return the root.
//3. else, compare the name of searching student with the current left/right node of student to find out which way to go, left node or right node.
//Note that this function doesn't return the address of the student's node, it returns address of parent's node of the student's node
//example: if searching for student C, and parent node of C is A then this function return A.
info* search(infoPtr root, info std) {
	if (root == NULL) //(1)
		return 0;
	else if (strcmp(root->name, std.name) == 0) { //(2)
		printf("%s\t%c\t%s\t%s\t%f\t%d\t%d\n",
			root->left->name, root->left->sex, root->left->city, root->left->dept, root->left->gpa, root->left->height, root->left->weight);
		return root;
	}
	else { //(3)
		if (root->left != NULL && 0 == strcmp(std.name, root->left->name)) {			//if name in current node->left == searching name -> print it
			printf("%s\t%c\t%s\t%s\t%f\t%d\t%d\n",								//and return current node
				root->left->name, root->left->sex, root->left->city, root->left->dept, root->left->gpa, root->left->height, root->left->weight);
			return root;
		}
		else if (root->right != NULL && strcmp(std.name, root->right->name) == 0) {	//if name in current node->right == searching name -> print it
			printf("%s\t%c\t%s\t%s\t%f\t%d\t%d\n",								//and return current node
				root->right->name, root->right->sex, root->right->city, root->right->dept, root->right->gpa, root->right->height,
				root->right->weight);
			return root;
		}
		else if (root->left == NULL && root->right == NULL)							//if it is a leaf, stop searching and return 0 as student is not found.
			return 0;
		else																		//else, recurse all above steps with the left or right node of the current node
			(0 > strcmp(std.name, root->name)) ? (search(root->left, std)) : (search(root->right, std));
	}
}
/*********************************************************************************************/
/*Removing a node has several steps bases on the position of the node, but the key of removing is changing the pointer of
 its parent to the NULL, that's why search() function doesn't return address of being searched node but the address of its parent.
 I just can see the perspective like follows:
 1. deleting the leaf: this is the easiest scenario because the leaf has no left node and right node. So just change the
 pointer of its parent to the NULL, then free it from memory.
 2. deleting 1 node: This scenario is most complexed. If the being deleted node has both left and right node, it needs to find new one to exchange removing
 node's position. as you mentioned during the class, I choose the second solution, which finds the most left node/leaf on the right side of being deleted node.
 After finding it, swap it by 4 step: (1)parent node of being deleted node points to most left, (2)the left of most left points to the left node of
 being deleted node, (3)the right of most left points to the right node of being deleted node and (4)changing the pointer of the parent node of the most
 left to NULL or to something else.
 After doing 4 above steps, the node is swapped. There is just 1 problem left, that is the most left node is the node which doesn't have left node, it means,
 the most left node can also have right node, and in this scenario, its right node need to be added to the suitable position, that is the use of tree_fix()
 function. tree_fix() function, will, simply add the right node of the most left node to the left of the parent of the most left node. i.e: If left of A is B
 and right of B is C, B is the most left then after swapping B to the being deleted node, left of A will be C.
 3. deleting the root, do the same as deleting one node, except the root has no parent so don't need to be change its parent pointer.
*/
void del(infoPtr * root, info std) {
	infoPtr tmp, tmp1, tmp2;
	//if being deleted student is the root
	if (strcmp(std.name, (*root)->name) == 0) {
		tmp = find_left((*root)->right);	//finding the most left node
		if (tmp != NULL) {					//if the most left node is found
			tmp1 = tmp->right;				//new root is the most left node.
			tmp->left = (*root)->left;
			tmp->right = (*root)->right;
			*root = tmp;
			fix_tree(tmp1, tmp->right, tmp->name);	//adding the right node of the most left to its parent.
			free(*root);					//free the memory
		}
		else								//if the root doesn't have right node, new root is its left node.
			*root = tmp->left;
	}//end if
	//if being deleted student is node or leaf
	else {
		tmp = search(*root, std);										//search for its position
		if (tmp != NULL) {
			//if student is found then the parent node of student is returned since search() returns the parent node of being searched student
			//so we can not know exactly where is the being searched student, left or right. In this case, just test both scenario.										
			if (tmp->left->name == std.name) {							//if the being searched student is the left (tmp->left)
				tmp2 = find_left(tmp->left->right);						//find the most left node on the right side and store it in tmp2
				if (tmp2 != NULL) {										//if most left (tmp2) is found:
					if (tmp2 == tmp->left->right) {						//if tmp2 is the first right node of tmp->left
						tmp2->left = tmp->left->left;					//swap tmp2 with the tmp->left
						tmp->left = tmp2;
					}
					else {												//else,
						tmp1 = tmp2->right;								//store the right node of the most left node to tmp1
						tmp2->left = tmp->left->left;					//delete the tmp->left by relinking
						tmp2->right = tmp->left->right;
						free(tmp->left);								//free the memory
						tmp->left = tmp2;
						fix_tree(tmp1, tmp2->right, tmp2->name);		//call fix_tree to link tmp1 to the left of parent of tmp2
					}//end else
				}//end if
				else	//if most left is not found, the being deleted node doesn't have right node.
					tmp->left = tmp->left->left;
			}
			//Case the being deleted student is the right (tmp->right), doing the same steps as above.
			else {
				tmp2 = find_left(tmp->right->right);
				if (tmp2 != NULL) {
					if (tmp2 == tmp->right->right) {
						tmp2->left = tmp->right->left;
						tmp->right = tmp2;
					}
					else {
						tmp1 = tmp2->right;
						tmp2->left = tmp->right->left;
						tmp2->right = tmp->right->right;
						free(tmp->right);
						tmp->right = tmp2;
						fix_tree(tmp1, tmp->right->right, tmp2->name);
					}
				}
				else
					tmp->right = tmp->right->left;
			}
		}
		//if search() return 0, it means student is not found.
		else
			puts("Student is not found!");
	}
}//end function
/*********************************************************************************************/
info* find_left(infoPtr treePtr) {
	if (treePtr == NULL)					//return NULL if input pointer is NULL
		return NULL;
	else if (treePtr->left == NULL) {		//if left node of one node is NULL, return its parent node
		return treePtr;
	}
	else
		return find_left(treePtr->left);//recurse above steps until function returns NULL or 1 pointer
}
/*********************************************************************************************/
void fix_tree(infoPtr tmp1, infoPtr root, char* data) {
	if (strcmp(root->left->name, data) == 0)			//if the left node of current node is the most left node
		root->left = tmp1;							//point its left node to the right node of the most left node
	else
		fix_tree(tmp1, root->left, data);			//recurse above step until the position is found.
}
/*********************************************************************************************/








