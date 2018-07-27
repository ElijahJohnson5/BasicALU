#include "print.h"

/**
 * @file print.c
 * @breif functions that print things out to the screen based off of the operations that have been done on them
 * @author Elijah Johnson
 * @date 11/15/16
 * @bug none known
 */

/**
 * makes every character in buf a null charcater so that new input can be taken in
 * @param *buf the array to be set to null characters
 */
void flush_buf(char *buf)
{
        int i;

        for (i = 0; i < SIZE; i++) { /*walk through every item and set them to null */
                buf[i] = '\0';
        }
}

/**
 * prints out the flags in the struct cpu_t
 * @param *cpu a pointer to the struct where the flags are held
 */
void print_flags(struct cpu_t *cpu)
{
        printf("flags\n");
        printf("OF: %d\nCF: %d\nSF: %d\nPF: %d\nZF: %d\n", cpu->overflow, cpu->carry, cpu->sign, cpu->parity, cpu->zero);
}

/**
 * prints out the addition pretty print
 * @param *cpu the struct which has the two registrys to be printed
 */
void print_addition(struct cpu_t *cpu)
{
        struct bit_t *tmp = cpu->r1_head;
        struct bit_t *tmp2 = cpu->r2_head; /* tmp values so the position of head for r1 and r2 does not change */

        while (tmp->next) {
                printf("%d", tmp->n); /* walk through the doubly linked list and print out the value of each node */
                tmp = tmp->next;
        }

        printf("%d\n", tmp->n); /* print out the last node */
        printf("+\n");

        while (tmp2->next) {
                printf("%d", tmp2->n); /*walk through the doubly linked list and prints out the value at each node */
                tmp2 = tmp2->next;
        }

        printf("%d\n", tmp2->n);
}

/**
 * prints out an amount of dashes based on the word size
 * @param word_size the word_size the user entered before, amount of dashes to be printed
 */
void print_dash(int word_size)
{
        int i;
        for (i = 0; i < word_size; i++) {
                printf("-"); /*prints out dashes whose number = the word size */
        }

        printf("\n");
}

/**
 * prints out the registry answer after the operation has occured on it
 * @param *cpu stores the value after the operation has occured, one registry to be printed from it
 */
void print_answer(struct cpu_t *cpu)
{
        /* tmp value so the head does not change position */
        struct bit_t *tmp = cpu->r3_head;

        while (tmp->next) {
                printf("%d", tmp->n); /*walk through list prints out value at node */
                tmp = tmp->next;
        }
        printf("%d\n", tmp->n);
}

/**
 * prints either the and or or xor calculations
 * @param *cpu the struct where the registrys and values are held to be printed
 * @param operation the operation that is being performed
 */
void print_and_or_xor(struct cpu_t *cpu, int operation)
{
        int i;
        char op;
        struct bit_t *tmp = cpu->r1_head;
        struct bit_t *tmp2 = cpu->r2_head; /*tmp values so the heads dont change position */

        switch (operation) {
        case AND:
                op = '&';
                break; /*set the char op to the correct character based on what the param operation was equal to */
        case OR:
                op = '|';
                break;
        case XOR:
                op = '^';
                break;
        }


        for (i = 0; i < cpu->word_size; i++) {
                printf("%d", tmp->n); /*walk through the list printing out every value */
                tmp = tmp->next;
        }

        printf("\n%c\n", op);

        for (i = 0; i < cpu->word_size; i++) {
                printf("%d", tmp2->n);
                tmp2 = tmp2->next;
        }

        printf("\n");
}
