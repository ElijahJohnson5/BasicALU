#include "registry.h"

/** 
 * @file init.h
 * @breif has the prototypes for funtions that initilize, take in input, and free allocated memory
 * @author Elijah Johnson
 * @date 11/14/2016
 * @bug none known
 * @todo seperate get_input into smaller functions that are more specific
 */

#ifndef _INPUT_H
#define _INPUT_H
int get_input(char *input, struct cpu_t *cpu, char **first, char **second);

void set_flags(struct cpu_t *cpu);

void init_cpu(struct cpu_t *cpu);

void free_list(struct cpu_t *cpu);
#endif
