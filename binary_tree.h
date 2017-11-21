/*
 * midterm_adamtonks.h
 *
 *  Created on: 20 Nov 2017
 *      Author: adtonks
 */

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

int power(int base, int expnt);
void init_tree_struct(tree *t, int elements);
int cmp_fun (const void *a, const void *b);
void paste_vals(tree *t, Node *curr_node, int val_arr[], int *pasted);
void balance_tree(tree *t);
void insert_node(tree *t, int index, int val);
void add_layer(tree *t);
int add_node(tree *t, int val);
void paste_nodes(tree *t, Node *curr_node, int index);
void repack_array(tree *t);
void delete_node(tree *t, int index);
int remove_node(tree *t, int val);
void print_nodes_at_level(tree *t, int level);
void print_tree(tree *t);
void delete_tree(tree *t);
int check_node(tree *t, int index, int oldval, int newval);
int edit_node(tree *t, int oldval, int newval);
tree *create_new_tree();
int find_depth(tree *t, int val);
int DFS_mod(tree *t, Node *node_ptr, int node2, int dist, int *found, Node *prev_ptr);
int shortest_distance(tree *t, int node1, int node2);
tree *generate_from_array(int *array, int size);
tree *generate_random_tree(int size);
