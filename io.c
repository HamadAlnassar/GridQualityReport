#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "io.h"

WaveformSample* read_sample_data(const char* filename, int* n) {
    char line[256];
    int count = 0;

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Fail: File did not open %s\n", filename);
        exit(1);
    }


    fgets(line, sizeof(line), file);
    while (fgets(line, sizeof(line), file)) {
        count++;
    }

    if (count == 0) {
        printf("Fail: File is empty\n");
        fclose(file);
        exit(1);
    }

    WaveformSample* data = malloc(sizeof(WaveformSample) * count);
    if (data == NULL) {
        printf("Fail: Memory allocation failed\n");
        fclose(file);
        exit(1);
    }

    rewind(file);
    fgets(line, sizeof(line), file);

    for (int i = 0; i < count; i++) {
        if (fgets(line, sizeof(line), file)) {

            int items = sscanf(line, "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf",
                               &data[i].timestamp,
                               &data[i].phase_A_voltage,
                               &data[i].phase_B_voltage,
                               &data[i].phase_C_voltage,
                               &data[i].line_current,
                               &data[i].frequency,
                               &data[i].power_factor,
                               &data[i].thd_percent);


            if (items < 8) {
                printf("Fail: Data error at row %d. Stopping program.\n", i + 1);
                free(data);
                fclose(file);
                exit(1);
            }
        }
    }

    *n = count;
    fclose(file);
    return data;
}