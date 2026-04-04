#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "io.h"

/**
 * Opens a CSV sensor log, parses the waveform data, and loads it into a dynamic array.
 * @param filename The path to the source CSV file.
 * @param n Pointer to an integer where the total valid row count will be stored.
 * @return A pointer to the dynamically allocated array of WaveformSample structs.
 */
WaveformSample* read_sample_data(const char* filename, int* n) {
    char line[256];
    int count = 0;

    // Attempt to open the file in read-only mode
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Critical Error: Unable to open file '%s'. Verify path and permissions.\n", filename);
        exit(1);
    }

    // Skip the CSV header and count the number of data rows
    // This allows for exact dynamic memory allocation based on the file size
    if (fgets(line, sizeof(line), file)) {
        while (fgets(line, sizeof(line), file)) {
            count++;
        }
    }

    // Validate that the file contains usable data
    if (count == 0) {
        printf("Data Error: The file appears to be empty or contains only headers.\n");
        fclose(file);
        exit(1);
    }

    // Allocate heap memory for the dataset; ensure the system has sufficient RAM
    WaveformSample* data = malloc(sizeof(WaveformSample) * count);
    if (data == NULL) {
        printf("System Error: Dynamic memory allocation failed for %d samples.\n", count);
        fclose(file);
        exit(1);
    }

    // Reset the file pointer to the beginning to begin data extraction
    rewind(file);
    fgets(line, sizeof(line), file); // Discard the header row again

    for (int i = 0; i < count; i++) {
        if (fgets(line, sizeof(line), file)) {

            // Parse comma-separated values into the struct members
            int items = sscanf(line, "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf",
                               &data[i].timestamp,
                               &data[i].phase_A_voltage,
                               &data[i].phase_B_voltage,
                               &data[i].phase_C_voltage,
                               &data[i].line_current,
                               &data[i].frequency,
                               &data[i].power_factor,
                               &data[i].thd_percent);

            // Verify that all 8 columns were successfully parsed
            if (items < 8) {
                printf("Parsing Error: Inconsistent data format detected at row %d.\n", i + 1);
                free(data); // Clean up memory before exiting
                fclose(file);
                exit(1);
            }
        }
    }

    // Assign the total count to the pointer and release the file handle
    *n = count;
    fclose(file);
    return data;
}


/**
 * Writes the final formatted report string to a physical text file.
 * * @param filename The name of the file to be created (e.g., "results.txt")
 * @param report_content The large buffer containing all processed data
 */
void write_report(const char* filename, const char* report_content) {
    // Open the file in "write" mode
    FILE* file = fopen(filename, "w");

    // Check if the file system allowed the creation of the file
    if (file == NULL) {
        printf("Critical Error: Could not create the report file '%s'.\n", filename);
        return;
    }

    // Write the entire string buffer to the file at once
    fprintf(file, "%s", report_content);

    // Close the file handle to ensure data is flushed to the disk
    fclose(file);

    printf("\nSuccess: Report has been saved to '%s'.\n", filename);
}
