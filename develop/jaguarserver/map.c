#include "./map.h"
#include <assert.h>
#include <stdlib.h>


enum Color { RED, BLACK };


struct ValueListStruct
{
    struct ValueListStruct* tail;
    void* head;
};


struct NodeStruct
{
    struct NodeStruct* left;
    struct NodeStruct* right;
    struct NodeStruct* parent;
    struct ValueListStruct* value_list;
    enum Color color;
};


struct MapStruct
{
    struct NodeStruct* nodes;
    struct ValueListStruct* values;
    struct NodeStruct* root;
    int capacity;
    int nodes_size;
    int values_size;
    HashFunction hash_function;
};

static inline enum Color color_of(const struct NodeStruct* node)
    { return node ? (node -> color) : BLACK; }


static void swap_values(struct NodeStruct* x, struct NodeStruct* y)
{
    struct ValueListStruct* a = x -> value_list;
    x -> value_list = y -> value_list;
    y -> value_list = a;
}


static struct NodeStruct* binary_tree_insert_node(Map map, void* value)
{
    const int map_capacity = map -> capacity;
    assert(map -> values_size >= map -> nodes_size);
    assert(map_capacity >= map -> values_size);
    assert(map -> values_size == 0 || map -> nodes_size > 0);

    if(map -> values_size >= map_capacity) {
        return NULL;
    } else {
        struct NodeStruct* new_node = (map -> nodes) + (map -> nodes_size);
        new_node -> left = NULL;
        new_node -> right = NULL;
        new_node -> parent = NULL;

        struct ValueListStruct* new_value_list = (map -> values) + (map -> values_size);
        new_value_list -> tail = NULL;
        new_value_list -> head = value;

        if(map -> nodes_size == 0) {
            map -> root = new_node;
        } else {
            const int hash = (map -> hash_function)(value);

            struct NodeStruct* cursor = map -> root;
            for(;;) {
                assert(cursor);
                assert(cursor -> value_list);

                const int cursor_hash = (map -> hash_function)(cursor -> value_list -> head);
                if(cursor_hash == hash) {
                    new_node = NULL;
                    new_value_list -> tail = cursor -> value_list;
                    cursor -> value_list = new_value_list;
                } else {
                    if(cursor_hash < hash) {
                        struct NodeStruct* next_cursor = cursor -> right;
                        if(next_cursor) {
                            cursor = next_cursor;
                            continue;
                        } else {
                            new_node -> parent = cursor;
                            cursor -> right = new_node;
                        }
                    } else if(cursor_hash > hash) {
                        struct NodeStruct* next_cursor = cursor -> left;
                        if(next_cursor) {
                            cursor = next_cursor;
                            continue;
                        } else {
                            new_node -> parent = cursor;
                            cursor -> left = new_node;
                        }
                    }
                }

                break;
            }
        }
        
        map -> values_size += 1;
        if(new_node) {
            map -> nodes_size += 1;
            new_node -> value_list = new_value_list;
        }
        return new_node;
    }
}

/**
 * ========================================================================== *
 * ===========================  PUBLIC INTERFACE  =========================== *
 * ========================================================================== *
*/

/**
 * -------------------------------------------------------------------------- *
 * --------------------------  Memory management   -------------------------- *
 * -------------------------------------------------------------------------- *
*/

void map_allocate(
    const int log2_size,
    const HashFunction hash_function,
    Map* target
)
{
    int size;
    struct NodeStruct* nodes;
    struct ValueListStruct* values;
    struct MapStruct* map;

    map = NULL;
    size = 1 << log2_size;

    nodes = malloc(sizeof(*nodes) * size);
    if(nodes) {
        values = malloc(sizeof(*values) * size);
        if(values) {
            map = malloc(sizeof(*map));
            if(map) {
                map -> nodes_size = 0;
                map -> values_size = 0;
                map -> root = NULL;
                map -> capacity = size;
                map -> nodes = nodes;
                map -> values = values;
                map -> hash_function = hash_function;

                *target = map;
                return;
            }
            free(values);
        }
        free(nodes);
    }
}

void map_dispose(Map map)
{
    if(map) {
        free(map -> nodes);
        free(map -> values);
        free(map);
    }
}

/**
 * -------------------------------------------------------------------------- *
 * ---------------------  Node insertion and deletion   --------------------- *
 * -------------------------------------------------------------------------- *
*/

void map_insert(Map map, void* value)
{
    struct NodeStruct* inserted_node = binary_tree_insert_node(map, value);
    if(inserted_node) {
        assert(inserted_node -> left == NULL);
        assert(inserted_node -> right == NULL);
        inserted_node -> color = RED;
        
        for(;color_of(inserted_node) != BLACK;) {
            struct NodeStruct* parent_node = inserted_node -> parent;
            if(parent_node) {
                if(color_of(parent_node) == RED) {
                    struct NodeStruct* uncle_node;
                    struct NodeStruct* grand_parent_node;

                    grand_parent_node = parent_node -> parent;
                    assert(grand_parent_node); // root is not red
                    assert(color_of(grand_parent_node) == BLACK);

                    if(grand_parent_node -> left == parent_node) {
                        uncle_node = grand_parent_node -> right;
                        if(color_of(uncle_node) == BLACK) {
                            if(inserted_node == parent_node -> left) {
                                inserted_node -> parent = NULL;
                                parent_node -> left = NULL;

                                { // Reorganise ancestor tree
                                    parent_node -> left = parent_node -> right;

                                    parent_node -> right = uncle_node;
                                    if(uncle_node) uncle_node -> parent = parent_node;
                                    
                                    grand_parent_node -> right = parent_node;

                                    swap_values(grand_parent_node, parent_node);
                                }
                                
                                grand_parent_node -> left = inserted_node;
                                inserted_node -> parent = grand_parent_node;
                            } else {
                                inserted_node -> parent = NULL;
                                parent_node -> right = NULL;

                                {
                                    if(inserted_node -> left)
                                        inserted_node -> left -> parent = parent_node;
                                    parent_node -> right = inserted_node -> left;

                                    inserted_node -> left = inserted_node -> right;

                                    inserted_node -> right = uncle_node;
                                    if(uncle_node)
                                        uncle_node -> parent = inserted_node;

                                    swap_values(grand_parent_node, inserted_node);
                                }

                                grand_parent_node -> right = inserted_node;
                                inserted_node -> parent = grand_parent_node;
                            }
                            continue;
                        }
                    } else {
                        uncle_node = grand_parent_node -> left;
                        if(color_of(uncle_node) == BLACK) {
                            if(inserted_node == parent_node -> right) {
                                inserted_node -> parent = NULL;
                                parent_node -> right = NULL;

                                { // Reorganise ancestor tree
                                    parent_node -> right = parent_node -> left;

                                    parent_node -> left = uncle_node;
                                    if(uncle_node) uncle_node -> parent = parent_node;
                                    
                                    grand_parent_node -> left = parent_node;

                                    swap_values(grand_parent_node, parent_node);
                                }
                                
                                grand_parent_node -> right = inserted_node;
                                inserted_node -> parent = grand_parent_node;
                            } else {
                                inserted_node -> parent = NULL;
                                parent_node -> left = NULL;

                                {
                                    if(inserted_node -> right)
                                        inserted_node -> right -> parent = parent_node;
                                    parent_node -> left = inserted_node -> right;

                                    inserted_node -> right = inserted_node -> left;

                                    inserted_node -> left = uncle_node;
                                    if(uncle_node)
                                        uncle_node -> parent = inserted_node;

                                    swap_values(grand_parent_node, inserted_node);
                                }
                                
                                grand_parent_node -> left = inserted_node;
                                inserted_node -> parent = grand_parent_node;
                            }
                            continue;
                        }
                    }
                    // If we are here, we continue previously
                    assert(color_of(uncle_node) == RED);
                    assert(uncle_node -> parent == grand_parent_node);
                    assert(uncle_node);
                    parent_node -> color = BLACK;
                    uncle_node -> color = BLACK;
                    grand_parent_node -> color = RED;
                    inserted_node = grand_parent_node;
                } else break;
            } else {
                map -> root = inserted_node;
                break;
            };
        }
    }
    map -> root -> color = BLACK;
}

/**
 * -------------------------------------------------------------------------- *
 * ----------------------  Node logging and debugging  ---------------------- *
 * -------------------------------------------------------------------------- *
*/

#ifdef MAP_DEBUG

#include <math.h>
#include <stdio.h>

void map_debug(const Map map)
{
    struct NodeStruct* nodes = map -> nodes;
    const int nodes_size = map -> nodes_size;
    const int values_size = map -> values_size;
    const int capacity = map -> capacity;

    int height = 0;
    if(map -> root) {
        int depth = 1;
        struct NodeStruct* cursor = map -> root;
        for(;;) {
            if(cursor -> left) {
                depth += 1;
                cursor = cursor -> left;
            } else if(cursor -> right) {
                depth += 1;
                cursor = cursor -> right;
            } else {
                height = height < depth ? depth : height;

                for(;;) {
                    if(! cursor -> parent) {
                        goto stop;
                    } else {
                        if(cursor == cursor -> parent -> left) {
                            if(cursor -> parent -> right) {
                                cursor = cursor -> parent -> right;
                                break;
                            } else {
                                cursor = cursor -> parent;
                                depth -= 1;
                            }
                        } else {
                            assert(cursor == cursor -> parent -> right);
                            cursor = cursor -> parent;
                            depth -= 1;
                        }
                    }
                }
            }
        }
        stop: ;
    }
    printf(
        "\n** Map object:"
            "\n\tCapacity: %d"
            "\n\tSize (nodes - values): %d - %d"
            "\n\tHeight: %d"
            "\n\tUnbalance factor: %f"
            "\n\tSize of a node structure (in bytes): %d"
            "\n\tNode iteration:"
        , capacity, nodes_size, values_size, height, height / log(nodes_size),
        (int) sizeof(struct NodeStruct) + (int) sizeof(struct ValueListStruct)
    );

    for(int i = 0; i < nodes_size; i++) {
        const struct NodeStruct node = nodes[i];

        int value_count = 0; {
            struct ValueListStruct* cursor = node.value_list;
            do {
                value_count += 1;
                cursor = cursor -> tail;
            } while(cursor);
        }

        printf(
            "\n\t\tThis node: %p"
                "\n\t\t\thash code: %d"
                "\n\t\t\tcolor: %s"
                "\n\t\t\tleft node: %p"
                "\n\t\t\tright node: %p"
                "\n\t\t\tparent node: %p"
                "\n\t\t\tvalue count: %d"
        , nodes + i
        , map -> hash_function((node.value_list) -> head)
        , node.color == BLACK ? "black" : "red"
        , node.left
        , node.right
        , node.parent
        , value_count
        );
    }

    printf("\n");
}

#endif