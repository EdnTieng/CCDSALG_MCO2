#include "hash.h"
#include <stdio.h>
#include <string.h>

/**
 * Determines whether a given number is prime.
 *
 * @param num The number to check.
 * @return true if the number is prime, false otherwise.
 */
bool is_prime(int num) {
    if (num < 2) return false; // Numbers less than 2 are not prime
    int i;
    for (i = 2; i * i <= num; i++) { // Check divisors up to the square root of num
        if (num % i == 0) return false; // If divisible by i, not a prime number
    }
    return true; // No divisors found; number is prime
}

/**
 * Finds the first prime number greater than a given number.
 * 
 * This function increments the input number until it finds a prime number.
 *
 * @param num The starting number (non-prime).
 * @return The next prime number greater than the input number.
 */
int find_next_prime(int num) {
    num++; // Start checking from the next number
    while (!is_prime(num)) { // Keep checking until a prime is found
        num++;
    }
    return num; // Return the next prime number
}

/**
 * Hash function using the division method.
 * 
 * This function computes the hash of a given string by summing the ASCII values
 * of its characters and uses modulo to get it's address.
 *
 * @param str The input string to hash.
 * @param table_size The size of the hash table (modulo divisor).
 * @return An index representing the home address of the string in the hash table.
 */
unsigned int hash_function(const char *str, unsigned int table_size) {
    unsigned int hash = 0;
    int i;
    for (i = 0; str[i] != '\0'; i++) { //get all chars in the string
        hash += str[i]; // get the sum of all the ascii values 
    }
    return hash % table_size; // Return the remainder as the hash index
}

/**
 * Resolves collisions using linear probing.
 * 
 * (k + j) % m
 *
 * @param home_index The original index (home address) of the string.
 * @param attempt The number of collision resolution attempts made so far.
 * @param table_size The size of the hash table.
 * @return The next index to try for storing the string.
 */
unsigned int resolve_collision(unsigned int home_index, unsigned int attempt, unsigned int table_size) {
    return (home_index + attempt) % table_size; // Calculate next index using linear probing
}

/**
 * Searches for a key in the hash table.
 * 
 * This function uses linear probing to locate the key. 
 * If the key is found, its index is returned. If the key is not found or
 * the table is empty, the function returns -1.
 *
 * @param key The string key to search for in the hash table.
 * @param hash_table The hash table (array of strings).
 * @param table_size The size of the hash table.
 * @return The index where the key is found, or -1 if the key is not present.
 */
int search(const char *key, char *hash_table[], unsigned int table_size) {
    unsigned int home_index = hash_function(key, table_size); // Compute the home address
    unsigned int current_index = home_index;
    unsigned int attempt = 0; // Track the number of search attempts

    // Perform linear probing to search for the key
    while (hash_table[current_index] != NULL) {
        if (strcmp(hash_table[current_index], key) == 0) { // Key found
            return current_index; // Return the index of the key
        }

        attempt++; // Increment the attempt counter
        current_index = resolve_collision(home_index, attempt, table_size); // Compute the next index

        if (current_index == home_index) { // If we've circled back to the home address
            break; // Key is not present in the table
        }
    }

    return -1; // Key not found in the hash table
}

