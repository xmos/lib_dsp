// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include <stdio.h>
#include <xs1.h>
#include <lib_dsp.h>



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

/** Global configuration defines for FFT input data, number of channels and FFT points
 */

// Choose Complex or Real Input Data
#define COMPLEX_FFT
// Number of channels
#define NUM_CHANS 6
// FFT Points
#define N_FFT_POINTS 4096

/** Activate timing check */
#define CHECK_TIMING

/****/

#ifdef COMPLEX_FFT
typedef struct {
    lib_dsp_fft_complex_short_t sample_blocks[NUM_CHANS][N_FFT_POINTS];
} multichannel_sample_block_s ;

#else
// SHORT_INT_TWOREALS_FFT
typedef struct {
    lib_dsp_fft_complex_short_t two_re[N_FFT_POINTS];
    lib_dsp_fft_complex_short_t two_im[N_FFT_POINTS];
} two_channel_sample_block_s ;
typedef struct {
    two_channel_sample_block_s two_channels[NUM_CHANS/2];
} multichannel_sample_block_s ;

  #if N_FFT_POINTS*NUM_CHANS > 16384
  //#error "Max N_FFT_POINTS*NUM_CHANS is 16384 for COMPLEX_FFT"
  #endif
#endif

#define SAMPLE_FREQ 48000
#define SAMPLE_PERIOD_CYCLES XS1_TIMER_HZ/SAMPLE_FREQ

/** Data structure to store blocks of samples for multiple digital signals
*/

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

void do_fft(server interface bufswap_i input,
        multichannel_sample_block_s * initial_buffer)
{
  multichannel_sample_block_s * movable buffer = initial_buffer;

  timer tmr; unsigned start_time, end_time, overhead_time;
  tmr :> start_time;
  tmr :> end_time;
  overhead_time = end_time - start_time;

#ifndef COMPLEX_FFT
  // the two real vectors are in the input buffer!
  // only the imaginary parts have to be declared locally
  int imag_result[NUM_CHANS][N_FFT_POINTS];
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

        tmr :> start_time;
        // process the new buffer "in place"
#ifdef COMPLEX_FFT
        for(int i=0; i<NUM_CHANS; i++) {
           lib_dsp_fft_bit_reverse_short( buffer->sample_blocks[i], N_FFT_POINTS );
           lib_dsp_fft_forward_complex_short(buffer->sample_blocks[i], N_FFT_POINTS, FFT_SINE_SHORT(N_FFT_POINTS) );
        }
#else
        for(int i=0; i<NUM_CHANS/2; i++) {
           // Process two real channels per call
           //is executed in lib_dsp_fft_forward_tworeals_short: lib_dsp_fft_bit_reverse_short(two_re, N);
           //Todo: Remove bit reversing from lib_dsp_fft_forward_tworeals_short for a consistent use model of complex and tworeals
           lib_dsp_fft_forward_tworeals_short(buffer->two_channels[i].two_re, buffer->two_channels[i].two_im, N_FFT_POINTS, FFT_SINE_SHORT(N_FFT_POINTS) );
        }
#endif
        tmr :> end_time;

        int cycles_taken = end_time-start_time-overhead_time;

#ifdef CHECK_TIMING
#ifdef COMPLEX_FFT
        printf("%d Point FFT processing of %d complex sequences 'in place' in double buffer %x took %d cycles\n"
                ,N_FFT_POINTS,NUM_CHANS,buffer,cycles_taken);
#else
        printf("%d Point FFT processing of %d tworeals sequences 'in place' in double buffer %x took %d cycles\n"
                ,N_FFT_POINTS,NUM_CHANS/2,buffer,cycles_taken);
#endif
        if(cycles_taken>SAMPLE_PERIOD_CYCLES*N_FFT_POINTS) {
            printf("Timing Check ERROR: Max allowed cycles at Fs %d Hz is %d\n",SAMPLE_FREQ,SAMPLE_PERIOD_CYCLES*N_FFT_POINTS);
        } else {
            printf("Timing Check PASS: Max allowed cycles at Fs %d Hz is %d\n",SAMPLE_FREQ,SAMPLE_PERIOD_CYCLES*N_FFT_POINTS);
        }
#endif


        break;
    }
    // fill the buffer with data
  }
}

/**
 The displaying task takes the other end of the interface connection
 and its initial buffer as arguments. It also creates a movable pointer
 to that buffer.
**/


  
void produce_samples(client interface bufswap_i filler,
        multichannel_sample_block_s * initial_buffer) {
  multichannel_sample_block_s * movable buffer = initial_buffer;
  timer tmr;
  int t;
  static int counter;

  tmr :> t;
  /** The main loop of the display task first calls the 'swap' transaction,
      which synchronizes with the fill task and updates the 'buffer' pointer
      to the new swapped memory location. After that it calls an auxiliary
      'display' function to do the actual displaying. This function is
      application dependent and not defined here.
   **/
  while (1) {
    //fill the next buffer

    for(int i=0; i<N_FFT_POINTS; i++) {
#ifdef COMPLEX_FFT
      for(int c=0; c<NUM_CHANS; c++) {
        buffer->sample_blocks[c][i].re = i; // left channel
        buffer->sample_blocks[c][i].im = 0;
        counter++;
      }
#else
      for(int c=0; c<NUM_CHANS/2; c++) {
         buffer->two_channels[c].two_re[i].re = i;
         buffer->two_channels[c].two_re[i].im = i;
         buffer->two_channels[c].two_im[i].re = 0;
         buffer->two_channels[c].two_im[i].im = 0;
         counter++;
      }
#endif
      // wait until next sample period
      tmr when timerafter(t+SAMPLE_PERIOD_CYCLES) :> t;
    }

    // swap buffers
    filler.swap(buffer);

  }
}

/**
 The application runs both of these tasks in parallel using a 'par'
 statement. The two buffers are declared at this level and passed into
 the two tasks:
**/

int main() {
  multichannel_sample_block_s buffer0;
  multichannel_sample_block_s buffer1;

  interface bufswap_i bufswap;
  par {
      produce_samples(bufswap, &buffer1);
      do_fft(bufswap, &buffer0);
  }
  return 0;
}

