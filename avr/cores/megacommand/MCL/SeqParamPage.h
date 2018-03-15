/* Justin Mammarella jmamma@gmail.com 2018 */

#ifndef SEQPARAMPAGE_H__
#define SEQPARAMPAGE_H__
#include "GUI.h"

class SeqParamPage : public SeqPage {

public:
  uint8_t param1;
  uint8_t param2;

  SeqParamPage(Encoder *e1 = NULL, Encoder *e2 = NULL, Encoder *e3 = NULL,
               Encoder *e4 = NULL)
      : SeqPage(e1, e2, e3, e4) {
  }
  void init(uint8_t p1, uint8_t p2);
  bool handleEvent(gui_event_t *event);
  bool displayPage();
  void setup();
};

#endif /* SEQPARAMPAGE_H__ */