// Copyright 2016-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdio.h>
#include <xs1.h>
#include <dsp.h>
#include <stdlib.h>
#include <math.h>

/**
Example showing FFT processing of a configurable number of input signals received through a double buffer.
----------------------------------------------------------------------------------------------------------


This example shows how two tasks can implement a double buffering
mechanism accessing a shared memory area.

The task produce_samples fills one buffer with a stereo signal
whilst the do_fft task processes the other buffer
"in place" which means the buffer is also used to store the real part of the result
Tasks access these buffers via *movable* pointers. These pointers can
be safely transferred between tasks without any race conditions
between the tasks using them.

When the the produce_samples taks has finished filling the buffer, it calls the
swap function of the interface to synchronise with the do_fft task and swap pointers (buffers)

.. aafig::

   +---------------+                     +------------------+
   |  `do_fft`     |   `bufswap_i`       | `produce_samples`|
   |               +<------------------->|                  |
   |   +---------+ |   `interface`       |   +---------+    |
   |   |`movable`| |                     |   |`movable`|    |
   |   |`pointer`| |                     |   |`pointer`|    |
   |   +----+----+ |                     |   +----+----+    |
   |        |      |                     |        |         |
   +--------|------+                     +--------|---------+
            |             `MEMORY`                |
            |          +------+------+            |
            |          |`buf1`|`buf2`|            |
            +--------->+      |      +<-----------+
                       |      |      |
                       |      |      |
                       +------+------+
**/

/** Global configuration defines **/

#ifndef NUM_CHANS    
// Number of input channels       
#define NUM_CHANS 4  
#endif 

#ifndef N_FFT_POINTS
// FFT Points
#define N_FFT_POINTS 256
#endif

#ifndef INT16_BUFFERS
#define INT16_BUFFERS 0 // Disabled by default
#endif

#ifndef TWOREALS
#define TWOREALS 0 // Processing two real signals with a single complex FFT. Disabled by default
#endif

#define SAMPLE_FREQ 48000
#define SAMPLE_PERIOD_CYCLES XS1_TIMER_HZ/SAMPLE_FREQ

#ifndef CHECK_TIMING
#define CHECK_TIMING 0
#endif

#ifndef PRINT_INPUTS_AND_OUTPUTS
#define PRINT_INPUTS_AND_OUTPUTS 0
#endif

/****/

/** Declaration of Data Types and Memory Buffers **/

// Array holding one complex signal or two real signals
#if INT16_BUFFERS
#define SIGNAL_ARRAY_TYPE dsp_complex_short_t
#define OUTPUT_SUM_TYPE int32_t // double precision variable to avoid overflow in addition
#else
#define SIGNAL_ARRAY_TYPE dsp_complex_t
#define OUTPUT_SUM_TYPE int64_t // double precision variable to avoid overflow in addition
#endif

#if TWOREALS
#define NUM_SIGNAL_ARRAYS NUM_CHANS/2
#else
#define NUM_SIGNAL_ARRAYS NUM_CHANS
#endif

/** Union to store blocks of samples for multiple digital signals
*/
typedef union {
    SIGNAL_ARRAY_TYPE data[NUM_SIGNAL_ARRAYS][N_FFT_POINTS];  // time domain or frequency domain signals
    SIGNAL_ARRAY_TYPE half_spectra[NUM_SIGNAL_ARRAYS*2][N_FFT_POINTS/2];  // frequency domain half spectra
} multichannel_sample_block_s ;
/****/

/** Print Functions **/
void print_signal(SIGNAL_ARRAY_TYPE signal[N_FFT_POINTS]) {
#if INT16_BUFFERS
       printf("re,      im         \n");
       for(int32_t i=0; i<N_FFT_POINTS; i++) {
           //printf( "0x%x, 0x%x\n", signal[i].re, signal[i].im);
           printf( "%.5f, %.5f\n", F14(signal[i].re), F14(signal[i].im));
       }

#else
       printf("re,      im         \n");
       for(int32_t i=0; i<N_FFT_POINTS; i++) {
           printf( "%.8f, %.8f\n", F24(signal[i].re), F24(signal[i].im));
       }
#endif  
}

void print_buffer(multichannel_sample_block_s *buffer) {
    for(int32_t c=0; c<NUM_SIGNAL_ARRAYS; c++) {
        print_signal(buffer->data[c]);
    }
}
/****/


/**
 The interface between the two tasks is a single transaction that swaps
 the movable pointers. It has an argument that is a reference to a
 movable pointer. Since it is a reference the server side of the
 connection can update the argument.
*/

interface bufswap_i {
  void swap(multichannel_sample_block_s * movable &x);
};

/**
 The do_fft task takes as arguments the server end of an interface
 connection and the initial buffer it is going to process. It is
 initialized by creating a movable pointer to this buffer and then
 processing it.
*/
#if !(XCC_VERSION_MAJOR >= 1403)
static SIGNAL_ARRAY_TYPE output[N_FFT_POINTS];
#endif

void do_fft(server interface bufswap_i input,
        multichannel_sample_block_s * initial_buffer)
{
  multichannel_sample_block_s * movable buffer = initial_buffer;

  timer tmr; uint32_t start_time, end_time, overhead_time;
  tmr :> start_time;
  tmr :> end_time;
  overhead_time = end_time - start_time;

#if INT16_BUFFERS
  printf("%d Point FFT Processing of %d int16_t signals received through a double buffer\n"
                ,N_FFT_POINTS,NUM_CHANS);
#else
  printf("%d Point FFT Processing of %d int32_t signals received through a double buffer\n"
                ,N_FFT_POINTS,NUM_CHANS);
#endif

  /** The main loop of the filling task waits for a swap transaction with
      the other task and implements the swap of pointers. After that it
      fills the new buffer it has been given:
  */
  while (1) {
    // swap buffers
    select {
      case input.swap(multichannel_sample_block_s * movable &input_buf):
        // Swapping uses the 'move' operator. This operator transfers the
        // pointer to a new variable, setting the original variable to null.
        // The 'display_buffer' variable is a reference, so updating it will
        // update the pointer passed in by the other task.
        multichannel_sample_block_s * movable tmp;
        tmp = move(input_buf);
        input_buf = move(buffer);
        buffer = move(tmp);

#if PRINT_INPUTS_AND_OUTPUTS
        print_buffer(buffer);
#endif

        tmr :> start_time;

        // Do FFTs
        for(int32_t a=0; a<NUM_SIGNAL_ARRAYS; a++) {
            // process the new buffer "in place"
    #if INT16_BUFFERS
            dsp_complex_t tmp_buffer[N_FFT_POINTS];
            dsp_fft_short_to_long(buffer->data[a], tmp_buffer, N_FFT_POINTS); // convert into tmp buffer
            dsp_fft_bit_reverse(tmp_buffer, N_FFT_POINTS);
            dsp_fft_forward(tmp_buffer, N_FFT_POINTS, FFT_SINE(N_FFT_POINTS));
    #if TWOREALS
            dsp_fft_split_spectrum(tmp_buffer, N_FFT_POINTS);

    #endif
            dsp_fft_long_to_short(tmp_buffer, buffer->data[a], N_FFT_POINTS); // convert from tmp buffer
    ////// 32 bit buffers        
    #else  
            dsp_fft_bit_reverse(buffer->data[a], N_FFT_POINTS);
            dsp_fft_forward(buffer->data[a], N_FFT_POINTS, FFT_SINE(N_FFT_POINTS));
    #if TWOREALS
            dsp_fft_split_spectrum(buffer->data[a], N_FFT_POINTS);
    #endif

    #endif

        }

        // Process the frequency domain of all NUM_CHANS channels.
        // 1. Lowpass
        // cutoff frequency = (Fs/N_FFT_POINTS * cutoff_index)
        // With cutoff_index = N_FFT_POINTS/M:
        // cutoff frequency = Fs/M

        uint32_t cutoff_idx = N_FFT_POINTS/4;
        // 2. Calculate average per frequency bin into the output signal array.

        // To calculate the average over all channels.
        // To divide by NUM_CHANS, shift down throughout the loop log2(NUM_CHANS) times to avoid overflow.
        uint32_t log_num_chan = log2(NUM_CHANS); 
        uint32_t step = NUM_CHANS/log_num_chan;
        uint shift_idx = step;

#if (XCC_VERSION_MAJOR >= 1403)
        static SIGNAL_ARRAY_TYPE output[N_FFT_POINTS];
#endif

    #if TWOREALS
        for(unsigned i = 0; i < N_FFT_POINTS/2; i++) {
          OUTPUT_SUM_TYPE output_re = 0;
          OUTPUT_SUM_TYPE output_im = 0;
          for(int32_t c=0; c<NUM_CHANS; c++) {
             if(i>=cutoff_idx) {
               buffer->half_spectra[c][i].re = 0;
               buffer->half_spectra[c][i].im = 0;
             }
             output_re += buffer->half_spectra[c][i].re;
             output_im += buffer->half_spectra[c][i].im;
          }
          output[i].re = (OUTPUT_SUM_TYPE) output_re/NUM_CHANS; // average
          output[i].im = (OUTPUT_SUM_TYPE) output_im/NUM_CHANS; // average 
        }
    #else // Complex
        for(unsigned i = 0; i < N_FFT_POINTS/2; i++) {
          OUTPUT_SUM_TYPE output_re = 0, output_re_ri = 0;
          OUTPUT_SUM_TYPE output_im = 0, output_im_ri = 0;
          uint32_t ri = N_FFT_POINTS-i; // reverse index
          for(int32_t c=0; c<NUM_CHANS; c++) {
            if(i>=cutoff_idx) {
              buffer->data[c][i].re = 0;
              buffer->data[c][i].im = 0;
              if(i>0) {
                buffer->data[c][N_FFT_POINTS-i].re = 0;
                buffer->data[c][N_FFT_POINTS-i].im = 0;
              }
            }
            output_re += buffer->data[c][i].re;
            output_im += buffer->data[c][i].im;
            if(i>0) {
              output_re_ri += buffer->data[c][ri].re;
              output_im_ri += buffer->data[c][ri].im;
            }
          }

          output[i].re = (OUTPUT_SUM_TYPE) output_re/NUM_CHANS; // average
          output[i].im = (OUTPUT_SUM_TYPE) output_im/NUM_CHANS; // average 
          if(i>0) {
            output[ri].re = (OUTPUT_SUM_TYPE) output_re_ri/NUM_CHANS; // average
            output[ri].im = (OUTPUT_SUM_TYPE) output_im_ri/NUM_CHANS; // average 
          }
        }
    #endif

        // Todo: Add iFFT

        tmr :> end_time;
        int32_t cycles_taken = end_time-start_time-overhead_time;

#if CHECK_TIMING
#if TWOREALS
        printf("%d Point FFT processing of %d real sequences 'in place' in double buffer %x took %d cycles\n"
                ,N_FFT_POINTS,NUM_CHANS,buffer,cycles_taken);
#else
        printf("%d Point FFT processing of %d complex sequences 'in place' in double buffer %x took %d cycles\n"
                ,N_FFT_POINTS,NUM_CHANS,buffer,cycles_taken);                
#endif
        if(cycles_taken>SAMPLE_PERIOD_CYCLES*N_FFT_POINTS) {
            printf("Timing Check ERROR: Max allowed cycles at Fs %d Hz is %d\n",SAMPLE_FREQ,SAMPLE_PERIOD_CYCLES*N_FFT_POINTS);
        } else {
            printf("Timing Check PASS: Max allowed cycles at Fs %d Hz is %d\n",SAMPLE_FREQ,SAMPLE_PERIOD_CYCLES*N_FFT_POINTS);
        }
#endif

#if PRINT_INPUTS_AND_OUTPUTS
        print_buffer(buffer);
#endif
        printf("Processed output signal\n");
        print_signal(output);

        break;
    }
    // fill the buffer with data
  }
}

/** Utility functions for signal generation **/
int32_t scaled_sin(q8_24 x) {
   q8_24 y = dsp_math_sin(x);
#if INT16_BUFFERS
   y >>= 10; // convert to Q14
#else
#endif
   return y;
}
int32_t scaled_cos(q8_24 x) {
   q8_24 y = dsp_math_cos(x);
#if INT16_BUFFERS
   y >>= 10; // convert to Q14
#else
#endif
   return y;
}
/****/

/**
 The displaying task takes the other end of the interface connection
 and its initial buffer as arguments. It also creates a movable pointer
 to that buffer.
**/

#define MAX_SAMPLE_PERIODS 1
  
void produce_samples(client interface bufswap_i filler,
        multichannel_sample_block_s * initial_buffer) {
  multichannel_sample_block_s * movable buffer = initial_buffer;
  timer tmr;
  int32_t t;
  static int32_t counter;
  int32_t done = 0;

  tmr :> t;
  /** The main loop of the display task first calls the 'swap' transaction,
      which synchronizes with the fill task and updates the 'buffer' pointer
      to the new swapped memory location. After that it calls an auxiliary
      'display' function to do the actual displaying. This function is
      application dependent and not defined here.
   **/
  while(!done) {
    //fill the next buffer
    for(int32_t a=0; a<NUM_SIGNAL_ARRAYS; a++) {
      // points per cycle. divide by power of two to ensure signals fit into the FFT window
      int32_t ppc = N_FFT_POINTS/(1<<a); 
      printf("Points Per Cycle is %d\n", ppc);

      for(int32_t i=0; i<N_FFT_POINTS; i++) {
        // generate input signals

        // Equation: x = 2pi * i/ppc = 2pi * ((i%ppc) / ppc))
        q8_24 factor = ((i%ppc) << 24) / ppc; // factor is always < Q24(1)
        q8_24 x = dsp_math_multiply(PI2_Q8_24, factor, 24);

#if TWOREALS
        buffer->data[a][i].re = scaled_sin(x);
        buffer->data[a][i].im = scaled_cos(x);
#else
        buffer->data[a][i].re = scaled_sin(x); 
        buffer->data[a][i].im = 0;
#endif

      }

      // wait until next sample period
      tmr when timerafter(t+SAMPLE_PERIOD_CYCLES) :> t;
    }

    // swap buffers
    filler.swap(buffer);
    counter++;

    if(counter == MAX_SAMPLE_PERIODS) {
        exit(0);
    }

  }
}

/**
 The application runs both of these tasks in parallel using a 'par'
 statement. The two global buffers are passed into
 the two tasks:
**/

// make global to enforce 64 bit alignment
multichannel_sample_block_s buffer0;
multichannel_sample_block_s buffer1;

int main() {

  interface bufswap_i bufswap;
  par {
      produce_samples(bufswap, &buffer1);
      do_fft(bufswap, &buffer0);
  }
  return 0;
}

