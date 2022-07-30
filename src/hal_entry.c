/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author        Notes
 * 2021-10-10     Sherman       first version
 * 2021-11-03     Sherman       Add icu_sample
 */

#include <rtthread.h>
#include "hal_data.h"
#include <rtdevice.h>
#include <string.h>
#include <stdio.h>

#define LED3_PIN    BSP_IO_PORT_01_PIN_06
#define USER_INPUT  "P105"

#define DAC_DEV_NAME "dac0"
#define DAC_DEV_NAME2 "dac1"
#define DAC_DEV_CHANNEL 0
#define DAC_DEV_CHANNEL_1 1
static rt_dac_device_t dac_device = RT_NULL;
static rt_dac_device_t dac_device2 = RT_NULL;
// DA0 P014
// DA1 P015

void display_dac(int ch, int val)
{
    if(val > 100)
    {
        val = 100;
    }
    if (ch == 0) {
        rt_dac_write(dac_device, DAC_DEV_CHANNEL, 3723*val/100);
    }
    else if (ch == 1) {
        rt_dac_write(dac_device2, DAC_DEV_CHANNEL, 3723*val/100);
    }

}

void hal_entry(void)
{
    rt_kprintf("\nHello RT-Thread!\n");

    dac_device = (rt_dac_device_t )rt_device_find(DAC_DEV_NAME);
    dac_device2 = (rt_dac_device_t )rt_device_find(DAC_DEV_NAME2);
    if (dac_device == RT_NULL || dac_device2 == RT_NULL)
    {
       rt_kprintf("dac sample run failed! can't find %s device!\n", DAC_DEV_NAME);
    }
    else {
        rt_dac_enable(dac_device, DAC_DEV_CHANNEL);
        rt_dac_write(dac_device,0,3723/2);

        rt_dac_enable(dac_device2, DAC_DEV_CHANNEL);
        rt_dac_write(dac_device,1,3723/3);

        display_dac(0, 50);
        display_dac(1, 80);
    }

    while (1)
    {
        rt_pin_write(LED3_PIN, PIN_HIGH);
        rt_thread_mdelay(500);
        rt_pin_write(LED3_PIN, PIN_LOW);
        rt_thread_mdelay(500);
    }
}

void irq_callback_test(void *args)
{
    rt_kprintf("\n IRQ00 triggered \n");
}

void icu_sample(void)
{
    /* init */
    rt_uint32_t pin = rt_pin_get(USER_INPUT);
    rt_kprintf("\n pin number : 0x%04X \n", pin);
    rt_err_t err = rt_pin_attach_irq(pin, PIN_IRQ_MODE_RISING, irq_callback_test, RT_NULL);
    if(RT_EOK != err)
    {
        rt_kprintf("\n attach irq failed. \n");
    }
    err = rt_pin_irq_enable(pin, PIN_IRQ_ENABLE);
    if(RT_EOK != err)
    {
        rt_kprintf("\n enable irq failed. \n");
    }
}
MSH_CMD_EXPORT(icu_sample, icu sample);


void dm(int argc, char **argv)
{
    int cpu_usage, mem_usage;
    sscanf(argv[1], "%d", &cpu_usage);
    sscanf(argv[2], "%d", &mem_usage);
    rt_kprintf("CPU %d MEM %d\n", cpu_usage, mem_usage);

    display_dac(1, cpu_usage);
    display_dac(0, mem_usage);
}
MSH_CMD_EXPORT(dm, desktop monitor);


