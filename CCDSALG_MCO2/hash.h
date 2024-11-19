#ifndef HASH_H
#define HASH_H

#include <stdbool.h>

#define MAX_1D_ARRAY 16384  // Maximum size of the hash table
#define MAX_STR_LEN 15      // Maximum length of a string (excluding null terminator)

// Function to check if a number is prime
bool is_prime(int num);

// Function to find the first prime number above a given number
int find_next_prime(int num);

// Hash function to compute the home address of a string
unsigned int hash_function(const char *str, unsigned int table_size);

// Collision resolution function to resolve collisions
unsigned int resolve_collision(unsigned int home_index, unsigned int attempt, unsigned int table_size);

// Search function to find the index of a key in the hash table
int search(const char *key, char *hash_table[], unsigned int table_size);

#endif

