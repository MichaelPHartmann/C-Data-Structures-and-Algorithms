// Simple hash table implemented in C

#ifndef _HT_H
#define _HT_H

#include <stdbool.h>
#include <stddef.h>

// Hash table structure
typedef struct hash_table hash_table;

// Create hash table and return pointer to it
hash_table* hash_table_create(void);

// Free memory allocated to hash table
void hash_table_destroy(hash_table* table);

// Get item with given key
void hash_table_get(hash_table* table, const char* key);

// Set item with given key
const char* hash_table_set(hash_table* table, const char* key, void* value);

// Number of items in hash table
size_t hash_table_length(hash_table* table);

// Hash table iterator
typedef struct {
	const char* key;
	void* value;

	// Don't use directly
	hash_table* _table;
	size_t _index;
} hash_table_iterator;

// Return new hash table iterator
hash_table_iterator hash_table_iteration(hash_table* table);

// Move iterator to next item in hash table
bool hash_table_next(hash_table_iterator* iterator);

#endif
