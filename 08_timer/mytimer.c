#include <linux/module.h>
#include <linux/init.h>
#include <linux/gpio.h>
#include <linux/jiffies.h>
#include <linux/timer.h>

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Johannes 4 GNU/Linux");
MODULE_DESCRIPTION("A simple test for LKM's timer");

#define MY_GPIO_PIN 512

/** variable for timer */
static struct timer_list my_timer;

void timer_callback(struct timer_list * data) {
	gpio_set_value(MY_GPIO_PIN, 0); /* Turn LED off */
	printk("set GPIO%d: to 0\n",MY_GPIO_PIN);
}

/**
 * @brief This function is called, when the module is loaded into the kernel
 */
static int __init ModuleInit(void) {
	printk("Hello, Kernel!\n");
	/* GPIO 4 init */
	int ret =  gpio_request(MY_GPIO_PIN, "rpi-gpio-4");
	
        if (ret) {
     	   if (ret == -EBUSY) {
        	    pr_err("GPIO %d is already in use (Error: %d)\n", MY_GPIO_PIN, ret);
	    } else if (ret == -EINVAL) {
        	 pr_err("Invalid GPIO %d (Error: %d)\n", MY_GPIO_PIN, ret);
	    } else if (ret == -EPERM) {
           	 pr_err("Permission denied for GPIO %d (Error: %d)\n", MY_GPIO_PIN, ret);
	    } else if (ret == -ENOSYS) {
            	pr_err("GPIO subsystem not supported (Error: %d)\n", ret);
	    } else {
            	pr_err("gpio_request failed for GPIO %d with error %d\n", MY_GPIO_PIN, ret);
       	    }
       	    return ret;
  	 }

	/* Set GPIO 4 direction */
	if(gpio_direction_output(MY_GPIO_PIN, 0)) {
		printk("Can not set GPIO 4 to output!\n");
		gpio_free(4);
		return -1;
	}

	/* Turn LED on */
	gpio_set_value(MY_GPIO_PIN, 1);

	/* Initialize timer */
	timer_setup(&my_timer, timer_callback, 0);
	mod_timer(&my_timer, jiffies + msecs_to_jiffies(1000));


	return 0;
}

/**
 * @brief This function is called, when the module is removed from the kernel
 */
static void __exit ModuleExit(void) {
	printk("Goodbye, Kernel\n");
	gpio_free(MY_GPIO_PIN);
	del_timer(&my_timer);
}

module_init(ModuleInit);
module_exit(ModuleExit);


