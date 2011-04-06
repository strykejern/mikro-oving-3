#include <fcntl.h>			//for open()
#include <stdlib.h>
#include <stdio.h>			//for printf()
#include <string.h>			//for memcpy()
#include <unistd.h>			//For sleep
#include <time.h>			//For random seed
#include <pthread.h>

#include <linux/soundcard.h>
#include <sys/ioctl.h>

#include "driver_interface.h"
#include "sound.h"

static const int BUFFER_SIZE = 2048;

static const int LOWER_VOLUME = 1;

static int SAMPLE_RATE = 22050;

static int BITRATE = 16;

static int sound_driver;

static FILE *music;

static FILE *paddle;

/*enum
{
 LEFT_PADDLE,
 RIGHT_PADDLE,
 LEFT_SCORE,
 RIGHT_SCORE
} SOUND_EFFECT;*/

void *threaded_music(void *arg)
{
	play_music();
	return NULL;
}

pthread_t music_thread;

void initialize_sound_driver()
{
	printf("Initializing the sound driver\n");
	sound_driver = open("/dev/dsp", O_WRONLY); // Loading the linux sound driver

	music = fopen("09-the-moon.au", "r"); // Loading music file
	
	paddle = fopen("sound46.au", "r"); // Loading sound effect
	
	printf("ioctl... ");
	ioctl( sound_driver, SOUND_PCM_WRITE_RATE, &SAMPLE_RATE );
	ioctl( sound_driver, SOUND_PCM_WRITE_BITS, &BITRATE );
	printf("done\n");
}
/*
void start_sound(SOUND_EFFECT type)
{
	
}*/

void play_sound(FILE *sound_file)
{
	short *buffer = malloc(BUFFER_SIZE * sizeof(short) + 1); // Allocate buffer
	
	if (buffer == NULL)
	{
		printf("Failed to allocate buffer, aborting sound");
		return;
	}
	
	int bytes_read = fread( buffer, 1, BUFFER_SIZE, sound_file );
	int bytes_written;
	
	printf("Starting sound loop");
	while ( !feof(sound_file) ) // While not at EOF
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
			printf("\nNo bytes read, BUG!\n");
		}
		
		bytes_read = fread( buffer, 1, BUFFER_SIZE, sound_file );
	}
	
	free(buffer);
	fclose(sound_file);
}

void play_music()
{
	if( !music ) return;

	printf("Playing music\n");
	
	pthread_create( &music_thread, NULL, threaded_music, (void*)0 );
	play_sound(music);
}

void sound_left_paddle()
{
	
}

void sound_right_paddle()
{
	
}

void sound_left_score()
{
	
}

void sound_right_score()
{
	
}
