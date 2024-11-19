#include "hash.h"
#include <stdio.h>
#include <string.h>

// Function to check if a number is prime
bool is_prime(int num) {
    if (num < 2) return false;
    int i;
    for (i = 2; i * i <= num; i++) {
        if (num % i == 0) return false;
    }
    return true;
}

// Function to find the first prime number above a given number
int find_next_prime(int num) {
	num++;
    while (!is_prime(num)) {
        num++;
    }
    return num;
}

/**
 * Hash function using the division method.
 * Computes the sum of ASCII values of characters in the string, modulo the table size.
 *
 * @param str The input string to hash.
 * @param table_size The size of the hash table (modulo divisor).
 * @return An index representing the home address of the string.
 */
unsigned int hash_function(const char *str, unsigned int table_size) {
    unsigned int hash = 0;
	int i;
    for (i = 0; str[i] != '\0'; i++) {
        hash += str[i];  // Sum of ASCII values
    }

    return hash % table_size;
}

/**
 * Collision resolution function using linear probing.
 *
 * @param home_index The original index (home address) of the string.
 * @param attempt The attempt number for resolving the collision.
 * @param table_size The size of the hash table.
 * @return The next index to try for storing the string.
 */
unsigned int resolve_collision(unsigned int home_index, unsigned int attempt, unsigned int table_size) {
    return (home_index + attempt) % table_size;
}

/**
 * Search function to find a key in the hash table.
 *
 * @param key The key to search for.
 * @param hash_table The hash table (array of strings).
 * @param table_size The size of the hash table.
 * @return The index where the key is found, or -1 if not found.
 */
int search(const char *key, char *hash_table[], unsigned int table_size) {
    unsigned int home_index = hash_function(key, table_size);
    unsigned int current_index = home_index;
    unsigned int attempt = 0;

    // Linear probing search
    while (hash_table[current_index] != NULL) {
        if (strcmp(hash_table[current_index], key) == 0) {
            return current_index;  // Key found
        }

        attempt++;
        current_index = resolve_collision(home_index, attempt, table_size);

        if (current_index == home_index) {
            // Back to the start; table is full or key not found
            break;
        }
    }

    return -1;  // Key not found
}

