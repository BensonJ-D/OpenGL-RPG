#include "audio.h"
#include <memory>

Audio::Audio(const char* filename)
{
	fopen_s(&hfile, (char*)filename, "rb");
	if (hfile == NULL)
	{
		printf("Path name: %s\n", filename);
		printf("Could not locate referenced image. Exiting\n");
		system("pause");
		exit(1);
	}
	fread(&riff, sizeof(riff), 1, hfile);
	printf("Header: %c%c%c%c\n", riff.RIFF[0], riff.RIFF[1], riff.RIFF[2], riff.RIFF[3]);
	
	if (riff.RIFF[0] == int('R') && riff.RIFF[1] == int('I')
		&& riff.RIFF[2] == int('F') && riff.RIFF[3] == int('F') &&
		riff.WAVE[0] == int('W') && riff.WAVE[1] == int('A')
		&& riff.WAVE[2] == int('V') && riff.WAVE[3] == int('E'))
	{
		while (!feof(hfile)) 
		{
			fread(&hdr, sizeof(hdr), 1, hfile);
			if ((hdr.hdr[0] == int('f') && hdr.hdr[1] == int('m')
				&& hdr.hdr[2] == int('t') && hdr.hdr[3] == int(' ')))
				break;
			fseek(hfile, hdr.chunkSize, SEEK_CUR);
		}
		fread(&fmt, sizeof(fmt), 1, hfile);
		while (!feof(hfile))
		{
			fread(&hdr, sizeof(hdr), 1, hfile);
			if ((hdr.hdr[0] == int('d') && hdr.hdr[1] == int('a')
				&& hdr.hdr[2] == int('t') && hdr.hdr[3] == int('a')))
				break;
			fseek(hfile, hdr.chunkSize, SEEK_CUR);
		}
		int sample_size = fmt.bitsPerSample / 8;
		printf("SAMPLE SIZE: %d\n", sample_size);
		int samples_count = hdr.chunkSize * 8 / fmt.bitsPerSample;

		data = new short[hdr.chunkSize];
		memset(data, 0, sizeof(short) * samples_count);

		//Reading data
		for (int i = 0; i < samples_count; i++)
		{
			fread(&data[i], sample_size*2, 1, hfile);
		}
		printf("DATA SIZE %d\n", sizeof(&data));
		printf("SAMPLES %d\n", samples_count);

		FILE *fout;
		fopen_s(&fout, "data.txt", "w");
		for (int i = 0; i < samples_count; i++)
		{
			fprintf(fout, "%u\n", data[i]);
		}
		fclose(fout);
	}
	else
		printf("Unsupported format\n");

	printf("WAV File Header read:\n");

	ALuint err = alutGetError();
	buffer = alutCreateBufferFromFile(filename);
	if (alutGetError() != ALUT_ERROR_NO_ERROR)
	{
		// handle the error
		return;
	}
	// create a buffer
	alGenBuffers(1, &buffer);
	if (alGetError() != AL_NO_ERROR) {
		// handle the error
		printf("Could not generate sound buffer, error code %d!\n", err);
		return;
	}

	alGenSources(1, &source);
	if (alGetError() != AL_NO_ERROR) {
		// handle the error
		printf("Could not generate source, error code %d!\n", err);
		return;
	}

	printf("buffer args: %d %d, %d, %d, %d, %d\n", source, buffer, AL_FORMAT_MONO16, (ALvoid*)data, hdr.chunkSize, fmt.SamplesPerSec);
	// move the data into the buffer
	alBufferData(buffer, AL_FORMAT_MONO16, data, hdr.chunkSize / 2, fmt.SamplesPerSec);

	err = alGetError();
	if ( err != AL_NO_ERROR) {
		// handle the error
		printf("Error in buffer, error code %d!\n", err);
		return;
	}
	fclose(hfile); 

}

void Audio::Play()
{
	printf("Playing sound %d\n", buffer);
	ALenum state;
	alGetSourcei(source, AL_SOURCE_STATE, &state);

	alSourcei(source, AL_BUFFER, buffer);
	if (state != AL_PLAYING)
		alSourcePlay(source);
}

void Audio::Stop()
{
	ALenum state;
	alGetSourcei(source, AL_SOURCE_STATE, &state);

	alSourcei(source, AL_BUFFER, buffer);
	if (state == AL_PLAYING)
		alSourceStop(source);
}