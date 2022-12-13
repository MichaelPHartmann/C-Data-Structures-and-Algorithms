#include "ht.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>

// Create the entry struct
typedef struct {
	const char* key;
	void* value;
} hash_table_entry;

// Create the hash table struct
struct hash_table {
	hash_table_entry* entries;
	size_t capacity;
	size_t length;
};

#define INITIAL_CAPACITY 16

hash_table* hash_table_create(void) {
	hash_table* table = malloc(sizeof(hash_table));
	if (table == NULL) {
		return NULL;
	}
	table->length = 0;
	table->capacity = INITIAL_CAPACITY;

	// Allocate space for entry buckets
	table->entries = calloc(table->capacity, sizeof(hash_table_entry));
	if (table->entries == NULL) {
		free(table);
		return NULL;
	}
	return table;
};

void ht_destroy(hash_table* table) {
	for (size_t i = 0; i < table->capacity; i++) {
		free((void*)table->entries[i].key);
	}
	free(table->entries);
	free(table);
}

#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

static uint64_t hash_key(const char* key) {
	uint64_t hash = FNV_OFFSET;
	for (const char* p = key; *p; p++) {
		hash ^= (uint64_t)(unsigned char)(*p);
		hash *= FNV_PRIME;
	}
	return hash;
}

void* hash_table_get(hash_table* table, const char* key) {
	uint64_t hash = hash_key(key);
	size_t index = (size_t)(hash & (uint64_t)(table->capacity - 1));
	while (table->entries[index].key != NULL) {
		if (strcmp(key, table->entries[index].key) == 0) {
			// Key found, return value
			return table->entries[index].value;
		}
		// Key not found, move to next
		index ++;
		if (index >= table->capacity) {
			index = 0;
		}
	}
	return NULL;
}

static const char* hash_table_set_entry(hash_table_entry* entries, size_t capacity, const char* key, void* value, size_t* plength) {
	uint64_t hash = hash_key(key);
	size_t index = (size_t)(hash & (uint64_t)(capacity - 1));
	while (entries[index].key != NULL) {
		if (strcmp(key, entries[index].key) == 0) {
			entries[index].value = value;
			return entries[index].key;
		}
		index++;
		if (index >= capacity) {
			index = 0;
		}
	}
	if (plength != NULL) {
		key = strdup(key);
		if (key == NULL) {
			return NULL;
		}
		(*plength)++;
	}
	entries[index].key = (char*)key;
	entries[index].value = value;
	return key;
}

static bool hash_table_expand(hash_table* table) {
	size_t new_capacity = table->capacity * 2;
	if (new_capacity < table->capacity) {
		return false;
	}
	hash_table_entry* new_entries = calloc(new_capacity, sizeof(hash_table_entry));
	if (new_entries == NULL) {
		return false;
	}
	for (size_t i = 0; i < table->capacity; i++) {
		hash_table_entry entry = table->entries[i];
		if (entry.key != NULL) {
			hash_table_set_entry(new_entries, new_capacity, entry.key, entry.value, NULL);
		}
	}
	free(table->entries);
	table->entries = new_entries;
	table->capacity = new_capacity;
	return true;
}

const char* hash_table_set(hash_table* table, const char* key, void* value) {
	assert(value != NULL);
	if (value == NULL) {
		return NULL;
	}
	if (table->length >= table->capacity / 2) {
		if (!hash_table_expand(table)) {
			return NULL;
		}
	}
	return hash_table_set_entry(table->entries, table->capacity, key, value, &table->length);
}

size_t hash_table_length(hash_table* table) {
	return table->length;
}

hash_table_iterator hash_table_iteration(hash_table* table) {
	hash_table_iterator it;
	it._table = table;
	it._index = 0;
	return it;
}

bool hash_table_next(hash_table_iterator* it) {
	hash_table* table = it->_table;
	while (it->_index < table->capacity) {
		size_t i = it->_index;
		it->_index++;
		if (table->entries[i].key != NULL) {
			hash_table_entry entry = table->entries[i];
			it->key = entry.key;
			it->value = entry.value;
			return true;
		}
	}
	return false;
}
