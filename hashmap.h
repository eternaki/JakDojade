#include <stdio.h>
#include <stdlib.h>

typedef struct Entry {
    char* key;
    City* value;
    struct Entry* next;
} Entry;

typedef struct {
    Entry** entries;
    int size;
} HashMap;

/*Этот код представляет функцию new_hashmap, которая создает и возвращает новый объект типа HashMap* 
(указатель на структуру HashMap) с заданным размером.*/
HashMap* new_hashmap(int size) {
    HashMap* map = (HashMap*)malloc(sizeof(HashMap));
    map->entries = (Entry**)calloc(size, sizeof(Entry*));
    map->size = size;
    return map;
}

/*Этот код представляет функцию hash, которая вычисляет хеш-код для заданного ключа типа char* и заданного размера хэш-таблицы.*/
unsigned int hash(char* key, int size) {
    unsigned int hash = 0;
    for (int i = 0; i < strlen(key); i++) {
        hash = (hash * 31 + key[i]) % size;
    }
    return hash;
}

/*Этот код представляет функцию hashmap_put, которая добавляет элемент в хеш-таблицу HashMap по указанному ключу.*/
void hashmap_put(HashMap* map, char* key, City* city) {
    int index = hash(key, map->size);
    Entry* entry = map->entries[index];
    Entry* prev = NULL;
    while (entry != NULL && strcmp(entry->key, key) != 0) {
        prev = entry;
        entry = entry->next;
    }
    if (entry == NULL) {
        entry = (Entry*)malloc(sizeof(Entry));
        entry->key = key;
        entry->value = city;
        entry->next = NULL;
        if (prev == NULL) {
            map->entries[index] = entry;
        }
        else {
            prev->next = entry;
        }
    }
    else {
        entry->value = city;
    }
}

/*Этот код представляет функцию hashmap_get, которая возвращает значение, связанное с указанным ключом, из хеш-таблицы HashMap.*/
City* hashmap_get(HashMap* map, char* key) {
    int index = hash(key, map->size);
    Entry* entry = map->entries[index];
    while (entry != NULL && strcmp(entry->key, key) != 0) {
        entry = entry->next;
    }

    return entry->value;
}