#include "common_audio/signal_processing_library.h"

int16_t WebRtcSpl_DivW32W16ResW16(int32_t num, int16_t den) {
    // Guard against division with 0
    if (den != 0)
    {
        return (int16_t)(num / den);
    } else
    {
        return (int16_t)0x7FFF;
    }
}

int32_t WebRtcSpl_DivW32W16(int32_t num, int16_t den)
{
    // Guard against division with 0
    if (den != 0)
    {
        return (int32_t)(num / den);
    } else
    {
        return (int32_t)0x7FFFFFFF;
    }
}

int32_t WebRtcSpl_SqrtLocal(int32_t in)
{

    int16_t x_half, t16;
    int32_t A, B, x2;

    /* The following block performs:
     y=in/2
     x=y-2^30
     x_half=x/2^31
     t = 1 + (x_half) - 0.5*((x_half)^2) + 0.5*((x_half)^3) - 0.625*((x_half)^4)
         + 0.875*((x_half)^5)
     */

    B = in / 2;

    B = B - ((int32_t)0x40000000); // B = in/2 - 1/2
    x_half = (int16_t)(B >> 16);  // x_half = x/2 = (in-1)/2
    B = B + ((int32_t)0x40000000); // B = 1 + x/2
    B = B + ((int32_t)0x40000000); // Add 0.5 twice (since 1.0 does not exist in Q31)

    x2 = ((int32_t)x_half) * ((int32_t)x_half) * 2; // A = (x/2)^2
    A = -x2; // A = -(x/2)^2
    B = B + (A >> 1); // B = 1 + x/2 - 0.5*(x/2)^2

    A >>= 16;
    A = A * A * 2; // A = (x/2)^4
    t16 = (int16_t)(A >> 16);
    B += -20480 * t16 * 2;  // B = B - 0.625*A
    // After this, B = 1 + x/2 - 0.5*(x/2)^2 - 0.625*(x/2)^4

    A = x_half * t16 * 2;  // A = (x/2)^5
    t16 = (int16_t)(A >> 16);
    B += 28672 * t16 * 2;  // B = B + 0.875*A
    // After this, B = 1 + x/2 - 0.5*(x/2)^2 - 0.625*(x/2)^4 + 0.875*(x/2)^5

    t16 = (int16_t)(x2 >> 16);
    A = x_half * t16 * 2;  // A = x/2^3

    B = B + (A >> 1); // B = B + 0.5*A
    // After this, B = 1 + x/2 - 0.5*(x/2)^2 + 0.5*(x/2)^3 - 0.625*(x/2)^4 + 0.875*(x/2)^5

    B = B + ((int32_t)32768); // Round off bit

    return B;
}


int32_t WebRtcSpl_Sqrt(int32_t value)
{

    int16_t x_norm, nshift, t16, sh;
    int32_t A;

    int16_t k_sqrt_2 = 23170; // 1/sqrt2 (==5a82)

    A = value;

    // The convention in this function is to calculate sqrt(abs(A)). Negate the
    // input if it is negative.
    if (A < 0) {
        if (A == WEBRTC_SPL_WORD32_MIN) {
            // This number cannot be held in an int32_t after negating.
            // Map it to the maximum positive value.
            A = WEBRTC_SPL_WORD32_MAX;
        } else {
            A = -A;
        }
    } else if (A == 0) {
        return 0;  // sqrt(0) = 0
    }

    sh = WebRtcSpl_NormW32(A); // # shifts to normalize A
    A = WEBRTC_SPL_LSHIFT_W32(A, sh); // Normalize A
    if (A < (WEBRTC_SPL_WORD32_MAX - 32767))
    {
        A = A + ((int32_t)32768); // Round off bit
    } else
    {
        A = WEBRTC_SPL_WORD32_MAX;
    }

    x_norm = (int16_t)(A >> 16);  // x_norm = AH

    nshift = (sh / 2);
		//To-Do: sparrow sanity check
    //RTC_DCHECK_GE(nshift, 0);
		if (nshift < 0)
			return -1;

    A = (int32_t)WEBRTC_SPL_LSHIFT_W32((int32_t)x_norm, 16);
    A = WEBRTC_SPL_ABS_W32(A); // A = abs(x_norm<<16)
    A = WebRtcSpl_SqrtLocal(A); // A = sqrt(A)

    if (2 * nshift == sh) {
        // Even shift value case

        t16 = (int16_t)(A >> 16);  // t16 = AH

        A = k_sqrt_2 * t16 * 2;  // A = 1/sqrt(2)*t16
        A = A + ((int32_t)32768); // Round off
        A = A & ((int32_t)0x7fff0000); // Round off

        A >>= 15;  // A = A>>16

    } else
    {
        A >>= 16;  // A = A>>16
    }

    A = A & ((int32_t)0x0000ffff);
    A >>= nshift;  // De-normalize the result.

    return A;
}


void WebRtcSpl_MemSetW32(int32_t *ptr, int32_t set_value, size_t length)
{
    size_t j;
    int32_t *arrptr = ptr;

    for (j = length; j > 0; j--)
    {
        *arrptr++ = set_value;
    }
}

void WebRtcSpl_DownsampleBy2(const int16_t* in, size_t len,
                             int16_t* out, int32_t* filtState) {
  int32_t tmp1, tmp2, diff, in32, out32;
  size_t i;

  register int32_t state0 = filtState[0];
  register int32_t state1 = filtState[1];
  register int32_t state2 = filtState[2];
  register int32_t state3 = filtState[3];
  register int32_t state4 = filtState[4];
  register int32_t state5 = filtState[5];
  register int32_t state6 = filtState[6];
  register int32_t state7 = filtState[7];

  for (i = (len >> 1); i > 0; i--) {
    // lower allpass filter
    in32 = (int32_t)(*in++) * (1 << 10);
    diff = in32 - state1;
    tmp1 = MUL_ACCUM_1(kResampleAllpass2[0], diff, state0);
    state0 = in32;
    diff = tmp1 - state2;
    tmp2 = MUL_ACCUM_2(kResampleAllpass2[1], diff, state1);
    state1 = tmp1;
    diff = tmp2 - state3;
    state3 = MUL_ACCUM_2(kResampleAllpass2[2], diff, state2);
    state2 = tmp2;

    // upper allpass filter
    in32 = (int32_t)(*in++) * (1 << 10);
    diff = in32 - state5;
    tmp1 = MUL_ACCUM_1(kResampleAllpass1[0], diff, state4);
    state4 = in32;
    diff = tmp1 - state6;
    tmp2 = MUL_ACCUM_1(kResampleAllpass1[1], diff, state5);
    state5 = tmp1;
    diff = tmp2 - state7;
    state7 = MUL_ACCUM_2(kResampleAllpass1[2], diff, state6);
    state6 = tmp2;

    // add two allpass outputs, divide by two and round
    out32 = (state3 + state7 + 1024) >> 11;

    // limit amplitude to prevent wrap-around, and write to output array
    *out++ = WebRtcSpl_SatW32ToW16(out32);
  }

  filtState[0] = state0;
  filtState[1] = state1;
  filtState[2] = state2;
  filtState[3] = state3;
  filtState[4] = state4;
  filtState[5] = state5;
  filtState[6] = state6;
  filtState[7] = state7;
}

int32_t WebRtcSpl_DotProductWithScale(const int16_t* vector1,
                                      const int16_t* vector2,
                                      size_t length,
                                      int scaling) {
  int64_t sum = 0;
  size_t i = 0;

  /* Unroll the loop to improve performance. */
  for (i = 0; i + 3 < length; i += 4) {
    sum += (vector1[i + 0] * vector2[i + 0]) >> scaling;
    sum += (vector1[i + 1] * vector2[i + 1]) >> scaling;
    sum += (vector1[i + 2] * vector2[i + 2]) >> scaling;
    sum += (vector1[i + 3] * vector2[i + 3]) >> scaling;
  }
  for (; i < length; i++) {
    sum += (vector1[i] * vector2[i]) >> scaling;
  }

  if (sum > WEBRTC_SPL_WORD32_MAX) {
		sum = WEBRTC_SPL_WORD32_MAX;
	} else if (sum < WEBRTC_SPL_WORD32_MIN) {
		sum = WEBRTC_SPL_WORD32_MIN;
	}	

	return (int32_t)(sum);
}


