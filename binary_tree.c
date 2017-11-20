/*
 * binary_tree.c
 *
 *  Created on: 20 Nov 2017
 *      Author: adtonks
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int power(int base, int expnt) {
	int result = 1;
	for(; 0<expnt; expnt--)
		result *= base;
	return(result);
}

typedef struct node {
	struct node *parent;
	struct node *l_child;
	struct node *r_child;
	int val;
} Node;

typedef struct m_tree {
	int size;
	int layers;
	Node **g_arr;
} tree;

tree *init_tree_struct(int elements) {
	// calculate smallest number of layers to store elements
	int layers;
	int arr_size;
	/* number of elements initialized */
	int elem_init = 0;
	int jump_size;

	/* size of layer */
	int i, j, ls;
	tree *t;

	for(layers = 1; power(2, layers) - 1 < elements; layers++);

	/* initialize graph array and initialize all values */
	arr_size = power(2, layers);
	Node **g_arr = malloc(sizeof(Node *) * arr_size);
	for(i=0; i<arr_size; i++)
		g_arr[i] = NULL;

	// initialize root node
	g_arr[1] = malloc(sizeof(Node));
	elem_init++;

	for(i=1; i<layers; i++) {
		ls = power(2, i);
		jump_size = ls/2;
		while(0 < jump_size) {
			for(j=ls; (j<ls+ls/2) && (elem_init < elements); j+=jump_size) {
				if(g_arr[j] == NULL) {
					/* initialize the boxes */
					g_arr[j] = malloc(sizeof(Node));
					elem_init++;
					if(elem_init < elements) {
						g_arr[j+ls/2] = malloc(sizeof(Node));
						elem_init ++;
					}
				}
			}
			jump_size /= 2;
		}

	}
	/* connect the graph */
	for(i=1; i<arr_size; i++) {
		/* skip if there is no element here */
		if(g_arr[i] == NULL)
			continue;
		/* connect parent if not root */
		if(i!=1)
			g_arr[i]->parent = g_arr[i/2];
		else
			g_arr[i]->parent = NULL;
		/* connect left child */
		/* check if node is in last layer */
		if(power(2, layers-1) <= i)
			g_arr[i]->l_child = NULL;
		else if(g_arr[2*i] != NULL)
			g_arr[i]->l_child = g_arr[2*i];
		else
			g_arr[i]->l_child = NULL;
		/* connect right child */
		/* check if node is in last layer */
		if(power(2, layers-1) <= i)
			g_arr[i]->r_child = NULL;
		else if(g_arr[2*i+1] != NULL)
			g_arr[i]->r_child = g_arr[2*i+1];
		else
			g_arr[i]->r_child = NULL;
		g_arr[i]->val = 0;
	}

	/* put information into tree struct */
	t = malloc(sizeof(tree));
	t->size = elements;
	t->layers = layers;
	t->g_arr = g_arr;

	return(t);
}

void insert_node(tree *t, int index, int val) {
	/* initialize node */
	t->g_arr[index] = malloc(sizeof(Node));
	t->g_arr[index]->parent = t->g_arr[index/2];
	t->g_arr[index]->l_child = NULL;
	t->g_arr[index]->r_child = NULL;
	t->g_arr[index]->val = val;
	/* set pointer from parent */
	if(index % 2 == 0)
		t->g_arr[index/2]->l_child = t->g_arr[index];
	else
		t->g_arr[index/2]->r_child = t->g_arr[index];
	t->size++;
}

void add_layer(tree *t) {
	int i;
	int spaces = power(2, t->layers) - 1;
	t->g_arr = realloc(t->g_arr, sizeof(Node *) * 2*(spaces+1));
	for(i=spaces+1; i<2*(spaces+1); i++)
		t->g_arr[i] = NULL;
	t->layers++;
}

int add_node(tree *t, int val) {
	Node *curr_node;
	int i = 1;
	int spaces = power(2, t->layers) - 1;
	/* if graph is empty */
	if(t->g_arr == NULL) {
		add_layer(t);
		insert_node(t, 1, val);
	} else {
		curr_node = t->g_arr[1];
		while(curr_node != NULL) {
			if(val == curr_node->val) {
				printf("key exists");
				return(-1);
			}
			else if(val < curr_node->val)
				i = 2*i;
			else if(curr_node->val < val)
				i = 2*i+1;
			/* check to see where we are */
			if(spaces < i) {
				add_layer(t);
				insert_node(t, i, val);
				curr_node = NULL;
			} else if(t->g_arr[i] == NULL) {
				insert_node(t, i, val);
				curr_node = NULL;
			} else {
				curr_node = t->g_arr[i];
			}
		}
	}
}

void delete_node(tree *t, int index) {
	Node *node_ptr = t->g_arr[index];
	Node *child, *min_node;
	int min_node_index;
	if((node_ptr->l_child == NULL) && (node_ptr->r_child == NULL)) {
		/* no children */
		if(index % 2 == 0) {
			t->g_arr[index/2]->l_child = NULL;
			free(t->g_arr[index]);
			t->g_arr[index] = NULL;
		} else {
			t->g_arr[index/2]->r_child = NULL;
			free(t->g_arr[index]);
			t->g_arr[index] = NULL;
		}
	} else if((node_ptr->l_child != NULL) != (node_ptr->r_child != NULL)) {
		/* one child */
		child = node_ptr->l_child != NULL?
				node_ptr->l_child: node_ptr->r_child;
		if(index % 2 == 0) {
			t->g_arr[index/2]->l_child = child;
			free(t->g_arr[index]);
			t->g_arr[index] = NULL;
		} else {
			t->g_arr[index/2]->r_child = child;
			free(t->g_arr[index]);
			t->g_arr[index] = NULL;
		}
		/* raise all elements by one level */
	} else {
		/* two children */
		min_node = node_ptr->r_child;
		min_node_index = 2*index+1;
		while(min_node->l_child != NULL) {
			min_node = min_node->l_child;
			min_node_index *= 2;
		}
		node_ptr->val = min_node->val;
		delete_node(t, min_node_index);
	}
	t->size--;
	/* delete last layer if possible */
	return;
}

int remove_node(tree *t, int val) {
	Node *curr_node;
	int i = 1;
	/* if graph is empty */
	if(t->g_arr == NULL) {
		return(-1);
	} else {
		curr_node = t->g_arr[1];
		while(curr_node != NULL) {
			if(val == curr_node->val) {
				delete_node(t, i);
				return(0);
			}
			else if(val < curr_node->val) {
				if(curr_node->l_child == NULL){
					return(-1);
				}
				else
					i = 2*i;
			} else if(curr_node->val < val) {
				if(curr_node->r_child == NULL) {
					return(-1);
				}
				else
					i = 2*i+1;
			}
				curr_node = t->g_arr[i];
		}
	}
}

void print_tree(tree *t) {
	int i, j, ls;
	printf("### BEGIN PRINTING TREE ###\n");
	for(i=0; i<t->layers; i++) {
		ls = power(2, i);
		for(j=ls; j<2*ls; j++) {
			if(t->g_arr[j] != NULL)
				printf("%d", t->g_arr[j]->val);
			else
				printf("X");
		}
		printf("\n");
	}
	printf("### END PRINTING TREE ###\n");
}

void delete_tree(tree *t) {
	int i;
	int spaces = power(2, t->layers) - 1;
	for(i=1; i<spaces+1; i++) {
		/* free data */
		if(t->g_arr[i] != NULL)
			free(t->g_arr[i]);
	}
	free(t->g_arr);
	free(t);
}

int main(int argc, char const *argv[]) {
	tree *t = init_tree_struct(5);
	print_tree(t);
	add_node(t, 3);
	print_tree(t);
	add_node(t, 2);
	print_tree(t);
	add_node(t, 4);
	print_tree(t);
	if(remove_node(t, 3) == 0)
		printf("remove success\n");
	else
		printf("remove fail\n");
	print_tree(t);
	delete_tree(t);
	return(0);
}
