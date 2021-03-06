/* Justin Mammarella jmamma@gmail.com 2018 */

#ifndef SEQPARAMPAGE_H__
#define SEQPARAMPAGE_H__

#include "SeqPage.h"

class SeqParamPageMidiEvents : public MidiCallback {
public:
  bool state;

  void setup_callbacks();
  void remove_callbacks();

  void onNoteOnCallback_Midi2(uint8_t *msg);
  void onNoteOffCallback_Midi2(uint8_t *msg);
};

class SeqParamPage : public SeqPage {

public:
  uint8_t p1;
  uint8_t p2;
  uint8_t page_id;
  SeqParamPageMidiEvents midi_events;
  SeqParamPage(Encoder *e1 = NULL, Encoder *e2 = NULL, Encoder *e3 = NULL,
               Encoder *e4 = NULL)
      : SeqPage(e1, e2, e3, e4) {}

  void construct(uint8_t p1, uint8_t p2);
  bool handleEvent(gui_event_t *event);
  void display();
  void loop();
  void setup();
  void init();
  void cleanup();
};

#endif /* SEQPARAMPAGE_H__ */
