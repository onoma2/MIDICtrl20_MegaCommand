/* Justin Mammarella jmamma@gmail.com 2018 */
#ifndef MDTRACK_H__
#define MDTRACK_H__

#include "Grid.h"
#include "GridTrack.h"
#include "MD.h"
#include "MDSeqTrack.h"
#include "MDSeqTrackData.h"
#include "MCLMemory.h"

#define LOCK_AMOUNT 256
#define MD_TRACK_TYPE 1

class ParameterLock {
public:
  uint8_t param_number;
  uint8_t value;
  uint8_t step;
};

class KitExtra {
public:
  /** The settings of the reverb effect. f**/
  uint8_t reverb[8];
  /** The settings of the delay effect. **/
  uint8_t delay[8];
  /** The settings of the EQ effect. **/
  uint8_t eq[8];
  /** The settings of the compressor effect. **/
  uint8_t dynamics[8];
  uint32_t swingAmount;
  uint8_t accentAmount;
  uint8_t patternLength;
  uint8_t doubleTempo;
  uint8_t scale;

  uint64_t accentPattern;
  uint64_t slidePattern;
  uint64_t swingPattern;

  uint32_t accentEditAll;
  uint32_t slideEditAll;
  uint32_t swingEditAll;


};

class MDTrackLight : public GridTrack {
  public:
  MDSeqTrackData seq_data;
  MDMachine machine;

  // Store/retrieve portion of track object in mem bank2
  bool store_in_mem(uint8_t column, uint32_t region = BANK1_R1_START);
  bool load_from_mem(uint8_t column, uint32_t region = BANK1_R1_START);
};

class MDTrack : public MDTrackLight {
public:

  uint8_t origPosition;
  uint8_t patternOrigPosition;
  uint8_t length;
  uint64_t trigPattern;
  uint64_t accentPattern;
  uint64_t slidePattern;
  uint64_t swingPattern;

  KitExtra kitextra;

  int arraysize;
  ParameterLock locks[LOCK_AMOUNT];

  void clear_track();

  void place_track_in_kit(int tracknumber, uint8_t column, MDKit *kit, bool levels = true);
  void load_seq_data(int tracknumber);
  void place_track_in_pattern(int tracknumber, uint8_t column,
                              MDPattern *pattern);
  void get_machine_from_kit(int tracknumber, uint8_t column);
  bool get_track_from_sysex(int tracknumber, uint8_t column);
  void place_track_in_sysex(int tracknumber, uint8_t column);
  bool load_track_from_grid(int32_t column, int32_t row, int32_t len);
  bool load_track_from_grid(int32_t column, int32_t row);
  bool store_track_in_grid(int32_t column, int32_t row, int track = 255);

  //scale machine track vol by percentage
  void scale_vol(float scale);

  //scale vol locks by percentage
  void scale_seq_vol(float scale);

  // normalize track level
  void normalize();

};

#endif /* MDTRACK_H__ */
