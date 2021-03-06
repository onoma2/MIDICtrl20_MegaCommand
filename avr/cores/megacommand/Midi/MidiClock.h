
/* Copyright (c) 2009 - http://ruinwesen.com/ */

#ifndef MIDICLOCK_H__
#define MIDICLOCK_H__

#include "Callback.hh"
#include "Vector.hh"
#include "WProgram.h"
#include "midi-common.hh"
#include <inttypes.h>

/**
 * \addtogroup Midi
 *
 * @{
 **/

/**
 * \addtogroup midi_clock Midi Clock
 *
 * @{
 **/

class ClockCallback {};

typedef void (ClockCallback::*midi_clock_callback_ptr_t)(uint32_t count);

class MidiClockClass {
  /**
   * \addtogroup midi_clock
   *
   * @{
   **/

public:

  volatile uint32_t div192th_counter_last;
  volatile uint32_t div192th_counter;

  volatile uint32_t div96th_counter_last;
  volatile uint32_t div96th_counter;
  volatile uint32_t div32th_counter;
  volatile uint16_t div16th_counter;

  volatile uint8_t mod12_counter;
  volatile uint8_t mod6_counter;
  volatile uint8_t mod3_counter;
  volatile uint8_t mod6_free_counter;

  volatile uint16_t clock_last_time;
  volatile uint16_t div192th_time;
  volatile uint16_t last_clock16;

  volatile uint8_t bar_counter;
  volatile uint8_t beat_counter;
  volatile uint8_t step_counter;

  volatile uint8_t inmod6_counter;

  volatile uint16_t interval;

  volatile uint16_t counter;
  volatile uint16_t rx_phase;

  volatile uint16_t rx_last_clock;
  volatile uint16_t rx_clock;
  volatile bool doUpdateClock;

  volatile bool useImmediateClock;

  volatile bool updateSmaller;
  uint16_t pll_x;
  float tempo;
  // bool transmit;
  bool transmit_uart1;
  bool transmit_uart2;
  bool isInit;

  //    volatile uint16_t mcl_clock;
  //   volatile uint16_t mcl_countbool
  volatile enum {
    PAUSED = 0,
    STARTING = 1,
    STARTED = 2,
  } state;

#if defined(MIDIDUINO) || defined(HOST_MIDIDUINO)

  typedef enum {
    OFF = 0,
    INTERNAL,
    EXTERNAL_UART1,
    EXTERNAL_UART2
  } clock_mode_t;
#define INTERNAL_MIDI INTERNAL
#define EXTERNAL_MIDI EXTERNAL_UART1

#else
  typedef enum {
    OFF = 0,
    INTERNAL_MIDI,
    EXTERNAL_UART1,
    EXTERNAL_UART2
  } clock_mode_t;
  // arduino

#ifndef BOARD_ID
#define BOARD_ID 0x80
#endif

#endif

  clock_mode_t mode;

  MidiClockClass();

  CallbackVector1<ClockCallback, 8, uint32_t> onMidiStartCallbacks;

  CallbackVector1<ClockCallback, 8, uint32_t> onMidiStartImmediateCallbacks;
  CallbackVector1<ClockCallback, 8, uint32_t> onMidiStopCallbacks;
  CallbackVector1<ClockCallback, 8, uint32_t> onMidiContinueCallbacks;

  CallbackVector1<ClockCallback, 8, uint32_t> on192Callbacks;
  CallbackVector1<ClockCallback, 8, uint32_t> on96Callbacks;
  CallbackVector1<ClockCallback, 8, uint32_t> on32Callbacks;
  CallbackVector1<ClockCallback, 8, uint32_t> on16Callbacks;

  CallbackVector1<ClockCallback, 8, uint32_t> onClockCallbacks;
  void addOnMidiStartImmediateCallback(ClockCallback *obj,
                              midi_clock_callback_ptr_t func) {
    onMidiStartImmediateCallbacks.add(obj, func);
  }
  void removeOnMidiStartImmediateCallback(ClockCallback *obj,
                                 midi_clock_callback_ptr_t func) {
    onMidiStartImmediateCallbacks.remove(obj, func);
  }
  void removeOnMidiStartImmediateCallback(ClockCallback *obj) {
    onMidiStartImmediateCallbacks.remove(obj);
  }

  void addOnMidiStartCallback(ClockCallback *obj,
                              midi_clock_callback_ptr_t func) {
    onMidiStartCallbacks.add(obj, func);
  }
  void removeOnMidiStartCallback(ClockCallback *obj,
                                 midi_clock_callback_ptr_t func) {
    onMidiStartCallbacks.remove(obj, func);
  }
  void removeOnMidiStartCallback(ClockCallback *obj) {
    onMidiStartCallbacks.remove(obj);
  }
  void addOnMidiStopCallback(ClockCallback *obj,
                             midi_clock_callback_ptr_t func) {
    onMidiStopCallbacks.add(obj, func);
  }
  void removeOnMidiStopCallback(ClockCallback *obj,
                                midi_clock_callback_ptr_t func) {
    onMidiStopCallbacks.remove(obj, func);
  }
  void removeOnMidiStopCallback(ClockCallback *obj) {
    onMidiStopCallbacks.remove(obj);
  }
  void addOnMidiContinueCallback(ClockCallback *obj,
                                 midi_clock_callback_ptr_t func) {
    onMidiContinueCallbacks.add(obj, func);
  }
  void removeOnMidiContinueCallback(ClockCallback *obj,
                                    midi_clock_callback_ptr_t func) {
    onMidiContinueCallbacks.remove(obj, func);
  }
  void removeOnMidiContinueCallback(ClockCallback *obj) {
    onMidiContinueCallbacks.remove(obj);
  }
  void addOn192Callback(ClockCallback *obj, midi_clock_callback_ptr_t func) {
    on192Callbacks.add(obj, func);
  }
  void removeOn192Callback(ClockCallback *obj, midi_clock_callback_ptr_t func) {
    on192Callbacks.remove(obj, func);
  }
  void removeOn192Callback(ClockCallback *obj) { on192Callbacks.remove(obj); }

  void addOn96Callback(ClockCallback *obj, midi_clock_callback_ptr_t func) {
    on96Callbacks.add(obj, func);
  }
  void removeOn96Callback(ClockCallback *obj, midi_clock_callback_ptr_t func) {
    on96Callbacks.remove(obj, func);
  }
  void removeOn96Callback(ClockCallback *obj) { on96Callbacks.remove(obj); }

  void addOn32Callback(ClockCallback *obj, midi_clock_callback_ptr_t func) {
    on32Callbacks.add(obj, func);
  }
  void removeOn32Callback(ClockCallback *obj, midi_clock_callback_ptr_t func) {
    on32Callbacks.remove(obj, func);
  }
  void removeOn32Callback(ClockCallback *obj) { on32Callbacks.remove(obj); }

  void addOn16Callback(ClockCallback *obj, midi_clock_callback_ptr_t func) {
    on16Callbacks.add(obj, func);
  }
  void removeOn16Callback(ClockCallback *obj, midi_clock_callback_ptr_t func) {
    on16Callbacks.remove(obj, func);
  }
  void removeOn16Callback(ClockCallback *obj) { on16Callbacks.remove(obj); }

  void addOnClockCallback(ClockCallback *obj, midi_clock_callback_ptr_t func) {
    onClockCallbacks.add(obj, func);
  }
  void removeOnClockCallback(ClockCallback *obj,
                             midi_clock_callback_ptr_t func) {
    onClockCallbacks.remove(obj, func);
  }
  void removeOnClockCallback(ClockCallback *obj) {
    onClockCallbacks.remove(obj);
  }

  void init();
  void handleClock();
  void handleImmediateClock();
  void updateClockInterval();
  void increment192Counter();
  void incrementCounters();
  void callCallbacks();
  bool clock_less_than(uint16_t a, uint16_t b);
  bool clock_less_than(uint32_t a, uint32_t b);
  uint32_t clock_diff_div192(uint32_t old_clock, uint32_t new_clock);

  void handleImmediateMidiStart();
  void handleImmediateMidiContinue();
  void handleImmediateMidiStop();
  void handleMidiStart();
  void handleMidiContinue();
  void handleMidiStop();
  void handleTimerInt();
  void handleSongPositionPtr(uint8_t *msg);
  void setSongPositionPtr(uint16_t pos);

  void start();
  void stop();
  void pause();
  void setTempo(uint16_t tempo);
  uint16_t getTempo();

  bool isStarted() { return state == STARTED; }

  /* @} */
};

extern MidiClockClass MidiClock;

/* @} @} */

#endif /* MIDICLOCK_H__ */
