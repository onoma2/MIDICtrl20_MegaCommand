#include "Grid.h"
#include "MCL.h"

void Grid::setup() {}
/*
char* Grid::get_slot_kit(int column, int row, bool load, bool scroll) {

  A4Track track_buf;
  if (grid_page.grid_models[column] == EMPTY_TRACK_TYPE) {
    return "----";
  }


  uint8_t char_position = 0;
  //if ((slowclock % 50) == 0) { row_name_offset++; }
  if (row_name_offset > 15) {
    row_name_offset = 0;
  }
  if (scroll) {

    for (uint8_t c = 0; c < 4; c++) {

      if (c + (uint8_t)row_name_offset > 15) {
        char_position =  c + (uint8_t)row_name_offset - 16;
      }
      else {
        char_position =  c + (uint8_t)row_name_offset;
      }
      //  char some_string[] = "hello my baby";
      //row_name[c] = some_string[char_position];
      if (char_position < 5) {
        row_name[c] = ' ';
      }
      else {
        row_name[c] = grid_page.currentkitName[char_position - 5];
      }
    }
    row_name[4] = '\0';

  }
  else {
    for (uint8_t a = 0; a < 16; a++) {
      row_name[a] = grid_page.currentkitName[a];
    }
    row_name[16] = '\0';

  }

  return row_name;

}
*/
uint8_t Grid::get_slot_model(int column, int row, bool load, A4Track *track_buf) {
  if (column < 16) {
    if ( load == true) {
      if (!temptrack.load_track_from_grid(column, row, 50)) {
        return NULL;
      }
    }


    if (temptrack.active == EMPTY_TRACK_TYPE) {
      return NULL;
    }
    else {
      return temptrack.machine.model;
    }
  }

  else {
    if ( load == true) {
      if (!track_buf->load_track_from_grid(column, row, 50)) {
        return NULL;
      }
    }
    return track_buf->active;


  }

}

int32_t Grid::get_slot_offset(int16_t column, int16_t row) {
  int32_t offset =
      (int32_t)GRID_SLOT_BYTES + (int32_t)((column + 1) + (row * (GRID_WIDTH + 1))) * (int32_t)GRID_SLOT_BYTES;
  return offset;
}

int32_t Grid::get_header_offset(int16_t row) {
  int32_t offset =
      (int32_t)GRID_SLOT_BYTES + (int32_t)(0 + (row * (GRID_WIDTH + 1))) * (int32_t)GRID_SLOT_BYTES;
  return offset;
}


bool Grid::clear_slot(int16_t column, int16_t row, bool update_header) {
  bool ret;
  int b;
  if (update_header) {
    GridRowHeader row_header;
    row_header.read(row);
    row_header.update_model(column, EMPTY_TRACK_TYPE, DEVICE_NULL);
    row_header.write(row);
  }

  temptrack.active = EMPTY_TRACK_TYPE;
  int32_t offset = get_slot_offset(column, row);

  ret = proj.file.seekSet(offset);

  if (!ret) {
    DEBUG_PRINT_FN();
    DEBUG_PRINTLN("Clear grid failed: ");
    DEBUG_PRINTLN(row);
    DEBUG_PRINTLN(column);
    return false;
  }
  // DEBUG_PRINTLN("Writing");
  // DEBUG_PRINTLN(sizeof(temptrack.active));

  ret = mcl_sd.write_data((uint8_t *)&(temptrack.active),
                          sizeof(temptrack.active), &proj.file);
  if (!ret) {
    DEBUG_PRINTLN("Write failed");
    return false;
  }
  return true;
}

bool Grid::clear_row(int16_t row) {
  bool update_header = false;
  GridRowHeader row_header;
  row_header.init();
  for (int x = 0; x < GRID_WIDTH; x++) {
    clear_slot(x, row, update_header);
  }
  row_header.write(row);

}

Grid grid;
