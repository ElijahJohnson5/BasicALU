#include <stdio.h>
#include "registry.h"

/**
 * @file print.h
 * @breif function declerations for any functions that print out things to the screen
 * @author Elijah Johnson
 * @date 11/15/16
 * @bug none known
 */

#ifndef _PRINT_H
#define _PRINT_H

void print_flags(struct cpu_t *cpu);

void flush_buf(char *buf);

void print_addition(struct cpu_t *cpu);

void print_dash(int word_size);

void print_answer(struct cpu_t *cpu);

void print_and_or_xor(struct cpu_t *cpu, int operation);
#endif
