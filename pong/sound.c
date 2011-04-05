#include <fcntl.h>			//for open()
#include <stdlib.h>
#include <stdio.h>			//for printf()
#include <string.h>			//for memcpy()
#include <unistd.h>			//For sleep
#include <time.h>			//For random seed

#include <linux/soundcard.h>

#include "pong.h"
#include "driver_interface.h"
#include "sound.h"

static const int BUFFER_SIZE = 2048;

static const int LOWER_VOLUME = 32;

static const int SAMPLE_RATE = 44100;

static FILE *music;

static int sound_driver;

void initialize_sound_driver()
{
	sound_driver = open("/dev/dsp", O_WRONLY);

	music = fopen("09-the-moon.au", "r");
	
	ioctl( sound_driver, SOUND_PCM_WRITE_RATE, SAMPLE_RATE );
}

void play_sound(FILE *sound_file)
{
	unsigned char *buffer = malloc(BUFFER_SIZE * sizeof(char) + 1); // Allocate buffer
	
	int bytes_read = fread( buffer, 1, BUFFER_SIZE, sound_file );
	int bytes_written;
	
	while ( !feof(sound_file) && !BUTTONS() ) // While not at EOF
	{
		if ( bytes_read )
		{
			int i;
			for (i = 0; i < bytes_read; i++)
			{
				buffer[i] /= LOWER_VOLUME;
			}
			bytes_written = write( sound_driver, buffer, bytes_read );
		}
		else
		{
			printf("No bytes read, BUG!\n");
		}
		
		bytes_read = fread( buffer, 1, BUFFER_SIZE, sound_file );
	}
	
	free(buffer);
	fclose(sound_file);
}

void play_music()
{
	play_sound(music);
}
