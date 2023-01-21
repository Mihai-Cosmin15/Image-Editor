// Copyright Mihai-Cosmin Nour 311CAb 2022-2023

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "my_defs.h"
#include "utils.h"
#include "my_alloc.h"
#include "read_print.h"

int which_command(char *command, const char **command_list, int n_commands);

void load_cmd(image_t *image, selection_t *sel);

void select_cmd(image_t image, selection_t *sel);

void histogram_cmd(image_t image);

void equalize_cmd(image_t *image);

void rotate_cmd(image_t *image, selection_t *sel);

void crop_cmd(image_t *image, selection_t *sel);

void apply_cmd(image_t *image, selection_t sel, const char *apply_list[]);

void save_cmd(image_t image);

void exit_cmd(image_t image);

#endif
