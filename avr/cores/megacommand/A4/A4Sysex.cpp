#include "WProgram.h"

#include "A4.h"
#include "A4Messages.hh"
#include "A4Sysex.hh"
#include "helpers.h"

A4SysexListenerClass A4SysexListener;

void A4SysexListenerClass::start() {

  msgType = 255;
  isA4Message = false;
}

void A4SysexListenerClass::handleByte(uint8_t byte) {

  if (isA4Message && MidiSysex2.len == sizeof(a4_sysex_hdr)) {
    msgType = byte;
    switch (byte) {
    case A4_GLOBAL_MESSAGE_ID:
      //      MidiSysex2.startRecord();
      break;

    case A4_KIT_MESSAGE_ID:
      //      MidiSysex2.startRecord();
      break;

    case A4_PATTERN_MESSAGE_ID:
      //      MidiSysex2.startRecord();
      break;

    case A4_SONG_MESSAGE_ID:
      //      MidiSysex2.startRecord();
      break;
    }
  }
}

void A4SysexListenerClass::end() {
  if (MidiSysex2.data[3] == 0x06) {
    isA4Message = true;
  } else {
    isA4Message = false;
    return;
  }

  msgType = MidiSysex2.data[sizeof(a4_sysex_hdr)];


  switch (msgType) {
    // case A4_STATUS_RESPONSE_ID:
    //  onStatusResponseCallbacks.call(MidiSysex2.data[6], MidiSysex2.data[7]);
    //  break;

  case A4_GLOBAL_MESSAGE_ID:
    onGlobalMessageCallbacks.call();
    break;

  case A4_GLOBALX_MESSAGE_ID:
    onGlobalMessageCallbacks.call();
    break;

  case A4_KIT_MESSAGE_ID:
    onKitMessageCallbacks.call();
    break;

  case A4_KITX_MESSAGE_ID:
    onKitMessageCallbacks.call();
    break;

  case A4_SOUND_MESSAGE_ID:
    onSoundMessageCallbacks.call();
    break;

  case A4_SOUNDX_MESSAGE_ID:
    onSoundMessageCallbacks.call();
    break;

  case A4_PATTERN_MESSAGE_ID:
    onPatternMessageCallbacks.call();
    break;

  case A4_PATTERNX_MESSAGE_ID:
    onPatternMessageCallbacks.call();
    break;

  case A4_SONG_MESSAGE_ID:
    onSongMessageCallbacks.call();
    break;

  case A4_SONGX_MESSAGE_ID:
    onSongMessageCallbacks.call();
    break;

  case A4_SETTINGS_MESSAGE_ID:
    onSettingsMessageCallbacks.call();
    break;

  case A4_SETTINGSX_MESSAGE_ID:
    onSettingsMessageCallbacks.call();
    break;
  }

}

void A4SysexListenerClass::end_immediate() {
}

void A4SysexListenerClass::setup() { MidiSysex2.addSysexListener(this); }
