//
// Created by Otto Winter on 22.01.18.
//

#ifndef ESPHOMELIB_ESPHAL_H
#define ESPHOMELIB_ESPHAL_H

#ifdef ARDUINO_ARCH_ESP32
  #include <esp32-hal.h>
#endif
#ifdef ARDUINO_ARCH_ESP8266
  #include "Arduino.h"
#endif
#include "esphomelib/espmath.h"

#ifdef bool
  #undef bool
#endif
#ifdef true
  #undef true
#endif
#ifdef false
  #undef false
#endif

namespace esphomelib {

/** A high-level abstraction class that can expose a pin together with useful options like pinMode.
 *
 * Set the parameters for this at construction time and use setup() to apply them. The inverted parameter will
 * automatically invert the input/output for you.
 *
 * Use read_value() and write_value() to use digitalRead() and digitalWrite(), respectively.
 */
class GPIOPin {
 public:
  /** Construct the GPIOPin instance.
   *
   * @param pin The GPIO pin number of this instance.
   * @param mode The Arduino pinMode that this pin should be put into at setup().
   * @param inverted Whether all digitalRead/digitalWrite calls should be inverted.
   */
  GPIOPin(uint8_t pin, uint8_t mode, bool inverted = false);

  /// Default constructor so that it can be stored in optionals
  GPIOPin();

  virtual GPIOPin *copy() const;;

  /// Setup the pin mode.
  virtual void setup() const;
  /// Read the binary value from this pin using digitalRead (and inverts automatically).
  virtual bool digital_read() const;
  /// Write the binary value to this pin using digitalWrite (and inverts automatically).
  virtual void digital_write(bool value) const;

  /// Set the inverted mode of this pin.
  void set_inverted(bool inverted);
  /// Set the GPIO pin number.
  void set_pin(uint8_t pin);
  /// Set the pinMode of this pin.
  void set_mode(uint8_t mode);

  /// Get the GPIO pin number.
  uint8_t get_pin() const;
  /// Get the pinMode of this pin.
  uint8_t get_mode() const;
  /// Return whether this pin shall be treated as inverted. (for example active-low)
  bool is_inverted() const;

 protected:
  uint8_t pin_;
  uint8_t mode_;
  bool inverted_;
};

/**  Basically just a GPIOPin, but defaults to OUTPUT pinMode.
 *
 * Note that theoretically you can still assign an INPUT pinMode to this - we intentionally don't check this.
 *
 * The constructor also allows for easy usage because of it's lack of "explicit" constructor.
 */
class GPIOOutputPin : public GPIOPin {
 public:
  GPIOOutputPin();

  GPIOOutputPin(uint8_t pin, uint8_t mode = OUTPUT, bool inverted = false); // NOLINT
};

/** Basically just a GPIOPin, but defaults to INPUT pinMode.
 *
 * Note that theoretically you can still assign an OUTPUT pinMode to this - we intentionally don't check this.
 *
 * The constructor also allows for easy usage because of it's lack of "explicit" constructor.
 */
class GPIOInputPin : public GPIOPin {
 public:
  GPIOInputPin();

  GPIOInputPin(uint8_t pin, uint8_t mode = INPUT, bool inverted = false); // NOLINT
};

} // namespace esphomelib

#endif //ESPHOMELIB_ESPHAL_H
