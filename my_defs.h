// Copyright Mihai-Cosmin Nour 311CAb 2022-2023

#ifndef MY_DEFS_H_
#define MY_DEFS_H_

#define LOAD_FAIL "Failed to load "
#define LOAD_SUCCES "Loaded "
#define NO_IMG_LOADED "No image loaded\n"
#define INVALID_COMMAND "Invalid command\n"
#define GRAY_IMG "Black and white image needed\n"
#define EQUALIZE_SUCCES "Equalize done\n"
#define INVALID_COORD "Invalid set of coordinates\n"
#define SELECTED "Selected "
#define IMG_CROPPED "Image cropped\n"
#define APPLIED_SHARPEN "APPLY SHARPEN done\n"
#define APPLIED_EDGE "APPLY EDGE done\n"
#define APPLIED_BLUR "APPLY BLUR done\n"
#define APPLIED_GAUSSIAN_BLUR "APPLY GAUSSIAN_BLUR done\n"
#define APPLY_PARAM_INVALID "APPLY parameter invalid\n"
#define APPLY_GRAY "Easy, Charlie Chaplin\n"
#define ANGLE_ERR "Unsupported rotation angle\n"
#define NO_SQUARE "The selection must be square\n"
#define ROTATED "Rotated "
#define SAVED "Saved "
#define BUFFER_SIZE 50
#define N_COMMANDS 9
#define N_APPLY 4
#define MAX_VALUE 256
#define SMALL_SIZE 3
#define ROTATION_NUMBER 4
#define SQUARE_ANGLE 90
#define CIRCLE_ANGLE 360

typedef struct {
	int red, green, blue;
} color_t;

typedef struct {
	char type[SMALL_SIZE + 1], header[BUFFER_SIZE];
	int col_num, row_num, max_value;
	int **gray_img;
	color_t **color_img;
} image_t;

typedef struct {
	int x1, y1;
	int x2, y2;
} selection_t;

#endif
