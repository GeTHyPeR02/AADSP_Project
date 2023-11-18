#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "WAVheader.h"
#include "iir.h"

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

// Filter coeficients
static double lpf_18k_coefs[] = { (0.6828), (1.3657), (0.6828), (1.0), (1.1314), (0.6) };
static double hpf_800_coefs[] = { (0.9617), (-1.9234), (0.9617), (1.0), (-1.9182), (0.9287) };
static double bpf_coefs[] = { (0.4139), (0.0), (-0.4139), (1.0), (-0.6384), (0.1722) };
static double bpf2_coefs[] = { (0.4139), (0.0), (-0.4139), (1.0), (-0.6384), (0.1722) };
static double hpf2_800_coefs[] = { (0.9617), (-1.9234), (0.9617), (1.0), (-1.9182), (0.9287) };
static double lpf2_18k_coefs[] = { (0.6828), (1.3657), (0.6828), (1.0), (1.1314), (0.6) };

// History
static double lpf_x_history[2];
static double lpf_y_history[2];
static double hpf_x_history[2];
static double hpf_y_history[2];
static double bpf_x_history[2];
static double bpf_y_history[2];
static double bpf2_x_history[2];
static double bpf2_y_history[2];
static double hpf2_x_history[2];
static double hpf2_y_history[2];
static double lpf2_x_history[2];
static double lpf2_y_history[2];

// IO Buffers
static double sampleBuffer[MAX_NUM_CHANNEL][BLOCK_SIZE];

// Controls
static int enable = 1;
static int mode = 1;

// Processing related variables
static double preGain1 = MINUS_3DB;
static double preGain2 = MINUS_3DB;

void processing()
{
	int i;
	double* sampleBuffer_left = sampleBuffer[LEFT_CH];
	double* sampleBuffer_center = sampleBuffer[CENTER_CH];
	double* sampleBuffer_leftSide= sampleBuffer[LEFT_SIDE_CH];
	double* sampleBuffer_rightSide = sampleBuffer[RIGHT_SIDE_CH];
	double* sampleBuffer_right = sampleBuffer[RIGHT_CH];
	double* sampleBuffer_lfe = sampleBuffer[LFE_CH];

	if (!mode) {
		for (i = 0; i < BLOCK_SIZE; i++)
		{
			*sampleBuffer_center = second_order_IIR(preGain1 * *sampleBuffer_left, lpf_18k_coefs, lpf_x_history, lpf_y_history);
			*sampleBuffer_leftSide = second_order_IIR(preGain1 * *sampleBuffer_left, hpf_800_coefs, hpf_x_history, hpf_y_history);
			*sampleBuffer_rightSide = second_order_IIR(preGain2 * *sampleBuffer_left, bpf_coefs, bpf_x_history, bpf_y_history);
			*sampleBuffer_right = second_order_IIR(preGain2 * *sampleBuffer_left, bpf2_coefs, bpf2_x_history, bpf2_y_history);
			*sampleBuffer_lfe = second_order_IIR(preGain2 * *sampleBuffer_left, hpf2_800_coefs, hpf2_x_history, hpf2_y_history);
			*sampleBuffer_left = preGain1 * *sampleBuffer_left;

			sampleBuffer_center++;
			sampleBuffer_leftSide++;
			sampleBuffer_rightSide++;
			sampleBuffer_right++;
			sampleBuffer_lfe++;
			sampleBuffer_left++;
		}
	}
	else{
		for (i = 0; i < BLOCK_SIZE; i++)
		{
			*sampleBuffer_center = second_order_IIR(preGain1 * *sampleBuffer_left, hpf_800_coefs, hpf_x_history, hpf_y_history);
			*sampleBuffer_leftSide = second_order_IIR(preGain1 * *sampleBuffer_left, bpf_coefs, bpf_x_history, bpf_y_history);
			*sampleBuffer_rightSide = second_order_IIR(preGain2 * *sampleBuffer_left, bpf2_coefs, bpf2_x_history, bpf2_y_history);
			*sampleBuffer_right = second_order_IIR(preGain2 * *sampleBuffer_left, hpf2_800_coefs, hpf2_x_history, hpf2_y_history);
			*sampleBuffer_lfe = second_order_IIR(preGain2 * *sampleBuffer_left, lpf2_18k_coefs, lpf2_x_history, lpf2_y_history);
			*sampleBuffer_left = second_order_IIR(preGain1 * *sampleBuffer_left, lpf_18k_coefs, lpf_x_history, lpf_y_history);

			sampleBuffer_center++;
			sampleBuffer_leftSide++;
			sampleBuffer_rightSide++;
			sampleBuffer_right++;
			sampleBuffer_lfe++;
			sampleBuffer_left++;
		}
	}
}


int main(int argc, char* argv[])
{
	if (argc < 3 || argc > 7) {
		printf("Wrong number of arguments\n");
		printf("Argument example: %s INPUT OUTPUT [Enable] [Mode] [Gain1] [Gain2]\n", argv[0]);
		return -1;
	}

	// preGain2
	if (argc > 6) {
		double gain2_db = atof(argv[6]);
		if (gain2_db <= 0) {
			preGain2 = gain2_db;
		}
		else {
			preGain2 = 0;
		}
	}

	// preGain1
	if (argc > 5) {
		double gain1_db = atof(argv[5]);
		if (gain1_db <= 0) {
			preGain1 = gain1_db;
		}
		else {
			preGain1 = 0;
		}
	}

	// Check mode
	if (argc > 4) {
		if (strcmp(argv[4], "0") != 0) {
			mode = 1;
		}
	}

	// Enable
	if (argc > 3) {
		if (strcmp(argv[3], "0") == 0) {
			enable = 0;
		}
	}

	FILE* wav_in = NULL;
	FILE* wav_out = NULL;
	char WavInputName[256];
	char WavOutputName[256];
	WAV_HEADER inputWAVhdr, outputWAVhdr;

	// Init channel buffers
	for (int i = 0; i < MAX_NUM_CHANNEL; i++)
		memset(&sampleBuffer[i], 0, BLOCK_SIZE);

	// Open input and output wav files
	//-------------------------------------------------
	strcpy(WavInputName, argv[1]);
	wav_in = OpenWavFileForRead(WavInputName, "rb");
	strcpy(WavOutputName, argv[2]);
	wav_out = OpenWavFileForRead(WavOutputName, "wb");
	//-------------------------------------------------

	// Read input wav header
	//-------------------------------------------------
	ReadWavHeader(wav_in, inputWAVhdr);
	//-------------------------------------------------

	// Set up output WAV header
	//-------------------------------------------------	
	outputWAVhdr = inputWAVhdr;
	outputWAVhdr.fmt.NumChannels = OUTPUT_NUM_CHANNELS; // change number of channels

	int oneChannelSubChunk2Size = inputWAVhdr.data.SubChunk2Size / inputWAVhdr.fmt.NumChannels;
	int oneChannelByteRate = inputWAVhdr.fmt.ByteRate / inputWAVhdr.fmt.NumChannels;
	int oneChannelBlockAlign = inputWAVhdr.fmt.BlockAlign / inputWAVhdr.fmt.NumChannels;

	outputWAVhdr.data.SubChunk2Size = oneChannelSubChunk2Size * outputWAVhdr.fmt.NumChannels;
	outputWAVhdr.fmt.ByteRate = oneChannelByteRate * outputWAVhdr.fmt.NumChannels;
	outputWAVhdr.fmt.BlockAlign = oneChannelBlockAlign * outputWAVhdr.fmt.NumChannels;


	// Write output WAV header to file
	//-------------------------------------------------
	WriteWavHeader(wav_out, outputWAVhdr);


	// Processing loop
	//-------------------------------------------------	
	{
		int sample;
		int BytesPerSample = inputWAVhdr.fmt.BitsPerSample / 8;
		const double SAMPLE_SCALE = -(double)(1 << 31);		//2^31
		int iNumSamples = inputWAVhdr.data.SubChunk2Size / (inputWAVhdr.fmt.NumChannels * inputWAVhdr.fmt.BitsPerSample / 8);

		// exact file length should be handled correctly...
		for (int i = 0; i < iNumSamples / BLOCK_SIZE; i++)
		{
			for (int j = 0; j < BLOCK_SIZE; j++)
			{
				for (int k = 0; k < inputWAVhdr.fmt.NumChannels; k++)
				{
					sample = 0; //debug
					fread(&sample, BytesPerSample, 1, wav_in);
					sample = sample << (32 - inputWAVhdr.fmt.BitsPerSample); // force signextend
					sampleBuffer[k][j] = sample / SAMPLE_SCALE;				 // scale sample to 1.0/-1.0 range		
				}
			}
			if (enable) {
				processing();
			}

			for (int j = 0; j < BLOCK_SIZE; j++)
			{
				for (int k = 0; k < outputWAVhdr.fmt.NumChannels; k++)
				{
					sample = sampleBuffer[k][j] * SAMPLE_SCALE;		// crude, non-rounding 			
					sample = sample >> (32 - inputWAVhdr.fmt.BitsPerSample);
					fwrite(&sample, outputWAVhdr.fmt.BitsPerSample / 8, 1, wav_out);
				}
			}
		}
	}

	// Close files
	//-------------------------------------------------	
	fclose(wav_in);
	fclose(wav_out);
	//-------------------------------------------------	

	return 0;
}