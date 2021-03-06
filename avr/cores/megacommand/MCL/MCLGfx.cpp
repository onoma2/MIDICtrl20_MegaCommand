#include "MCL.h"
#include "MCLGfx.h"

void MCLGfx::init_oled() {
#ifdef OLED_DISPLAY
  oled_display.begin();

  oled_display.clearDisplay();
  oled_display.invertDisplay(0);
  oled_display.setRotation(2);
  oled_display.setTextSize(1);
  oled_display.setTextColor(WHITE, BLACK);
  oled_display.setCursor(0, 0);
  oled_display.display();
#endif
}

#ifdef OLED_DISPLAY
#define BITMAP_MCL_LOGO_W 58
#define BITMAP_MCL_LOGO_H 19

const unsigned char mcl_logo_bitmap[] PROGMEM = {
    0x01, 0xff, 0xf3, 0xff, 0xf3, 0xfe, 0x00, 0x00, 0x03, 0xff, 0xf3, 0xff,
    0xf3, 0xfe, 0x00, 0x00, 0x07, 0xff, 0xf3, 0xff, 0xf3, 0xfe, 0x00, 0x00,
    0x0f, 0xff, 0xf3, 0xff, 0xf3, 0xfe, 0x00, 0x00, 0x1f, 0xff, 0xf3, 0xff,
    0xf3, 0xfe, 0x00, 0x00, 0x3f, 0xff, 0xf3, 0xff, 0xf3, 0xfe, 0x00, 0x00,
    0x7f, 0xff, 0xf3, 0xf8, 0x03, 0xfe, 0x00, 0x00, 0xff, 0xff, 0xf3, 0xf8,
    0x03, 0xfe, 0x00, 0x00, 0xff, 0xff, 0xf3, 0xf8, 0x03, 0xfe, 0x00, 0x00,
    0x7f, 0xff, 0xf3, 0xf8, 0x03, 0xfe, 0x00, 0x00, 0x3f, 0xdf, 0xf3, 0xf8,
    0x03, 0xfe, 0x7f, 0xc0, 0x1f, 0x9f, 0xf3, 0xf8, 0x03, 0xfe, 0x7f, 0xc0,
    0x0f, 0x1f, 0xf3, 0xf8, 0x03, 0xfe, 0x7f, 0xc0, 0x06, 0x1f, 0xf3, 0xff,
    0xf3, 0xfe, 0x7f, 0x80, 0x00, 0x1f, 0xf3, 0xff, 0xf3, 0xfe, 0x7f, 0x00,
    0x00, 0x1f, 0xf3, 0xff, 0xf3, 0xfe, 0x7e, 0x00, 0x00, 0x1f, 0xf3, 0xff,
    0xf3, 0xfe, 0x7c, 0x00, 0x00, 0x1f, 0xf3, 0xff, 0xf3, 0xfe, 0x78, 0x00,
    0x00, 0x1f, 0xf3, 0xff, 0xf3, 0xfe, 0x70, 0x00};
#endif

void MCLGfx::splashscreen() {
#ifdef OLED_DISPLAY
  DEBUG_PRINTLN("OLED enabled");
  oled_display.setTextSize(2);
  oled_display.setTextColor(WHITE, BLACK);
  oled_display.setCursor(40, 0);

  oled_display.drawBitmap(35, 8, mcl_logo_bitmap, BITMAP_MCL_LOGO_W,
                          BITMAP_MCL_LOGO_H, WHITE);
  /* oled_display.println("MEGA");
   oled_display.setCursor(22, 15);
   oled_display.println("COMMAND");
   */
  oled_display.setCursor(90, 8);
  oled_display.setTextSize(1);
  oled_display.print("V");
  oled_display.print(VERSION_STR);
  /*  for (float length = 0; length < 32; length += 0.7) {

      // display.fillRect(0, 0, 128, length, BLACK);
      for (uint8_t x = 0; x < 50 + (length * 5); x++) {

        oled_display.drawPixel(random(20, 110), 30 - random(0, (int)length),
                               BLACK);
      }

   √   //:    display.drawLine(0, length, 128, length, BLACK);

      oled_display.display();
    }  oled_display.clearDisplay();
    */
  oled_display.display();

  delay(800);
#else

  DEBUG_PRINTLN("HD44780 enabled");
  char str1[17] = "MEGACOMMAND LIVE";
  char str2[17] = VERSION_STR;
  str1[16] = '\0';
  LCD.goLine(0);
  LCD.puts(str1);
  LCD.goLine(1);
  LCD.puts(str2);

  delay(200);
#endif
  // while (rec_global == 0) {

  //  GUI.setPage(&grid_page);
}

void MCLGfx::alert(char *str1, char *str2) {
#ifdef OLED_DISPLAY
  GFXfont *old_font = oled_display.getFont();
  oled_display.clearDisplay();
  oled_display.setFont(&TomThumb);
  oled_display.setCursor(0, 8);
  oled_display.setTextColor(WHITE, BLACK);
  oled_display.println(str1);
  oled_display.setCursor(0, 18);
  oled_display.println(str2);
  oled_display.display();
  delay(700);
  oled_display.setFont(old_font);
#else
  GUI.flash_strings_fill(str1, str2);
  GUI.display();
#endif
  DEBUG_PRINTLN(str1);
  DEBUG_PRINTLN(str2);
}

MCLGfx gfx;
