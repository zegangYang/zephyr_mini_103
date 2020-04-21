#include <stdint.h>
#include <stdio.h>
#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/gpio.h>

int64_t g_circle_count = 0;

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)

#if DT_HAS_NODE(LED0_NODE)
#define LED0	DT_GPIO_LABEL(LED0_NODE, gpios)
#define PIN	DT_GPIO_PIN(LED0_NODE, gpios)
#if DT_PHA_HAS_CELL(LED0_NODE, gpios, flags)
#define FLAGS	DT_GPIO_FLAGS(LED0_NODE, gpios)
#endif
#else
/* A build error here means your board isn't set up to blink an LED. */
#error "Unsupported board: led0 devicetree alias is not defined"
#define LED0	""
#define PIN	0
#endif

#ifndef FLAGS
#define FLAGS	0
#endif

void main(void) {
  struct device *dev = nullptr;
  bool led_is_on = true;
  int ret = -1;

  dev = device_get_binding(LED0);

  if (!dev) {
    return;
  }
  ret = gpio_pin_configure(dev, PIN, GPIO_OUTPUT_ACTIVE | FLAGS);

  if (ret < 0) {
    return;
  }
  while (1) {
    gpio_pin_set(dev, PIN, (int)led_is_on);
    led_is_on = !led_is_on;
    k_msleep(500);
  }
}