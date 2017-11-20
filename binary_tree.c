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

void init_tree(int elements) {
	// calculate smallest number of layers to store elements
	int layers = 3;
	int arr_size = power(2, layers);
	/* number of elements initialized */
	int elem_init = 1;
	int jump_size;

	/* size of layer */
	int i, j, ls;

	/* initialize graph array and initialize all values */
	int g_arr[arr_size];
	for(i=0; i<arr_size; i++)
		g_arr[i] = 0;

	// initialize root node
	g_arr[1] = -1;

	for(i=1; i<layers; i++) {
		ls = power(2, i);
		jump_size = ls/2;
		while(0 < jump_size) {
			printf("layer %d jump size %d\n", i, jump_size);
			for(j=ls; j<2*ls; j+=jump_size) {
				if(g_arr[j] == 0) {
					/* initialize the boxes */
					g_arr[j] = elem_init;
					g_arr[j+ls/2] = elem_init;
					elem_init += 1;
				}
			}
			jump_size /= 2;
		}

	}

	printf("array size %d\n", arr_size);
	for(i=0; i<arr_size; i++) {
		printf("%d\n", g_arr[i]);
	}

	return;

}

int main(int argc, char const *argv[]) {
	init_tree(4);
	return(0);
}
