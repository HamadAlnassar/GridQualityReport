#ifndef GRIDQUALIYREPORT_WAVEFORM_H
#define GRIDQUALIYREPORT_WAVEFORM_H

#include <stdint.h>
#include <math.h>


#define CLIPPING_LIMIT 324.99


typedef struct {
    double timestamp;
    double phase_A_voltage;
    double phase_B_voltage;
    double phase_C_voltage;
    double line_current;
    double frequency;
    double power_factor;
    double thd_percent;
} WaveformSample;


typedef struct {
    double timestamp;
    double faulty_voltage;
    char phase;
} FaultyReading;


double compute_rms(WaveformSample* samples, int n, int phase_choice , FaultyReading* fault_log , int* fault_count);

#endif