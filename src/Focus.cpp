/* -*- mode: c++ -*-
 * Model01-Sketch -- algernon's Model01 Sketch
 * Copyright (C) 2016, 2017  Gergely Nagy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "00-config.h"

#include "Focus.h"

#include <Kaleidoscope-Focus.h>
#include <Kaleidoscope-LEDControl.h>
#include <Kaleidoscope-EEPROM-Settings.h>
#include <Kaleidoscope-EEPROM-Keymap.h>

namespace algernon {
  namespace FocusSetup {
    void configure (void) {
      USE_PLUGINS (&Focus, &EEPROMSettings, &EEPROMKeymap);

      Focus.addHook (FOCUS_HOOK_HELP);
      Focus.addHook (FOCUS_HOOK_VERSION);
      Focus.addHook (FOCUS_HOOK_KEYMAP);
#if WITH_PROGMEM_KEYMAP
      Focus.addHook (FOCUS_HOOK_KEYMAP_TRANSFER);
#endif
      Focus.addHook (FOCUS_HOOK_EEPROM);
      Focus.addHook (FOCUS_HOOK_KALEIDOSCOPE);
      Focus.addHook (FOCUS_HOOK_LEDCONTROL);
      Focus.addHook (FOCUS_HOOK_SETTINGS);
    }
  };
};

namespace FocusCommands {

  bool led (const char *command) {
    enum {
      GET,
      SET,
      SETALL,
      OFF,
    } subCommand;

    if (strncmp_P (command, PSTR ("led."), 4) != 0)
      return false;
    if (strcmp_P (command + 4, PSTR ("get")) == 0)
      subCommand = GET;
    else if (strcmp_P (command + 4, PSTR ("set")) == 0)
      subCommand = SET;
    else if (strcmp_P (command + 4, PSTR ("setAll")) == 0)
      subCommand = SETALL;
    else if (strcmp_P (command + 4, PSTR ("off")) == 0)
      subCommand = OFF;
    else
      return false;

    switch (subCommand) {
    case GET:
      {
        uint8_t idx = Serial.parseInt ();
        cRGB c = LEDControl.led_get_crgb_at (idx);
        const __FlashStringHelper *spc = F(" ");

        Serial.print (c.r);
        Serial.print (spc);
        Serial.print (c.g);
        Serial.print (spc);
        Serial.println (c.b);
        break;
      }
    case SET:
      {
        uint8_t idx = Serial.parseInt ();
        cRGB c;

        c.r = Serial.parseInt ();
        c.g = Serial.parseInt ();
        c.b = Serial.parseInt ();

        LEDControl.led_set_crgb_at (idx, c);
        break;
      }
    case SETALL:
      {
        cRGB c;

        c.r = Serial.parseInt ();
        c.g = Serial.parseInt ();
        c.b = Serial.parseInt ();

        LEDControl.set_all_leds_to (c);

        break;
      }
    case OFF:
      LEDControl.set_all_leds_to (0, 0, 0);
      break;
    }

    Serial.read ();
    return true;
  }

}
