#include "level_hashing.h"
#include "my_include/my_include.h"

/*  Test:
    This is a simple test example to test the creation, insertion, search, deletion, update in Level hashing
*/
int main(int argc, char* argv[])                        
{
    int level_size = atoi(argv[1]);                     // INPUT: the number of addressable buckets is 2^level_size
    int insert_num = atoi(argv[2]);                     // INPUT: the number of items to be inserted

    level_hash *level = level_init(level_size);
    uint64_t inserted = 0, i = 0, j = 0, it = 0;
    uint8_t key[KEY_LEN];
    uint8_t value[VALUE_LEN];
    
    // save buckets
    

    for (i = 0; i < level->addr_capacity; i++) {
        crucial_data(&(level->buckets[0] + i)->token, "uint8_t", ASSOC_NUM);
        for (j = 0; j < ASSOC_NUM; j++) {
            crucial_data(&(level->buckets[0] + i)->slot[j].key, "uint8_t", KEY_LEN);
            crucial_data(&(level->buckets[0] + i)->slot[j].value, "uint8_t", VALUE_LEN);
        }
    }
  
    for (i = 0; i < level->total_capacity - level->addr_capacity; i++) {
        crucial_data(&(level->buckets[1] + i)->token, "uint8_t", ASSOC_NUM);
        for (j = 0; j < ASSOC_NUM; j++) {
            crucial_data(&(level->buckets[1] + i)->slot[j].key, "uint8_t", KEY_LEN);
            crucial_data(&(level->buckets[1] + i)->slot[j].value, "uint8_t", VALUE_LEN);
        }
    }
    

    crucial_data(&level->level_item_num[0], "uint64_t", 1);
    crucial_data(&level->level_item_num[1], "uint64_t", 1);
    crucial_data(&level->addr_capacity, "uint64_t", 1);
    crucial_data(&level->total_capacity, "uint64_t", 1);
    crucial_data(&level->level_size, "uint64_t", 1);
    crucial_data(&level->level_expand_time, "uint8_t", 1);
    crucial_data(&level->resize_state, "uint8_t", 1);
    crucial_data(&level->f_seed, "uint64_t", 1);
    crucial_data(&level->s_seed, "uint64_t", 1);
    consistent_data(&it, "uint64_t", 1);

    flush_whole_cache();
    start_crash();
    for (it = 1; it < insert_num + 1; it ++)
    {
        snprintf(key, KEY_LEN, "%ld", it);
        snprintf(value, VALUE_LEN, "%ld", it);
        puts(key);
        puts(value);
        if (!level_insert(level, key, value))                               
        {
            inserted ++;
        } else
        {
            printf("Expanding: space utilization & total entries: %f  %ld\n", \
                (float)(level->level_item_num[0]+level->level_item_num[1])/(level->total_capacity*ASSOC_NUM), \
                level->total_capacity*ASSOC_NUM);
            level_expand(level);
            level_insert(level, key, value);
            inserted ++;
        }
        clwb(&it);
    }
    end_crash();   
    printf("%ld items are inserted\n", inserted);

    printf("The static search test begins ...\n");
    for (i = 1; i < insert_num + 1; i ++)
    {
        snprintf(key, KEY_LEN, "%ld", i);
        uint8_t* get_value = level_static_query(level, key);
        if(get_value == NULL)
            printf("Search the key %s: ERROR! \n", key);
   }

    printf("The dynamic search test begins ...\n");
    for (i = 1; i < insert_num + 1; i ++)
    {
        snprintf(key, KEY_LEN, "%ld", i);
        uint8_t* get_value = level_dynamic_query(level, key);
        if(get_value == NULL)
            printf("Search the key %s: ERROR! \n", key);
   }

    printf("The update test begins ...\n");
    for (i = 1; i < insert_num + 1; i ++)
    {
        snprintf(key, KEY_LEN, "%ld", i);
        snprintf(value, VALUE_LEN, "%ld", i*2);
        if(level_update(level, key, value))
            printf("Update the value of the key %s: ERROR! \n", key);
   }

    printf("The deletion test begins ...\n");
    for (i = 1; i < insert_num + 1; i ++)
    {
        snprintf(key, KEY_LEN, "%ld", i);
        if(level_delete(level, key))
            printf("Delete the key %s: ERROR! \n", key);
   }

    printf("The number of items stored in the level hash table: %ld\n", level->level_item_num[0]+level->level_item_num[1]);    
    level_destroy(level);

    return 0;
}
