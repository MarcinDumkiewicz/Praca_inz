#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Derek Molloy");
MODULE_DESCRIPTION("A Button/LED test driver for the BBB");
MODULE_VERSION("0.1");

static unsigned int gpioLED =25;
static unsigned int gpioButton = 21;
static unsigned int irqNumber;
static unsigned int numberPresses = 0;
static bool ledOn = 0;
static irq_handler_t ebbgpio_irq_handler(unsigned int irq, void *dev_id, struct pt_regs *regs);

static int __init ebbgpio_init(void)
{
	int result = 0;
	printk(KERN_INFO "GPIO_TEST: Initializing the GPIO_TEST LKM\n");
	if (!gpio_is_valid(gpioLED))
	{
		printk(KERN_INFO "GPIO_TEST: invalid LED GPIO\n");
		return -ENODEV;
	}
	
	ledOn = true;
	gpio_request(gpioLED, "sysfs");
	gpio_direction_output(gpioLED, ledOn);
	gpio_export(gpioLED, false);
	gpio_request(gpioButton, "sysfs");
	gpio_direction_input(gpioButton);
	gpio_set_debounce(gpioButton, 200);
	gpio_export(gpioButton, false);
	printk(KERN_INFO "GPIO_TEST: The button state is currently: %d\n", gpio_get_value(gpioButton));
	irqNumber = gpio_to_irq(gpioButton);
	printk(KERN_INFO "GPIO_TEST: The button is mapped to IRQ: %d\n", irqNumber);
	result = request_irq(irqNumber,
	(irq_handler_t) ebbgpio_irq_handler,
	IRQF_TRIGGER_RISING,
	"ebb_gpio_handler",
	NULL);
	printk(KERN_INFO "GPIO_TEST: The interrupt request result is: %d\n", result);
	return result;
}

static void __exit ebbgpio_exit(void)
{
	printk(KERN_INFO "GPIO_TEST: The button state is currently: %d\n", gpio_get_value(gpioButton));
	printk(KERN_INFO "GPIO_TEST: The button was pressed %d times\n", numberPresses);
	GPIO     gpio_set_value(gpioLED, 0);
	GPIO     gpio_unexport(gpioLED);
	free_irq(irqNumber, NULL);
	gpio_unexport(gpioButton);
	gpio_free(gpioLED);
	gpio_free(gpioButton);
	printk(KERN_INFO "GPIO_TEST: Goodbye from the LKM!\n");
}

static irq_handler_t ebbgpio_irq_handler(unsigned int irq, void *dev_id, struct pt_regs *regs)
{
	ledOn = !ledOn; 
	gpio_set_value(gpioLED, ledOn);
	printk(KERN_INFO "GPIO_TEST: Interrupt! (button state is %d)\n", gpio_get_value(gpioButton));
	numberPresses++; 
	return (irq_handler_t) IRQ_HANDLED;
}

module_init(ebbgpio_init);
module_exit(ebbgpio_exit);
