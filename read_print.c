// Copyright Mihai-Cosmin Nour 311CAb 2022-2023

#include "read_print.h"

// Read gray image from text file
void read_gray_img_text(image_t *image, FILE **image_file)
{
	image->gray_img = alloc_matrix(image->row_num, image->col_num);
	for (int i = 0; i < image->row_num; ++i) {
		for (int j = 0; j < image->col_num; ++j)
			fscanf(*image_file, "%d", &image->gray_img[i][j]);
	}
}

// Read color image from text file
void read_color_img_text(image_t *image, FILE **image_file)
{
	image->color_img = alloc_color_matrix(image->row_num, image->col_num);
	for (int i = 0; i < image->row_num; ++i) {
		for (int j = 0; j < image->col_num; ++j)
			fscanf(*image_file, "%d%d%d", &image->color_img[i][j].red,
				   &image->color_img[i][j].green,
				   &image->color_img[i][j].blue);
	}
}

// Read gray image from binary file
void read_gray_img_binary(image_t *image, FILE **image_file)
{
	unsigned char byte;
	image->gray_img = alloc_matrix(image->row_num, image->col_num);
	for (int i = 0; i < image->row_num; ++i) {
		for (int j = 0; j < image->col_num; ++j) {
			fread(&byte, 1, 1, *image_file);
			image->gray_img[i][j] = (int)byte;
		}
	}
}

// Read color image from binary file
void read_color_img_binary(image_t *image, FILE **image_file)
{
	unsigned char byte;
	image->color_img = alloc_color_matrix(image->row_num, image->col_num);
	for (int i = 0; i < image->row_num; ++i) {
		for (int j = 0; j < image->col_num; ++j) {
			fread(&byte, 1, 1, *image_file);
			image->color_img[i][j].red = (int)byte;
			fread(&byte, 1, 1, *image_file);
			image->color_img[i][j].green = (int)byte;
			fread(&byte, 1, 1, *image_file);
			image->color_img[i][j].blue = (int)byte;
		}
	}
}

// Print gray image into binary file
void print_gray_img_binary(image_t image, FILE **save_file)
{
	for (int i = 0; i < image.row_num; ++i) {
		for (int j = 0; j < image.col_num; ++j)
			fwrite(&image.gray_img[i][j], 1, 1, *save_file);
	}
}

// Print color image into binary file
void print_color_img_binary(image_t image, FILE **save_file)
{
	for (int i = 0; i < image.row_num; ++i) {
		for (int j = 0; j < image.col_num; ++j) {
			fwrite(&image.color_img[i][j].red, 1, 1, *save_file);
			fwrite(&image.color_img[i][j].green, 1, 1, *save_file);
			fwrite(&image.color_img[i][j].blue, 1, 1, *save_file);
		}
	}
}

// Print gray image into text file
void print_gray_img_text(image_t image, FILE **save_file)
{
	for (int i = 0; i < image.row_num; ++i) {
		for (int j = 0; j < image.col_num; ++j) {
			fprintf(*save_file, "%d", image.gray_img[i][j]);
			if (j != image.col_num - 1)
				fprintf(*save_file, " ");
		}
		fprintf(*save_file, "\n");
	}
}

// Print color image into text file
void print_color_img_text(image_t image, FILE **save_file)
{
	for (int i = 0; i < image.row_num; ++i) {
		for (int j = 0; j < image.col_num; ++j) {
			fprintf(*save_file, "%d %d %d", image.color_img[i][j].red,
					image.color_img[i][j].green, image.color_img[i][j].blue);
			if (j != image.col_num - 1)
				fprintf(*save_file, " ");
		}
		fprintf(*save_file, "\n");
	}
}
