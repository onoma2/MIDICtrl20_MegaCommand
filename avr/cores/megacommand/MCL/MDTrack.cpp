#include "MCL.h"
#include "MDTrack.h"

void MDTrack::get_machine_from_kit(int tracknumber, uint8_t column) {
  //  trackName[0] = '\0';
  memcpy(machine.params, MD.kit.params[tracknumber], 24);

  machine.track = tracknumber;
  machine.level = MD.kit.levels[tracknumber];
  machine.model = MD.kit.models[tracknumber];

  /*Check to see if LFO is modulating host track*/
  /*IF it is then we need to make sure that the LFO destination is updated to
   * the new row posiiton*/

  if (MD.kit.lfos[tracknumber].destinationTrack == tracknumber) {
    MD.kit.lfos[tracknumber].destinationTrack = column;
    machine.track = column;
  }
  /*Copies Lfo data from the kit object into the machine object*/
  memcpy(&machine.lfo, &MD.kit.lfos[tracknumber], sizeof(machine.lfo));

  machine.trigGroup = MD.kit.trigGroups[tracknumber];
  machine.muteGroup = MD.kit.muteGroups[tracknumber];
}
bool MDTrack::get_track_from_sysex(int tracknumber, uint8_t column) {

  active = MD_TRACK_TYPE;
  trigPattern = MD.pattern.trigPatterns[tracknumber];
  accentPattern = MD.pattern.accentPatterns[tracknumber];
  slidePattern = MD.pattern.slidePatterns[tracknumber];
  swingPattern = MD.pattern.swingPatterns[tracknumber];
  length = MD.pattern.patternLength;
  kitextra.swingAmount = MD.pattern.swingAmount;
  kitextra.accentAmount = MD.pattern.accentAmount;
  kitextra.patternLength = MD.pattern.patternLength;
  kitextra.doubleTempo = MD.pattern.doubleTempo;
  kitextra.scale = MD.pattern.scale;

  kitextra.accentEditAll = MD.pattern.accentEditAll;
  kitextra.slideEditAll = MD.pattern.slideEditAll;
  kitextra.swingEditAll = MD.pattern.swingEditAll;

  kitextra.accentPattern = MD.pattern.accentPattern;
  kitextra.slidePattern = MD.pattern.slidePattern;
  kitextra.swingPattern = MD.pattern.swingPattern;

  // Extract parameter lock data and store it in a useable data structure
  int n = 0;
  arraysize = 0;
  for (int i = 0; i < 24; i++) {
    if (IS_BIT_SET32(MD.pattern.lockPatterns[tracknumber], i)) {
      int8_t idx = MD.pattern.paramLocks[tracknumber][i];
      if (idx >= 0) {
        for (int s = 0; s < 64; s++) {

          if ((MD.pattern.locks[idx][s] <= 127) &&
              (MD.pattern.locks[idx][s] >= 0)) {
            if (IS_BIT_SET64(trigPattern, s)) {

              locks[n].step = s;
              locks[n].param_number = i;
              locks[n].value = MD.pattern.locks[idx][s];
              n++;
            }
          }
        }
      }
    }
  }

  //  itoa(n,&str[2],10);

  arraysize = n;

  /*Don't forget to copy the Machine data as well
    Which is obtained from the received Kit object MD.kit*/
  //  m_strncpy(kitName, MD.kit.name, 17);
  /*
  uint8_t white_space = 0;
  for (uint8_t c = 0; c < 17; c++) {
    if (white_space == 0) {
      trackName[c] = MD.kit.name[c];
    } else {
      trackName[c] = ' ';
    }
    if (MD.kit.name[c] == '\0') {
      white_space = 1;
    }
  }
  */
  memcpy(&seq_data, &mcl_seq.md_tracks[tracknumber], sizeof(seq_data));
  //  trackName[0] = '\0';
  get_machine_from_kit(tracknumber, column);

  memcpy(&kitextra.reverb, &MD.kit.reverb, sizeof(kitextra.reverb));
  memcpy(&kitextra.delay, &MD.kit.delay, sizeof(kitextra.delay));
  memcpy(&kitextra.eq, &MD.kit.eq, sizeof(kitextra.eq));
  memcpy(&kitextra.dynamics, &MD.kit.dynamics, sizeof(kitextra.dynamics));
  origPosition = MD.kit.origPosition;
  patternOrigPosition = MD.pattern.origPosition;
}

void MDTrack::place_track_in_kit(int tracknumber, uint8_t column, MDKit *kit,
                                 bool levels) {
  if (active != MD_TRACK_TYPE) {
    return;
  }
  memcpy(kit->params[tracknumber], &(machine.params), 24);
  if (levels) {
    kit->levels[tracknumber] = machine.level;
  }
  kit->models[tracknumber] = machine.model;

  if (machine.lfo.destinationTrack == column) {

    machine.lfo.destinationTrack = tracknumber;
  }

  memcpy(&(kit->lfos[tracknumber]), &machine.lfo, sizeof(machine.lfo));

  kit->trigGroups[tracknumber] = machine.trigGroup;
  kit->muteGroups[tracknumber] = machine.muteGroup;
}

void MDTrack::clear_track() {
  arraysize = 0;
  trigPattern = 0;
  accentPattern = 0;
  slidePattern = 0;
  swingPattern = 0;
}

void MDTrack::place_track_in_pattern(int tracknumber, uint8_t column,
                                     MDPattern *pattern) {
  if (active != MD_TRACK_TYPE) {
    return;
  }

  for (int x = 0; x < 64; x++) {
    pattern->clear_step_locks(tracknumber, x);
  }

  // MD.pattern.lockPatterns[tracknumber] = 0;
  // Write pattern lock data to pattern
  uint8_t a;
  pattern->trigPatterns[tracknumber] = trigPattern;
  pattern->accentPatterns[tracknumber] = accentPattern;
  pattern->slidePatterns[tracknumber] = slidePattern;
  pattern->swingPatterns[tracknumber] = swingPattern;

  for (a = length; a < pattern->patternLength; a += length) {
    pattern->trigPatterns[tracknumber] |= trigPattern << a;
    pattern->accentPatterns[tracknumber] |= accentPattern << a;
    pattern->slidePatterns[tracknumber] |= slidePattern << a;
    pattern->swingPatterns[tracknumber] |= swingPattern << a;
  }

  for (int n = 0; n < arraysize; n++) {
    // DEBUG_PRINTLN();
    // DEBUG_PRINTLN("Adding");
    // DEBUG_PRINTLN(step[n]);
    // DEBUG_PRINTLN(param_number[n]);
    // DEBUG_PRINTLN(value[n]);

    //  if (arraysize > 5) {     GUI.flash_string_fill("greater than 5"); }
    for (a = 0; a < pattern->patternLength; a += length) {
      pattern->addLock(tracknumber, locks[n].step + a, locks[n].param_number,
                       locks[n].value);
    }
  }
}

void MDTrack::load_seq_data(int tracknumber) {
  memcpy(&mcl_seq.md_tracks[tracknumber], &seq_data, sizeof(seq_data));

  mcl_seq.md_tracks[tracknumber].set_length(
      mcl_seq.md_tracks[tracknumber].length);
  mcl_seq.md_tracks[tracknumber].update_params();
}

void MDTrack::place_track_in_sysex(int tracknumber, uint8_t column) {
  place_track_in_pattern(tracknumber, column, &(MD.pattern));
  place_track_in_kit(tracknumber, column, &(MD.kit));
  load_seq_data(tracknumber);
}

bool MDTrack::load_track_from_grid(int32_t column, int32_t row, int32_t len) {

  bool ret;
  int b = 0;

  int32_t offset = grid.get_slot_offset(column, row);

  ret = proj.file.seekSet(offset);
  if (!ret) {
    DEBUG_PRINTLN("Seek failed");
    return false;
  }

  ret = mcl_sd.read_data((uint8_t *)this, len, &proj.file);

  if (!ret) {
    DEBUG_PRINTLN("read failed");
    return false;
  }
  return true;
}

bool MDTrack::load_track_from_grid(int32_t column, int32_t row) {

  bool ret;
  int b = 0;

  //  DEBUG_PRINT_FN();
  int32_t offset = grid.get_slot_offset(column, row);

  int32_t len;

  ret = proj.file.seekSet(offset);
  if (!ret) {
    DEBUG_PRINTLN("Seek failed");
    return false;
  }

  len = (sizeof(MDTrack) - (LOCK_AMOUNT * 3));

  // len = (sizeof(MDTrack)  - (LOCK_AMOUNT * 3));

  ret = mcl_sd.read_data((uint8_t *)this, len, &proj.file);

  if (!ret) {
    DEBUG_PRINTLN("read failed");
    return false;
  }
  if ((arraysize < 0) || (arraysize > LOCK_AMOUNT)) {
    DEBUG_PRINTLN("lock array size is wrong");
    return false;
  }
  ret =
      mcl_sd.read_data((uint8_t *)&(this->locks[0]), arraysize * 3, &proj.file);
  if (!ret) {
    DEBUG_PRINTLN("read failed");
    return false;
  }
  return true;
}

void MDTrack::scale_seq_vol(float scale) {
  for (uint8_t n = 0; n < arraysize; n++) {
    if ((machine.lfo.destinationParam == MODEL_VOL) &&
        (machine.lfo.destinationTrack == machine.track)) {
      if (locks[n].param_number == MODEL_LFOD) {
        locks[n].value = (uint8_t)(scale * (float)locks[n].value);
        if (locks[n].value > 127) {
          locks[n].value = 127;
        }
      }
    }
    if (locks[n].param_number == MODEL_VOL) {
      locks[n].value = (uint8_t)(scale * (float)locks[n].value);
      if (locks[n].value > 127) {
        locks[n].value = 127;
      }
    }
  }

  for (uint8_t c = 0; c < 4; c++) {
    if (seq_data.locks_params[c] > 0) {
      if ((seq_data.locks_params[c] - 1 == MODEL_LFOD) ||
          (seq_data.locks_params[c] - 1 == MODEL_VOL)) {
        for (uint8_t n = 0; n < 64; n++) {
          if (seq_data.locks[c][n] > 0) {
            seq_data.locks[c][n] =
                (uint8_t)(scale * (float)(seq_data.locks[c][n] - 1)) + 1;
            if (seq_data.locks[c][n] > 127) {
              seq_data.locks[c][n] = 127;
            }
          }
        }
      }
    }
  }
}

void MDTrack::scale_vol(float scale) {
  normalize();
  machine.scale_vol(scale);
  scale_seq_vol(scale);
}

void MDTrack::normalize() {
  float scale = machine.normalize_level();
  scale_seq_vol(scale);
}

bool MDTrack::store_track_in_grid(int32_t column, int32_t row, int track) {
  /*Assign a track to Grid i*/
  /*Extraact track data from received pattern and kit and store in track
   * object*/
  active = MD_TRACK_TYPE;

  bool ret;
  int b = 0;
  DEBUG_PRINT_FN();
  int32_t len;

  int32_t offset = grid.get_slot_offset(column, row);

  ret = proj.file.seekSet(offset);
  if (!ret) {
    DEBUG_PRINTLN("seek failed");
    return false;
  }

  if (track != 255) {
    get_track_from_sysex(track, column);
  }
  // Normalise level and vol locks
  if (mcl_cfg.auto_normalize == 1) {
    normalize();
  }

  if (mcl_cfg.auto_merge == 1) {
    DEBUG_PRINTLN("auto merge");
    MDSeqTrack md_seq_track;
    memcpy(&(md_seq_track), &(this->seq_data), sizeof(MDSeqTrackData));
    md_seq_track.merge_from_md(this);
    clear_track();
    memcpy(&(this->seq_data), &(md_seq_track), sizeof(MDSeqTrackData));
  }

  len = sizeof(MDTrack) - (LOCK_AMOUNT * 3);
  DEBUG_PRINTLN(len);
  ret = mcl_sd.write_data((uint8_t *)(this), len, &proj.file);
  if (!ret) {
    DEBUG_PRINTLN("write failed");
    return false;
  }

  ret = mcl_sd.write_data((uint8_t *)&(this->locks[0]), arraysize * 3,
                          &proj.file);

  if (!ret) {
    DEBUG_PRINTLN("write failed");
    return false;
  }

  uint8_t model = machine.model;
  grid_page.row_headers[grid_page.cur_row].update_model(column, model,
                                                        MD_TRACK_TYPE);

  DEBUG_PRINTLN("Track stored in grid");
  DEBUG_PRINT(column);
  DEBUG_PRINT(" ");
  DEBUG_PRINT(row);
  DEBUG_PRINT("model");
  DEBUG_PRINT(model);
  return true;
}

bool MDTrackLight::store_in_mem(uint8_t column, uint32_t region) {
  uint32_t len = sizeof(GridTrack) + sizeof(MDSeqTrackData) + sizeof(MDMachine);

  uint32_t pos = region + len * (uint32_t)column;

  volatile uint8_t *ptr;

  ptr = reinterpret_cast<uint8_t *>(pos);
  // cbi(TIMSK0, TOIE0);
  switch_ram_bank(1);
  memcpy(ptr, this, len);

  switch_ram_bank(0);
  // sbi(TIMSK0, TOIE0);
  return true;
}

bool MDTrackLight::load_from_mem(uint8_t column, uint32_t region) {
  uint32_t len = sizeof(GridTrack) + sizeof(MDSeqTrackData) + sizeof(MDMachine);

  uint32_t pos = region + len * (uint32_t)column;

  volatile uint8_t *ptr;

  ptr = reinterpret_cast<uint8_t *>(pos);
  // cbi(TIMSK0, TOIE0);
  switch_ram_bank(1);
  memcpy(this, ptr, len);
  switch_ram_bank(0);
  // sbi(TIMSK0, TOIE0);
  return true;
}


