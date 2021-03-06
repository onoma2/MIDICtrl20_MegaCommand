/* Copyright Justin Mammarella jmamma@gmail.com 2018 */

#ifndef GRIDROW_H__
#define GRIDROW_H__

#define GRID_WIDTH 22

class GridRowHeader {
 public:
  bool active;
  char name[17];
  uint8_t track_type[GRID_WIDTH];
  uint8_t model[GRID_WIDTH];

  void update_model(int16_t column, uint8_t model_, uint8_t track_type_);
  bool write(int16_t row);
  bool read(int16_t row);
  void init();
};

#endif /* GRIDROW_H__ */
