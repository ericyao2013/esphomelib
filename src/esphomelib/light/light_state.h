//
// Created by Otto Winter on 28.11.17.
//

#ifndef ESPHOMELIB_LIGHT_LIGHT_STATE_H
#define ESPHOMELIB_LIGHT_LIGHT_STATE_H

#include <memory>
#include <functional>
#include <vector>
#include "esphomelib/light/light_color_values.h"

#include "esphomelib/light/light_effect.h"
#include "esphomelib/light/light_transformer.h"
#include "esphomelib/component.h"
#include "esphomelib/helpers.h"
#include "esphomelib/defines.h"

#ifdef USE_LIGHT

namespace esphomelib {

namespace light {

using light_send_callback_t = std::function<void()>;

class LightEffect;

/** This class represents the communication layer between the front-end MQTT layer and the
 * hardware output layer.
 */
class LightState : public Nameable, public Component {
 public:
  /// Construct this LightState using the provided traits and name.
  explicit LightState(const std::string &name, const LightTraits &traits);

  /** Start an effect by name. Uses light_effect_entries in light_effect.h for finding the correct effect.
   *
   * @param name The name of the effect, case insensitive.
   */
  void start_effect(const std::string &name);

  /// Stop the current effect (if one is active).
  void stop_effect();

  /// Load state from preferences
  void setup() override;
  /// Shortly after HARDWARE.
  float get_setup_priority() const override;

  /** Start a linear transition to the provided target values for a specific amount of time.
   *
   * If this light doesn't support transition (see set_traits()), sets the target values immediately.
   *
   * @param target The target color.
   * @param length The transition length in ms.
   */
  void start_transition(const LightColorValues &target, uint32_t length);

  /** Start a flash for the specified amount of time.
   *
   * Resets to the values that were active when the flash was started after length ms.
   *
   * @param target The target flash color.
   * @param length The flash length in ms.
   */
  void start_flash(const LightColorValues &target, uint32_t length);

  void set_transformer(std::unique_ptr<LightTransformer> transformer);

  /// Set the color values immediately.
  void set_immediately(const LightColorValues &target);

  void start_default_transition(const LightColorValues &target);

  /// Applies the effect, transformer and then returns the current values.
  LightColorValues get_current_values();

  /// Lazily get the last current values. Returns the values last returned by get_current_values().
  const LightColorValues &get_current_values_lazy();

  /// Return the values that should be reported to the remote.
  LightColorValues get_remote_values();

  /// Return the name of the current effect, or if no effect is active "None".
  std::string get_effect_name();

  /** This lets front-end components subscribe to light change events, for example when a transition is done.
   *
   * Note the callback should get the output values through get_remote_values().
   *
   * @param send_callback
   */
  void add_send_callback(light_send_callback_t &&send_callback);

  /// Causes the callback defined by add_send_callback() to trigger.
  void send_values();

  /// Return whether the light has any effects that meet the trait requirements.
  bool supports_effects() const;

  const LightTraits &get_traits() const;
  void set_traits(const LightTraits &traits);

  /// Parse and apply the provided JSON payload.
  void parse_json(const JsonObject &root);

  /// Dump the state of this light as JSON.
  void dump_json(JsonBuffer &buffer, JsonObject &root);

  /// Defaults to 1 second (1000 ms).
  uint32_t get_default_transition_length() const;
  /// Set the default transition length, i.e. the transition length when no transition is provided.
  void set_default_transition_length(uint32_t default_transition_length);

 protected:
  uint32_t default_transition_length_{1000};
  std::unique_ptr<LightEffect> effect_{nullptr};
  std::unique_ptr<LightTransformer> transformer_{nullptr};
  LightColorValues values_{};
  CallbackManager<void()> send_callback_{};
  LightTraits traits_;
};

} // namespace light

} // namespace esphomelib

#endif //USE_LIGHT

#endif //ESPHOMELIB_LIGHT_LIGHT_STATE_H
