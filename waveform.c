#include <stdio.h>
#include <math.h>
#include "waveform.h"

/**
 * Calculates the Root Mean Square (RMS) voltage for a specific phase.
 * Identifies and logs sensor clipping faults (values >= 324.99V) to a separate struct.
 * Faulty samples are excluded from the RMS calculation to ensure data integrity.
 * * @param samples Pointer to the array of waveform data
 * @param n Total number of data rows in the dataset
 * @param phase_choice Integer representing Phase A (1), B (2), or C (3)
 * @param fault_log Pointer to the struct array where clipped samples are stored
 * @param fault_count Pointer to the integer tracking total faults found across all phases
 * @return Calculated RMS voltage for the selected phase
 */
double compute_rms(WaveformSample* samples, int n, int phase_choice, FaultyReading* fault_log, int* fault_count) {

    double sum_sq = 0.0;
    int valid_samples = 0;
    char phase_letter;

    // Process Phase A
    if (phase_choice == 1) {
        phase_letter = 'A';

        for (int i = 0; i < n; i++) {
            // Access current sample using pointer arithmetic
            double v = (samples + i)->phase_A_voltage;

            // Check if current voltage exceeds the sensor's physical clipping limit
            if (fabs(v) >= CLIPPING_LIMIT) {
                // Record the timestamp, voltage, and phase of the fault
                (fault_log + *fault_count)->timestamp = (samples + i)->timestamp;
                (fault_log + *fault_count)->faulty_voltage = v;
                (fault_log + *fault_count)->phase = phase_letter;

                // Increment the global fault counter
                (*fault_count)++;
            }
            else {
                // Accumulate the square of valid voltages for RMS calculation
                sum_sq += (v * v);
                valid_samples++;
            }
        }
    }
    // Process Phase B
    else if (phase_choice == 2) {
        phase_letter = 'B';
        for (int i = 0; i < n; i++) {
            double v = (samples + i)->phase_B_voltage;

            // Log faults for Phase B using the same threshold logic
            if (fabs(v) >= CLIPPING_LIMIT) {
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
    // Process Phase C
    else {
        phase_letter = 'C';
        for (int i = 0; i < n; i++) {
            double v = (samples + i)->phase_C_voltage;

            // Log faults for Phase C to complete the 3-phase analysis
            if (fabs(v) >= CLIPPING_LIMIT) {
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

    // Protect against division by zero and return the final RMS result
    if (valid_samples == 0) return 0.0;
    return sqrt(sum_sq / valid_samples);
}

/**
 * Determines the Peak-to-Peak voltage (Vpp) by finding the difference
 * between the maximum and minimum valid voltages recorded.
 * * @param samples Pointer to the array of waveform data
 * @param n Total number of data rows
 * @param phase_choice Integer representing the selected phase
 * @return The total voltage swing (Max - Min) excluding clipped data
 */
double compute_peak_to_peak(WaveformSample* samples, int n, int phase_choice) {
    // Initialize search boundaries outside the expected voltage range
    double max_v = -1000.0;
    double min_v = 1000.0;

    // Analyze Phase A
    if (phase_choice == 1) {
        for (int i = 0; i < n; i++) {
            WaveformSample* curr = (samples + i);
            double v = curr->phase_A_voltage;

            // Only consider samples within the valid sensor range
            if (fabs(v) < CLIPPING_LIMIT) {
                if (v > max_v) max_v = v;
                if (v < min_v) min_v = v;
            }
        }
    }
    // Analyze Phase B
    else if (phase_choice == 2) {
        for (int i = 0; i < n; i++) {
            WaveformSample* curr = (samples + i);
            double v = curr->phase_B_voltage;

            if (fabs(v) < CLIPPING_LIMIT) {
                if (v > max_v) max_v = v;
                if (v < min_v) min_v = v;
            }
        }
    }
    // Analyze Phase C
    else {
        for (int i = 0; i < n; i++) {
            WaveformSample* curr = (samples + i);
            double v = curr->phase_C_voltage;

            if (fabs(v) < CLIPPING_LIMIT) {
                if (v > max_v) max_v = v;
                if (v < min_v) min_v = v;
            }
        }
    }

    // Result is the total span from the lowest trough to the highest peak
    return (max_v - min_v);
}

/**
 * @def  Calculates the DC Offset (Average Voltage) for a selected phase.
 * @param samples Pointer to the waveform data array
 * @param n Total number of samples in the dataset
 * @param phase_choice Integer (1=A, 2=B, 3=C)
 * @return The average DC voltage level (DC Offset)
 */
double compute_dc_offset(WaveformSample* samples, int n, int phase_choice) {
    double sum_raw = 0.0;
    int valid_samples = 0;

    // Process Phase A
    if (phase_choice == 1) {
        for (int i = 0; i < n; i++) {
            double v = (samples + i)->phase_A_voltage;
            // Filter out positive clipping to match assignment requirements
            if (fabs(v) < CLIPPING_LIMIT) {
                sum_raw += v;
                valid_samples++;
            }
        }
    }
    // Process Phase B
    else if (phase_choice == 2) {
        for (int i = 0; i < n; i++) {
            double v = (samples + i)->phase_B_voltage;
            if (fabs(v) < CLIPPING_LIMIT) {
                sum_raw += v;
                valid_samples++;
            }
        }
    }
    // Process Phase C
    else {
        for (int i = 0; i < n; i++) {
            double v = (samples + i)->phase_C_voltage;
            if (fabs(v) < CLIPPING_LIMIT) {
                sum_raw += v;
                valid_samples++;
            }
        }
    }

    // Return the arithmetic mean (Sum / Count)
    if (valid_samples == 0) return 0.0;
    return (sum_raw / valid_samples);
}