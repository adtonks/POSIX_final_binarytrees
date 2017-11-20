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

typedef struct data_wrap {
	int key;
	int val;
} Data_wrap;

typedef struct node {
	struct node *parent;
	struct node *l_child;
	struct node *r_child;
	Data_wrap * data;
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
	for(i=1; i<=elements; i++) {
		/* skip if there is no element here */
		if(g_arr[i] == NULL)
			break;
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
	}

	/* put information into tree struct */
	t = malloc(sizeof(tree));
	t->size = elements;
	t->layers = layers;
	t->g_arr = g_arr;

	return(t);

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
	tree *t = init_tree_struct(10000);
	delete_tree(t);
	return(0);
}
