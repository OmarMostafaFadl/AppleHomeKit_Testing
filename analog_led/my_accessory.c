/*
 * my_accessory.c
 * Define the accessory in C language using the Macro in characteristics.h
 *
 *  Created on: 2020-05-15
 *      Author: Mixiaoxiao (Wang Bin)
 */

#include <Arduino.h>
#include <homekit/types.h>
#include <homekit/homekit.h>
#include <homekit/characteristics.h>
#include <stdio.h>
#include <port.h>

#define PIN_LED  12 // D6

int led_bri = 100; // [0, 100]
bool led_power = false; // true or flase

// Called to identify this accessory. See HAP section 6.7.6 Identify Routine
// Generally this is called when paired successfully or click the "Identify Accessory" button in Home APP.
void my_accessory_identify(homekit_value_t _value) {
  printf("accessory identify\n");
  for (int j = 0; j < 3; j++) {
    led_power = true;
    led_update();
    delay(100);
    led_power = false;
    led_update();
    delay(100);
  }
}
// led_power is a boolean for led status
// led_bri is an int for brightness %
void led_update() {
  if (led_power) { 
    int pwm = (int) (led_bri * 1.0 * PWMRANGE / 100.0 + 0.5f); // 0->100 to 0->PWMRANGE (+0.5 for some reason)
    analogWrite(PIN_LED, pwm);
    printf("ON  %3d (pwm: %4d of %d)\n", led_bri, pwm, PWMRANGE);
  } else {
    printf("OFF\n");
    digitalWrite(PIN_LED, LOW);
  }
}

void accessory_init() {
  pinMode(PIN_LED, OUTPUT);
  led_update();
}
// callback for led digital getter
homekit_value_t led_on_get() { // return led status, typecasted with homekit-boolean datatype
  return HOMEKIT_BOOL(led_power);
}
// callback for led digital setter
void led_on_set(homekit_value_t value) { // sets led to on, if brightness not set, will give 100% brightness
 if (value.format != homekit_format_bool) {
    printf("Invalid on-value format: %d\n", value.format);
    return;
  }
  led_power = value.bool_value;
  if (led_power) {
    if (led_bri < 1) {
      led_bri = 100;
    }
  }
  led_update();
}
// callback for led analog getter
homekit_value_t light_bri_get() {
  return HOMEKIT_INT(led_bri);
}
// callback for led analog setter
void led_bri_set(homekit_value_t value) {
  if (value.format != homekit_format_int) {
    return;
  }
  led_bri = value.int_value;
  led_update();
}

homekit_accessory_t *accessories[] = { // container of accessories.
    HOMEKIT_ACCESSORY(.id=1, .category=homekit_accessory_category_lightbulb, .services=(homekit_service_t*[]) { // initialize accesory with ~id=1,
    // choose the right category, can be found in notes. not sure if it matters or not.    
        HOMEKIT_SERVICE(ACCESSORY_INFORMATION, .characteristics=(homekit_characteristic_t*[]) { // identify sensor info bardo
            HOMEKIT_CHARACTERISTIC(NAME, "LED Light"),
            HOMEKIT_CHARACTERISTIC(MANUFACTURER, "Arduino HomeKit"),
            HOMEKIT_CHARACTERISTIC(SERIAL_NUMBER, "0123456"),
            HOMEKIT_CHARACTERISTIC(MODEL, "ESP8266/ESP32"),
            HOMEKIT_CHARACTERISTIC(FIRMWARE_REVISION, "1.0"),
            HOMEKIT_CHARACTERISTIC(IDENTIFY, my_accessory_identify),
            NULL
        }),
        HOMEKIT_SERVICE(LIGHTBULB, .primary=true, .characteristics=(homekit_characteristic_t*[]) { // add bulb servise
            HOMEKIT_CHARACTERISTIC(NAME, "Led"),
            HOMEKIT_CHARACTERISTIC(BRIGHTNESS, 100, .getter=light_bri_get, .setter=led_bri_set), // callback functions for the brightness (analog) characteristic
            HOMEKIT_CHARACTERISTIC(ON, false,.getter=led_on_get,.setter=led_on_set), // callback functions for the status (digital) characteristic
            NULL
        }),
        NULL
    }),
    NULL
};

homekit_server_config_t config = { // configuration object
    .accessories = accessories, // accessories object with all the accessories added.
    .password = "111-11-111" // password for home app
};
