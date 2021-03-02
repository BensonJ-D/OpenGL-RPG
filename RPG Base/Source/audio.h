#pragma once
#include <stdlib.h>
#include <stdio.h>
//#include "OpenAL.h"

typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef unsigned char BYTE;

struct  RIFFCHUNK
{
	/* RIFF Chunk Descriptor */
	BYTE         RIFF[4];        // RIFF Header Magic header
	DWORD        ChunkSize;      // RIFF Chunk Size
	BYTE         WAVE[4];        // WAVE Header
};

struct  CHUNKHEADER
{
	BYTE         hdr[4];         // FMT header
	DWORD        chunkSize;  // Size of the fmt chunk
};
 struct  FMTCHUNK
{
	WORD         AudioFormat;    // Audio format 1=PCM,6=mulaw,7=alaw,     257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
	WORD         NumOfChan;      // Number of channels 1=Mono 2=Sterio
	DWORD        SamplesPerSec;  // Sampling Frequency in Hz
	DWORD        bytesPerSec;    // bytes per second
	WORD         blockAlign;     // 2=16-bit mono, 4=16-bit stereo
	WORD         bitsPerSample;  // Number of bits per sample
};

class Audio
{
public:
	FILE* hfile;
	RIFFCHUNK riff;
	CHUNKHEADER hdr;
	FMTCHUNK fmt;

	Audio() {};
	Audio(const char* Filename);
	~Audio() {};

	void Play();
	void Pause();
	void Stop();

protected:
	short* data;
	unsigned long size;
	//ALuint buffer, source;
};
