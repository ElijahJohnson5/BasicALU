#include "registry.h"
#include "init.h"
#include "operations.h"
#include "print.h"

/**
 * @file main.c
 * @breif logic for the "cpu" implements the functions from registry.h, init.h, operations.h and print.h
 * @author Elijah Johnson
 * @date 11/15/16
 * @bug none known
 */


int main()
{
        char buf[SIZE];
        struct cpu_t *cpu = malloc(sizeof(struct cpu_t));
        char *expression = NULL;
        char *first = NULL; /* declare all the variables needed and allocate the space for cpu */
        char *second = NULL;
        char repeat;
        int op = 0;

        /* check if malloc worked, if not exit */
        if (cpu == NULL) {
                printf("Could not malloc memory exiting\n");
                exit(MALLOC);
        }

LOOP:
        init_cpu(cpu);
        first = NULL;
        second = NULL; /* initilize all of the values to what they should be set to */
        expression = NULL;
        repeat = 'y';

GET_WORD_SIZE:
        printf("Enter the word size: ");
        flush_buf(buf);
        fgets(buf, SIZE, stdin);
        sscanf(buf, "%d", &cpu->word_size); /*gets input from the user about the word size, if it is not between 1 and 64 ask them to enter it again */

        if (cpu->word_size > MAX_WORD_SIZE || cpu->word_size < MIN_WORD_SIZE) {
                printf("Error in word size, must be between 1 and 64, please try again\n");
                goto GET_WORD_SIZE;
        }

        /* ask user if they want signed or unsigned values, if nothing is entered then they are signed values */
        printf("Unsigned values [y/N]: ");
        flush_buf(buf); 
        fgets(buf, SIZE, stdin);
        sscanf(buf, "%c", &repeat);

        cpu->unsign = repeat;

        if (cpu->unsign == 'y' || cpu->unsign == 'Y') {
                cpu->unsign = 1;
        } else {
                cpu->unsign = 0;
        }

BINARY_EXPRESSION:
        printf("Enter binary expression: ");
        flush_buf(buf);
        fgets(buf, SIZE, stdin);
        /* gets input from the user about what expression they want evaluated */
        expression = malloc(sizeof(char) * strlen(buf));
        /*make expression equal to the size of buf, dynamically allocate that space and then copy buf into expression */
        expression = strncpy(expression, buf, strlen(buf) + 1);

        /* search the string to see if the user entered an expression and set the variable op equal to that expression*/
        if (strchr(expression, '+')) {
                op = ADDITION;
        } else if (strchr(expression, '-')) {
                op = SUBTRACTION;
        } else if (strchr(expression, '|')) {
                op = OR;
        } else if (strchr(expression, '&')) {
                op = AND;
        } else if (strchr(expression, '^')) {
                op = XOR;
        } else {
                printf("Invalid entry, no operation in expression\n"); /*if no expression is found ask them to enter another binary expression */
                goto BINARY_EXPRESSION;
        }

        /* if get_input returns 1 then it failed so ask the user to enter another binary expression */
        if (get_input(expression, cpu, &first, &second)) {
                printf("error in input, please enter a binary expression\n");
                goto BINARY_EXPRESSION;
        }


        /* check if the expression they entered was equal to or less than the word size they entered. if it is not ask them to enter a new binary expression */
        if (strlen(first) > cpu->word_size) {
                printf("Error in input, first entry bigger than the word size\n");
                printf("Error in first half of input\n");
                goto BINARY_EXPRESSION;
        } 
        if (strlen(second) > cpu->word_size) {
                printf("Error in input, second entry bigger than the word size\n");
                printf("Error in second half of input\n");
                goto BINARY_EXPRESSION;
        }

        /* fill the registrys from the tail based on what they entered */
        fill_registry_tail(first, &cpu->r1_head, &cpu->r1_tail, cpu->word_size);
        fill_registry_tail(second, &cpu->r2_head, &cpu->r2_tail, cpu->word_size);

        /* figure out what operation they wanted to perform */
        switch (op) {
        case ADDITION:
                print_addition(cpu);

                if (adder(cpu))
                        cpu->carry = 1; /*if adder returns a value that means that carry should be set to one */

                set_flags(cpu); /* set flags print out the dashes and the answer to the operation */
                print_dash(cpu->word_size);
                print_answer(cpu);
                break;
        case SUBTRACTION:
                twos_compliment(cpu->r2_head, cpu->r2_tail); /*perform twos compliment on the second number the user entered */
                print_addition(cpu);
                if (adder(cpu)) /*then do the same thing as addition */
                        cpu->carry = 1;

                set_flags(cpu);
                print_dash(cpu->word_size);
                print_answer(cpu);
                break;
        case OR:
                or(cpu); /* perform the or calculation */ 
                set_flags(cpu); /* set the flags */
                cpu->overflow = 0; /*set overflow and carry to 0 because or does not set those flags */
                cpu->carry = 0;
                print_and_or_xor(cpu, op);
                print_dash(cpu->word_size);
                print_answer(cpu);
                break;
        case AND:
                and(cpu); /*perform the and calculation then it is exactly the same as or */
                set_flags(cpu);
                cpu->overflow = 0;
                cpu->carry = 0;
                print_and_or_xor(cpu, op);
                print_dash(cpu->word_size);
                print_answer(cpu);
                break;
        case XOR:
                xor(cpu); /*perform the xor calculation then it is exactly the same as or and and */
                set_flags(cpu);
                cpu->overflow = 0;
                cpu->carry = 0;
                print_and_or_xor(cpu, op);
                print_dash(cpu->word_size);
                print_answer(cpu);
                break;
        }

        if (cpu->r3_head->n == 1) {
                cpu->sign = 1; /*if the first bit is one set the sign flag to one otherwise it stays as zero */
        }

        print_flags(cpu); /*print out the set flags */

        /* print out the decimal answer and based on if it was unsigned or signed prints out Unsigned decimal or decimal */
        printf("%s %ld\n", (cpu->unsign == 1 ? "Unsigned Decimal" : "Decimal"), convert_decimal(cpu->r3_tail, cpu->word_size, cpu->unsign)); 

        free_list(cpu);
        free(expression); /* free everything that was allocated */
        free(first);
        free(second);

        printf("Do you want to continue [Y/n] ");
        flush_buf(buf);
        fgets(buf, SIZE, stdin);
        sscanf(buf, "%c", &repeat); /*ask the user if they want to continue, if nothing is entered automatically continue */

        if (repeat == 'n' || repeat == 'N') {
                printf("Exiting\n");
        } else {
                goto LOOP; /* goto the top of the program and reinitilize the values that need to be reinitilized */
        }

        free(cpu); /*cpu needs to be used up until they exit the program, once they want to quit then free the cpu pointer */

        return 0;

}
