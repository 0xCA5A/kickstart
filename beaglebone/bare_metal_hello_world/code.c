#define uint32_t unsigned int


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


int c_entry(void)
{
    //enable the GPIO module
    //CM_PER Registers -> CM_PER_GPIO1_CLKCTRL
    write32((0x2 << 0) | (1 << 18), (uint32_t *)(0x44e00000 + 0xac));

    //GPIO1 -> GPIO_IRQSTATUS_CLR_0 (3Ch)
    setbits_le32((uint32_t *)(0x4804c000 + 0x3c), 0xf << 21);

    //enable output
    //GPIO1 -> GPIO_OE (134h)
    clrbits_le32((uint32_t *)(0x4804c000 + 0x134), 0xf << 21);

    //clear led gpio values to 0
    //GPIO1 -> GPIO_DATAOUT (13ch)
    clrbits_le32((uint32_t *)(0x4804c000 + 0x13c), 0xf << 21);

    //set led gpio values to 1
    //GPIO1 -> GPIO_DATAOUT (13ch)
    setbits_le32((uint32_t *)(0x4804c000 + 0x13c), 0x5 << 21);

    return 0;
}
