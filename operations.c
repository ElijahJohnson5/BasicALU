#include "operations.h"

/**
 * @file operations.c
 * @brief function definitions for the operations on binary expressions and converting those numbers as needed
 * @author Elijah Johnson
 * @date 11/14/2016
 * @bug none known
 */

/**
 * compares the values in the first two registrys and stores the bitwise or of that value in the third registry
 * @param *cpu the cpu with the registrys to be compared and for the compared value to be stored
 */
void or(struct cpu_t *cpu)
{
        struct bit_t *tmp = cpu->r1_head; /* tmp values for the head of the registrys to be walked through so the position of head does not change */
        struct bit_t *tmp2 = cpu->r2_head;
        int i;

        insert_tail(&cpu->r3_tail, create_node(tmp->n | tmp2->n)); /* creates the first node at the tail and the head and then advances tmp and tmp2 */
        cpu->r3_head = cpu->r3_tail;
        tmp = tmp->next;
        tmp2 = tmp2->next;

        for (i = 1; i < cpu->word_size; i++) {
                insert_tail(&cpu->r3_tail, create_node(tmp->n | tmp2->n)); /* walks through list and compares the two values with the or operator */
                tmp = tmp->next;
                tmp2 = tmp2->next;
        }
}

/**
 * compares the values in the first two registrys using the and operator and stores the value in the third registry
 * @param *cpu the struct with the three registrys in which all of the values are stored
 */
void and(struct cpu_t *cpu)
{
        struct bit_t *tmp = cpu->r1_head;
        struct bit_t *tmp2 = cpu->r2_head; /* tmp values so that the position of the head nodes dont change */
        int i;

        insert_tail(&cpu->r3_tail, create_node(tmp->n & tmp2->n));
        cpu->r3_head = cpu->r3_tail;
        tmp = tmp->next;                /*same logic as the or function, walks through the list created nodes and inserting them at the tail */
        tmp2 = tmp2->next;

        for (i = 1; i < cpu->word_size; i++) {
                insert_tail(&cpu->r3_tail, create_node(tmp->n & tmp2->n));
                tmp = tmp->next;
                tmp2 = tmp2->next;
        }
}

/**
 * compares the values in the first two registrys using the xor operator and stores it in the third registry
 * @param *cpu the struct in which all of the values are stored in the registrys
 */
void xor(struct cpu_t *cpu)
{
        struct bit_t *tmp = cpu->r1_head;
        struct bit_t *tmp2 = cpu->r2_head; /* tmp values for the head, head never changes position */
        int i;

        insert_tail(&cpu->r3_tail, create_node(tmp->n ^ tmp2->n));
        cpu->r3_head = cpu->r3_tail; /* same logic as the and and or functions walk through list comparing the values and storing them */
        tmp = tmp->next;
        tmp2 = tmp2->next;

        for (i = 1; i < cpu->word_size; i++) {
                insert_tail(&cpu->r3_tail, create_node(tmp->n ^ tmp2->n));
                tmp = tmp->next;
                tmp2 = tmp2->next;
        }
}

/**
 * adds one to a registry so that twos compliment can be performed on it
 * @param *registry_head the head of the registry one is to be added to
 * @param *registry_tail the tail of the registry one is to be added to
 */
void add_one(struct bit_t *registry_head, struct bit_t *registry_tail)
{
        int carry = 0;
        while (registry_tail->prev) {
                if (registry_tail->n + 1 + carry == 1) { /* simple adder, to add one to the registry, using a carry value too */
                        registry_tail->n = 1; /* if there is no more carry then return because there is no more to do */
                        return;
                } else if (registry_tail->n + 1 + carry == 2) { 
                        registry_tail->n = 0;
                        carry = 1;
                } else if (registry_tail->n + 1 + carry == 3) {
                        registry_tail->n = 1;
                        carry = 1;
                }
                registry_tail = registry_tail->prev;
        }

        if (registry_tail->n + 1 + carry == 1) {
                registry_tail->n = 1;
                return;
        } else if (registry_tail->n + 1 + carry == 2) { /* check the last element of the registry, the head */
                registry_tail->n = 0;
                carry = 1;
        } else if (registry_tail->n + 1 + carry == 3) {
                registry_tail->n = 1;
                carry = 1;
        }
}

/**
 * flips the bits in the registry that registry head is associated with
 * @param *registry_head the registry in which the bits will be flipped and added one too
 * @param *registry_tail the end of the registry in whcih one will be added and the bits will be flipped
 */
void twos_compliment(struct bit_t *registry_head, struct bit_t *registry_tail) 
{
        while (registry_head->next) {
                if (registry_head->n == 0) {
                        registry_head->n = 1; /* change the bit to be opposite of what it currently is */
                } else {
                        registry_head->n = 0; 
                }
                registry_head = registry_head->next; 
        }

        if (registry_head->n == 0) {
                registry_head->n = 1; /*change the value that is stored in the registry */
        } else {
                registry_head->n = 0;
        }

        add_one(registry_head, registry_tail); /* add one to the flipped binary number */

}

/**
 * adds two registrys together
 * @param *cpu the cpu which has the registrys in it and the place to store the added value
 * @return carry to set the carry flag 
 */
unsigned int adder(struct cpu_t *cpu) 
{
        int carry = 0;
        int i = 0;
        struct bit_t *tmp = cpu->r1_tail; /*tmp values for the tails so the position of tail doesnt change */
        struct bit_t *tmp2 = cpu->r2_tail;
        char *answer = malloc(sizeof(char) * cpu->word_size);

        while (tmp->prev) {
              if (tmp->n + tmp2->n + carry == 0) { /* checks what the two values added together are and the carry value, then sets the answer[i] to that value */
                      answer[i] = '0';
                      carry = 0;
              } else if (tmp->n + tmp2->n + carry == 1) { /* also sets the carry value based on the value of all three added */
                      answer[i] = '1';
                      carry = 0;
              } else if (tmp->n + tmp2->n + carry == 2) {
                      answer[i] = '0';
                      carry = 1;
              } else if (tmp->n + tmp2->n + carry == 3) {
                      answer[i] = '1';
                      carry = 1;
              }

              i++;
              tmp = tmp->prev; /* increments tmp and tmp2 to the previous node in the linked list */
              tmp2 = tmp2->prev;
        }

        if (tmp->n + tmp2->n + carry == 0) { /* checks the last value and adds them together */
                answer[i] = '0';
                carry = 0;
        } else if (tmp->n + tmp2->n + carry == 1) {
                answer[i] = '1';
                carry = 0;
        } else if (tmp->n + tmp2->n + carry == 2) {
                answer[i] = '0';
                carry = 1;
        } else if (tmp->n + tmp2->n + carry == 3) {
                answer[i] = '1';
                carry = 1;
        }

        fill_registry_head(answer, &cpu->r3_head, &cpu->r3_tail, cpu->word_size); /* fills the third registry with the values from answer */

        free(answer); /* frees answer so there is no memory leaks */

        return carry; /* returns carry for the carry flag to be set */
}

/**
 * converts the binary answer to decimal to be printed
 * @param *answer_tail the tail pointer for the answer linked list
 * @param word_size the word_size of the current calculation
 * @param unsign either 1 or 0 if the value is unsigned or signed
 * @return the decimal value of the answer
 */
long int convert_decimal(struct bit_t *answer_tail, int word_size, int unsign) 
{
        int i = 0;
        int j = 0;
        long int answer_decimal = 0; /* value to be added to based on the conversion and then to be returned */
        int tmp = 1;
        struct bit_t *tmp2 = answer_tail; /*tmp value so the tail pointer never moves */
        
        if (unsign == 1) {
                for (i = 0; i < word_size; i++) {
                        if (tmp2->n == 1) {
                                if (i == 0) { /* if it is the leftmost digit and it is a one add one to the answer */
                                        answer_decimal += 1;
                                } else {
                                        tmp = 1; /*if it is any other digit and is a 1 then walk through and multiply 2 by itself i times, stored in tmp */
                                        for (j = i; j > 0; j--) {
                                                tmp *= 2;
                                        }
                                        answer_decimal += tmp; /* tmp gets added to answer so that it is kept track off walk through the whole linked list */
                                }
                        }
                        tmp2 = tmp2->prev;
                }
        } else { /*if value is signed */
                for (i = 0; i < word_size; i++) {
                        if (tmp2->n == 1) {
                                if (i == 0) {
                                        answer_decimal += 1;
                                } else if (i == word_size - 1) { /*same as above expect for if it is the last value then it is negative and gets subtracted from*/
                                        tmp = 1;                /* answer */
                                        for (j = i; j > 0; j--) {
                                                tmp *= 2;
                                        }
                                        answer_decimal -= tmp;
                                } else {
                                        tmp = 1;
                                        for (j = i; j > 0; j--) {
                                                tmp *= 2;
                                        }
                                        answer_decimal += tmp;
                                }
                        }
                        tmp2 = tmp2->prev;
                }
        }

        return answer_decimal;
}
