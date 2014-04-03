#include <stdio.h>
#include <math.h>
#include "ltc.h"

#include "PhTools/PhDebug.h"

#define BUFFER_SIZE (8192)

/**
 * @brief The application main entry point
 * @param argc Command line argument count
 * @param argv Command line argument list
 * @return 0 if the application works well.
 */
int main(int argc, char *argv[])
{
	if (argc < 2) {
		printf("Usage: %s <filename> [audio-frames-per-video-frame]\n", argv[0]);
		return -1;
	}

	char *fileName = argv[1];
	FILE *f = fopen(fileName, "r");
	if (!f) {
		fprintf(stderr, "error opening '%s'\n", fileName);
		return -1;
	}
	fprintf(stderr, "* reading from: %s\n", fileName);
	offset = 0;
	LTCDecoder *decoder = ltc_decoder_create(1920, 3840);
	ltcsnd_sample_t sound[BUFFER_SIZE];

	long int offset = 0;
	size_t byteRead = 0;
	do {
		byteRead = fread(sound, 1, BUFFER_SIZE, f);

		ltc_decoder_write(decoder, sound, byteRead, offset);

		LTCFrameExt frame;
		while (ltc_decoder_read(decoder, &frame)) {
			SMPTETimecode stime;
			ltc_frame_to_time(&stime, &frame.ltc, 1);
			PHDEBUG << stime.hours << stime.mins << stime.secs << ":" << stime.frame;
		}
		offset += byteRead;
	}
	while (byteRead);

	fclose(f);
	ltc_decoder_free(decoder);

	return 0;
}
