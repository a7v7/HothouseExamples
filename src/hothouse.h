/**
    Hardware proxy for Hothouse DIY DSP Platform
    Copyright (C) 2024  Cleveland Music Co.  <code@clevelandmusicco.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "daisy_seed.h"
#include "optional"

using daisy::AdcChannelConfig;
using daisy::AnalogControl;
using daisy::AudioHandle;
using daisy::DaisySeed;
using daisy::Led;
using daisy::Pin;
using daisy::SaiHandle;
using daisy::Switch;

namespace clevelandmusicco {
class Hothouse {
 public:
  /** Switches */
  enum Switches {
    SWITCH_1_UP,   /** Up position of toggleswitch 1 */
    SWITCH_1_DOWN, /** Down position of toggleswitch 1 */
    SWITCH_2_UP,   /** Up position of toggleswitch 2 */
    SWITCH_2_DOWN, /** Down position of toggleswitch 2 */
    SWITCH_3_UP,   /** Up position of toggleswitch 3 */
    SWITCH_3_DOWN, /** Down position of toggleswitch 3 */
    FOOTSWITCH_1,  /** Footswitch 1 */
    FOOTSWITCH_2,  /** Footswitch 2 */
    SWITCH_LAST,   /**< Last enum item */
  };

  /** Knobs */
  enum Knob {
    KNOB_1,    /**< & */
    KNOB_2,    /**< & */
    KNOB_3,    /**< & */
    KNOB_4,    /**< & */
    KNOB_5,    /**< & */
    KNOB_6,    /**< & */
    KNOB_LAST, /**< & */
  };

  /** footswitch leds */
  enum Led {
    LED_1 = 22, /**< & */
    LED_2 = 23, /**< & */
    LED_LAST,   /**< & */
  };

  enum ToggleswitchPosition {
    TOGGLESWITCH_UP,
    TOGGLESWITCH_MIDDLE,
    TOGGLESWITCH_DOWN,
    TOGGLESWITCH_UNKNOWN,
  };

  enum Toggleswitch {
    TOGGLESWITCH_1,
    TOGGLESWITCH_2,
    TOGGLESWITCH_3,
  };

  /** Constructor */
  Hothouse() {}
  /** Destructor */
  ~Hothouse() {}

  /** Initialize daisy petal */
  void Init(bool boost = false);

  /**
     Wait before moving on.
     \param del Delay time in ms.
   */
  void DelayMs(size_t del);

  /** Starts the callback
  \param cb Interleaved callback function
  */
  void StartAudio(AudioHandle::InterleavingAudioCallback cb);

  /** Starts the callback
  \param cb multichannel callback function
  */
  void StartAudio(AudioHandle::AudioCallback cb);

  /**
     Switch callback functions
     \param cb New interleaved callback function.
  */
  void ChangeAudioCallback(AudioHandle::InterleavingAudioCallback cb);

  /**
     Switch callback functions
     \param cb New multichannel callback function.
  */
  void ChangeAudioCallback(AudioHandle::AudioCallback cb);

  /** Stops the audio if it is running. */
  void StopAudio();

  /** Updates the Audio Sample Rate, and reinitializes.
   ** Audio must be stopped for this to work.
   */
  void SetAudioSampleRate(SaiHandle::Config::SampleRate samplerate);

  /** Returns the audio sample rate in Hz as a floating point number.
   */
  float AudioSampleRate();

  /** Sets the number of samples processed per channel by the audio callback.
     \param size Audio block size
   */
  void SetAudioBlockSize(size_t size);

  /** Returns the number of samples per channel in a block of audio. */
  size_t AudioBlockSize();

  /** Returns the rate in Hz that the Audio callback is called */
  float AudioCallbackRate();

  /** Start analog to digital conversion. */
  void StartAdc();

  /** Stops Transfering data from the ADC */
  void StopAdc();

  /** Call at the same frequency as controls are read for stable readings.*/
  void ProcessAnalogControls();

  /** Process Analog and Digital Controls */
  inline void ProcessAllControls() {
    ProcessAnalogControls();
    ProcessDigitalControls();
  }

  /** Get value per knobs.
  \param k Which knobs to get
  \return Floating point knobs position.
  */
  float GetKnobValue(Knob k);

  /** Process digital controls */
  void ProcessDigitalControls();

  /** Get the current position of a toggleswitch (up, down, or middle).
  \param tsw Which toggleswitch to interogate (TOGGLESWITCH_1, TOGGLESWITCH_2,
  or TOGGLESWITCH_3) \return TOGGLESWITCH_UP, TOGGLESWITCH_DOWN, or
  TOGGLESWITCH_MIDDLE. \note If the toggleswitch in question is ON-ON (rather
  than ON-OFF-ON), then TOGGLESWITCH_MIDDLE can never be the return value. Write
  your code with this in mind.
  */
  ToggleswitchPosition GetToggleswitchPosition(Toggleswitch tsw);

  DaisySeed seed; /**< & */

  AnalogControl knobs[KNOB_LAST]; /**< & */
  Switch switches[SWITCH_LAST];   /**< & */

 private:
  void SetHidUpdateRates();
  void InitSwitches();
  void InitAnalogControls();
  ToggleswitchPosition GetLogicalSwitchPosition(Switch up, Switch down);

  inline uint16_t* adc_ptr(const uint8_t chn) { return seed.adc.GetPtr(chn); }
};

}  // namespace clevelandmusicco
