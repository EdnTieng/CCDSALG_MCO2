#include "hash.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Reads strings from a file. The first line contains the total number of strings (n).
 * 
 * @param filename The name of the input file.
 * @param strings Array to store the strings read from the file.
 * @return The number of strings in the file
 */
int read_input_file(const char *filename, char strings[][MAX_STR_LEN + 1]) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return -1;
    }

    // Read the total number of strings from the first line
    int count;
    fscanf(file, "%d", &count);

    // Read the strings into the array
    int read_strings = 0;
    char buffer[MAX_STR_LEN + 1];
    while (read_strings < count && fscanf(file, " %15s", buffer) == 1) {
        strncpy(strings[read_strings], buffer, MAX_STR_LEN);
        strings[read_strings][MAX_STR_LEN] = '\0'; // Ensure null termination
        read_strings++;
    }

    fclose(file);

    return count; // Return the total number of strings read
}

/**
 * Writes the hash table statistics and data to an output file.
 * 
 *  Writes the contents of the hash table into an output file 
 * 
 * 
 * @param filename The name of the output file to write.
 * @param total_strings The total number of strings read from the input file.
 * @param stored_strings The number of unique strings successfully stored in the hash table.
 * @param home_addresses The number of keys stored in their home addresses (no collision).
 * @param collisions The number of keys not stored in their home addresses (collision occurred).
 * @param avg_comparisons The average number of string comparisons during the search operation.
 * @param hash_table The hash table containing the stored strings.
 * @param home_indices Array storing the home addresses of the strings in the hash table.
 * @param comparisons Array storing the number of string comparisons for each string.
 * @param hash_table_size The size of the hash table.
 */
void write_output_file(
    const char *filename,
    int total_strings,
    int stored_strings,
    int home_addresses,
    int collisions,
    double avg_comparisons,
    char *hash_table[],
    unsigned int *home_indices,
    unsigned int *comparisons,
    int hash_table_size) {
    
    char output_string[MAX_STR_LEN + 1];
    
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Error writing file");
        return;
    }
	
    // Write statistics
    fprintf(file, "%d\n", total_strings);          // Total strings read
    fprintf(file, "%d\n", stored_strings);         // Strings stored in the hash table
    fprintf(file, "%d\n", home_addresses);         // Keys in home addresses
    fprintf(file, "%d\n", collisions);             // Keys not in home addresses
    fprintf(file, "%.6f\n", avg_comparisons);      // Average string comparisons

    // Write hash table data
    int i;
    for (i = 0; i < hash_table_size; i++) {
        if (hash_table[i] != NULL) {
            fprintf(file, "%d %s %d %s %d\n",
                    i,                               // Index
                    hash_table[i],                   // Stored string
                    home_indices[i],                 // Home address
                    (i == home_indices[i]) ? "YES" : "NO", // Collision status
                    comparisons[i]);                 // String comparisons
        } else{
            fprintf(file, "%d --- --- --- ---\n", i);
        }
    }

    fclose(file);
}

int main() {
    char input_filename[100], output_filename[100];
    char strings[MAX_1D_ARRAY][MAX_STR_LEN + 1]; 
    int i;

    // Prompt user for input and output file names
    printf("Enter the input file name: ");
    scanf("%s", input_filename);
    printf("Enter the output file name: ");
    scanf("%s", output_filename);

    // Read strings from the file
    int total_strings = read_input_file(input_filename, strings);
    if (total_strings == -1) {
        printf("Failed to read input file. Exiting.\n");
        return 0;
    }

    // Determine hash table size and initialize
    int hash_table_size = total_strings * 1.1;
    hash_table_size = find_next_prime(hash_table_size);
    char *hash_table[hash_table_size];
    unsigned int home_indices[hash_table_size]; // Home address of each key
    unsigned int comparisons[hash_table_size];  // Number of comparisons for each key

    // Initialize the hash table and metadata
    for (i = 0; i < hash_table_size; i++) {
        hash_table[i] = NULL;
        home_indices[i] = 0;
        comparisons[i] = 0;
    }

    int stored_strings = 0, home_addresses = 0, collisions = 0;
    unsigned int total_comparisons = 0;

    // Insert strings into the hash table
    for (i = 0; i < total_strings; i++) {
        // Check if the string is already in the hash table
        if (search(strings[i], hash_table, hash_table_size) != -1) {
            continue; // String already exists, skip it
        }

        unsigned int home_index = hash_function(strings[i], hash_table_size);
        unsigned int current_index = home_index;
        unsigned int attempt = 0;

        // Handle collision resolution
        while (hash_table[current_index] != NULL) {
            attempt++;
            current_index = resolve_collision(home_index, attempt, hash_table_size);
        }

        // Insert the string into the hash table
        hash_table[current_index] = strdup(strings[i]);
        home_indices[current_index] = home_index;
        comparisons[current_index] = attempt + 1;
        total_comparisons += comparisons[current_index];

        // Update statistics
        stored_strings++;
        if (current_index == home_index) {
            home_addresses++;
        } else {
            collisions++;
        }
    }

    // Calculate average comparisons
    double avg_comparisons = (double)total_comparisons / stored_strings;

    // Write output to file
    write_output_file(output_filename, total_strings, stored_strings, home_addresses, collisions, avg_comparisons, hash_table, home_indices, comparisons, hash_table_size);

    printf("Processed hash table written to %s\n", output_filename);

    return 0;
}

