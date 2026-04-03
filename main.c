#include <stdio.h>
#include <stdlib.h>
#include "waveform.h"
#include "io.h"

int main() {
    char filename[100];
    int row_count = 0;
    int total_faults = 0;

    printf("Enter sensor log file name to analysis: ");
    scanf("%s", filename);

    WaveformSample* my_data = read_sample_data(filename, &row_count);

    if (my_data != NULL) {

        FaultyReading* faults = malloc(sizeof(FaultyReading) * 100);

        double rmsA = compute_rms(my_data, row_count, 1, faults, &total_faults);
        double rmsB = compute_rms(my_data, row_count, 2, faults, &total_faults);
        double rmsC = compute_rms(my_data, row_count, 3, faults, &total_faults);


        printf("\nSuccessfully processed %d rows.\n", row_count);
        printf("Total Faulty Readings (Clipped) Found: %d\n", total_faults);

        printf("RMS Phase A: %.2f V\n", rmsA);
        printf("RMS Phase B: %.2f V\n", rmsB);
        printf("RMS Phase C: %.2f V\n", rmsC);

        free(my_data);
        free(faults);
    }

    return 0;
}