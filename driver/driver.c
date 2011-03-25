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

static volatile avr32_pio_t *pioc = &AVR32_PIOC;	//LED

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

/** This function initializes the LED lamps **/
void LED_initialize( const unsigned int bits ) 
{
	//Enable LED
	pioc->per = bits;		//Register enable
	pioc->oer = bits;		//Output enable

	//Disable leds that arent used
	pioc->pdr = ~bits;
}

/*****************************************************************************/
/* init-funksjon (kalles når modul lastes) */

static int __init driver_init (void) {

        printk( KERN_INFO "Initializing custom LED driver...\n" );

	 /* allokere device-nummer */
	//alloc_chrdev_region()

 	 /* be om tilgang til I/O-porter */
	//request_region();
  
 	 /* initialisere PIO-maskinvaren (som i øving 2) */
	LED_initialize( 0xFF );
 
  	/* registrere device i systemet (må gjøres når alt annet er initialisert) */

  	return 0;
}

/*****************************************************************************/
/* exit-funksjon (kalles når modul fjernes fra systemet) */

static void __exit driver_exit (void) {

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
  return 0;
}

/*---------------------------------------------------------------------------*/

static ssize_t driver_write (struct file *filp, const char __user *buff,
               size_t count, loff_t *offp) {
  return 0;
}

/*****************************************************************************/
/* modul-beskrivelsesmakroer */

module_init (driver_init);  /* angir hva som er init-funksjon */
module_exit (driver_exit);  /* angir hva som er exit-funksjon */

MODULE_LICENSE ("GPL");     /* programlisens for modulen */
MODULE_DESCRIPTION ("");    /* tekstlig beskrivelse */
MODULE_VERSION ("");        /* versjonsnummer */
MODULE_AUTHOR ("");         /* forfatter(e) */
