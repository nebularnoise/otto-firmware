#pragma once

#include <Arduino.h>
//#include <initializer_list>

namespace otto_mcu {

  template<typename... Args>
  void log(const char* msg, Args... args)
  {
    char buffer[256];
    sprintf(buffer, msg, args...);
    Serial.println(buffer);
    Serial.flush();
  }

  inline void wait_for_input()
  {
    while (Serial.available() == 0)
      ;
    Serial.read();
  }

  struct GPIOPin {
    gpio_dev* port = GPIOA;
    uint8_t pin = 0;
    bool invert = false;

    GPIOPin(gpio_dev* port, uint8_t pin, bool invert = false) : port(port), pin(pin), invert(invert)
    {}

    void set_output(bool val = false) const
    {
      gpio_set_mode(port, pin, GPIO_OUTPUT_PP);
      write(val);
    }

    void set_input() const
    {
      gpio_set_mode(port, pin, GPIO_INPUT_PD);
    }

    bool read() const
    {
      return invert ^ gpio_read_bit(port, pin);
    }

    void write(bool b) const
    {
      gpio_write_bit(port, pin, (b ^ invert) ? HIGH : LOW);
    }

    void toggle() const
    {
      gpio_toggle_bit(port, pin);
    }

    void flash(int time)
    {
      write(true);
      log("FLASH ON");
      delay(time);
      write(false);
      log("FLASH OFF");
      delay(time);
    }
  };

} // namespace otto_mcu
