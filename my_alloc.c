// Copyright Mihai-Cosmin Nour 311CAb 2022-2023

#include "my_alloc.h"

int **alloc_matrix(int n, int m)
{
	int **a = (int **)malloc(n * sizeof(int *));
	if (!a) {
		fprintf(stderr, "Malloc failed.\n");
		exit(-1);
	}

	for (int i = 0; i < n; ++i) {
		*(a + i) = (int *)malloc(m * sizeof(int));
		if (*(a + i) == NULL) {
			fprintf(stderr, "Malloc failed.\n");
			free_matrix(i, a);
			exit(-1);
		}
	}
	return a;
}

color_t **alloc_color_matrix(int n, int m)
{
	color_t **a = (color_t **)malloc(n * sizeof(color_t *));
	if (!a) {
		fprintf(stderr, "Malloc failed.\n");
		exit(-1);
	}

	for (int i = 0; i < n; ++i) {
		*(a + i) = (color_t *)malloc(m * sizeof(color_t));
		if (*(a + i) == NULL) {
			fprintf(stderr, "Malloc failed.\n");
			free_color_matrix(i, a);
			exit(-1);
		}
	}
	return a;
}

int *alloc_array(int n)
{
	int *v = (int *)malloc(n * sizeof(int));
	if (!v) {
		fprintf(stderr, "Malloc failed.\n");
		exit(-1);
	}
	return v;
}

void free_matrix(int n, int **a)
{
	for (int i = 0; i < n; ++i)
		free(*(a + i));
	free(a);
}

void free_color_matrix(int n, color_t **a)
{
	for (int i = 0; i < n; ++i)
		free(*(a + i));
	free(a);
}
