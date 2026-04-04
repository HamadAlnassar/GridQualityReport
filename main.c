#include <stdio.h>
#include <stdlib.h>
#include "waveform.h"
#include "io.h"

int main() {

    // Variable declarations for file handling and dataset tracking
    char filename[100];
    int row_count = 0;
    int total_faults = 0;

    // Prompt user for the source CSV path (e.g., "waveform_data.csv")
    printf("Enter sensor log file for analysis: ");
    scanf("%s", filename);

    // Call the I/O module to load the waveform data into a dynamic array
    WaveformSample* my_data = read_sample_data(filename, &row_count);

    // Proceed only if the file was opened and parsed successfully
    if (my_data != NULL) {

        // Allocate memory for the fault log (Buffer for up to 100 clipped samples)
        FaultyReading* faults = malloc(sizeof(FaultyReading) * 100);

        // Calculate Root Mean Square (RMS) for each phase
        double rmsA = compute_rms(my_data, row_count, 1, faults, &total_faults);
        double rmsB = compute_rms(my_data, row_count, 2, faults, &total_faults);
        double rmsC = compute_rms(my_data, row_count, 3, faults, &total_faults);

        // --- DATA SUMMARY REPORT
        printf("Successfully processed %d data points.\n", row_count);
        printf("Total Clipped Samples Identified: %d\n", total_faults);
        printf("------------------------------------------\n");

        // Display RMS results for each phase
        printf("RMS Phase A: %10.2f V\n", rmsA);
        printf("RMS Phase B: %10.2f V\n", rmsB);
        printf("RMS Phase C: %10.2f V\n", rmsC);
        printf("------------------------------------------\n");

        // Calculate Peak-to-Peak (Vpp) values for the dataset
        double vppA = compute_peak_to_peak(my_data, row_count, 1);
        double vppB = compute_peak_to_peak(my_data, row_count, 2);
        double vppC = compute_peak_to_peak(my_data, row_count, 3);

        // Display Peak-to-Peak voltage swings
        printf("Vpp Phase A: %10.2f V\n", vppA);
        printf("Vpp Phase B: %10.2f V\n", vppB);
        printf("Vpp Phase C: %10.2f V\n", vppC);
        printf("------------------------------------------\n");


        // Calculate DC offset for give dataset
        double dc_offset_a = compute_dc_offset(my_data, row_count, 1);
        double dc_offset_b = compute_dc_offset(my_data, row_count, 1);
        double dc_offset_c = compute_dc_offset(my_data, row_count, 1);

        // Display DC Offset for every phase
        printf("DC Offset Phase A: %10.2f V\n", dc_offset_a);
        printf("DC Offset Phase B: %10.2f V\n", dc_offset_b);
        printf("DC Offset Phase C: %10.2f V\n", dc_offset_c);
        printf("------------------------------------------\n");


        // Calculate compliance status
        int statusA = check_tolerance(rmsA);
        int statusB = check_tolerance(rmsB);
        int statusC = check_tolerance(rmsC);


        printf("\n--- TOLERANCE COMPLIANCE ---\n");

        // Check Phase A
        if (statusA == 1) {
            printf("Phase A: PASS [Within Limits]\n");
        } else {
            printf("Phase A: FAIL [Out of Tolerance]\n");
        }

        // Check Phase B
        if (statusB == 1) {
            printf("Phase B: PASS [Within Limits]\n");
        } else {
            printf("Phase B: FAIL [Out of Tolerance]\n");
        }

        // Check Phase C
        if (statusC == 1) {
            printf("Phase C: PASS [Within Limits]\n");
        } else {
            printf("Phase C: FAIL [Out of Tolerance]\n");
        }

        // Free dynamically allocated memory to prevent memory leaks
        free(my_data);
        free(faults);
    } else {
        printf("Error: Could not process file data.\n");
    }

    return 0;
}