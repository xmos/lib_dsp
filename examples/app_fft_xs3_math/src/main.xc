// Copyright 2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#include <xs1.h>
#include <xclib.h>
#include <stdio.h>
#include <stdlib.h>

#include "bfp_math.h"

// The length (in elements) of our BFP vectors.
#define LENGTH    (128)
#
void forward_fft_xs3_math() {
  printf("#########################\n");
  printf("### Convert FP to BFP ###\n");
  printf("#########################\n\n");

  printf("Using random BFP values\n");


  // TODO: Set the correct input values here
  // bfp_fft_forward_mono() requires an input BFP vector of type bfp_s32_t, whose mantissa vector
  // is backed by an int32_t array.
  // The +2 below is so spectrum unpacking can be demonstrated (more on this below).
  int32_t buffer[LENGTH+2];

  // We'll just fill that vector with random mantissas.
  for(int k = 0; k < LENGTH; k++){
    buffer[k] = rand() << 1;
  }

  // Before doing the forward FFT the input vector needs to be initialized. In many situations there
  // is no obvious natural exponent to associate with the time-domain signal (such as PCM audio
  // streams). In such a situation, it is often convenient to select an exponent that normalizes the
  // data to a known range, such as [-1.0, 1.0). For 32-bit signed data, an exponent of -31 does
  // that.
  // Note that the final parameter is instructing the init function to compute the headroom of the
  // input vector. If we instead chose to fill buffer[] with random data *after* initializing x,
  // there would be no point to computing the headroom here, as it is invalidated the moment we
  // modify x.data[].
  bfp_s32_t x;
  bfp_s32_init(&x, buffer, -31, LENGTH, 1);



  printf("################\n");
  printf("### Mono FFT ###\n");
  printf("################\n\n");

  /*
    This function demonstrates how apply the forward and inverse FFT to a single channel of
    real data.  This is accomplished using the bfp_fft_forward_mono() and bfp_fft_inverse_mono()
    functions respectively.

    bfp_s32_t         --bfp_fft_forward_mono()--> bfp_complex_s32_t
    bfp_complex_s32_t --bfp_fft_inverse_mono()--> bfp_s32_t
  */


  // Print out the floating point equivalent values of the input vector's elements prior to applying
  // the FFT.
  unsafe {
    printf("x = [");
    for(int k = 0; k < x.length; k++)
      printf("%0.04f, ", ldexp(x.data[k], x.exp));
    printf("]\n\n");

    // bfp_fft_forward_mono() operates on data in-place. We'll print out the buffer address before
    // and after the transformation to convince ourselves of this.
#ifdef __xcore__
    printf("&x.data[0] --> 0x%08X\n", (unsigned) &x.data[0]);
#endif
    printf("x.length --> %u\n\n", x.length);

    // Apply the FFT.
    // This function takes a pointer to the input (time-domain, usually) BFP vector, and returns
    // a pointer *to the same address* recast to a complex BFP vector.
    bfp_complex_s32_t* X = (bfp_complex_s32_t*) bfp_fft_forward_mono(&x);

    /*
      There are two important things to be aware of with the mono FFT.
      1) The complex output vector X is only half the length of the input vector.
      2) The real part of the Nyquist rate component of the FFT is packed into the first element of X.
      The DFT of an N-element signal is periodic with period N. Additionally, the DFT of a real
      signal has a complex conjugate symmetry about angular frequency 0. Whereas a fully complex
      DFT requires N complex elements to fully specify the signal, the symmetry property of a
      real DFT means that only N/2 complex elements are needed.
      A consequence of the periodicity and conjugate symmetry properties taken together is that the
      components of the DFT at DC and at the Nyquist rate must have 0 imaginary part. Rather than
      require the user to supply an output vector with space allocated for N/2+1 complex elements,
      we opt to compute the DFT in-place, and pack the Nyquist rate component into the imaginary
      part of the DC component.
      If the user wishes, she can allocate space for 2 extra int32_t elements (same as 1 complex_s32_t
      element) in the time-domain buffer (but *not* identified in the length of the time-domain BFP
      vector, as that must be a power of 2), and unpack the Nyquist component after the FFT is applied
      (making sure to modify the frequency-domain BFP vectors length by adding 1). Note that
      bfp_fft_inverse_mono() however expects the data to be packed as bfp_fft_forward_mono() left it.
      Note also that the inverse mono FFT also assumes the conjugate symmetry of the frequency
      spectrum, meaning that the inverse transform is guaranteed to yield a purely real signal.
    */

#define UNPACK_SPECTRUM_MONO    1

    // Unpack the mono spectrum
    if(UNPACK_SPECTRUM_MONO) {
      bfp_fft_unpack_mono(X);
    }

    // Print out the buffer address and vector length of X, the frequency-domain signal
#ifdef __xcore__
    printf("&X->data[0] --> 0x%08X\n", (unsigned) &X->data[0]);
#endif
    printf("X->length --> %u\n\n", X->length);

    // Print out the complex BFP vector X
    printf("X = [");
    for(int k = 0; k < X->length; k++)
      printf("(%0.04f + %0.04fj), ", ldexp(X->data[k].re, X->exp), ldexp(X->data[k].im, X->exp) );
    printf("]\n\n");

    ///////////////////////////////
    // ...
    // Here you are free to do any required frequency-domain processing of the signal.
    // ...
    ///////////////////////////////

    // Re-pack the mono spectrum
    if(UNPACK_SPECTRUM_MONO) {
      bfp_fft_pack_mono(X);
    }

    // Apply the inverse FFT.
    // Like the forward mono FFT, this function returns a pointer, this time a bfp_s32_t*, which is
    // the function's input parameter recast. In this case we can ignore the returned value, as it
    // would simply point to x, which is already of the correct type and readily available. In other
    // situations (e.g. when x exists in a different scope) the returned value may be useful.
    bfp_fft_inverse_mono(X);

    // Finally, print out the inverse transformed signal, which should match the original signal to
    // within the arithmetic precision of the forward and inverse transform pair.
    printf("x = [");
    for(int k = 0; k < x.length; k++)
      printf("%0.04f, ", ldexp(x.data[k], x.exp));
    printf("]\n\n");
  } // unsafe
}

#define RAND_SEED   3457345734

int main()
{

  // Seed the random number generator, using a constant for reproducibility
  srand(RAND_SEED);

  forward_fft_xs3_math();
  return 0;
}
