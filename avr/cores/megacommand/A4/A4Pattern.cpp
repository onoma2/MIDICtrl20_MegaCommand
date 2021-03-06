
#include "A4.h"
#include "A4Pattern.hh"
#include "Elektron.hh"
#include "A4Messages.hh"
#include "helpers.h"
#include "A4Params.hh"
#include "GUI.h"

#ifdef HOST_MIDIDUINO
#include <stdio.h>
#endif

// #include "GUI.h"
bool A4Pattern::fromSysex(uint8_t *data, uint16_t len) {
	init();
	
	if ((len != (0xACA - 6)) && (len != (0x1521 - 6)))  {
#ifndef HOST_MIDIDUINO
		GUI.flash_string_fill("WRONG LENGTH");
#else
		printf("WRONG LENGTH: %x\n", len);
#endif
		return false;
	}
	
	
	if (!ElektronHelper::checkSysexChecksum(data, len)) {
		return false;
	}
	

	return true;
}




