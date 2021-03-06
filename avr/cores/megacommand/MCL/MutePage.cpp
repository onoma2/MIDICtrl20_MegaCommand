#include "MCL.h"
#include "MutePage.h"

void MutePage::setup() {}

void MutePage::init() {
  note_interface.state = true;
#ifdef OLED_DISPLAY
  classic_display = false;
  oled_display.clearDisplay();
#endif
}
void MutePage::cleanup() {
  //  md_exploit.off();
  note_interface.state = false;
#ifdef OLED_DISPLAY
  oled_display.clearDisplay();
#endif
}

void MutePage::draw_mutes(uint8_t line_number) {
  if (line_number == 0) {
    GUI.setLine(GUI.LINE1);
  } else {
    GUI.setLine(GUI.LINE2);
  }
  /*Initialise the string with blank steps*/
  char str[17] = "----------------";

  for (int i = 0; i < 16; i++) {

#ifdef OLED_DISPLAY
    if (note_interface.notes[i] > 0) {

      oled_display.fillRect(0 + i * 8, 2, 6, 6, WHITE);
    }

    else if (!IS_BIT_SET32(mcl_cfg.mutes, i)) {

      oled_display.fillRect(0 + i * 8, 2, 6, 6, BLACK);
      oled_display.drawRect(0 + i * 8, 2, 6, 6, WHITE);

    }

    else {

      oled_display.fillRect(0 + i * 8, 2, 6, 6, BLACK);
      oled_display.drawLine(+i * 8, 5, 5 + (i * 8), 5, WHITE);
    }

#else

    str[i] = (char)219;

    if (IS_BIT_SET32(mcl_cfg.mutes, i)) {

      str[i] = (char)'-';
    }
    if (note_interface.notes[i] > 0) {

      str[i] = (char)255;
    }
#endif
  }
#ifndef OLED_DISPLAY
  GUI.put_string_at(0, str);
#endif
}
void MutePage::toggle_mute(int i) {
  if (IS_BIT_SET32(mcl_cfg.mutes, i)) {
    CLEAR_BIT32(mcl_cfg.mutes, i);
    if (encoders[2]->getValue() == 7) {
      mixer_page.set_level(i, 0);
    }
    MD.muteTrack(i, false);
  } else {
    SET_BIT32(mcl_cfg.mutes, i);
    if (encoders[2]->getValue() == 7) {
      //  set_level(i, 127);
    }

    MD.muteTrack(i, true);
  }
}
void MutePage::toggle_mutes_batch() {

  midi_events.remove_callbacks();
  uint16_t quantize_mute;
  quantize_mute = 1 << encoders[2]->getValue();
  int i;
  if ((encoders[2]->getValue() > 0) && (encoders[2]->getValue() < 7)) {
    while ((MidiClock.state == 2) && ((((MidiClock.div32th_counter - mcl_actions.start_clock32th) +
          3) %
         (quantize_mute * 2)) != 0)) {
      GUI.display();
    }
  }

  // send the track to master before unmuting

  for (i = 0; i < 16; i++) {
    if (note_interface.notes[i] == 3) {
      toggle_mute(i);
    }
    note_interface.notes[i] = 0;
    // trackinfo_page.display();
  }
  detect_time = slowclock;
  midi_events.setup_callbacks();
}
void MutePage::display() {
#ifdef OLED_DISPLAY
  if (!classic_display) {
    oled_display.clearDisplay();
  }
#endif
  GUI.setLine(GUI.LINE2);
  uint8_t x;
  // GUI.put_string_at(12,"Mute");
  GUI.put_string_at(0, "MUTE     ");

  GUI.put_string_at(9, "Q:");
  if (encoders[2]->getValue() == 0) {
    GUI.put_string_at(11, "--");
  } else if (encoders[2]->getValue() == 7) {
    GUI.put_string_at(11, "LV");
  } else {
    x = 1 << encoders[2]->getValue();

    GUI.put_value_at2(11, x);
  }
  uint8_t step_count =
      (MidiClock.div16th_counter - mcl_actions.start_clock32th / 2) -
      (64 * ((MidiClock.div16th_counter -
              mcl_actions.start_clock32th / 2) /
             64));
  GUI.put_value_at2(14, step_count);
  draw_mutes(0);
#ifdef OLED_DISPLAY
  LCD.goLine(1);
  LCD.puts(GUI.lines[1].data);
  oled_display.display();
#endif
}

bool MutePage::handleEvent(gui_event_t *event) {
  if (note_interface.is_event(event)) {
    uint8_t track = event->source - 128;
    if (midi_active_peering.get_device(event->port) != DEVICE_MD) {
      return true;
    }
    note_interface.draw_notes(0);
    if (event->mask == EVENT_BUTTON_RELEASED) {
      //  if ((encoders[2]->getValue() == 0)) {

      //    toggle_mute(track);
      //// }

      // else {
      if (note_interface.notes_all_off_md()) {
        toggle_mutes_batch();
        note_interface.init_notes();
      }
      //  }
    }
    return true;
  }
  if (EVENT_PRESSED(event, Buttons.BUTTON1)) {
    GUI.setPage(&mixer_page);
    return true;
  }
  if (EVENT_PRESSED(event, Buttons.BUTTON2)) {
    GUI.setPage(&page_select_page);
    return true;
  }
  if (EVENT_PRESSED(event, Buttons.ENCODER1) ||
      EVENT_PRESSED(event, Buttons.ENCODER2) ||
      EVENT_PRESSED(event, Buttons.ENCODER3) ||
      EVENT_PRESSED(event, Buttons.ENCODER1)) {
    GUI.setPage(&grid_page);
    return true;
  }

  return false;
}

void MuteMidiEvents::setup_callbacks() {
  if (state) {
    return;
  }
  Midi.addOnNoteOnCallback(
      this, (midi_callback_ptr_t)&MuteMidiEvents::onNoteOnCallback_Midi);
  //  Midi.addOnNoteOffCallback(
  //    this, (midi_callback_ptr_t)&MuteMidiEvents::onNoteOffCallback_Midi);

  state = true;
}

void MuteMidiEvents::remove_callbacks() {
  if (!state) {
    return;
  }

  DEBUG_PRINTLN("remove calblacks");
  Midi.removeOnNoteOnCallback(
      this, (midi_callback_ptr_t)&MuteMidiEvents::onNoteOnCallback_Midi);
  Midi.removeOnNoteOffCallback(
      this, (midi_callback_ptr_t)&MuteMidiEvents::onNoteOffCallback_Midi);

  state = false;
}
uint8_t MuteMidiEvents::note_to_trig(uint8_t note_num) {
  uint8_t trig_num = 0;
  for (uint8_t i = 0; i < sizeof(MD.global.drumMapping); i++) {
    if (note_num == MD.global.drumMapping[i]) {
      trig_num = i;
    }
  }
  return trig_num;
}
void MuteMidiEvents::onNoteOnCallback_Midi(uint8_t *msg) {
  uint8_t note_num = msg[1];
  uint8_t channel = MIDI_VOICE_CHANNEL(msg[0]);
  uint8_t n = note_to_trig(msg[1]);
  if (clock_diff(mute_page.detect_time, slowclock) < 1000) {
    return;
  }
  //  if ((msg[0] != 153) && (md_exploit.state)) {
  //  CLEAR_BIT32(mcl_cfg.mutes, n);
  //  }
  else if ((msg[0] == 153) && (!md_exploit.state)) {
    CLEAR_BIT32(mcl_cfg.mutes, n);
  }
}
void MuteMidiEvents::onNoteOffCallback_Midi(uint8_t *msg) {}
