#include "linked_list.h"
#include <stdlib.h>

// Helper function to create a new node
struct list_node *new_node(size_t value) {
    struct list_node *node = (struct list_node *)malloc(sizeof(struct list_node));
    if (node) {
        node->value = value;
        node->next = NULL;
    }
    return node;
}

void insert_at_head(struct linked_list *list, size_t value) {
    struct list_node *node = new_node(value);
    if (!node) return; // Memory allocation failed
    node->next = list->head;
    list->head = node;
}

void insert_at_tail(struct linked_list *list, size_t value) {
    struct list_node *node = new_node(value);
    if (!node) return; // Memory allocation failed
    if (!list->head) {
        list->head = node;
    } else {
        struct list_node *current = list->head;
        while (current->next) {
            current = current->next;
        }
        current->next = node;
    }
}

size_t remove_from_head(struct linked_list *list) {
    if (!list->head) return 0; // Empty list
    struct list_node *temp = list->head;
    size_t value = temp->value;
    list->head = temp->next;
    free(temp);
    return value;
}

size_t remove_from_tail(struct linked_list *list) {
    if (!list->head) return 0; // Empty list
    if (!list->head->next) {
        size_t value = list->head->value;
        free(list->head);
        list->head = NULL;
        return value;
    }
    struct list_node *current = list->head;
    while (current->next->next) {
        current = current->next;
    }
    size_t value = current->next->value;
    free(current->next);
    current->next = NULL;
    return value;
}

void free_list(struct linked_list list) {
    struct list_node *current = list.head;
    while (current) {
        struct list_node *next = current->next;
        free(current);
        current = next;
    }
}

// Utility function (provided, no need to modify)
void dump_list(FILE *fp, struct linked_list list) {
    fprintf(fp, "[ ");
    for (struct list_node *cur = list.head; cur != NULL; cur = cur->next) {
        fprintf(fp, "%zu ", cur->value);
    }
    fprintf(fp, "]\n");
}