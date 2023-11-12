#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>

struct WAV_HEADER_RIFF
{
		char		ChunkID[4];			// "RIFF"
		DWORD		ChunkSize;			// = 36 + SubChunk2Size
		char		Format[4];			// "WAVE"
};

struct WAV_HEADER_FMT
{
		char		SubChunk1ID[4];		// "fmt "
		DWORD		SubChunk1Size;		// 16 for PCM
			WORD	AudioFormat;		// PCM = 1 (i.e. Linear quantization)
			WORD	NumChannels;
			DWORD	SampleRate;
			DWORD	ByteRate;			// = SampleRate * NumChannels * BitsPerSample/8
			WORD	BlockAlign;			// = NumChannels * BitsPerSample/8
			WORD	BitsPerSample;
 };
struct WAV_HEADER_DATA
{
		char		SubChunk2ID[4];		// "data"
		DWORD		SubChunk2Size;		// = NumSamples * NumChannels * BitsPerSample/8
};

struct WAV_HEADER
{
	WAV_HEADER_RIFF		riff;
	WAV_HEADER_FMT		fmt;
	WAV_HEADER_DATA		data;
	long				HeaderSize;
};

int ReadWavHeader (FILE *refFile, WAV_HEADER &inWAVhdr);
int WriteWavHeader (FILE *refFile, WAV_HEADER &inWAVhdr);
FILE* OpenWavFileForRead (char *FileName,char *stMode );