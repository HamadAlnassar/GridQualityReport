#include <stdio.h>
#include <stdlib.h>
#include "waveform.h"
#include "io.h"

int main() {

    // Variable declarations for file handling and dataset tracking
    char filename[100];
    int row_count = 0;
    int total_faults = 0;

    // Prompt user for the source CSV path
    printf("Enter sensor log file for analysis: ");
    scanf("%s", filename);

    // Call the I/O module to load the waveform data into a dynamic array
    WaveformSample* my_data = read_sample_data(filename, &row_count);

    // Proceed only if the file was opened and parsed successfully
    if (my_data != NULL) {

        // Allocate memory for the fault log
        FaultyReading* faults = malloc(sizeof(FaultyReading) * 100);

        // Allocate a large buffer for the final report string
        char report_content[8192];
        int offset = 0;

        // Calculate Root Mean Square (RMS) for each phase
        double rmsA = compute_rms(my_data, row_count, 1, faults, &total_faults);
        double rmsB = compute_rms(my_data, row_count, 2, faults, &total_faults);
        double rmsC = compute_rms(my_data, row_count, 3, faults, &total_faults);

        // Calculate compliance status
        int statusA = check_tolerance(rmsA);
        int statusB = check_tolerance(rmsB);
        int statusC = check_tolerance(rmsC);

        // Calculate Peak-to-Peak (Vpp) values for the dataset
        double vppA = compute_peak_to_peak(my_data, row_count, 1);
        double vppB = compute_peak_to_peak(my_data, row_count, 2);
        double vppC = compute_peak_to_peak(my_data, row_count, 3);

        // 4. DC Offset (Fixed the phase_choice error)
        double dcA = compute_dc_offset(my_data, row_count, 1);
        double dcB = compute_dc_offset(my_data, row_count, 2);
        double dcC = compute_dc_offset(my_data, row_count, 3);

        // 1. Number of data points in file
        offset += sprintf(report_content + offset, "--- POWER QUALITY REPORT ---\n");
        offset += sprintf(report_content + offset, "Number of data rows processed: %d\n\n", row_count);

        // 2. RMS and Tolerance Status
        offset += sprintf(report_content + offset, "--- RMS VOLTAGE & COMPLIANCE ---\n");
        offset += sprintf(report_content + offset, "Phase A: %10.2f V | %s\n", rmsA, (statusA == 1 ? "PASS" : "FAIL"));
        offset += sprintf(report_content + offset, "Phase B: %10.2f V | %s\n", rmsB, (statusB == 1 ? "PASS" : "FAIL"));
        offset += sprintf(report_content + offset, "Phase C: %10.2f V | %s\n\n", rmsC, (statusC == 1 ? "PASS" : "FAIL"));

        // 3. Peak to Peak
        offset += sprintf(report_content + offset, "--- PEAK-TO-PEAK VOLTAGES ---\n");
        offset += sprintf(report_content + offset, "Phase A: %10.2f V\n", vppA);
        offset += sprintf(report_content + offset, "Phase B: %10.2f V\n", vppB);
        offset += sprintf(report_content + offset, "Phase C: %10.2f V\n\n", vppC);


        offset += sprintf(report_content + offset, "--- DC OFFSETS ---\n");
        offset += sprintf(report_content + offset, "Phase A: %10.2f V\n", dcA);
        offset += sprintf(report_content + offset, "Phase B: %10.2f V\n", dcB);
        offset += sprintf(report_content + offset, "Phase C: %10.2f V\n\n", dcC);

        // 5. Fault Summary and Locations
        offset += sprintf(report_content + offset, "--- CLIPPING FAULT LOG ---\n");
        offset += sprintf(report_content + offset, "Total Clipped Samples Identified (>324.99V): %d\n\n", total_faults);
        offset += sprintf(report_content + offset, "%-12s | %-12s | %-5s\n", "Timestamp", "Voltage", "Phase");
        offset += sprintf(report_content + offset, "-------------------------------------\n");

        for (int i = 0; i < total_faults; i++) {
            offset += sprintf(report_content + offset, "%-12.4f | %-12.2f | %-5c\n",
                      (faults + i)->timestamp,
                      (faults + i)->faulty_voltage,
                      (faults + i)->phase);
        }

        offset += sprintf(report_content + offset, "\n--- END OF REPORT ---\n");

        
        // Free dynamically allocated memory to prevent memory leaks
        free(my_data);
        free(faults);

    } else {
        printf("Error: Could not process file data.\n");
    }

    return 0;
}