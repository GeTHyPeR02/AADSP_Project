#pragma once

#ifndef COMMON_H
#define COMMON_H

#include "fixed_point_math.h"
#include "stdfix_emu.h"

#define BLOCK_SIZE 16
#define MAX_NUM_CHANNEL 8

// Number of channels
#define INPUT_NUM_CHANNELS 1
#define OUTPUT_NUM_CHANNELS 6

// Channel IDs
#define LEFT_CH 0
#define CENTER_CH 1
#define LEFT_SIDE_CH 2
#define RIGHT_SIDE_CH 3
#define RIGHT_CH 4
#define LFE_CH 5

// Gain linear values
#define MINUS_3DB 0.707946

typedef short DSPshort;					/* DSP integer */
typedef unsigned short DSPushort;		/* DSP unsigned integer */
typedef int DSPint;						/* native integer */
typedef fract DSPfract;					/* DSP fixed-point fractional */
typedef long_accum DSPaccum;			/* DSP fixed-point fractional */

#endif
