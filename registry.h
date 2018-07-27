#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

/**
 * @file registry.h
 * @breif main header file containing structs #define statements and any functions that have to do with filling registrys
 * @author Elijah Johnson
 * @date 11/15/16
 * @bug none known
 */

#ifndef _REGISTRY_H
#define _REGISTRY_H


#define MALLOC 1000
#define SIZE 4096
#define MAX_WORD_SIZE 64
#define MIN_WORD_SIZE 1
#define INVALID_ENTRY 8008 /*defines used for errors and checking values */
#define ADDITION 1
#define SUBTRACTION 2
#define OR 3
#define AND 4
#define XOR 5

struct bit_t {
        unsigned char n;

        struct bit_t *next;
        struct bit_t *prev;
};

struct cpu_t {

        int word_size;
        int unsign;

        int overflow;
        int carry;
        int sign;
        int parity;
        int zero;

        struct bit_t *r1_head;
        struct bit_t *r1_tail;
        struct bit_t *r2_head;
        struct bit_t *r2_tail;
        struct bit_t *r3_head;
        struct bit_t *r3_tail;

};

void insert_tail(struct bit_t **tail, struct bit_t *node);

void fill_registry_tail(char *exprssion, struct bit_t **head, struct bit_t **tail, int word_size);

void fill_registry_head(char *exprssion, struct bit_t **head, struct bit_t **tail, int word_size);

struct bit_t *create_node(unsigned char n);

void insert_head(struct bit_t **head, struct bit_t *node);

#endif
