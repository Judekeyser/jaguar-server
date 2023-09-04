#ifndef __MAP__
#define __MAP__

typedef struct MapStruct* Map;

typedef int (*HashFunction)(void*);

/**
 * -------------------------------------------------------------------------- *
 * --------------------------  Memory management   -------------------------- *
 * -------------------------------------------------------------------------- *
*/

void map_allocate(
    const int log2_size,
    const HashFunction hash_function,
    Map* target
);

void map_dispose(Map map);

/**
 * -------------------------------------------------------------------------- *
 * ---------------------  Node insertion and deletion   --------------------- *
 * -------------------------------------------------------------------------- *
*/

void map_insert(Map map, void* value);

/**
 * -------------------------------------------------------------------------- *
 * ----------------------  Node logging and debugging  ---------------------- *
 * -------------------------------------------------------------------------- *
*/

#ifdef MAP_DEBUG

void map_debug(const Map map);

#endif

#endif