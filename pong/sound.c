#include <fcntl.h>			//for open()
#include <stdlib.h>
#include <stdio.h>			//for printf()
#include <string.h>			//for memcpy()
#include <unistd.h>			//For sleep
#include <time.h>			//For random seed

#include <linux/soundcard.h>
#include <sys/ioctl.h>

#include "driver_interface.h"
#include "sound.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
 
#define NUM_THREADS     5
 
void *TaskCode(void *argument)
{
   int tid;
 
   tid = *((int *) argument);
   printf("Hello World! It's me, thread %d!\n", tid);
 
   /* optionally: insert more useful stuff here */
 
   return NULL;
}
 
int test()
{
   pthread_t threads[NUM_THREADS];
   int thread_args[NUM_THREADS];
   int rc, i;
 
   /* create all threads */
   for (i=0; i<NUM_THREADS; ++i) {
      thread_args[i] = i;
      printf("In main: creating thread %d\n", i);
      rc = pthread_create(&threads[i], NULL, TaskCode, (void *) &thread_args[i]);
      assert(0 == rc);
   }
 
   /* wait for all threads to complete */
   for (i=0; i<NUM_THREADS; ++i) {
      rc = pthread_join(threads[i], NULL);
      assert(0 == rc);
   }
 
   exit(EXIT_SUCCESS);
}

static const int BUFFER_SIZE = 2048;

static const int LOWER_VOLUME = 1;

static int SAMPLE_RATE = 22050;

static int BITRATE = 16;

static FILE *music;

static int sound_driver;

void initialize_sound_driver()
{
	printf("Initializing the sound driver\n");
	sound_driver = open("/dev/dsp", O_WRONLY);

	music = fopen("09-the-moon.au", "r");
	
	printf("ioctl... ");
	ioctl( sound_driver, SOUND_PCM_WRITE_RATE, &SAMPLE_RATE );
	ioctl( sound_driver, SOUND_PCM_WRITE_BITS, &BITRATE );
	printf("done\n");

	test();
}

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
			printf("\nNo bytes read, BUG!\n");
		}
		
		bytes_read = fread( buffer, 1, BUFFER_SIZE, sound_file );
		usleep(100);
	}
	
	free(buffer);
	fclose(sound_file);
}

void play_music()
{
	printf("Playing music\n");
	play_sound(music);
}
