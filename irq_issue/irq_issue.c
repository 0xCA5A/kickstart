#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>

#include <linux/interrupt.h>
#include <linux/irq.h>

#include <linux/delay.h>
#include <linux/gpio.h>

#include <asm/mach-ath79/ar71xx_regs.h>

extern void __iomem *ath79_gpio_base;
short int g_irq_gpio = 0;

#define DEBUG_PIN_ID		22

#define IRQ_SOURCE_PIN_ID	11
#define IRQ_GPIO_DESC           "irq source"
#define IRQ_GPIO_DEVICE_DESC    "device description"

static irqreturn_t irq_handler(int irq, void *dev_id)
{
	static u32 last_pin_level = 0xDEADBEEF;
	u32 pin_level;
	unsigned long irq_flags;
	u32 gpio_input_value;

	// disable hard interrupts (remember them in flag 'flags')
	local_irq_save(irq_flags);

	// GPIO interrupt trigger is configured as IRQF_TRIGGER_RISING
	// if handler is called, set and clear pin
	// if handler is called, check if pin level is high and set / reset pin (workaround)

	// this results in two pulses on the GPIO if the pin level is high (rising edge detected)

	gpio_set_value(DEBUG_PIN_ID, 1);
	gpio_set_value(DEBUG_PIN_ID, 0);

#if 0
	usleep(5);
#endif

	//FIXME: dirty workaround to detect rising edge due to problems with interrups triggered on rising and falling edge
	gpio_input_value = __raw_readl(ath79_gpio_base + AR71XX_GPIO_REG_IN);

#if 1
	printk("DEBUG: gpio_input_value: 0x%x\n", gpio_input_value);

	pin_level =
	    (gpio_input_value & (0x1 << IRQ_SOURCE_PIN_ID)) >>
	    IRQ_SOURCE_PIN_ID;

	printk("DEBUG: rising? %d\n", pin_level);

	// check if GPIO signal is high (bit n is 1)
	if (pin_level) {
		// toggle pin
		gpio_set_value(DEBUG_PIN_ID, 1);
		gpio_set_value(DEBUG_PIN_ID, 0);

	}
	// first level init
	if (last_pin_level == 0xDEADBEEF) {
		last_pin_level = pin_level;
		goto restore_irq_and_exit;
	}
	// compare irq values, detect loss
	if (pin_level == last_pin_level) {
		printk
		    ("DEBUG: irq loss detected! last level was %d, %d expected\n",
		     last_pin_level, ~last_pin_level & 0x00000001);

	}
	last_pin_level = pin_level;

#endif

#if 1

	printk("register dump:\n");

#define REGISTER_OFFSET 0x14
	printk("@%p: 0x%x\n", ath79_gpio_base + REGISTER_OFFSET, (unsigned int)
	       __raw_readl(ath79_gpio_base + REGISTER_OFFSET));
#undef REGISTER_OFFSET

#define REGISTER_OFFSET 0x18
	printk("@%p: 0x%x\n", ath79_gpio_base + REGISTER_OFFSET, (unsigned int)
	       __raw_readl(ath79_gpio_base + REGISTER_OFFSET));
#undef REGISTER_OFFSET

#define REGISTER_OFFSET 0x1C
	printk("@%p: 0x%x\n", ath79_gpio_base + REGISTER_OFFSET, (unsigned int)
	       __raw_readl(ath79_gpio_base + REGISTER_OFFSET));
#undef REGISTER_OFFSET

#endif

 restore_irq_and_exit:
	local_irq_restore(irq_flags);

	return IRQ_HANDLED;
}

/****************************************************************************/
/* configure GPIO interrupt                                                 */
/****************************************************************************/
inline int init_irq(uint8_t gpio_id)
{
	if ((g_irq_gpio = gpio_to_irq(gpio_id)) < 0) {
		printk("GPIO to IRQ mapping faiure for GPIO%d: %s\n",
		       gpio_id, IRQ_GPIO_DESC);
		return -2;
	}
	printk("successfully mapped GPIO%d to IRQ\n", gpio_id);

	if (request_irq(g_irq_gpio,
			(irq_handler_t) irq_handler,
			IRQF_TRIGGER_RISING,
			IRQ_GPIO_DESC, IRQ_GPIO_DEVICE_DESC)) {
		printk("IRQ request failure for GPIO%d\n", gpio_id);
		return -3;
	}

	return 0;
}

static int __init irq_latency_init(void)
{
	int err;

	//unsigned long flags;
	printk("irq latency init\n");

	gpio_request(IRQ_SOURCE_PIN_ID, "irq_source");
	gpio_direction_input(IRQ_SOURCE_PIN_ID);
	gpio_export(IRQ_SOURCE_PIN_ID, true);

	gpio_request(DEBUG_PIN_ID, "debug_pin");
	gpio_direction_output(DEBUG_PIN_ID, 0);
	gpio_export(DEBUG_PIN_ID, true);

	/*init gpio irq */
	err = init_irq(IRQ_SOURCE_PIN_ID);
	if (err)
		goto init_fail;

	/* graceful init */
	return 0;

 init_fail:

	return err;
}

static void __exit irq_latency_cleanup(void)
{
	free_irq(g_irq_gpio, IRQ_GPIO_DEVICE_DESC);
	gpio_free(IRQ_SOURCE_PIN_ID);

	gpio_free(DEBUG_PIN_ID);

	printk("signing out\n");
}

module_init(irq_latency_init);
module_exit(irq_latency_cleanup);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("010100010101");
