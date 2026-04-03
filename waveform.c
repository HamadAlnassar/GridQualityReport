#include <stdio.h>
#include <math.h>
#include "waveform.h"

double compute_rms(WaveformSample* samples, int n, int phase_choice, FaultyReading* fault_log, int* fault_count) {
    double sum_sq = 0.0;
    int valid_samples = 0;


    char phase_letter;
    if (phase_choice == 1) {
        phase_letter = 'A';
        for (int i = 0; i < n; i++) {
            double v = 0;
            v = (samples + i)->phase_A_voltage;
            if (fabs(v) > CLIPPING_LIMIT) {

                (fault_log + *fault_count)->timestamp = (samples + i)->timestamp;
                (fault_log + *fault_count)->faulty_voltage = v;
                (fault_log + *fault_count)->phase = phase_letter;

                (*fault_count)++;
            }
            else {

                sum_sq += (v * v);
                valid_samples++;
            }
        }
    }
    else if (phase_choice == 2) {
        phase_letter = 'B';
        for (int i = 0; i < n; i++) {
            double v = 0;
            v = (samples + i)->phase_B_voltage;
            if (fabs(v) > CLIPPING_LIMIT) {

                (fault_log + *fault_count)->timestamp = (samples + i)->timestamp;
                (fault_log + *fault_count)->faulty_voltage = v;
                (fault_log + *fault_count)->phase = phase_letter;

                (*fault_count)++;
            }
            else {

                sum_sq += (v * v);
                valid_samples++;
            }
        }
    }
    else {
        phase_letter = 'C';

        for (int i = 0; i < n; i++) {
            double v = 0;
            v = (samples + i)->phase_C_voltage;
            if (fabs(v) > CLIPPING_LIMIT) {

                (fault_log + *fault_count)->timestamp = (samples + i)->timestamp;
                (fault_log + *fault_count)->faulty_voltage = v;
                (fault_log + *fault_count)->phase = phase_letter;

                (*fault_count)++;
            }
            else {

                sum_sq += (v * v);
                valid_samples++;
            }
        }
    }


    return sqrt(sum_sq / valid_samples);
}