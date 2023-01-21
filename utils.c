// Copyright Mihai-Cosmin Nour 311CAb 2022-2023

#include "utils.h"

void swap_ints(int *x, int *y)
{
	int aux = *x;
	*x = *y;
	*y = aux;
}

int is_power_2(int x)
{
	return (x & (x - 1)) == 0;
}

int is_number(char *s)
{
	// Check if a string is a number (even negative)
	int len = strlen(s);
	if (s[0] == '-' && strlen(s) < 2)
		return 0;
	if (s[0] != '-' && (s[0] < '0' || s[0] > '9'))
		return 0;
	for (int i = 1; i < len; ++i)
		if (s[i] < '0' || s[i] > '9')
			return 0;
	return 1;
}

void remove_newline(char *s)
{
	int len = strlen(s);
	if (s[--len] == '\n')
		s[len] = '\0';
}

void remove_trailing_whitespace(char *s)
{
	int len = strlen(s);
	for (int i = len - 1; i >= 0; --i) {
		if (s[i] == ' ')
			s[i] = '\0';
		else
			break;
	}
}

int max_array(int *v, int n)
{
	int max = 0;
	for (int i = 0; i < n; ++i)
		if (v[i] > max)
			max = v[i];
	return max;
}

int max_matrix(int **a, int n, int m)
{
	int max = 0;
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < m; ++j)
			if (a[i][j] > max)
				max = a[i][j];
	return max;
}

// Determine the frequency of each pixel value based on a given image
void det_freq(int *freq, image_t image)
{
	for (int i = 0; i < MAX_VALUE; ++i)
		freq[i] = 0;
	for (int i = 0; i < image.row_num; ++i) {
		for (int j = 0; j < image.col_num; ++j)
			freq[image.gray_img[i][j]]++;
	}
}

// Ensure the pixels have values between 0 and 255
void clamp(int *value)
{
	if (*value < 0)
		*value = 0;
	if (*value > MAX_VALUE - 1)
		*value = MAX_VALUE - 1;
}

// Division required for apply
void div_apply(int *value, int div)
{
	*value = round((double)*value / div);
}

// Convert the parts of the header to string and then add them to the header
void generate_header(image_t *image)
{
	char content[BUFFER_SIZE];
	strcpy(image->header, "");
	strcat(image->header, image->type);
	strcat(image->header, "\n");

	sprintf(content, "%d", image->col_num);
	strcat(image->header, content);
	strcat(image->header, " ");

	sprintf(content, "%d", image->row_num);
	strcat(image->header, content);
	strcat(image->header, "\n");

	sprintf(content, "%d", image->max_value);
	strcat(image->header, content);
	strcat(image->header, "\n");
}

// Read the parts of the header of the image
void read_header(image_t *image, FILE **image_file)
{
	// except the type (magic word) which is handled in the load command
	char *word, content[BUFFER_SIZE];
	strcat(image->header, image->type);
	remove_newline(image->type);
	fgets(content, BUFFER_SIZE, *image_file);

	// Handle comment
	while (content[0] == '#')
		fgets(content, BUFFER_SIZE, *image_file);
	strcat(image->header, content);
	remove_newline(content);

	word = strtok(content, " ");
	image->col_num = atoi(word);

	word = strtok(NULL, " ");
	image->row_num = atoi(word);

	fgets(content, BUFFER_SIZE, *image_file);

	// Handle the other possible location for comment
	while (content[0] == '#')
		fgets(content, BUFFER_SIZE, *image_file);
	strcat(image->header, content);
	remove_newline(content);

	image->max_value = atoi(content);
}

void crop(image_t *image, selection_t sel)
{
	int old_rows = image->row_num;

	// Determine the size of the new image (after crop)
	image->col_num = sel.y2 - sel.y1;
	image->row_num = sel.x2 - sel.x1;

	// Put the corresponding values from the old image to the new one
	// depending on its type
	if (image->type[1] == '2' || image->type[1] == '5') {
		int **new_img = alloc_matrix(image->row_num, image->col_num);

		for (int i = 0; i < image->row_num; ++i)
			for (int j = 0; j < image->col_num; ++j)
				new_img[i][j] = image->gray_img[sel.x1 + i][sel.y1 + j];

		// Eliminate the old image from memory and replace it
		free_matrix(old_rows, image->gray_img);
		image->gray_img = new_img;
	} else {
		color_t **new_img = alloc_color_matrix(image->row_num, image->col_num);

		for (int i = 0; i < image->row_num; ++i)
			for (int j = 0; j < image->col_num; ++j)
				new_img[i][j] = image->color_img[sel.x1 + i][sel.y1 + j];

		// Eliminate the old image from memory and replace it
		free_color_matrix(old_rows, image->color_img);
		image->color_img = new_img;
	}
}

// Add 2 color_t variables (required for apply)
color_t color_t_add(color_t x, color_t y)
{
	color_t sum;
	sum.red = x.red + y.red;
	sum.green = x.green + y.green;
	sum.blue = x.blue + y.blue;
	return sum;
}

// Multiply a color_t variable with an integer (required for apply)
color_t color_t_mult(color_t x, int y)
{
	color_t prod;
	prod.red = x.red * y;
	prod.green = x.green * y;
	prod.blue = x.blue * y;
	return prod;
}

// Apply a kernel over a selection of the image
void apply(image_t *image, const int apply_matrix[SMALL_SIZE][SMALL_SIZE],
		   const int div, selection_t sel)
{
	color_t **temp_img = alloc_color_matrix(image->row_num,
											image->col_num);
	for (int i = sel.x1; i < sel.x2; ++i) {
		for (int j = sel.y1; j < sel.y2; ++j) {
			// Ignore border pixels
			if (i == 0 || i == image->row_num - 1 ||
				j == 0 || j == image->col_num - 1) {
				temp_img[i][j] = image->color_img[i][j];
				continue;
			}
			temp_img[i][j].red = 0;
			temp_img[i][j].green = 0;
			temp_img[i][j].blue = 0;

			// Multiply each of the pixels in a 3x3 grid by each value from
			// the corresponding kernel and then add them together
			for (int k = 0; k < 9; ++k) {
				int apply_i = k / SMALL_SIZE, apply_j = k % SMALL_SIZE;
				int img_i = i - 1 + apply_i, img_j = j - 1 + apply_j;
				temp_img[i][j] = color_t_add(color_t_mult
								 (image->color_img[img_i][img_j],
								 apply_matrix[apply_i][apply_j]),
								 temp_img[i][j]);
			}
			// Divide by the corresponding div
			div_apply(&temp_img[i][j].red, div);
			div_apply(&temp_img[i][j].green, div);
			div_apply(&temp_img[i][j].blue, div);

			// Clamp the value
			clamp(&temp_img[i][j].red);
			clamp(&temp_img[i][j].green);
			clamp(&temp_img[i][j].blue);
		}
	}

	// copy the new image in over the old image where the selection was made
	for (int i = sel.x1; i < sel.x2; ++i) {
		for (int j = sel.y1; j < sel.y2; ++j)
			image->color_img[i][j] = temp_img[i][j];
	}

	// Remove the temporary matrix from memory
	free_color_matrix(image->row_num, temp_img);
}

// Rotate the entire image
void rotate_image(image_t *image)
{
	if (image->type[1] == '2' || image->type[1] == '5') {
		// Create a new image containing the rotated image
		int **new_img = alloc_matrix(image->col_num, image->row_num);
		for (int i = 0; i < image->row_num; ++i) {
			for (int j = 0; j < image->col_num; ++j)
				new_img[j][image->row_num - 1 - i] = image->gray_img[i][j];
		}

		// Remove the old matrix from memory and replace it with the new one
		free_matrix(image->row_num, image->gray_img);
		image->gray_img = new_img;
	} else {
		// Same process for a color image
		color_t **new_img = alloc_color_matrix(image->col_num, image->row_num);
		for (int i = 0; i < image->row_num; ++i) {
			for (int j = 0; j < image->col_num; ++j)
				new_img[j][image->row_num - 1 - i] = image->color_img[i][j];
		}
		free_color_matrix(image->row_num, image->color_img);
		image->color_img = new_img;
	}

	// Swap the sizes to match the new image
	swap_ints(&image->row_num, &image->col_num);
}

// Rotate a selection of the image
void rotate_selection(image_t *image, selection_t sel)
{
	if (image->type[1] == '2' || image->type[1] == '5') {
		// Determine the size of the selected area
		int sel_rows = sel.x2 - sel.x1, sel_cols = sel.y2 - sel.y1;

		// Create a new image containing the rotated image
		int **temp_img = alloc_matrix(sel_rows, sel_cols);
		for (int i = 0; i < sel_rows; ++i) {
			for (int j = 0; j < sel_cols; ++j)
				temp_img[j][sel_cols - 1 - i] =
				image->gray_img[sel.x1 + i][sel.y1 + j];
		}

		// Copy the rotated image over the selected area in the old image
		for (int i = sel.x1; i < sel.x2; ++i) {
			for (int j = sel.y1; j < sel.y2; ++j)
				image->gray_img[i][j] = temp_img[i - sel.x1][j - sel.y1];
		}

		// Remove the temporary image from memory
		free_matrix(sel_rows, temp_img);
	} else {
		// Same process for a color image
		int sel_rows = sel.x2 - sel.x1, sel_cols = sel.y2 - sel.y1;

		color_t **temp_img = alloc_color_matrix(sel_rows, sel_cols);
		for (int i = 0; i < sel_rows; ++i) {
			for (int j = 0; j < sel_cols; ++j)
				temp_img[j][sel_cols - 1 - i] =
				image->color_img[sel.x1 + i][sel.y1 + j];
		}

		for (int i = sel.x1; i < sel.x2; ++i) {
			for (int j = sel.y1; j < sel.y2; ++j)
				image->color_img[i][j] = temp_img[i - sel.x1][j - sel.y1];
		}
		free_color_matrix(sel_rows, temp_img);
	}
}

// Checks if the selection is inside the image
int valid_coord(image_t image, selection_t sel)
{
	if (sel.x1 < 0 || sel.x1 > image.row_num)
		return 0;
	if (sel.x2 < 0 || sel.x2 > image.row_num)
		return 0;
	if (sel.y1 < 0 || sel.y1 > image.col_num)
		return 0;
	if (sel.y2 < 0 || sel.y2 > image.col_num)
		return 0;
	if (sel.x1 == sel.x2 || sel.y1 == sel.y2)
		return 0;
	return 1;
}

int is_sel_square(selection_t sel)
{
	if (sel.x2 - sel.x1 == sel.y2 - sel.y1)
		return 1;
	return 0;
}

int is_whole_img(image_t image, selection_t sel)
{
	if (sel.x1 == 0 && sel.y1 == 0 &&
		sel.x2 == image.row_num && sel.y2 == image.col_num)
		return 1;
	return 0;
}

void select_all(image_t image, selection_t *sel)
{
	sel->x1 = 0;
	sel->y1 = 0;
	sel->x2 = image.row_num;
	sel->y2 = image.col_num;
}
