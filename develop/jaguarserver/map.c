#include "./map.h"
#include <assert.h>
#include <stdlib.h>


enum Color { RED, BLACK };


struct NodeStruct
{
    struct NodeStruct* left;
    struct NodeStruct* right;
    struct NodeStruct* parent;
    void* value;
    enum Color color;
};


struct MapStruct
{
    struct NodeStruct* data;
    struct NodeStruct* root;
    int capacity;
    int size;
    HashFunction hash_function;
};

static inline enum Color color_of(const struct NodeStruct* node)
    { return node ? (node -> color) : BLACK; }


static struct NodeStruct* binary_tree_insert_node(Map map, void* value)
{
    const int map_capacity = map -> capacity;

    if(map -> size == map_capacity) {
        return NULL;
    } else {
        assert(map -> size < map_capacity);

        const int hash = (map -> hash_function)(value);
        struct NodeStruct* node = (map -> data) + (map -> size);
        node -> value = value;
        node -> left = NULL;
        node -> right = NULL;
        node -> parent = NULL;
        map -> size += 1;

        if(map -> size == 1) {
            map -> root = node;
            assert(node -> left == NULL);
            assert(node -> right == NULL);
            assert(node -> parent == NULL);
        } else {
            struct NodeStruct* cursor = map -> root;
            int i;
            for(i = 0; i <= map_capacity && cursor; i++) {
                const int cursor_hash_code = (map -> hash_function)(cursor -> value);
                if(cursor_hash_code < hash) {
                    struct NodeStruct* next_cursor = cursor -> right;
                    if(next_cursor) {
                        cursor = next_cursor;
                    } else {
                        node -> parent = cursor;
                        cursor -> right = node;
                        break;
                    }
                } else if(cursor_hash_code > hash) {
                    struct NodeStruct* next_cursor = cursor -> left;
                    if(next_cursor) {
                        cursor = next_cursor;
                    } else {
                        node -> parent = cursor;
                        cursor -> left = node;
                        break;
                    }
                } else {
                    // TODO: Handle collisions
                }
            }
            assert(i < map_capacity);
        }
        assert(node -> left == NULL);
        assert(node -> right == NULL);
        return node;
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
    struct NodeStruct* data;
    struct MapStruct* map;

    map = NULL;
    size = 1 << log2_size;
    data = malloc(sizeof(*data) * size);
    if(data) {
        map = malloc(sizeof(*map));
        if(map) {
            map -> size = 0;
            map -> root = NULL;
            map -> capacity = size;
            map -> data = data;
            map -> hash_function = hash_function;
        } else {
            free(data);
        }
    }

    *target = map;
}

void map_dispose(Map map)
{
    if(map) {
        free(map -> data);
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

                                    { // Finalize by swapping values
                                        void* grand_parent_value = grand_parent_node -> value;
                                        grand_parent_node -> value = parent_node -> value;
                                        parent_node -> value = grand_parent_value;
                                    }
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

                                    {
                                        void* grand_parent_value = grand_parent_node -> value;
                                        grand_parent_node -> value = inserted_node -> value;
                                        inserted_node -> value = grand_parent_value;
                                    }
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

                                    { // Finalize by swapping values
                                        void* grand_parent_value = grand_parent_node -> value;
                                        grand_parent_node -> value = parent_node -> value;
                                        parent_node -> value = grand_parent_value;
                                    }
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

                                    {
                                        void* grand_parent_value = grand_parent_node -> value;
                                        grand_parent_node -> value = inserted_node -> value;
                                        inserted_node -> value = grand_parent_value;
                                    }
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
    struct NodeStruct* data = map -> data;
    const int size = map -> size;
    const int capacity = map -> capacity;

    int height = 0;
    /**/
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
    /**/
    printf(
        "\n** Map object:"
            "\n\tCapacity: %d"
            "\n\tSize: %d"
            "\n\tHeight: %d"
            "\n\tUnbalance factor: %f"
            "\n\tSize of a node structure (in bytes): %d"
            "\n\tNode iteration:"
        , capacity, size, height, height / log(size), (int) sizeof(struct NodeStruct)
    );

    for(int i = 0; i < size; i++) {
        const struct NodeStruct node = data[i];

        printf(
            "\n\t\tThis node: %p"
                "\n\t\t\thash code: %d"
                "\n\t\t\tcolor: %s"
                "\n\t\t\tleft node: %p"
                "\n\t\t\tright node: %p"
                "\n\t\t\tparent node: %p"
        , data + i
        , map -> hash_function(node.value)
        , node.color == BLACK ? "black" : "red"
        , node.left
        , node.right
        , node.parent
        );
    }

    printf("\n");
}

#endif