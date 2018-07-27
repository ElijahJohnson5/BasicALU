#include "registry.h"

/**
 * @file operations.h
 * @breif contains the prototypes for the functions that do operations on the binary sequences
 * @author Elijah Johnson
 * @date 11/14/2016
 * @bug none known
 */

#ifndef _OPERATIONS_H
#define _OPERATIONS_H

unsigned int adder(struct cpu_t *cpu);

void or(struct cpu_t *cpu);

void and(struct cpu_t *cpu);

void xor(struct cpu_t *cpu);

void twos_compliment(struct bit_t *registry_head, struct bit_t *registry_tail);

long int convert_decimal(struct bit_t *answer_tail, int word_size, int unsign);
#endif
