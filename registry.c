#include "registry.h"

/**
 * @file registry.c
 * @breif functions that have to do with filling registrys, creating nodes, or inserting nodes
 * @author Elijah Johnson
 * @date 11/15/16
 * @bug none known
 */

/**
 * creates a node and fills its value with either a 1 or 0 returns a pointer to that node
 * @param n the char to fill the node with, only works if it is '1' '0' or 1, 0
 * @return t the pointer to the new node
 */
struct bit_t *create_node(unsigned char n)
{
        struct bit_t *t = NULL;

        t = malloc(sizeof(struct bit_t)); /*allocates space for the new node */

        if (t == NULL) {
                printf("Malloc failed\nExiting\n"); /* checks if malloc failed */
                exit(MALLOC);
        }

        /* checks if the unsigned char n is a valid character for a node to be created with if not it exits the program */
        if (n == '1' || n == 1) {
                t->n = 1;
        } else if (n == '0' || n == 0) {
                t->n = 0;
        } else {
                printf("Invalid character to create a bit_t node with\n");
                exit(INVALID_ENTRY);
        }
        t->next = NULL;
        t->prev = NULL; /*set the two pointers inside of t to null */

        return t;
}

/**
 * inserts a node at the tail of a doubly linked list
 * @param **tail a pointer to the pointer of tail. Pass it as a double pointer so you can change the position of tail
 * @param *node the new node to be inserted at the tail
 */
void insert_tail(struct bit_t **tail, struct bit_t *node)
{

        if (*tail == NULL) {
                node->next = *tail; /* check if the tail exists yet, if not easy case */
                *tail = node;
                return;
        }

        /* inserts node at the end making sure the prev node isnt lost */
        (*tail)->next = node;
        node->prev = *tail;
        *tail = node;
}

/**
 * fills a registry at the tail, the first node inserted ends up being the head and the last node ends up being the tail
 * @param *expression the values to fill the nodes with
 * @param **head the head of the registry you want to fill
 * @param **tail the tail of the registry you want to fill
 * @param word_size the word_size the user enterted, how many registrys to create
 */
void fill_registry_tail(char *expression, struct bit_t **head, struct bit_t **tail, int word_size)
{
        int i;

        /* check if the list exsits yet */
        if (*head == NULL) {
                insert_tail(head, create_node(*expression));
                *tail = *head;
        }

        /* insert at tail a number of times equal to word_size and the length of expression */
        for (i = 1; i < word_size; i++) {
                insert_tail(tail, create_node(*(expression + i)));
        }

}

/**
 * fills a registry at the tail, the first node inserted ends up being the tail and the last node inserted is the head
 * @param *expression the values to fill the nodes with
 * @param **head the head of the registry you want to fill
 * @param **tail the tail of the registry you want to fill
 * @param word_size the word_size the user entered, how many registrys to create
 */
void fill_registry_head(char *expression, struct bit_t **head, struct bit_t **tail, int word_size)
{
        int i;
        
        /* create the head and tail of the list */
        insert_head(head, create_node(*expression));
        *tail = *head;

        /* insert at head a number of times equal to word_size and the length of expression */
        for (i = 1; i < word_size; i++) {
                insert_head(head, create_node(*(expression + i)));
        }
}

/**
 * inserts a new node at the head of a doubly linked list
 * @param **head the head of the list to be set to the new node
 * @param *node the new node that will become the head of the list
 */
void insert_head(struct bit_t **head, struct bit_t *node)
{
        if (*head == NULL) {
                node->next = *head; /*if the list does not exsist, easy case */
                *head = node;
                return;
        }

        /* inserts node at the head making sure not to lose the list */
        node->next = *head;
        (*head)->prev = node;
        *head = node;
}
