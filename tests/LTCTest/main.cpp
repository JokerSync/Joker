#include <QCoreApplication>

#include <stdio.h>
#include <math.h>
#include "ltc.h"

#include "PhTools/PhDebug.h"

#define BUFFER_SIZE (1024)

/**
 * @brief The application main entry point
 * @param argc Command line argument count
 * @param argv Command line argument list
 * @return 0 if the application works well.
 */
int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	for(int i = 0; i < argc; i++)
		PHDEBUG << argv[i];
	PHDEBUG << "\n\n";

	int apv = 1920;
	ltcsnd_sample_t sound[BUFFER_SIZE];
	size_t n;
	long int total;
	FILE* f;
	char* filename;
	LTCDecoder *decoder;
	LTCFrameExt frame;

	// fIXME
	if (argc > 1) {
		filename = argv[1];
		if (argc > 2) {
			sscanf(argv[2], "%i", &apv);
		}
	}
	else {
		printf("Usage: %s <filename> [audio-frames-per-video-frame]\n", argv[0]);
		return -1;
	}
	f = fopen(filename, "r");
	if (!f) {
		fprintf(stderr, "error opening '%s'\n", filename);
		return -1;
	}
	fprintf(stderr, "* reading from: %s\n", filename);
	total = 0;
	decoder = ltc_decoder_create(apv, 3840);

	do {
		//PHDEBUG << "in da do";
		n = fread(sound, sizeof(ltcsnd_sample_t), BUFFER_SIZE, f);

		//PHDEBUG << n;
		ltc_decoder_write(decoder, sound, n, total);
		//PHDEBUG << "2";

		while (ltc_decoder_read(decoder, &frame)) {
			SMPTETimecode stime;
			ltc_frame_to_time(&stime, &frame.ltc, 1);
			//PHDEBUG << stime.years + 1900 << stime.months << stime.days; // << stime.timezone;
			PHDEBUG << stime.hours << stime.mins << stime.secs << ":" << stime.frame << "\toffStart " << frame.off_start << "\toffEnd" << frame.off_end;
		}
		total += n;
		//PHDEBUG << n;
	}
	while (n);
	fclose(f);
	ltc_decoder_free(decoder);

	return 0;
}
