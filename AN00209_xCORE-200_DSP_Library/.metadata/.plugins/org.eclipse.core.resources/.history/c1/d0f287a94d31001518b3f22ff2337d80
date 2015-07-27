// XMOS DSP Library - Filtering Functions Test Program
// Uses Q24 format

// Include files
#include <stdio.h>
#include <xs1.h>
#include <xmos_dsp_elements.h>

// Define constants

#define Q_M               8
#define Q_N               24

#define FIR_FILTER_LENGTH     30
#define IIR_STATE_LENGTH      4
#define IIR_CASCADE_DEPTH     3
#define SAMPLE_LENGTH         50

// Declare global variables and arrays
int  Src[] = { Q24(.11), Q24(.12), Q24(.13), Q24(.14), Q24(.15), Q24(.16), Q24(.17), Q24(.18), Q24(.19), Q24(.20),
               Q24(.21), Q24(.22), Q24(.23), Q24(.24), Q24(.25), Q24(.26), Q24(.27), Q24(.28), Q24(.29), Q24(.30),
               Q24(.31), Q24(.32), Q24(.33), Q24(.34), Q24(.35), Q24(.36), Q24(.37), Q24(.38), Q24(.39), Q24(.40),
               Q24(.41), Q24(.42), Q24(.43), Q24(.44), Q24(.45), Q24(.46), Q24(.47), Q24(.48), Q24(.49), Q24(.50),
               Q24(.51), Q24(.52), Q24(.53), Q24(.54), Q24(.55), Q24(.56), Q24(.57), Q24(.58), Q24(.59), Q24(.60)};
int           Dst[SAMPLE_LENGTH];

int firCoeffs[] = { Q24(.11), Q24(.12), Q24(.13), Q24(.14), Q24(.15), Q24(.16), Q24(.17), Q24(.18), Q24(.19), Q24(.20),
                    Q24(.21), Q24(.22), Q24(.23), Q24(.24), Q24(.25), Q24(.26), Q24(.27), Q24(.28), Q24(.29), Q24(.30),
                    Q24(.31), Q24(.32), Q24(.33), Q24(.34), Q24(.35), Q24(.36), Q24(.37), Q24(.38), Q24(.39), Q24(.40)};

int iirCoeffs[] = { Q24(.11), Q24(.12), Q24(.13), Q24(.14), Q24(.15),
                    Q24(.21), Q24(.22), Q24(.23), Q24(.24), Q24(.25),
                    Q24(.31), Q24(.32), Q24(.33), Q24(.34), Q24(.35)};

int filterState[FIR_FILTER_LENGTH];


int main(void)
{
  int i;

                 // Initiaize FIR filter state array
  for (i = 0; i < FIR_FILTER_LENGTH; i++)
  {
    filterState[i] = 0;
  }

                // Apply FIR filter and store filtered data
  for (i = 0; i < SAMPLE_LENGTH; i++)
  {
    Dst[i] =
      xmos_dsp_filters_fir (Src[i],                 // Input data sample to be filtered
                            firCoeffs,              // Pointer to filter coefficients
                            filterState,            // Pointer to filter state array
                            FIR_FILTER_LENGTH,      // Filter length
                            Q_N);                   // Q Format N
  }

  printf ("FIR Filter Results\n");
  for (i = 0; i < SAMPLE_LENGTH; i++)
  {
      printf ("Dst[%d] = %lf\n", i, F24 (Dst[i]));
  }

                 // Initiaize IIR filter state array
  for (i = 0; i < IIR_STATE_LENGTH; i++)
  {
    filterState[i] = 0;
  }

                // Apply IIR filter and store filtered data
  for (i = 0; i < SAMPLE_LENGTH; i++)
  {
    Dst[i] =
      xmos_dsp_filters_biquad (Src[i],              // Input data sample to be filtered
                               iirCoeffs,           // Pointer to filter coefficients
                               filterState,         // Pointer to filter state array
                               Q_N);                // Q Format N
  }

  printf ("IIR Biquad Filter Results\n");
  for (i = 0; i < SAMPLE_LENGTH; i++)
  {
      printf ("Dst[%d] = %lf\n", i, F24 (Dst[i]));
  }


                 // Initiaize IIR filter state array
  for (i = 0; i < (IIR_CASCADE_DEPTH * IIR_STATE_LENGTH); i++)
  {
    filterState[i] = 0;
  }

                // Apply IIR filter and store filtered data
  for (i = 0; i < SAMPLE_LENGTH; i++)
  {
    Dst[i] =
      xmos_dsp_filters_biquads (Src[i],             // Input data sample to be filtered
                                iirCoeffs,          // Pointer to filter coefficients
                                filterState,        // Pointer to filter state array
                                IIR_CASCADE_DEPTH,  // Number of cascaded sections
                                Q_N);               // Q Format N
  }

  printf ("Cascaded IIR Biquad Filter Results\n");
  for (i = 0; i < SAMPLE_LENGTH; i++)
  {
      printf ("Dst[%d] = %lf\n", i, F24 (Dst[i]));
  }


  return (0);
}

