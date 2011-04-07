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

static const int BUFFER_SIZE = 512; // Buffer size for the play_sound function

static int SAMPLE_RATE = 22050;     // Sample rate to be set by ioctl

static int BITRATE = 16;            // Bitrate to be set by ioctl

static int sound_driver;

static FILE *music;

static FILE *paddle;

void play_sound(FILE *sound_file, int music);

// Thread for the music
void *threaded_music(void *arg)
{
	play_sound(music, 1);
	return NULL;
}

int waiting = 0; // Indicates wether a sound effect is currently playing

// Thread for the sound-effect
void *threaded_effects(void *arg)
{
	while (waiting);        // Waiting for other sound-effects to finish
	waiting = 1;            // Indicate that we are playing a sound effect
	play_sound(paddle, 0);  // Play a sound effect
	waiting = 0;            // Indicate that we are done playing
	return NULL;
}

pthread_t music_thread;
pthread_t effects_thread;

void initialize_sound_driver()
{
	printf("Initializing the sound driver\n");
	sound_driver = open("/dev/dsp", O_WRONLY);  // Loading the linux sound driver

	music = fopen("09-the-moon.au", "r");       // Loading music file
	if (!music) printf("Failed to load music\n");
	
	paddle = fopen("sound46.au", "r");          // Loading sound effect
	if (!paddle) printf("Failed to load sound effect\n");
	
	printf("ioctl... ");
        // Setting the sample rate
	ioctl( sound_driver, SOUND_PCM_WRITE_RATE, &SAMPLE_RATE );
        // Setting the sample size ( This somehow made sound-files work )
	ioctl( sound_driver, SOUND_PCM_WRITE_BITS, &BITRATE );
	printf("done\n");
}
/*
void start_sound(SOUND_EFFECT type)
{
	
}*/

void play_sound(FILE *sound_file, int music)
{
        // Always seek to the start of the sound-file
	fseek( sound_file, 0, SEEK_SET );

        // Allocate buffer in memory for the data we read from the soundfile
	short *buffer = malloc(BUFFER_SIZE * sizeof(short) + 1);
	
	if (buffer == NULL)
	{
		printf("Failed to allocate buffer, aborting sound\n");
		return;
	}

        // Reading data from the file
	int bytes_read = fread( buffer, 1, BUFFER_SIZE, sound_file );
	int bytes_written;
	
	while ( !feof(sound_file) ) // While not at EOF
	{
		if ( bytes_read )
		{
                        // If we are the music and a sound-effect is playing,
                        // we wait until it is finished
			while(waiting && music);
                        // Write the sound-data to the sound-driver
			bytes_written = write( sound_driver, buffer, bytes_read );
		}
		else
		{
			printf("No bytes read, BUG!\n");
		}

                // Read more bytes from the files
		bytes_read = fread( buffer, 1, BUFFER_SIZE, sound_file );
	}

        // Free up the buffer we used earlier
	free(buffer);
}

void play_music()
{
	if( !music ) return;

	printf("Playing music\n");

        // Spawn thread playing music
	pthread_create( &music_thread, NULL, threaded_music, (void*)0 );
}

// Functions meant to play different sound-effects, but lack of time lead
// to them all just playing the same effect

void sound_left_paddle()
{
	if( !paddle ) return;

        // Spawn thread playing sound-effect
	pthread_create( &effects_thread, NULL, threaded_effects, (void*)1 );
}

void sound_right_paddle()
{
	if( !paddle ) return;

        // Spawn thread playing sound-effect
	pthread_create( &effects_thread, NULL, threaded_effects, (void*)1 );
}

void sound_left_score()
{
	if( !paddle ) return;

        // Spawn thread playing sound-effect
	pthread_create( &effects_thread, NULL, threaded_effects, (void*)1 );
}

void sound_right_score()
{
	if( !paddle ) return;

        // Spawn thread playing sound-effect
	pthread_create( &effects_thread, NULL, threaded_effects, (void*)1 );
}
