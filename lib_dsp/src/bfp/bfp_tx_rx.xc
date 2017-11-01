#include <xs1.h>
#include <stdint.h>
#include <stdio.h>
#include <dsp_bfp.h>

// All of these are in assembly code

void dsp_bfp_tx_xc(chanend x, int32_t data[], uint32_t N, int32_t shr) {
    chkct(x, 1);
    outuint(x, shr);
    for(int i = N-1; i >= 0; i--) {
        outuint(x, data[i]);
    }
    outct(x, 1);
}

void dsp_bsp_rx_state_init_xc(uint64_t state[], int N) {
    for(int i = 0; i < N; i++) {
        state[i] = 0;
    }
}

int32_t dsp_bfp_rx_xc(chanend x, uint64_t state[],
                      int32_t target[],
                      uint32_t N, uint32_t advance,
                      uint32_t headroom) {
    int old_data = N - advance;
    // First copy the old data from the state into the target array
    for(int i = 0; i < old_data; i++) {
        target[i] = (state, int32_t[])[i];
    }
    // Now advance the state, removing the oldest data.
    for(int i = 0; i < (old_data - advance)/2; i++) {
        state[i] = state[i+(advance/2)];
    }
    // Now receive the new data straight into the target array
    outct(x, 1);
    int exponent = inuint(x);
    for(int i = N-1; i >= N-advance; i--) {
        target[i] = inuint(x);
    }
    chkct(x, 1);
    // Finally, shift the new data down
    for(int i = N-1; i >= N-advance; i--) {
        int y = target[i] >> exponent;
        target[i] = y;
        (state, int32_t[])[i-advance] = y;
    }
    unsigned ls = dsp_bfp_cls((target, dsp_complex_t[]),N/2);
    ls -= headroom;
    dsp_bfp_shl((target, dsp_complex_t[]), N/2, ls);

    return ls;
}
