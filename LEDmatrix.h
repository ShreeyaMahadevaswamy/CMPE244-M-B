#include "gpio.h"
#include "lpc40xx.h"
#include "stdint.h"
#define swap(a, b)                                                                                                     \
  {                                                                                                                    \
    int16_t temp = a;                                                                                                  \
    a = b;                                                                                                             \
    b = temp;                                                                                                          \
  }

typedef enum {
  black,
  blue1,
  green1,
  cyan1,
  red1,
  purple1,
  yellow1,
  white1,

  blue2 = 8,
  green2 = 16,
  cyan2 = 24,
  red2 = 32,
  purple2 = 40,
  yellow2 = 48,
  white2 = 56
} color_codes;

typedef enum { Still = 0, Up, Down } autorunner_direction;

// void drawPixel(uint8_t row, uint8_t col, uint8_t color);   // Shr
void drawPixel(uint8_t row, uint8_t col, uint8_t color);
void clearLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color);
void drawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color);
void clear_pixel(uint8_t row, uint8_t col); // Shr
void color_pixel_position(int8_t *row, int8_t *color);
void display_digit(uint8_t x0, uint8_t y0, uint8_t value, uint8_t color);
void display_letter(uint8_t x, uint8_t y, char alpha, uint8_t color);
void display_game_name(uint8_t x, uint8_t y);
void autorunner_moving(void);
void led_display_init();
void draw_autorunner(uint8_t x, uint8_t color);
void generate_obstacle(void);
void draw_obstacle(void);
void shift_obstacle_left(void);
void getDirection(void);
void autorunner_movement(autorunner_direction get_direction);
void clear_display(void);
bool detect_collision(void);
void display_game_over(uint8_t x, uint8_t y);
void update_display(void);
void update_display2(void);
void get_colour(void);
void autorunner_colour_change(color_codes colour);
void behind_autorunner_clear_pixels(void);
void score_display(void);
void update_score(void);
void clear_score(void);
bool generate_obstacle_rand(void);
void final_score(void);
bool get_reset_ip(void);
void reset_score(void);
void reset_fn(void);