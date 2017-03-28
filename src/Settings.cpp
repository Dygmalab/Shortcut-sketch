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

#include "Layers.h"
#include "Settings.h"
#include <Kaleidoscope-EEPROM-Settings.h>
#include <Kaleidoscope-EEPROM-Keymap.h>
#include <Kaleidoscope-Focus.h>

namespace algernon {
  namespace Settings {
    static uint16_t base;

#if WITH_CYCLE_REPORT
    static bool
    focusSettings (const char *command) {
      if (strcmp (command, PSTR ("settings.cycleTimer")) != 0)
        return false;

      if (Serial.peek () == '\n') {
        Focus.printBool (settings.cycleTimer);
        Serial.println ();
      } else {
        uint8_t state = Serial.parseInt ();
        settings.cycleTimer = !!state;

        EEPROM.put (base, settings);
      }

      return true;
    }
#endif

    void
    configure (void) {
      USE_PLUGINS (&EEPROMSettings,
                   &EEPROMKeymap);

      EEPROMKeymap.reserveSpace (LAYER_MAX - 1);
      base = EEPROMSettings.requestSlice (sizeof (settings));

#if WITH_CYCLE_REPORT
      Focus.addHook (FOCUS_HOOK (focusSettings,
                                 "settings.cycleTimer [1|0]\n"
                                 "-------------------------\n"
                                 "Enable/disable the cycle timer; or display the setting if called without arguments."));
#endif
      EEPROM.get (base, settings);
    }

    void
    seal (void) {
      EEPROMSettings.seal ();

      if (!EEPROMSettings.isValid ()) {
        EEPROMSettings.version (0);
        EEPROMSettings.update ();
      }
    }

    settings_ settings;
  }
}
