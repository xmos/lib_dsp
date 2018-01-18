#include <xs1.h>
#include <stdint.h>
#include <stdio.h>
#include <dsp_bfp.h>

// All of these are in assembly code

void dsp_bfp_tx_xc(chanend x, int32_t data[],
                   uint32_t CHANS, uint32_t advance, int32_t shr) {
    slave {
        x <: shr;
        for(int c = CHANS-1; c >= 0; c--) {
            for(int i = advance-1; i >= 0; i--) {
                x <: data[c*advance + i];
            }
        }
    }
}

void dsp_bfp_rx_state_init_xc(uint64_t state[], int N) {
    for(int i = 0; i < N; i++) {
        state[i] = 0;
    }
}

int32_t dsp_bfp_rx_xc(chanend x, uint64_t state[],
                      int32_t target[],
                      uint32_t CHANS,
                      uint32_t N,
                      uint32_t advance,
                      uint32_t headroom) {
    int old_data = N - advance;
    // First copy the old data from the state into the target array
    for(int c = CHANS-1; c >= 0; c--) {
        for(int i = 0; i < old_data; i++) {
            target[N*c + i] = (state, int32_t[])[(N-advance)*c + i];
        }
    }
    // Now advance the state, removing the oldest data.
    for(int c = CHANS-1; c >= 0; c--) {
        for(int i = 0; i < (int)(old_data - advance)/2; i++) {
            state[(N-advance)/2*c + i] = state[(N-advance)/2*c + i+(advance/2)];
        }
    }
    // Now receive the new data straight into the target array
    int exponent;
    master {
        x :> exponent;
        for(int c = CHANS-1; c >= 0; c--) {
            for(int i = N-1; i >= (int) (N-advance); i--) {
                x :> target[N * c + i];
            }
        }
    }
    // Finally, shift the new data down
    for(int c = CHANS-1; c >= 0; c--) {
        for(int i = N-1; i >= (int) (N-advance); i--) {
            int y = target[N * c + i] >> exponent;
            target[N * c + i] = y;
            (state, int32_t[])[(N - advance) * c  + i-advance] = y;
        }
    }
    unsigned ls = dsp_bfp_cls((target, dsp_complex_t[]),CHANS * N/2) - 1;
    ls -= headroom;
    dsp_bfp_shl((target, dsp_complex_t[]), CHANS * N/2, ls);

    return ls;
}

void dsp_bfp_tx_pairs(chanend x, dsp_complex_t data[],
                      uint32_t channels, uint32_t advance, int32_t shr) {
///    chkct(x, 1); // extra sync to trap pairs/singles mismatch
    dsp_bfp_tx_xc(x, (data, int32_t[]),
                  channels >> 1, advance << 1, shr);
}

int32_t dsp_bfp_rx_pairs(chanend x, uint64_t state[],
                         dsp_complex_t target[],
                         uint32_t channels,
                         uint32_t N,
                         uint32_t advance,
                         uint32_t headroom) {
//    outct(x, 1); // extra sync to trap pairs/singles mismatch
    return dsp_bfp_rx_xc(x, state, (target, int32_t[]),
                         channels >> 1, N << 1, advance << 1, headroom);
}

