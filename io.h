
#ifndef GRIDQUALIYREPORT_IO_H
#define GRIDQUALIYREPORT_IO_H

#include "waveform.h"

WaveformSample* read_sample_data(const char* filename, int* n);
void write_report(const char* filename, const char* report_content);

#endif
