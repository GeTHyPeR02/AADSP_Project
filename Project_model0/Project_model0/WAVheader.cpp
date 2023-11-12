
#include "WAVheader.h"

// Type defs
//-----------------------------------------------------------------------------
#pragma pack(1)	// needed for proper calculation for: sizeof(struct) - round to BYTE /zp1


/*################################################################################*/
int ReadWavHeader (FILE *refFile, WAV_HEADER &inWAVhdr )
{

	BYTE tempBuff[256];

	inWAVhdr.HeaderSize = 0;

	//get Ref WAV header
	fread(&inWAVhdr.riff, 1, sizeof(WAV_HEADER_RIFF),refFile);
	fread(&inWAVhdr.fmt, 1, sizeof(WAV_HEADER_FMT),refFile);
	if ((inWAVhdr.riff.ChunkID[0]=='R') &&
		(inWAVhdr.riff.ChunkID[1]=='I') &&
		(inWAVhdr.riff.ChunkID[2]=='F') &&
		(inWAVhdr.riff.ChunkID[3]=='F')
		&&
		(inWAVhdr.riff.Format[0]=='W') &&
		(inWAVhdr.riff.Format[1]=='A') &&
		(inWAVhdr.riff.Format[2]=='V') &&
		(inWAVhdr.riff.Format[3]=='E')
		&&
			(inWAVhdr.fmt.SubChunk1ID[0]=='f') &&
			(inWAVhdr.fmt.SubChunk1ID[1]=='m') &&
			(inWAVhdr.fmt.SubChunk1ID[2]=='t') &&
			(inWAVhdr.fmt.SubChunk1ID[3]==' ')
			&&
			(inWAVhdr.fmt.AudioFormat == 1)		// not PCM content
			&&
			(inWAVhdr.fmt.BitsPerSample != 0)
			&&
			(inWAVhdr.fmt.NumChannels != 0)
			&&
			(inWAVhdr.fmt.SampleRate != 0)
			&&
			(inWAVhdr.fmt.BlockAlign != 0))
	{ //------------------------------------
		// read any additional fmt data - not really elegant...
		if (inWAVhdr.fmt.SubChunk1Size > 16)
		{
			fread(tempBuff,1,inWAVhdr.fmt.SubChunk1Size-16,refFile);
		}
		//get Ref WAV header - Data
		fread(&inWAVhdr.data, 1, sizeof(WAV_HEADER_DATA),refFile);
		if ((inWAVhdr.data.SubChunk2ID[0]=='d') &&
			(inWAVhdr.data.SubChunk2ID[1]=='a') &&
			(inWAVhdr.data.SubChunk2ID[2]=='t') &&
			(inWAVhdr.data.SubChunk2ID[3]=='a')
			&&
			(inWAVhdr.data.SubChunk2Size != 0))
		{
			//Get RefWavHeader Size
			inWAVhdr.HeaderSize = ftell(refFile) ;
		}
	}//------------------------------------

	return true;
}
//-------------------------------------------------------------------------------


/*################################################################################*/
int WriteWavHeader (FILE *refFile, WAV_HEADER &inWAVhdr )
{

	fwrite(&inWAVhdr, 1, inWAVhdr.HeaderSize, refFile);

	return true;
}
//-------------------------------------------------------------------------------


/*################################################################################*/
FILE* OpenWavFileForRead (char *FileName,char *stMode )
{

	FILE *File = fopen(FileName,stMode); /// stMod=="rb" ili "wb"
	if(File == NULL)
	{
		printf("Cannot open file %s\n", FileName);
		exit(1);
	}

	return File;
}
//-------------------------------------------------------------------------------
