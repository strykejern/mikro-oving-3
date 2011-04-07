/*****************************************************************************
 *
 * Øving 3 uCSysDes, driverkoden
 *
 *****************************************************************************/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/ioport.h>
#include <asm/io.h>
#include <asm/uaccess.h>

#include "ap7000.h"

/* prototyper */
static const char * DRIVER_NAME = "stk1000_driver";
static const int DRIVER_MAJOR = 1;
static const int DRIVER_MINOR = 0;

static volatile avr32_pio_t *piob = &AVR32_PIOB;	//LED

static int  __init driver_init  (void);
static void __exit driver_exit  (void);
static int  driver_open         (struct inode *inode, struct file *filp);
static int  driver_release      (struct inode *inode, struct file *filp);
static int  driver_read         (struct file *filp, char __user *buff,
                                    size_t count, loff_t *offp);
static int  driver_write        (struct file *filp, const char __user *buff,
                                    size_t count, loff_t *offp);

/* fops-struct */

static struct file_operations driver_fops = {
  .owner    = THIS_MODULE,
  .read     = driver_read,
  .write    = driver_write,
  .open     = driver_open,
  .release  = driver_release
};

/** Use this function to enable the LED specified in the BITFIELD **/
void LED_set_enabled( const unsigned int bits ) 
{
	piob->codr &= 0xFFFFFF00;   // Reset parts we are going to use
	piob->codr |= ~bits;        // Disable the other LEDs
	
	piob->sodr &= 0xFFFFFF00;   // Reset parts we are going to use
	piob->sodr |= bits;         // Enable the selected LEDs
}

/** This function initializes the LED lamps **/
void LED_initialize( void ) 
{
	//Enable LED
	piob->per &= 0xFFFFFF00;    // Reset parts we are going to use
	piob->per  = 0xFF;          // Enable the LED pins
	piob->oer &= 0xFFFFFF00;    // Reset parts we are going to use
	piob->oer  = 0xFF;          // Set LED pins to output

	//Disable leds that arent used
	piob->pdr &= 0xFF;
	piob->pdr = ~0xFF;
}

void BUTTONS_initialize( void )
{
	int dsbl = 0x000000FF;      // The pins we are not going to use
	int enbl = 	0x100 +
		   	0x200 +
			0x400 +
			0x2000 +
			0x4000 +
			0x8000 +
			0x10000 +
			0x40000000; // Adding all the button pins' addresses
	
	piob->odr &= dsbl;          // Reset parts we are going to use
	piob->odr |= enbl;          // Disable output on buttons

        //Enable switches
	piob->per &= dsbl;          // Reset parts we are going to use
	piob->per |= enbl;          // Enable button pins
	
	piob->puer &= dsbl;         // Reset parts we are going to use
	piob->puer |= enbl;         // Enable pull-up on button pins

        //Disable everything that isn't enabled
	piob->pdr = 0x0;
	piob->pdr = (~enbl) & (~0xFF); // Considering both LEDs and buttons
}


// init-function (called on module initiation)
static struct cdev *device;

static int __init driver_init (void) {
	int success;

	device = cdev_alloc(); // Allocate memory space

        printk( KERN_INFO "Initializing driver %s...\n", DRIVER_NAME );

        // Allocating region
	success = alloc_chrdev_region( &device->dev, DRIVER_MINOR, 1, DRIVER_NAME);

	// Allocating device number
	if( 0 > success )
	{
		printk( KERN_WARNING "FAILED! could not allocate a major number\n" );
		return success;
	}

	printk( KERN_INFO "%s allocated - Major: %i, Minor: %i\n", DRIVER_NAME, MAJOR(device->dev), MINOR(device->dev) );


 	// Request I/O access on PIOB
	if( NULL == request_region( AVR32_PIOB_ADDRESS, 0x78, DRIVER_NAME ) )
	{
		printk( KERN_WARNING "FAILED! could not request region\n" );
	}
	else
	{
		printk( KERN_INFO "Success! managed to request the region\n" );
	}
  
 	// Initialization of the I/O as done in Exercise 2
	LED_initialize(  );
	LED_set_enabled( 0xAA );
	
	BUTTONS_initialize(  );
 

  	// Registering the device in the system
	device->ops = &driver_fops;
	device->owner = THIS_MODULE;
        
	success = cdev_add(device, device->dev, 1);
	if( success < 0 )
	{
		printk( KERN_WARNING "FAILED! could not register device\n" );
		return -1;
	}
	
	printk( KERN_INFO "Success! Finished loading driver\n" );

  	return success;
}

static void __exit driver_exit (void) {
	cdev_del( device );
}

static int driver_open (struct inode *inode, struct file *filp) {
  return 0;
}

static int driver_release (struct inode *inode, struct file *filp) {
  return 0;
}

static ssize_t driver_read (struct file *filp, char __user *buff,
              size_t count, loff_t *offp) {
	
	int buttons = 0;

        // Checking pins and adding together to form a 8bit binary
        // representation of the buttons like they are oriented on the board
	if ( (~piob->pdsr) & (0x100) )      buttons += 1;
	if ( (~piob->pdsr) & (0x200) )      buttons += 2;
	if ( (~piob->pdsr) & (0x400) )      buttons += 4;
	if ( (~piob->pdsr) & (0x2000) )     buttons += 8;
	if ( (~piob->pdsr) & (0x4000) )     buttons += 16;
	if ( (~piob->pdsr) & (0x8000) )     buttons += 32;
	if ( (~piob->pdsr) & (0x10000) )    buttons += 64;
	if ( (~piob->pdsr) & (0x40000000) ) buttons += 128;
	
	// Send the button status data to the user/program
	copy_to_user( buff, &buttons, sizeof(buttons) );
	
  	return 0;
}

static ssize_t driver_write (struct file *filp, const char __user *buff,
               size_t count, loff_t *offp) {
	
	int input;

        // Reading input data from user/program
	if ( copy_from_user( &input, buff, sizeof(buff) ) )
	{
		printk( KERN_WARNING "Invalid user input" );
		return 0;
	}

	LED_set_enabled( input ); // Enable the LEDs according to the input

	return 0;
}

module_init( driver_init );      // Sets the init function
module_exit( driver_exit );      // Sets the exit function

// Self-explanatory extra information on the module
MODULE_LICENSE      ( "GPL" );
MODULE_DESCRIPTION  ( "STK1000 LED and Button Drivers" );
MODULE_VERSION      ( "1.00" );
MODULE_AUTHOR       ( "Johan Jansen and Anders Eie" );

