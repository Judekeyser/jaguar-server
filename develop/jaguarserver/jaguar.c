#include <stdio.h>
#include <assert.h>

#include "./map.h"


struct Int {
    int value;
};

int hashCode(void* value) {
    struct Int* intValue = (struct Int*) value;
    return intValue -> value;
}


int main()
{
    printf("** Map demo file **\n");

    Map map;
    map_allocate(4, hashCode, &map);

    struct Int values[8] = {
        {.value = 20},
        {.value = 18},
        {.value = 27},
        {.value = 22},
        {.value = 25},
        {.value = 23},
        {.value = 21},
        {.value = 24}
    };

    for(int i = 0; i < 8; i++) {
        map_insert(map, values+i);
    }
    printf("Done insertion");
    map_debug(map);

    return 0;
}