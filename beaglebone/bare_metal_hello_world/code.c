#define uint32_t        unsigned int

//four leds, 0x0 - 0xf
#define LED_PATTERN     0x1

static inline void write32(uint32_t val, const void *addr)
{
    *(volatile uint32_t *)addr = val;
}


static inline void clrbits_le32(const void* addr, uint32_t pattern)
{
    uint32_t register_value = *(volatile uint32_t *) addr;
    register_value = register_value & ~pattern;
    *(volatile uint32_t *)addr = register_value;
}


static inline void setbits_le32(const void* addr, uint32_t pattern)
{
    uint32_t register_value = *(volatile uint32_t *) addr;
    register_value = register_value | pattern;
    *(volatile uint32_t *)addr = register_value;
}


static void __attribute__((optimize("O0"))) dummy_wait(uint32_t nr_of_nops)
{
    // compiler optimizations disabled for this function using __attribute__,
    // pragma is as well possible to protect bad code...
    // http://stackoverflow.com/questions/2219829/how-to-prevent-gcc-optimizing-some-statements-in-c

    // i need this statement that the bad code below is not optimized away
    asm("");

    uint32_t counter = 0;
    for (; counter < nr_of_nops; ++counter)
    {
        ;
    }
}


static inline void init_led_output()
{
    //enable the GPIO module
    //CM_PER Registers -> CM_PER_GPIO1_CLKCTRL
    write32((0x2 << 0) | (1 << 18), (uint32_t *)(0x44e00000 + 0xac));

    //GPIO1 -> GPIO_IRQSTATUS_CLR_0 (3Ch)
    setbits_le32((uint32_t *)(0x4804c000 + 0x3c), 0xf << 21);

    //enable output
    //GPIO1 -> GPIO_OE (134h)
    clrbits_le32((uint32_t *)(0x4804c000 + 0x134), 0xf << 21);
}


static inline void dummy_blink_forever(void)
{
    for (;;)
    {
        //clear led gpio values to 0
        //GPIO1 -> GPIO_DATAOUT (13ch)
        clrbits_le32((uint32_t *)(0x4804c000 + 0x13c), 0xf << 21);

        dummy_wait(0x002BABE5);

        //set led gpio values to 1
        //GPIO1 -> GPIO_DATAOUT (13ch)
        setbits_le32((uint32_t *)(0x4804c000 + 0x13c), LED_PATTERN << 21);

        dummy_wait(0x002BABE5);
    }
}


static inline void heartbeat_forever(void)
{
    for (;;)
    {
        //on
        setbits_le32((uint32_t *)(0x4804c000 + 0x13c), LED_PATTERN << 21);
        dummy_wait(0x007ABE5);

        //off
        clrbits_le32((uint32_t *)(0x4804c000 + 0x13c), LED_PATTERN << 21);
        dummy_wait(0x005BBE5);

        //on
        setbits_le32((uint32_t *)(0x4804c000 + 0x13c), LED_PATTERN << 21);
        dummy_wait(0x003ABE5);

        //off
        clrbits_le32((uint32_t *)(0x4804c000 + 0x13c), LED_PATTERN << 21);
        dummy_wait(0x0024ABE5);
    }
}


int c_entry(void)
{
    init_led_output();
    heartbeat_forever();

    return 0;
}
