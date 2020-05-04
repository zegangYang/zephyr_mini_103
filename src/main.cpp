#include <stdint.h>
#include <stdio.h>
#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/gpio.h>
#include <drivers/pwm.h>

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

// pwm test
#if defined(DT_ALIAS_PWM_LED0_PWMS_CONTROLLER) && defined(DT_ALIAS_PWM_LED0_PWMS_CHANNEL)
/* get the defines from dt (based on alias 'pwm-led0') */
#define PWM_DRIVER	DT_ALIAS_PWM_LED0_PWMS_CONTROLLER
#define PWM_CHANNEL	DT_ALIAS_PWM_LED0_PWMS_CHANNEL
#ifdef DT_ALIAS_PWM_LED0_PWMS_FLAGS
#define PWM_FLAGS	DT_ALIAS_PWM_LED0_PWMS_FLAGS
#else
#define PWM_FLAGS	0
#endif
#else
#error "Choose supported PWM driver"
#endif

/* in microseconds */
#define MIN_PERIOD	0

/* in microseconds */
#define MAX_PERIOD	500

void main(void) {
  struct device *pwm_device = nullptr;
  uint32_t max_period;
  uint32_t period;
  uint8_t dir = 0;

  pwm_device = device_get_binding(PWM_DRIVER);

  if (!pwm_device) {
    printf("cannot find %s \n", PWM_DRIVER);
    return;
  }

  max_period = MAX_PERIOD;
  while (pwm_pin_set_usec(pwm_device, PWM_CHANNEL,
                          max_period, max_period / 2U, PWM_FLAGS)) {
    max_period /= 2U;
    if (max_period < (4U * MIN_PERIOD)) {
      printf("This sample needs to set a period that is "
             "not supported by the used PWM driver \n");
      return;
    }
  }
  period = max_period;

  int pulse = 0;

  while (1) {
    pulse = 0;
    for (int i=0; i< MAX_PERIOD; i++) {
      pulse++;
      if (pwm_pin_set_usec(pwm_device, PWM_CHANNEL,
                           period, pulse, PWM_FLAGS)) {
        printk("pwm pin set fails\n");
        return;
      }
      k_msleep(5);
    }
    for (int i=0; i< MAX_PERIOD; i++) {
      pulse--;
      if (pwm_pin_set_usec(pwm_device, PWM_CHANNEL,
                           period, pulse, PWM_FLAGS)) {
        printk("pwm pin set fails\n");
        return;
      }
      k_msleep(5);
    }
    // printf("circle once \n");
  }
}
void main1(void) {
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
    printf("hello world\n");
  }
}