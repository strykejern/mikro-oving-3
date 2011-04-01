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

static int __init driver_init(void);
static void __exit driver_exit(void);
static int driver_open (struct inode *inode, struct file *filp);
static int driver_release (struct inode *inode, struct file *filp);
static int driver_read (struct file *filp, char __user *buff,
                     size_t count, loff_t *offp);
static int driver_write (struct file *filp, const char __user *buff,
                      size_t count, loff_t *offp);

/* fops-struct */

static struct file_operations driver_fops = {
  .owner = THIS_MODULE,
  .read = driver_read,
  .write = driver_write,
  .open = driver_open,
  .release = driver_release
};

/** Use this function to enable the LED specified in the BITFIELD **/
void LED_set_enabled( const unsigned int bits ) 
{
	piob->codr &= 0xFF00;
	piob->codr |= ~bits;
	
	piob->sodr &= 0xFF00;
	piob->sodr |= bits;
}

/** This function initializes the LED lamps **/
void LED_initialize( const unsigned char bits ) 
{
	
	//Enable LED
	piob->per &= 0xFF00;
	piob->per |= bits;		//Register enable
	piob->oer &= 0xFF00;
	piob->oer |= bits;		//Output enable

	//Disable leds that arent used
	piob->pdr &= 0x00FF;
	piob->pdr |= ~bits;
}

void BUTTONS_initialize( void )
{
	int dsbl = 0x000000FF;
	int enbl = 0x4001E700;
	
	piob->odr &= dsbl;
	piob->odr |= enbl;               //Disable output on buttons

        //Enable switches
	piob->per &= dsbl;
	piob->per |= dsbl;               //Register enable
	
	piob->puer &= dsbl;
	piob->puer |= enbl;              //Pullup enable

        //Disable everything that isn't enabled
	piob->pdr &= dsbl;
	piob->pdr |= ~enbl - 0xFF;
	
	piob->idr &= dsbl;
	piob->idr |= ~enbl - 0xFF;
}

/*****************************************************************************/
/* init-funksjon (kalles når modul lastes) */
static struct cdev *device;

static int __init driver_init (void) {
	int success;

	//get memory space
	device = cdev_alloc();

        printk( KERN_INFO "Initializing driver %s...\n", DRIVER_NAME );

	success = alloc_chrdev_region( &device->dev, DRIVER_MINOR, 1, DRIVER_NAME);

	 /* allokere device-nummer */
	if( 0 > success )
	{
		printk( KERN_WARNING "FAILED! could not allocate a major number\n" );
		return success;
	}

	printk( KERN_INFO "%s allocated - Major: %i, Minor: %i\n", DRIVER_NAME, MAJOR(device->dev), MINOR(device->dev) );


 	 /* be om tilgang til I/O-porter */
	if( NULL == request_region( AVR32_PIOB_ADDRESS, 0x78, DRIVER_NAME ) )
	{
		printk( KERN_WARNING "FAILED! could not request region\n" );
	}
	else
	{
		printk( KERN_INFO "Success! managed to request the region\n" );
	}
  
 	 /* initialisere PIO-maskinvaren (som i øving 2) */
	LED_initialize( 0xFF );
	LED_set_enabled( 0xAA );
	
	BUTTONS_initialize(  );
 

  	/* registrere device i systemet (må gjøres når alt annet er initialisert) */
	//register_chrdev_region( device, 1, DRIVER_NAME )
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

/*****************************************************************************/
/* exit-funksjon (kalles når modul fjernes fra systemet) */

static void __exit driver_exit (void) {
	cdev_del( device );
	//unregister_chrdev_region( device->dev, DRIVER_MINOR );
	//release_region( device->dev, 0x78 );
}

/*****************************************************************************/
/* fops-funksjoner */

static int driver_open (struct inode *inode, struct file *filp) {
  return 0;
}

/*---------------------------------------------------------------------------*/

static int driver_release (struct inode *inode, struct file *filp) {
  return 0;
}

/*---------------------------------------------------------------------------*/

static ssize_t driver_read (struct file *filp, char __user *buff,
              size_t count, loff_t *offp) {
	
	int buttons = 0;
	
	if ( (~piob->pdsr) & 1 ) buttons += 1;
	if ( (~piob->pdsr) & 2 ) buttons += 2;
	if ( (~piob->pdsr) & 4 ) buttons += 4;
	if ( (~piob->pdsr) & 32 ) buttons += 8;
	if ( (~piob->pdsr) & 64 ) buttons += 16;
	if ( (~piob->pdsr) & 128 ) buttons += 32;
	if ( (~piob->pdsr) & 256 ) buttons += 64;
	if ( (~piob->pdsr) & 536870912 ) buttons += 128;
	
	
	copy_to_user( buff, &buttons, sizeof(buttons) );
	
  	return 0;
}

/*---------------------------------------------------------------------------*/

static ssize_t driver_write (struct file *filp, const char __user *buff,
               size_t count, loff_t *offp) {
	
	int input;

	if ( copy_from_user( &input, buff, sizeof(buff) ) )
	{
		printk( KERN_WARNING "Invalid user input" );
		return 0;
	}

	LED_set_enabled( input );

	return 0;
}

/*****************************************************************************/
/* modul-beskrivelsesmakroer */

module_init (driver_init);  /* angir hva som er init-funksjon */
module_exit (driver_exit);  /* angir hva som er exit-funksjon */

MODULE_LICENSE ("GPL");     /* programlisens for modulen */
MODULE_DESCRIPTION ("STK1000 LED and Button Drivers");    /* tekstlig beskrivelse */
MODULE_VERSION ("1.00");        /* versjonsnummer */
MODULE_AUTHOR ("Johan Jansen and Anders Eie");         /* forfatter(e) */

