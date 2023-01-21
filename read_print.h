// Copyright Mihai-Cosmin Nour 311CAb 2022-2023

#ifndef READ_PRINT_H_
#define READ_PRINT_H_

#include <stdio.h>
#include <stdlib.h>
#include "my_defs.h"
#include "my_alloc.h"

void read_gray_img_text(image_t *image, FILE * *image_file);

void read_color_img_text(image_t *image, FILE * *image_file);

void read_gray_img_binary(image_t *image, FILE * *image_file);

void read_color_img_binary(image_t *image, FILE * *image_file);

void print_gray_img_binary(image_t image, FILE * *image_file);

void print_color_img_binary(image_t image, FILE * *image_file);

void print_gray_img_text(image_t image, FILE * *save_file);

void print_color_img_text(image_t image, FILE * *save_file);

#endif
