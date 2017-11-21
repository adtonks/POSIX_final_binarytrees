/*
 * binary_tree.c
 *
 *  Created on: 20 Nov 2017
 *      Author: adtonks
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

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

void init_tree_struct(tree *t, int elements) {
	// calculate smallest number of layers to store elements
	int layers;
	int arr_size;
	/* number of elements initialized */
	int elem_init = 0;
	int jump_size;
	/* size of layer */
	int i, j, ls;
	for(layers = 1; power(2, layers) - 1 < elements; layers++);
	/* initialize graph array and initialize all values */
	arr_size = power(2, layers);
	t->g_arr = realloc(t->g_arr, sizeof(Node *) * arr_size);
	for(i=0; i<arr_size; i++)
		t->g_arr[i] = NULL;
	// initialize root node
	t->g_arr[1] = malloc(sizeof(Node));
	elem_init++;
	for(i=1; i<layers; i++) {
		ls = power(2, i);
		jump_size = ls/2;
		while(0 < jump_size) {
			for(j=ls; (j<ls+ls/2) && (elem_init < elements); j+=jump_size) {
				if(t->g_arr[j] == NULL) {
					/* initialize the boxes */
					t->g_arr[j] = malloc(sizeof(Node));
					elem_init++;
					if(elem_init < elements) {
						t->g_arr[j+ls/2] = malloc(sizeof(Node));
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
		if(t->g_arr[i] == NULL)
			continue;
		/* connect parent if not root */
		if(i!=1)
			t->g_arr[i]->parent = t->g_arr[i/2];
		else
			t->g_arr[i]->parent = NULL;
		/* connect left child */
		/* check if node is in last layer */
		if(power(2, layers-1) <= i)
			t->g_arr[i]->l_child = NULL;
		else if(t->g_arr[2*i] != NULL)
			t->g_arr[i]->l_child = t->g_arr[2*i];
		else
			t->g_arr[i]->l_child = NULL;
		/* connect right child */
		/* check if node is in last layer */
		if(power(2, layers-1) <= i)
			t->g_arr[i]->r_child = NULL;
		else if(t->g_arr[2*i+1] != NULL)
			t->g_arr[i]->r_child = t->g_arr[2*i+1];
		else
			t->g_arr[i]->r_child = NULL;
		t->g_arr[i]->val = 0;
	}
	/* put information into tree struct */
	t->layers = layers;
	return;
}

int cmp_fun (const void *a, const void *b) {
   return(*(int *) a - *(int *) b);
}

void paste_vals(tree *t, Node *curr_node, int val_arr[], int *pasted) {
	if(curr_node == NULL)
		return;
	/* process left subtree */
	paste_vals(t, curr_node->l_child, val_arr, pasted);
	/* process this node */
	curr_node->val = val_arr[*pasted];
	(*pasted)++;
	/* process right subtree */
	paste_vals(t, curr_node->r_child, val_arr, pasted);
	return;
}

void balance_tree(tree *t) {
	if(t->layers <= 2)
		return;
	int val_arr[t->size];
	int i;
	int j=0;
	int arr_size = power(2, t->layers);
	int pasted = 0;
	/* copy vals */
	for(i=1; i<arr_size; i++) {
		if(t->g_arr[i] != NULL) {
			val_arr[j] = t->g_arr[i]->val;
			free(t->g_arr[i]);
			t->g_arr[i] = NULL;
			j++;
		}
	}
	init_tree_struct(t, t->size);
	qsort(val_arr, t->size, sizeof(int), cmp_fun);
	paste_vals(t, t->g_arr[1], val_arr, &pasted);
	return;
}

void insert_node(tree *t, int index, int val) {
	/* initialize node */
	t->g_arr[index] = malloc(sizeof(Node));
	if(index != 1)
		t->g_arr[index]->parent = t->g_arr[index/2];
	else
		t->g_arr[index]->parent = NULL;
	t->g_arr[index]->l_child = NULL;
	t->g_arr[index]->r_child = NULL;
	t->g_arr[index]->val = val;
	/* set pointer from parent */
	if(index == 1)
		;
	else if(index % 2 == 0)
		t->g_arr[index/2]->l_child = t->g_arr[index];
	else
		t->g_arr[index/2]->r_child = t->g_arr[index];
	t->size++;
	return;
}

void add_layer(tree *t) {
	int i;
	int spaces = power(2, t->layers) - 1;
	t->g_arr = realloc(t->g_arr, sizeof(Node *) * 2*(spaces+1));
	for(i=spaces+1; i<2*(spaces+1); i++)
		t->g_arr[i] = NULL;
	t->layers++;
	return;
}

int add_node(tree *t, int val) {
	Node *curr_node;
	int i = 1;
	int spaces = power(2, t->layers) - 1;
	/* if graph is empty */
	if(t->layers == 0) {
		add_layer(t);
		insert_node(t, 1, val);
	} else {
		curr_node = t->g_arr[1];
		while(curr_node != NULL) {
			if(val == curr_node->val) {
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
	balance_tree(t);
	return(0);
}

void paste_nodes(tree *t, Node *curr_node, int index) {
	if(curr_node == NULL)
		return;
	/* process left subtree */
	paste_nodes(t, curr_node->l_child, 2*index);
	/* process this node */
	t->g_arr[index] = curr_node;
	/* process right subtree */
	paste_nodes(t, curr_node->r_child, 2*index+1);
	return;
}

void repack_array(tree *t) {
	Node *curr_node = t->g_arr[1];
	int i;
	int index = 1;
	int spaces = power(2, t->layers) - 1;
	/* clear pointers */
	for(i=1; i<=spaces; i++)
		t->g_arr[i] = NULL;
	paste_nodes(t, curr_node, index);
	return;
}

void delete_node(tree *t, int index) {
	Node *node_ptr = t->g_arr[index];
	Node *child, *min_node;
	int min_node_index, layer_empty, i;
	if((node_ptr->l_child == NULL) && (node_ptr->r_child == NULL)) {
		/* no children */
		if(index == 1) {
			free(t->g_arr[index]);
			t->g_arr[index] = NULL;
		} else if(index % 2 == 0) {
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
		if(index == 1) {
			free(t->g_arr[index]);
			t->g_arr[1] = child;
		} else if(index % 2 == 0) {
			t->g_arr[index/2]->l_child = child;
			free(t->g_arr[index]);
			t->g_arr[index] = NULL;
		} else {
			t->g_arr[index/2]->r_child = child;
			free(t->g_arr[index]);
			t->g_arr[index] = NULL;
		}
		/* raise all elements by one level */
		repack_array(t);
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
		return;
	}
	t->size--;
	/* delete last layer if possible */
	layer_empty = 1;
	for(i=power(2, t->layers-1); i<power(2, t->layers); i++) {
		if(t->g_arr[i] != NULL)
			layer_empty = 0;
	}
	if(layer_empty) {
		t->g_arr = realloc(t->g_arr, sizeof(Node *)*power(2, t->layers-1));
		t->layers--;
	}
	return;
}

int remove_node(tree *t, int val) {
	Node *curr_node;
	int i = 1;
	/* if graph is empty */
	if(t->layers == 0) {
		return(-1);
	} else {
		curr_node = t->g_arr[1];
		while(curr_node != NULL) {
			if(val == curr_node->val) {
				delete_node(t, i);
				balance_tree(t);
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

void print_nodes_at_level(tree *t, int level) {
	int ls, j;
	if((level<0) || t->layers <= level) {
		printf("Level does not exist\n");
		return;
	}
	ls = power(2, level);
	for(j=ls; j<2*ls; j++) {
		if(t->g_arr[j] != NULL)
			printf("(%d) ", t->g_arr[j]->val);
		else
			/* specification says to always print 2^level nodes */
			printf("(empty) ");
	}
	printf("\n");
	return;
}

void print_tree(tree *t) {
	int i, j, k, ls;
	printf("### BEGIN PRINTING TREE ###\n");
	for(i=0; i<t->layers; i++) {
		ls = power(2, i);
		/* print branches */
		if(i != 0) {
			for(j=0; j<power(2, t->layers-i-1); j++)
				printf("   ");
			for(j=ls; j<2*ls; j++) {
				printf(" | ");
				if(j != 2*ls-1) {
					for(k=0; k<power(2, t->layers-i)-1; k++) {
						if(j % 2 == 1)
							printf("   ");
						else
							printf("¯¯¯");
					}
				}
			}
			printf("\n");
		}
		for(j=0; j<power(2, t->layers-i-1); j++)
			printf("   ");
		for(j=ls; j<2*ls; j++) {
			if(t->g_arr[j] != NULL)
				printf("%03d", t->g_arr[j]->val);
			else
				printf("   ");
			if(j != 2*ls-1) {
				for(k=0; k<power(2, t->layers-i)-1; k++)
					printf("   ");
			}
		}
		printf("\n");
		if(i != t->layers-1) {
			for(j=0; j<power(2, t->layers-i-1); j++)
				printf("   ");
			for(j=ls; j<2*ls; j++) {
					printf(" | ");
					if(j != 2*ls-1) {
						for(k=0; k<power(2, t->layers-i)-1; k++)
							printf("   ");
					}
			}
			printf("\n");
		}
	}
	printf("### END PRINTING TREE ###\n");
	return;
}

void delete_tree(tree *t) {
	int i;
	int arr_size = power(2, t->layers);
	for(i=1; i<arr_size; i++) {
		/* free nodes */
		if(t->g_arr[i] != NULL)
			free(t->g_arr[i]);
	}
	free(t->g_arr);
	free(t);
	return;
}

int check_node(tree *t, int index, int oldval, int newval) {
	Node *node_ptr = t->g_arr[index];
	int test_flag = 1;
	if(node_ptr->parent != NULL) {
		if(index % 2 == 0) {
			/* this is a left child */
			if(newval >= node_ptr->parent->val)
				return(0);
		} else {
			/* this is a right child */
			if(newval <= node_ptr->parent->val)
				return(0);
		}
	}
	if(node_ptr->l_child != NULL) {
		if(newval <= node_ptr->l_child->val)
			return(0);
	}
	if(node_ptr->r_child != NULL) {
		if(newval >= node_ptr->l_child->val)
			return(0);
	}
	return(1);
}

int edit_node(tree *t, int oldval, int newval) {
	Node *curr_node;
	int i = 1;
	/* if graph is empty */
	if(t->layers == 0) {
		return(-1);
	} else {
		curr_node = t->g_arr[1];
		while(curr_node != NULL) {
			if(oldval == curr_node->val) {
				if(check_node(t, i, oldval, newval)) {
					curr_node->val = newval;
					return(0);
				} else {
					/* check if newval exists */
					if(add_node(t, newval) == -1)
						return(-3);
					remove_node(t, oldval);
					return(-2);
				}
			}
			else if(oldval < curr_node->val) {
				if(curr_node->l_child == NULL){
					return(-1);
				}
				else
					i = 2*i;
			} else if(curr_node->val < oldval) {
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

tree *create_new_tree() {
	tree *t = malloc(sizeof(tree));
	t->layers = 0;
	t->g_arr = malloc(sizeof(Node *) * power(2, t->layers));
	t->g_arr[0] = NULL;
	t->size = 0;
	return(t);
}

int find_depth(tree *t, int val) {
	Node *curr_node;
	int depth = 0;
	/* if graph is empty */
	if(t->layers == 0) {
		return(-1);
	} else {
		curr_node = t->g_arr[1];
		while(curr_node != NULL) {
			if(val == curr_node->val) {
				return(depth);
				return(0);
			}
			else if(val < curr_node->val) {
				if(curr_node->l_child == NULL) {
					return(-1);
				} else {
					depth++;
					curr_node = curr_node->l_child;
				}
			} else if(curr_node->val < val) {
				if(curr_node->r_child == NULL) {
					return(-1);
				} else {
					depth++;
					curr_node = curr_node->r_child;
				}
			}
		}
	}
}

int shortest_distance(tree *t, int node1, int node2) {
	Node *curr_node;
	int dist = 0;
	Node *node1_ptr
	/* if graph is empty */
	if(t->layers == 0) {
		return(-1);
	} else {
		node1_ptr = t->g_arr[1];
		while(node1_ptr != NULL) {
			if(val == node1_ptr->val) {
				break;
			}
			else if(val < node1_ptr->val) {
				if(node1_ptr->l_child == NULL)
					return(-1);
				else
					node1_ptr = node1_ptr->l_child;
			} else if(node1_ptr->val < val) {
				if(node1_ptr->r_child == NULL)
					return(-1);
				else
					node1_ptr = node1_ptr->r_child;
			}
		}
	}

	/* DFS on node1_ptr */

}

int main(int argc, char const *argv[]) {
	tree *t = create_new_tree();
	print_tree(t);
	add_node(t, 3);
	print_tree(t);
	add_node(t, 1);
	print_tree(t);
	add_node(t, 4);
	print_tree(t);
	add_node(t, 2);
	print_tree(t);
	edit_node(t, 1, 5);
	print_tree(t);
	if(remove_node(t, 3) == 0)
		printf("3 remove success\n");
	else
		printf("3 remove fail\n");
	print_tree(t);
	if(remove_node(t, 4) == 0)
		printf("4 remove success\n");
	else
		printf("4 remove fail\n");
	print_tree(t);
	if(remove_node(t, 1) == 0)
		printf("1 remove success\n");
	else
		printf("1 remove fail\n");
	print_tree(t);
	if(remove_node(t, 2) == 0)
		printf("2 remove success\n");
	else
		printf("2 remove fail\n");
	print_tree(t);
	if(remove_node(t, 1) == 0)
		printf("1 remove success\n");
	else
		printf("1 remove fail\n");
	add_node(t, 432);
	add_node(t, 213);
	add_node(t, 23);
	add_node(t, 343);
	add_node(t, -23);
	add_node(t, -1);
	add_node(t, 500);
	print_tree(t);
	add_node(t, 1);
	print_tree(t);
	balance_tree(t);
	print_tree(t);
	print_tree(t);
	add_node(t, 8);
	print_tree(t);
	print_tree(t);
	add_node(t, 10);
	print_tree(t);
	add_node(t, 543);
	add_node(t, 23);
	add_node(t, 3);
	add_node(t, -4);
	add_node(t, -43);
	add_node(t, -10);
	add_node(t, -32);
	add_node(t, 54);
	add_node(t, 834);
	add_node(t, -34);
	add_node(t, -5);
	add_node(t, -44);
	add_node(t, 420);
	add_node(t, 432);
	add_node(t, 476);
	add_node(t, -65);
	add_node(t, 737);
	add_node(t, -99);
	balance_tree(t);
	print_tree(t);
	print_nodes_at_level(t, 0);
	print_nodes_at_level(t, 1);
	print_nodes_at_level(t, 2);
	print_nodes_at_level(t, 3);
	printf("depth of -34 is %d\n", find_depth(t, -34));
	printf("depth of 4124 is %d\n", find_depth(t, 4124));
	printf("depth of 5 is %d\n", find_depth(t, 5));
	delete_tree(t);
	tree *t2 = create_new_tree();
	printf("depth of 5 is %d\n", find_depth(t2, 5));
	delete_tree(t2);

	return(0);
}
