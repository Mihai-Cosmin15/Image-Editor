// Copyright Mihai-Cosmin Nour 311CAb 2022-2023

#ifndef MY_ALLOC_H_
#define MY_ALLOC_H_

#include <stdio.h>
#include <stdlib.h>
#include "my_defs.h"

int **alloc_matrix(int n, int m);

color_t **alloc_color_matrix(int n, int m);

int *alloc_array(int n);

void free_matrix(int n, int **a);

void free_color_matrix(int n, color_t **a);

#endif
