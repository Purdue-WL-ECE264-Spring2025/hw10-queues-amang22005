#include "queue.h"
#include "tile_game.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h> // For memcmp

// For tracking visited states in BFS
#define MAX_STATES 362880 // 9! / 2, accounting for solvable states
struct hash_set {
    uint64_t states[MAX_STATES];
    int size;
};

// Simple hash set implementation
static bool contains(struct hash_set *set, uint64_t state) {
    for (int i = 0; i < set->size; i++) {
        if (set->states[i] == state) return true;
    }
    return false;
}

static void add(struct hash_set *set, uint64_t state) {
    if (set->size < MAX_STATES && !contains(set, state)) {
        set->states[set->size++] = state;
    }
}

void enqueue(struct queue *q, struct game_state state) {
    uint64_t serialized = serialize(state);
    insert_at_tail(&q->data, serialized);
}

struct game_state dequeue(struct queue *q) {
    uint64_t serialized = remove_from_head(&q->data);
    return deserialize(serialized);
}

// Goal state for comparison
static const struct game_state goal = {
    .tiles = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 0}},
    .empty_row = 3,
    .empty_col = 3,
    .num_steps = 0
};

int number_of_moves(struct game_state start) {
    struct queue q = {.data = {NULL}};
    struct hash_set visited = {0};
    enqueue(&q, start);
    add(&visited, serialize(start));

    while (q.data.head) {
        struct game_state current = dequeue(&q);
        uint64_t current_serialized = serialize(current);

        // Check if current state matches the goal state
        if (memcmp(current.tiles, goal.tiles, sizeof(goal.tiles)) == 0) {
            free_list(q.data);
            return current.num_steps;
        }

        // Try all possible moves
        struct game_state next = current;
        move_up(&next);
        if (serialize(next) != current_serialized && !contains(&visited, serialize(next))) {
            enqueue(&q, next);
            add(&visited, serialize(next));
        }

        next = current;
        move_down(&next);
        if (serialize(next) != current_serialized && !contains(&visited, serialize(next))) {
            enqueue(&q, next);
            add(&visited, serialize(next));
        }

        next = current;
        move_left(&next);
        if (serialize(next) != current_serialized && !contains(&visited, serialize(next))) {
            enqueue(&q, next);
            add(&visited, serialize(next));
        }

        next = current;
        move_right(&next);
        if (serialize(next) != current_serialized && !contains(&visited, serialize(next))) {
            enqueue(&q, next);
            add(&visited, serialize(next));
        }
    }

    free_list(q.data);
    return -1; // Unsolvable (though all valid inputs should be solvable)
}