#include "init.h"

/**
 * @file init.c
 * @breif functions to get input, free memory and initilize values
 * @author Elijah Johnson
 * @date 11/14/2016
 * @bug none known
 * @todo break get_input into smaller easier to manage functions
 */

/**
 * takes the input the user entered, tokenizes it, and zero pads it making it the right word size
 * @param *input the string that the user entered in the form of a binary expression
 * @param *cpu the pointer to the cpu structure in which to make registrys based on the values the user input
 * @param **first a pointer to the pointer of a string that will be set to the first part of the binary expression after being handled
 * @param **second a pointer to the pointer of a string that will be set to the second part of the binary expression after being handled
 * @return either one or zero, returns one if the input is invlaid and zero if the function worked correctly
 */
int get_input(char *input, struct cpu_t *cpu, char **first, char **second)
{
        char *zeros = NULL;     /*Will be an array of zeros to pad the input if it is smaller than the word size */
        char *tmp  = NULL;
        char *tmp2 = malloc(sizeof(char) * cpu->word_size); /*temporary values to be able to parse the strings better */
        int count = 0;
        int i;

        if (tmp2 == NULL) {
                printf("Malloc failed\n");
                exit(MALLOC);
        }

        if (strchr(input, '+')) {
                *first = strtok(input, "+");
                *second = strtok(NULL, "+");
        } else if (strchr(input, '-')) {        /*Check what operator the user entered and then tokenize the string based on that operator */
                *first = strtok(input, "-");    /* Can be either +, -, |, &, ^ */
                *second = strtok(NULL, "-");
        } else if (strchr(input, '&')) {
                *first = strtok(input, "&");
                *second = strtok(NULL, "&");
        } else if (strchr(input, '|')) {
                *first = strtok(input, "|");
                *second = strtok(NULL, "|");
        } else if (strchr(input, '^')) {
                *first = strtok(input, "^");
                *second = strtok(NULL, "^");
        } else {                                /* If there isn't an operator the inpur is invalid and it returns one */
                printf("Invalid Entry\n");
                return 1;
        }

        tmp = *first;

        while (*tmp) {
                if (!isspace(*tmp) && (*tmp == '1' || *tmp == '0')) {
                     tmp2[count] = *tmp;
                     count++;
                }
                /*tmp2 holds an array of the characters in first which is being walked through by tmp, removes any "space" characters from the string */
                /* If there are any values besides "spaces" or 1's and 0's return 1 input is invalid */
                if (*tmp != ' ' && *tmp != '\n' && *tmp != '\0' && *tmp != '1' && *tmp != '0') {
                        return 1;
                }

                tmp++;
        }

        /* Make the last character of tmp2 a null character */
        tmp2[count] = '\0';

        *first = tmp2;

        /* Check if zero padding is needed */
        if (strlen(*first) < cpu->word_size) {
                /* Set zeros to an array equal to the size of the word_size in the cpu struct */
                zeros = malloc(cpu->word_size + 1);

                if (zeros == NULL) {
                        printf("Malloc failed\n");
                        exit(MALLOC);
                }
                /*Sets the value of zeros to zero so that it can be concatenated afterwards */
                for (i = 0; i < cpu->word_size - strlen(*first); i++) { 
                        zeros[i] = '0';
                }
                /*Sets last character to the null character so that strncat works */
                zeros[i] = '\0';
                
                *first = strncat(zeros, *first, strlen(*first));
                free(tmp2); /* Free tmp2 so there is no memory leak */
                zeros = NULL;
                
        }


        tmp = NULL; 
        tmp = *second;

        /* Reallocate for tmp 2 */
        tmp2 = malloc(sizeof(char) * cpu->word_size);

        if (!tmp2) {
                printf("Malloc failed\n");
                exit(MALLOC);
        }
        
        /* Set all of the values of tmp2 to the null character */
        for (i = 0; i < cpu->word_size; i++) {
                tmp2[i] = '\0';
        }

        count = 0;

        /* repeat parsing the string for **second */
        while (*tmp) {
                if (!isspace(*tmp) && (*tmp == '1' || *tmp == '0')) {
                        tmp2[count]  = *tmp;
                        count++;
                }
                if (*tmp != ' ' && *tmp != '\n' && *tmp != '\0' && *tmp != '1' && *tmp != '0') {
                        return 1;
                }

                tmp++;
        }

        tmp2[count] = '\0';
        
        *second = tmp2;
        /*Same zero pading for second if it is less than the word size */
        if (strlen(*second) < cpu->word_size) {
                zeros = malloc(cpu->word_size + 1);

                if (zeros == NULL) {
                        printf("Malloc Failed\n");
                        exit(MALLOC);
                }

                for (i = 0; i < cpu->word_size - strlen(*second); i++) {
                        zeros[i] = '0';
                }

                zeros[i] = '\0';

                *second = strncat(zeros, *second, strlen(*second));
                free(tmp2);
                zeros = NULL;
        }

        return 0;

}

/**
 * sets the flags in the struct cpu_t structure based on the output of the operation 
 * @param *cpu a pointer to the cpu struct in which the flags are stored
 */
void set_flags(struct cpu_t *cpu)
{
        int count = 0;
        int i = 0;
        struct bit_t *tmp = cpu->r3_head; /* tmp value so the position of head doesn't change */

        if (cpu->r3_head->n == 1 && cpu->r1_head->n == 0 && cpu->r2_head->n == 0) {
                cpu->overflow = 1; /*sets overflow to 1 if both the result has a sign but the opperands didnt */
        }

        if (cpu->r3_head->n == 0 && cpu->r1_head->n == 1 && cpu->r2_head->n == 1) {
                cpu->overflow = 1; /*sets overlow to 1 if both the result doesnt have a sign but the opperands did */
        }

        cpu->sign = cpu->r3_head->n; /*set the sign flag to the head of the result */

        for ( ; i < cpu->word_size; i++) {
                if (tmp->n == 1) { /* counts how many ones are in the result list */
                        count++;
                }
                tmp = tmp->next;
        }

        tmp = cpu->r3_head;
        /*sets the parity flag if the count of ones is even and the count is also not 0 */
        if (!(count % 2) && count != 0) {
                cpu->parity = 1;
        } else { /*any other time parity is one */
                cpu->parity = 0;
        }

        count = 0;
        for (i = 0; i < cpu->word_size; i++) {
                if (tmp->n == 0) {
                        count++; /*counts the number of 0's in the result list */
                }
                tmp = tmp->next;
        }

        if (count == cpu->word_size) {
                cpu->zero = 1; /*if all of the result are zeros sets the zero flag to 1 */
        } else {
                cpu->zero = 0;
        }

        tmp = NULL;
}

/**
 * sets all of the values in the struct cpu_t to zero or NULL
 * @param *cpu the pointer to the struct to be zeroed
 */
void init_cpu(struct cpu_t *cpu) 
{
        cpu->unsign = 0;
        cpu->overflow = 0;
        cpu->carry = 0;
        cpu->zero = 0;
        cpu->parity = 0;
        cpu->sign = 0; /* Set everything to zero to initilize the struct */
        cpu->word_size = 0;
        cpu->r1_head = NULL;
        cpu->r1_tail = NULL;
        cpu->r2_head = NULL;
        cpu->r2_tail = NULL;
        cpu->r3_head = NULL;
        cpu->r3_tail = NULL;
}

/**
 * frees all of the values for the three registrys in the struct cpu_t
 * @param *cpu the cpu struct in which the registrys are to be freed
 */
void free_list(struct cpu_t *cpu)
{
        while (cpu->r1_head->next) {
                cpu->r1_head = cpu->r1_head->next; /*walk through the list freeing each previous node */
                free(cpu->r1_head->prev);
        }

        free(cpu->r1_head);

        while (cpu->r2_head->next) {
                cpu->r2_head = cpu->r2_head->next; /* repeates for all three registrys */
                free(cpu->r2_head->prev);
        }

        free(cpu->r2_head);

        while (cpu->r3_tail->prev) {
                cpu->r3_tail = cpu->r3_tail->prev;
                free(cpu->r3_tail->next);
        }

        free(cpu->r3_tail);
}
