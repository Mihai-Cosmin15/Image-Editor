// Copyright Mihai-Cosmin Nour 311CAb 2022-2023

#include <stdio.h>
#include <string.h>
#include "commands.h"

int main(void)
{
	char command[BUFFER_SIZE] = "";
	const char *command_list[N_COMMANDS] = {"LOAD", "SELECT", "HISTOGRAM",
											"EQUALIZE", "ROTATE", "CROP",
											"APPLY", "SAVE", "EXIT"};
	const char *apply_list[N_APPLY] = {"EDGE", "SHARPEN", "BLUR",
									   "GAUSSIAN_BLUR"};
	image_t image;
	selection_t sel;
	image.row_num = 0;
	image.col_num = 0;
	image.max_value = 0;
	image.gray_img = NULL;
	image.color_img = NULL;
	strcpy(image.header, "");
	while (1) {
		scanf("%s", command);
		switch (which_command(command, command_list, N_COMMANDS)) {
		case 0:
			load_cmd(&image, &sel);
			break;
		case 1:
			select_cmd(image, &sel);
			break;
		case 2:
			histogram_cmd(image);
			break;
		case 3:
			equalize_cmd(&image);
			break;
		case 4:
			rotate_cmd(&image, &sel);
			break;
		case 5:
			crop_cmd(&image, &sel);
			break;
		case 6:
			apply_cmd(&image, sel, apply_list);
			break;
		case 7:
			save_cmd(image);
			break;
		case 8:
			exit_cmd(image);
			return 0;
		default:
			fgets(command, BUFFER_SIZE, stdin);
			printf(INVALID_COMMAND);
			break;
		}
	}
	return 0;
}
