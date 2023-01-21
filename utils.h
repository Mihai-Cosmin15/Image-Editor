// Copyright Mihai-Cosmin Nour 311CAb 2022-2023

#ifndef UTILS_H_
#define UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "my_defs.h"
#include "my_alloc.h"

void swap_ints(int *x, int *y);

int is_power_2(int x);

int is_number(char *s);

void remove_newline(char *s);

void remove_trailing_whitespace(char *s);

int max_array(int *v, int n);

int max_matrix(int **a, int n, int m);

void det_freq(int *freq, image_t image);

void clamp(int *value);

void div_apply(int *value, int div);

void generate_header(image_t *image);

void read_header(image_t *image, FILE **image_file);

void crop(image_t *image, selection_t sel);

color_t color_t_add(color_t x, color_t y);

color_t color_t_mult(color_t x, int y);

void apply(image_t *image, const int apply_matrix[SMALL_SIZE][SMALL_SIZE],
		   const int div, selection_t sel);

void rotate_image(image_t *image);

void rotate_selection(image_t *image, selection_t sel);

int valid_coord(image_t image, selection_t sel);

int is_sel_square(selection_t sel);

int is_whole_img(image_t image, selection_t sel);

void select_all(image_t image, selection_t *sel);

#endif
