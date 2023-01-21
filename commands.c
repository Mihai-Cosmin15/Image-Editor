// Copyright Mihai-Cosmin Nour 311CAb 2022-2023

#include "commands.h"

// Search for a given command inside a list of commands
int which_command(char *command, const char **command_list, int n_commands)
{
	for (int i = 0; i < n_commands; ++i)
		if (strcmp(command, command_list[i]) == 0)
			return i;
	return -1;
}

// Open a file and load the image
void load_cmd(image_t *image, selection_t *sel)
{
	FILE *image_file = NULL;
	char file_name[BUFFER_SIZE] = "";
	scanf("%s", file_name);

	// clear from memory the existing image
	if (image->gray_img) {
		free_matrix(image->row_num, image->gray_img);
		image->gray_img = NULL;
	}
	if (image->color_img) {
		free_color_matrix(image->row_num, image->color_img);
		image->color_img = NULL;
	}
	strcpy(image->header, "");

	// Open the file as "read text"
	image_file = fopen(file_name, "rt");
	if (!image_file) {
		printf("%s%s\n", LOAD_FAIL, file_name);
		return;
	}

	// Obtain the type of image and, if it's binary, reopen the file as binary
	fgets(image->type, BUFFER_SIZE, image_file);
	if (image->type[1] == '5' || image->type[1] == '6') {
		fclose(image_file);
		image_file = fopen(file_name, "rb");
		if (!image_file) {
			printf("%s%s\n", LOAD_FAIL, file_name);
			return;
		}
		fgets(image->type, BUFFER_SIZE, image_file);
	}
	read_header(image, &image_file);

	// Depending on the type of file and image, read the image
	switch (image->type[1]) {
	case '2':
		read_gray_img_text(image, &image_file);
		break;
	case '3':
		read_color_img_text(image, &image_file);
		break;
	case '5':
		read_gray_img_binary(image, &image_file);
		break;
	case '6':
		read_color_img_binary(image, &image_file);
		break;
	default:
		break;
	}

	// Select all and close the file
	select_all(*image, sel);
	fclose(image_file);

	printf("%s%s\n", LOAD_SUCCES, file_name);
}

// Select
void select_cmd(image_t image, selection_t *sel)
{
	char content[BUFFER_SIZE] = "", command[BUFFER_SIZE] = "", *word;

	// Read the entire command
	fgets(content, BUFFER_SIZE, stdin);
	strcpy(command, content + 1);
	remove_newline(command);
	remove_trailing_whitespace(command);

	if (!image.gray_img && !image.color_img) {
		printf(NO_IMG_LOADED);
		return;
	}
	if (strlen(command) == 0) {
		printf(INVALID_COMMAND);
		return;
	}

	if (strcmp(command, "ALL") == 0) {
		select_all(image, sel);
		printf("%s%s\n", SELECTED, command);
	} else {
		selection_t temp_sel;

		// Get each coordinate from the command string
		word = strtok(command, " ");
		if (!word || !is_number(word)) {
			printf(INVALID_COMMAND);
			return;
		}
		temp_sel.y1 = atoi(word);
		word = strtok(NULL, " ");
		if (!word || !is_number(word)) {
			printf(INVALID_COMMAND);
			return;
		}
		temp_sel.x1 = atoi(word);
		word = strtok(NULL, " ");
		if (!word || !is_number(word)) {
			printf(INVALID_COMMAND);
			return;
		}
		temp_sel.y2 = atoi(word);
		word = strtok(NULL, " ");
		if (!word || !is_number(word)) {
			printf(INVALID_COMMAND);
			return;
		}
		temp_sel.x2 = atoi(word);
		word = strtok(NULL, " ");
		if (word) {
			printf(INVALID_COMMAND);
			return;
		}

		// Put the coordinates in the right place
		if (temp_sel.x1 > temp_sel.x2)
			swap_ints(&temp_sel.x1, &temp_sel.x2);
		if (temp_sel.y1 > temp_sel.y2)
			swap_ints(&temp_sel.y1, &temp_sel.y2);
		if (valid_coord(image, temp_sel) == 0) {
			printf(INVALID_COORD);
			return;
		}
		sel->x1 = temp_sel.x1;
		sel->y1 = temp_sel.y1;
		sel->x2 = temp_sel.x2;
		sel->y2 = temp_sel.y2;
		printf("%s%d %d %d %d\n", SELECTED, sel->y1, sel->x1, sel->y2, sel->x2);
	}
}

// Determine the histogram of a gray image
void histogram_cmd(image_t image)
{
	char *word, content[BUFFER_SIZE] = "", command[BUFFER_SIZE] = "";
	int *histogram = NULL, freq[MAX_VALUE] = {0};
	int nr_stars = 0, nr_bins = 0, pixels_interval = 0, max_freq = 0;

	// Read the entire command
	fgets(content, BUFFER_SIZE, stdin);
	strcpy(command, content + 1);
	remove_newline(command);
	remove_trailing_whitespace(command);

	if (!image.gray_img && !image.color_img) {
		printf(NO_IMG_LOADED);
		return;
	}

	// Get the number of stars and the number of bins from the command string
	word = strtok(command, " ");
	if (!word || !is_number(word)) {
		printf(INVALID_COMMAND);
		return;
	}
	nr_stars = atoi(word);
	word = strtok(NULL, " ");
	if (!word || !is_number(word)) {
		printf(INVALID_COMMAND);
		return;
	}
	nr_bins = atoi(word);
	word = strtok(NULL, " ");
	if (word) {
		printf(INVALID_COMMAND);
		return;
	}
	if (image.type[1] == '3' || image.type[1] == '6') {
		printf(GRAY_IMG);
		return;
	}
	if (nr_stars < 0 || nr_bins < 0 ||
		nr_bins > MAX_VALUE || !is_power_2(nr_bins)) {
		printf(INVALID_COORD);
		return;
	}

	// Determine the frequency of each pixel and add it in the right interval
	histogram = alloc_array(nr_bins);
	det_freq(freq, image);
	pixels_interval = MAX_VALUE / nr_bins;
	for (int i = 0; i < nr_bins; ++i) {
		histogram[i] = 0;
		for (int j = 0; j < pixels_interval; ++j)
			histogram[i] += freq[i * pixels_interval + j];
	}

	// Determine the number of stars of each line by using the formula
	max_freq = max_array(histogram, nr_bins);
	for (int i = 0; i < nr_bins; ++i)
		histogram[i] = histogram[i] * nr_stars / max_freq;

	// Print the histogram
	for (int i = 0; i < nr_bins; ++i) {
		printf("%d\t|\t", histogram[i]);
		for (int j = 0; j < histogram[i]; ++j)
			printf("*");
		printf("\n");
	}

	// Remove the histogram array from memory
	free(histogram);
}

// Equalize a gray image
void equalize_cmd(image_t *image)
{
	int freq[MAX_VALUE] = {0};
	if (!image->gray_img && !image->color_img) {
		printf(NO_IMG_LOADED);
		return;
	}
	if (image->type[1] == '3' || image->type[1] == '6') {
		printf(GRAY_IMG);
		return;
	}

	// Determine the frequency of each pixel and apply the formula
	det_freq(freq, *image);
	for (int i = 0; i < image->row_num; ++i) {
		for (int j = 0; j < image->col_num; ++j) {
			int hist_sum = 0;

			// Sum of the frequencies from all pixels from 0 to current
			for (int k = 0; k <= image->gray_img[i][j]; ++k)
				hist_sum += freq[k];

			// Formula
			image->gray_img[i][j] = round((double)hist_sum * 255 /
									(image->row_num * image->col_num));
			clamp(&image->gray_img[i][j]);
		}
	}
	printf(EQUALIZE_SUCCES);
}

// Rotate the selection of the image
void rotate_cmd(image_t *image, selection_t *sel)
{
	char content[BUFFER_SIZE] = "", command[BUFFER_SIZE] = "";
	int angle = 0;

	// Read the entire command
	fgets(content, BUFFER_SIZE, stdin);
	strcpy(command, content + 1);
	remove_newline(command);
	remove_trailing_whitespace(command);

	if (!image->gray_img && !image->color_img) {
		printf(NO_IMG_LOADED);
		return;
	}

	// Get the angle from the string
	angle = atoi(command);
	if (angle % SQUARE_ANGLE != 0) {
		printf(ANGLE_ERR);
		return;
	}
	if (!is_sel_square(*sel) && !is_whole_img(*image, *sel)) {
		printf(NO_SQUARE);
		return;
	}

	angle %= CIRCLE_ANGLE;
	if (angle < 0)
		angle += CIRCLE_ANGLE;

	// Rotate the desired selection of the image
	if (is_sel_square(*sel)) {
		for (int i = 0; i < angle; i += SQUARE_ANGLE)
			rotate_selection(image, *sel);
	} else {
		for (int i = 0; i < angle; i += SQUARE_ANGLE) {
			rotate_image(image);
			select_all(*image, sel);
		}
	}
	printf("%s%s\n", ROTATED, command);
}

// Crop the selection of the image
void crop_cmd(image_t *image, selection_t *sel)
{
	if (!image->gray_img && !image->color_img) {
		printf(NO_IMG_LOADED);
		return;
	}
	crop(image, *sel);
	select_all(*image, sel);
	printf(IMG_CROPPED);
}

// Apply kernel over the selection of the image
void apply_cmd(image_t *image, selection_t sel, const char *apply_list[])
{
	char content[BUFFER_SIZE] = "", command[BUFFER_SIZE] = "";

	// The kernels and div for each type of apply
	const int edge_det[SMALL_SIZE][SMALL_SIZE] = {{-1, -1, -1},
												  {-1, 8, -1},
												  {-1, -1, -1}};
	const int sharpen[SMALL_SIZE][SMALL_SIZE] = {{0, -1, 0},
												 {-1, 5, -1},
												 {0, -1, 0}};
	const int box_blur[SMALL_SIZE][SMALL_SIZE] = {{1, 1, 1},
												  {1, 1, 1},
												  {1, 1, 1}};
	const int gauss_blur[SMALL_SIZE][SMALL_SIZE] = {{1, 2, 1},
													{2, 4, 2},
													{1, 2, 1}};
	const int edge_det_div = 1, sharpen_div = 1,
	box_blur_div = 9, gauss_blur_div = 16;

	// Read the entire command
	fgets(content, BUFFER_SIZE, stdin);
	strcpy(command, content + 1);
	remove_newline(command);
	remove_trailing_whitespace(command);

	if (!image->gray_img && !image->color_img) {
		printf(NO_IMG_LOADED);
		return;
	}
	if (strlen(command) == 0) {
		printf(INVALID_COMMAND);
		return;
	}
	if (image->type[1] == '2' || image->type[1] == '5') {
		printf(APPLY_GRAY);
		return;
	}

	// Apply the requested kernel
	switch (which_command(command, apply_list, N_APPLY)) {
	case 0:
		apply(image, edge_det, edge_det_div, sel);
		printf(APPLIED_EDGE);
		break;
	case 1:
		apply(image, sharpen, sharpen_div, sel);
		printf(APPLIED_SHARPEN);
		break;
	case 2:
		apply(image, box_blur, box_blur_div, sel);
		printf(APPLIED_BLUR);
		break;
	case 3:
		apply(image, gauss_blur, gauss_blur_div, sel);
		printf(APPLIED_GAUSSIAN_BLUR);
		break;
	default:
		printf(APPLY_PARAM_INVALID);
		break;
	}
}

// Save the image in a file
void save_cmd(image_t image)
{
	FILE *save_file = NULL;
	char file_name[BUFFER_SIZE] = "", command[BUFFER_SIZE] = "", *word;

	// Read the entire command
	fgets(command, BUFFER_SIZE, stdin);
	if (!image.gray_img && !image.color_img) {
		printf(NO_IMG_LOADED);
		return;
	}
	remove_newline(command);
	word = strtok(command, " ");
	strcpy(file_name, word);
	word = strtok(NULL, " ");

	// Save as binary
	if (!word) {
		if (image.type[1] < '4') {
			image.type[1] += 3;
			image.header[1] += 3;
		}
		generate_header(&image);

		// Open the save file as "write binary"
		save_file = fopen(file_name, "wb");
		fwrite(image.header, sizeof(char), strlen(image.header), save_file);
		if (image.type[1] == '5')
			print_gray_img_binary(image, &save_file);
		else
			print_color_img_binary(image, &save_file);
		fclose(save_file);
	} else {
		// Save as text
		if (strcmp(word, "ascii") == 0) {
			if (image.type[1] > '4') {
				image.type[1] -= 3;
				image.header[1] -= 3;
			}
			generate_header(&image);
			remove_newline(file_name);
			remove_trailing_whitespace(file_name);

			// Open the save file as "write text"
			save_file = fopen(file_name, "wt");
			fprintf(save_file, "%s", image.header);
			if (image.type[1] == '2')
				print_gray_img_text(image, &save_file);
			else
				print_color_img_text(image, &save_file);
			fclose(save_file);
		} else {
			printf(INVALID_COMMAND);
			return;
		}
	}
	printf("%s%s\n", SAVED, file_name);
}

// Exit the program
void exit_cmd(image_t image)
{
	if (!image.gray_img && !image.color_img) {
		printf(NO_IMG_LOADED);
		return;
	}

	// Free the memory
	if (image.gray_img)
		free_matrix(image.row_num, image.gray_img);
	if (image.color_img)
		free_color_matrix(image.row_num, image.color_img);
}
