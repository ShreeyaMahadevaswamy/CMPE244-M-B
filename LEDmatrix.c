#include "gpio.h"
#include "stdint.h"
#include <stdio.h>

#include "LEDmatrix.h"
#include "delay.h"
#include "stdint.h"
//#include "GPIO_driver.h"
#include "lpc40xx.h"

#define MATRIX_HEIGHT 32
#define MATRIX_WIDTH 64
//#define MATRIX_NROWS 32
volatile uint8_t ledmatrix_buffer[MATRIX_HEIGHT][MATRIX_WIDTH] = {{0}};
// volatile uint8_t temp_buffer[MATRIX_HEIGHT][MATRIX_WIDTH] = {{0}};

volatile uint8_t autorunner_row = 11, autorunner_col = 3;
volatile uint8_t autorunner_colour = 2;
volatile uint8_t score_unit = 0;
volatile uint8_t score_tens_place = 0;
uint8_t obstacle_gap = 5;
volatile uint8_t rand_row, row_old = 9, colour_old = red1;

volatile gpio_s r1;
volatile gpio_s g1;
volatile gpio_s b1;
volatile gpio_s r2;
volatile gpio_s g2;
volatile gpio_s b2;
volatile gpio_s a;
volatile gpio_s b;
volatile gpio_s c;
volatile gpio_s d;
volatile gpio_s clk;
volatile gpio_s lat;
volatile gpio_s oe;

void led_display_init() {
  r1 = gpio__construct_as_output(GPIO__PORT_0, 8);
  g1 = gpio__construct_as_output(GPIO__PORT_0, 26);
  b1 = gpio__construct_as_output(GPIO__PORT_1, 31);
  r2 = gpio__construct_as_output(GPIO__PORT_1, 20);
  g2 = gpio__construct_as_output(GPIO__PORT_2, 2);
  b2 = gpio__construct_as_output(GPIO__PORT_2, 5);
  a = gpio__construct_as_output(GPIO__PORT_2, 7);
  b = gpio__construct_as_output(GPIO__PORT_2, 9);
  c = gpio__construct_as_output(GPIO__PORT_0, 15);
  d = gpio__construct_as_output(GPIO__PORT_0, 18);
  clk = gpio__construct_as_output(GPIO__PORT_0, 7);
  lat = gpio__construct_as_output(GPIO__PORT_0, 9);
  oe = gpio__construct_as_output(GPIO__PORT_0, 25);

  gpio__reset(r1);
  gpio__reset(g1);
  gpio__reset(b1);
  gpio__reset(r2);
  gpio__reset(g2);
  gpio__reset(b2);
  gpio__reset(a);
  gpio__reset(b);
  gpio__reset(c);
  gpio__reset(d);
  gpio__reset(clk);
  gpio__reset(lat);
  gpio__reset(oe);

  memset(ledmatrix_buffer, 0, MATRIX_HEIGHT * MATRIX_WIDTH);
}

// Draw pixel function
void drawPixel(uint8_t row, uint8_t col, uint8_t color) {
  color_pixel_position(&row, &color);
  ledmatrix_buffer[row][col] |= color;
}

// Clear pixel function
void clear_pixel(uint8_t row, uint8_t col) {
  // if (((row >= 0) || (row <= 31)) && ((col >= 0) || (col <= 63))) {
  ledmatrix_buffer[row][col] = 0;
  //}
}

// Colour value update for the rows below 16 which are controlled by 3 different wires
void color_pixel_position(int8_t *row, int8_t *color) {
  if (*row > 15) {
    switch (*color) {
    case blue1:
      *color = blue2;
      break;
    case cyan1:
      *color = cyan2;
      break;
    case green1:
      *color = green2;
      break;
    case purple1:
      *color = purple2;
      break;
    case red1:
      *color = red2;
      break;
    case white1:
      *color = white2;
      break;
    case yellow1:
      *color = yellow2;
      break;
    }
  }
}

void update_display(void) {
  for (uint8_t row = 0; row < 32; row++) {
    for (uint8_t col = 0; col < 64; col++) {
      if (ledmatrix_buffer[row][col] & 0x1) {
        gpio__set(b1);
      } else {
        gpio__reset(b1);
      }
      if (ledmatrix_buffer[row][col] & 0x2) {
        gpio__set(g1);
      } else {
        gpio__reset(g1);
      }
      if (ledmatrix_buffer[row][col] & 0x4) {
        gpio__set(r1);
      } else {
        gpio__reset(r1);
      }
      if (ledmatrix_buffer[row][col] & 0x8) {
        gpio__set(b2);
      } else {
        gpio__reset(b2);
      }
      if (ledmatrix_buffer[row][col] & 0x10) {
        gpio__set(g2);
      } else {
        gpio__reset(g2);
      }
      if (ledmatrix_buffer[row][col] & 0x20) {
        gpio__set(r2);
      } else {
        gpio__reset(r2);
      }

      gpio__set(clk);
      gpio__reset(clk);
    }
    gpio__set(oe);
    gpio__set(lat);

    delay__us(250);

    gpio__reset(a);
    gpio__reset(b);
    gpio__reset(c);
    gpio__reset(d);

    if (row & 0x1) {
      gpio__set(a);
    }

    if (row & 0x2) {
      gpio__set(b);
    }

    if (row & 0x4) {
      gpio__set(c);
    }

    if (row & 0x8) {
      gpio__set(d);
    }

    gpio__reset(lat);
    gpio__reset(oe);
  }

  clear_runner();
  draw_autorunner(autorunner_row, autorunner_colour);
  clear_score();
  score_display();
}

void update_display2(void) {

  for (uint8_t row = 0; row < 32; row++) {
    for (uint8_t col = 0; col < 64; col++) {
      if (ledmatrix_buffer[row][col] & 0x1) {
        gpio__set(b1);
      } else {
        gpio__reset(b1);
      }
      if (ledmatrix_buffer[row][col] & 0x2) {
        gpio__set(g1);
      } else {
        gpio__reset(g1);
      }
      if (ledmatrix_buffer[row][col] & 0x4) {
        gpio__set(r1);
      } else {
        gpio__reset(r1);
      }
      if (ledmatrix_buffer[row][col] & 0x8) {
        gpio__set(b2);
      } else {
        gpio__reset(b2);
      }
      if (ledmatrix_buffer[row][col] & 0x10) {
        gpio__set(g2);
      } else {
        gpio__reset(g2);
      }
      if (ledmatrix_buffer[row][col] & 0x20) {
        gpio__set(r2);
      } else {
        gpio__reset(r2);
      }
      gpio__set(clk);
      gpio__reset(clk);
    }
    gpio__set(oe);
    gpio__set(lat);

    delay__us(250);

    gpio__reset(a);
    gpio__reset(b);
    gpio__reset(c);
    gpio__reset(d);

    if (row & 0x1) {
      gpio__set(a);
    }

    if (row & 0x2) {
      gpio__set(b);
    }

    if (row & 0x4) {
      gpio__set(c);
    }

    if (row & 0x8) {
      gpio__set(d);
    }

    gpio__reset(lat);
    gpio__reset(oe);
  }
}

// Draw autorunner
void draw_autorunner(uint8_t x, uint8_t color) {
  drawPixel(x, 4, color);
  drawPixel(x + 1, 4, color);
  drawPixel(x + 2, 4, color);
  drawPixel(x + 1, 3, color);
  drawPixel(x + 1, 5, color);
  drawPixel(x + 3, 3, color);
  drawPixel(x + 3, 5, color);
  behind_autorunner_clear_pixels();
}

// Display game name
void display_game_name(uint8_t x, uint8_t y) {
  display_letter(x, y, 'M', purple1);
  display_letter(x, y + 7, 'O', purple1);
  display_letter(x, y + 15, 'R', purple1);
  display_letter(x, y + 22, 'P', purple1);
  display_letter(x, y + 28, 'H', purple1);

  display_letter(x + 10, y + 13, '&', white1);

  display_letter(x + 20, y, 'B', blue1);
  display_letter(x + 20, y + 7, 'L', blue1);
  display_letter(x + 20, y + 13, 'E', blue1);
  display_letter(x + 20, y + 19, 'n', blue1);
  display_letter(x + 20, y + 25, 'D', blue1);
}

// moving autorunner in the initial screen
void autorunner_moving(void) {
  for (int x = 13; x < 20; x++) {
    autorunner_row = x;
    draw_autorunner(x, blue1);
    delay__ms(100);
    clear_runner();
  }
  for (int x = 19; x > 14; x--) {
    autorunner_row = x;
    draw_autorunner(x, blue1);
    delay__ms(100);
    clear_runner();
  }
}

// Clearing pixels behind autorunner
void behind_autorunner_clear_pixels(void) {
  for (int i = autorunner_row; i < autorunner_row + 4; i++) {
    for (int j = 0; j < autorunner_col; j++) {
      ledmatrix_buffer[i][j] = 0;
    }
  }
  ledmatrix_buffer[autorunner_row][autorunner_col] = 0;
  ledmatrix_buffer[autorunner_row + 2][autorunner_col] = 0;
  ledmatrix_buffer[autorunner_row + 3][autorunner_col + 1] = 0;
}

// To Clear entire display
void clear_display(void) {
  for (uint8_t row = 0; row < 32; row++) {
    for (uint8_t col = 0; col < 64; col++) {
      // clear_pixel(row, col, color);
      ledmatrix_buffer[row][col] = 0;
    }
  }
}

// To Clear the autorunner
void clear_runner(void) {
  clear_pixel(autorunner_row, 4);
  clear_pixel(autorunner_row + 1, 4);
  clear_pixel(autorunner_row + 2, 4);
  clear_pixel(autorunner_row + 1, 3);
  clear_pixel(autorunner_row + 1, 5);
  clear_pixel(autorunner_row + 3, 3);
  clear_pixel(autorunner_row + 3, 5);
}

gpio_s jump_up;
gpio_s jump_down;

// Configure GPIOs for getting autorunner movement inputs
void getDirection(void) {
  jump_up = gpio__construct_as_input(GPIO__PORT_2, 6);
  jump_down = gpio__construct_as_input(GPIO__PORT_2, 8);
  autorunner_direction direction;
  bool a = gpio2__get_level(6);
  bool b = gpio2__get_level(8);

  printf("bool a %d b %d\n", a, b);
  if ((!a)) {
    direction = Up;
    autorunner_movement(Up);
  } else if ((!b)) {
    direction = Down;
    autorunner_movement(Down);
  } else {
    direction = Still;
  }
}

gpio_s red;
gpio_s blue;
gpio_s yellow;
gpio_s green;

// Configure GPIOs for getting autorunner colour inputs
void get_colour(void) {
  red = gpio__construct_as_input(GPIO__PORT_0, 16);
  blue = gpio__construct_as_input(GPIO__PORT_0, 17);
  yellow = gpio__construct_as_input(GPIO__PORT_0, 22);
  green = gpio__construct_as_input(GPIO__PORT_0, 0);
  bool r = gpio0__get_level(16);
  bool b = gpio0__get_level(17);
  bool y = gpio0__get_level(22);
  bool g = gpio0__get_level(0);

  // printf("bool r %d b %d\n", r, b);
  if (!r) {
    autorunner_colour = red1;
    // autorunner_colour_change(red1);
  } else if (!b) {
    autorunner_colour = blue1;
    // autorunner_colour_change(blue1);
  } else if (!y) {
    autorunner_colour = yellow1;
    //  autorunner_colour_change(yellow1);
  } else if (!g) {
    autorunner_colour = green1;
    //  autorunner_colour_change(green1);
  } else {
    // nothing
  }
}

// Autorunner colour update function
void autorunner_colour_change(color_codes colour) {
  if (autorunner_row > 15) {
    switch (colour) {
    case red1:
      autorunner_colour = red2;
      clear_runner();
      break;
    case blue1:
      autorunner_colour = blue2;
      clear_runner();
      break;
    case yellow1:
      autorunner_colour = yellow2;
      clear_runner();
      break;
    case green1:
      autorunner_colour = green2;
      clear_runner();
      break;
    default:
      // nothing
      break;
    }
  }
}

void reset_fn(void) {
  autorunner_colour = blue1;
  autorunner_row = 11;
}
// Autorunner position update function
void autorunner_movement(autorunner_direction get_direction) {
  switch (get_direction) {
  case Up:
    if (autorunner_row >= 3) {
      clear_runner();
      autorunner_row--;
      draw_autorunner(autorunner_row, autorunner_colour);
      // gpio1__set_low(23);
    }
    break;
  case Down:
    if (autorunner_row <= 24) {
      clear_runner();
      autorunner_row++;
      draw_autorunner(autorunner_row, autorunner_colour);
      // gpio1__set_low(29);
    }
    break;
  default:
    // nothing
    break;
  }
  printf(" row %0d\n", autorunner_row);
}

// To generate random obstacles
bool generate_obstacle_rand(void) {
  bool ret;
  uint8_t randomize_color = (rand() % 4) + 1;
  rand_row = (rand() % 17) + 9;
  while (!(((row_old - rand_row) >= obstacle_gap) || ((rand_row - row_old) >= obstacle_gap))) {
    rand_row = (rand() % 17) + 9;
  }
  while (colour_old == randomize_color || ((colour_old == yellow1) && (randomize_color == cyan1))) {
    randomize_color = (rand() % 3) + 1;
  }
  if (randomize_color == cyan1) {
    randomize_color = yellow1;
  }
  int8_t row = rand_row;
  for (int8_t col = 63; col > 52; col--) {
    drawPixel(row, col, randomize_color);
    drawPixel(row + 1, col, randomize_color);
    ret = 0;
  }
  row_old = rand_row;
  colour_old = randomize_color;
  ret = 1;
  return (ret);
}

// To shift obstacles towards the autorunner continuously
void shift_obstacle_left(void) {
  int8_t temp;
  for (int8_t row1 = 9; row1 < 28; row1++) {
    for (int8_t col1 = 0; col1 < 63; col1++) {
      ledmatrix_buffer[row1][col1] = ledmatrix_buffer[row1][col1 + 1]; // move all element to the left except first
    }
    ledmatrix_buffer[row1][63] = 0;
  }
  behind_autorunner_clear_pixels();
}

// To detect colour mismatch during blending
bool detect_collision(void) {
  bool detect = 0;
  if (((ledmatrix_buffer[autorunner_row][4] != ledmatrix_buffer[autorunner_row][5]) &&
       (ledmatrix_buffer[autorunner_row][5] != 0)) ||
      ((ledmatrix_buffer[autorunner_row + 1][5] != ledmatrix_buffer[autorunner_row + 1][6]) &&
       (ledmatrix_buffer[autorunner_row + 1][6] != 0)) ||
      ((ledmatrix_buffer[autorunner_row + 2][4] != ledmatrix_buffer[autorunner_row + 2][5]) &&
       (ledmatrix_buffer[autorunner_row + 2][5] != 0)) ||
      ((ledmatrix_buffer[autorunner_row + 3][5] != ledmatrix_buffer[autorunner_row + 3][6]) &&
       (ledmatrix_buffer[autorunner_row + 3][6] != 0))) {
    detect = 1;
    if (detect) {
      printf("colours %d %d \n %d %d \n %d %d \n %d %d \n", ledmatrix_buffer[autorunner_row][4],
             ledmatrix_buffer[autorunner_row][5], ledmatrix_buffer[autorunner_row + 1][5],
             ledmatrix_buffer[autorunner_row + 1][6], ledmatrix_buffer[autorunner_row + 2][4],
             ledmatrix_buffer[autorunner_row + 2][5], ledmatrix_buffer[autorunner_row + 3][5],
             ledmatrix_buffer[autorunner_row + 3][6]);

      // printf("temp buffer colours %d %d \n %d %d  \n %d %d \n %d %d \n", temp_buffer[autorunner_row][4],
      //        ledmatrix_buffer[autorunner_row][5], temp_buffer[autorunner_row + 1][5],
      //        ledmatrix_buffer[autorunner_row + 1][6], temp_buffer[autorunner_row + 2][4],
      //        ledmatrix_buffer[autorunner_row + 2][5], temp_buffer[autorunner_row + 3][5],
      //        ledmatrix_buffer[autorunner_row + 3][6]);
    }
  } else {
    detect = 0;
    // printf("no collision");
  }
  // printf("detect: %d\n", detect);
  return (detect);
}

// Score display on top of game screen
void score_display(void) {
  display_letter(0, 5, 'S', blue1);
  display_letter(0, 11, 'C', blue1);
  display_letter(0, 18, 'O', blue1);
  display_letter(0, 26, 'R', blue1);
  display_letter(0, 34, 'E', blue1);
  display_digit(0, 46, score_tens_place, blue1);
  display_digit(0, 53, score_unit, blue1);
}

// To display score at the end of the game
void final_score(void) {
  display_letter(4, 17, 'F', blue1);
  display_letter(4, 23, 'I', blue1);
  display_letter(4, 30, 'N', blue1);
  display_letter(4, 38, 'A', blue1);
  display_letter(4, 45, 'L', blue1);
  display_letter(14, 17, 'S', blue1);
  display_letter(14, 23, 'C', blue1);
  display_letter(14, 30, 'O', blue1);
  display_letter(14, 38, 'R', blue1);
  display_letter(14, 45, 'E', blue1);
  display_digit(24, 28, score_tens_place, blue1);
  display_digit(24, 35, score_unit, blue1);
}
// To clear the score digits in game screen
void clear_score() {
  for (int i = 0; i < 8; i++) {
    for (int j = 46; j < 64; j++) {
      ledmatrix_buffer[i][j] = 0;
    }
  }
}

void reset_score(void) {
  score_unit = 0;
  score_tens_place = 0;
}

// incrementing the score dynamically with each successful blend
void update_score(void) {
  int blend = 0;
  if (((ledmatrix_buffer[autorunner_row][4] == ledmatrix_buffer[autorunner_row][5]) &&
       (ledmatrix_buffer[autorunner_row][5] != 0)) ||
      ((ledmatrix_buffer[autorunner_row + 1][5] == ledmatrix_buffer[autorunner_row + 1][6]) &&
       (ledmatrix_buffer[autorunner_row + 1][6] != 0)) ||
      ((ledmatrix_buffer[autorunner_row + 2][4] == ledmatrix_buffer[autorunner_row + 2][5]) &&
       (ledmatrix_buffer[autorunner_row + 2][5] != 0)) ||
      ((ledmatrix_buffer[autorunner_row + 3][5] == ledmatrix_buffer[autorunner_row + 3][6]) &&
       (ledmatrix_buffer[autorunner_row + 3][6] != 0))) {
    blend = 1;

    printf("colours %d %d \n %d %d \n %d %d \n %d %d \n", ledmatrix_buffer[autorunner_row][4],
           ledmatrix_buffer[autorunner_row][5], ledmatrix_buffer[autorunner_row + 1][5],
           ledmatrix_buffer[autorunner_row + 1][6], ledmatrix_buffer[autorunner_row + 2][4],
           ledmatrix_buffer[autorunner_row + 2][5], ledmatrix_buffer[autorunner_row + 3][5],
           ledmatrix_buffer[autorunner_row + 3][6]);
  } else {
    blend = 0;
  }
  if (score_unit < 9) {
    if (blend == 1) {
      score_unit++;
      printf("score: %d\n", score_unit);
      delay__ms(600);
    }
  } else if (score_unit == 9) {
    if (blend == 1) {
      score_unit = 0;
      score_tens_place++;
      printf("score: %d\n", score_unit);
      delay__ms(600);
    }
  }
}

// Display game over when there is a color blend mismatch
void display_game_over(uint8_t x, uint8_t y) {
  display_letter(x, y, 'G', purple1);
  display_letter(x, y + 7, 'A', purple1);
  display_letter(x, y + 15, 'M', purple1);
  display_letter(x, y + 22, 'E', purple1);

  display_letter(x + 10, y + 5, 'O', red1);
  display_letter(x + 10, y + 13, 'V', white1);
  display_letter(x + 10, y + 23, 'E', red1);
  display_letter(x + 10, y + 31, 'R', blue1);
}

// Function to draw line
void drawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color) {
  uint16_t slope = (abs(y1 - y0) > abs(x1 - x0)) ? 1 : 0;
  int16_t dx, dy;
  int16_t err;
  int16_t ystep;

  if (slope) {
    swap(x0, y0);
    swap(x1, y1);
  }

  if (x0 > x1) {
    swap(x0, x1);
    swap(y0, y1);
  }

  dx = x1 - x0;
  dy = abs(y1 - y0);
  err = dx / 2;

  if (y0 < y1) {
    ystep = 1;
  } else {
    ystep = -1;
  }

  for (; x0 <= x1; x0++) {
    if (slope) {
      drawPixel(y0, x0, color);
    } else {
      drawPixel(x0, y0, color);
    }

    err -= dy;

    if (err < 0) {
      y0 += ystep;
      err += dx;
    }
  }
}

// Function to  clear line
void clearLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color) {
  int16_t slope = (abs(y1 - y0) > abs(x1 - x0)) ? 1 : 0;
  int16_t dx, dy;
  int16_t err;
  int16_t ystep;

  if (slope) {
    swap(x0, y0);
    swap(x1, y1);
  }
  if (x0 > x1) {
    swap(x0, x1);
    swap(y0, y1);
  }

  dx = x1 - x0;
  dy = abs(y1 - y0);
  err = dx / 2;

  if (y0 < y1) {
    ystep = 1;
  } else {
    ystep = -1;
  }

  for (; x0 <= x1; x0++) {
    if (slope) {
      clearPixel(y0, x0, color);
    } else {
      clearPixel(x0, y0, color);
    }

    err -= dy;

    if (err < 0) {
      y0 += ystep;
      err += dx;
    }
  }
}

// Function to display digits
void display_digit(uint8_t x0, uint8_t y0, uint8_t value, uint8_t color) {
  if (value == 0) {
    // drawPixel(x0, y0 + 1, color);
    // drawPixel(x0, y0 + 2, color);
    // drawPixel(x0, y0 + 3, color);
    // drawPixel(x0, y0 + 4, color);
    // drawPixel(x0, y0 + 5, color);
    // drawPixel(x0, y0 + 6, color);
    // drawPixel(x0 + 1, y0, color);
    // drawPixel(x0 + 1, y0 + 7, color);
    // drawPixel(x0 + 2, y0, color);
    // drawPixel(x0 + 2, y0 + 7, color);
    // drawPixel(x0 + 3, y0, color);
    // drawPixel(x0 + 3, y0 + 7, color);
    // drawPixel(x0 + 4, y0 + 1, color);
    // drawPixel(x0 + 4, y0 + 2, color);
    // drawPixel(x0 + 4, y0 + 3, color);
    // drawPixel(x0 + 4, y0 + 4, color);
    // drawPixel(x0 + 4, y0 + 5, color);
    // drawPixel(x0 + 4, y0 + 6, color);

    drawPixel(x0, y0 + 1, color);
    drawPixel(x0, y0 + 2, color);
    drawPixel(x0, y0 + 3, color);
    drawPixel(x0, y0 + 4, color);
    // drawPixel(x0, y0 + 5, color);
    // drawPixel(x0, y0 + 6, color);
    drawPixel(x0 + 1, y0, color);
    drawPixel(x0 + 1, y0 + 5, color);
    drawPixel(x0 + 2, y0, color);
    drawPixel(x0 + 2, y0 + 5, color);
    drawPixel(x0 + 3, y0, color);
    drawPixel(x0 + 3, y0 + 5, color);
    drawPixel(x0 + 4, y0 + 1, color);
    drawPixel(x0 + 4, y0 + 2, color);
    drawPixel(x0 + 4, y0 + 3, color);
    drawPixel(x0 + 4, y0 + 4, color);
    // drawPixel(x0 + 4, y0 + 5, color);
    // drawPixel(x0 + 4, y0 + 6, color);
  } else if (value == 1) {
    drawLine(x0, y0, x0 + 4, y0, color);
    drawPixel(x0 + 1, y0 - 1, color);
    drawLine(x0 + 4, y0 - 1, x0 + 4, y0 + 1, color);
  } else if (value == 2) {
    drawPixel(x0 + 1, y0, color);
    drawPixel(x0, y0 + 1, color);
    drawPixel(x0, y0 + 2, color);
    drawPixel(x0 + 1, y0 + 3, color);
    drawPixel(x0 + 2, y0 + 2, color);
    drawPixel(x0 + 3, y0 + 1, color);
    drawPixel(x0 + 4, y0, color);
    drawPixel(x0 + 4, y0 + 1, color);
    drawPixel(x0 + 4, y0 + 2, color);
    drawPixel(x0 + 4, y0 + 3, color);

  } else if (value == 3) {
    drawPixel(x0, y0 + 1, color);
    drawPixel(x0, y0 + 2, color);
    drawPixel(x0 + 1, y0 + 3, color);
    drawPixel(x0 + 2, y0 + 2, color);
    drawPixel(x0 + 3, y0 + 3, color);
    drawPixel(x0 + 4, y0 + 1, color);
    drawPixel(x0 + 4, y0 + 2, color);
  } else if (value == 4) {
    drawPixel(x0, y0 + 3, color);
    drawPixel(x0 + 1, y0 + 3, color);
    drawPixel(x0 + 1, y0 + 2, color);
    drawPixel(x0 + 2, y0 + 3, color);
    drawPixel(x0 + 2, y0 + 1, color);
    drawPixel(x0 + 3, y0, color);
    drawPixel(x0 + 3, y0 + 1, color);
    drawPixel(x0 + 3, y0 + 2, color);
    drawPixel(x0 + 3, y0 + 3, color);
    drawPixel(x0 + 3, y0 + 4, color);
    drawPixel(x0 + 4, y0 + 3, color);
  } else if (value == 5) {
    drawPixel(x0, y0, color);
    drawPixel(x0, y0 + 1, color);
    drawPixel(x0, y0 + 2, color);
    drawPixel(x0, y0 + 3, color);
    drawPixel(x0 + 1, y0, color);
    drawPixel(x0 + 2, y0, color);
    drawPixel(x0 + 2, y0 + 1, color);
    drawPixel(x0 + 2, y0 + 2, color);
    drawPixel(x0 + 3, y0 + 3, color);
    drawPixel(x0 + 4, y0, color);
    drawPixel(x0 + 4, y0 + 1, color);
    drawPixel(x0 + 4, y0 + 2, color);
    printf("display 5");
  } else if (value == 6) {
    drawPixel(x0 + 1, y0, color);
    drawPixel(x0 + 2, y0, color);
    drawPixel(x0 + 3, y0, color);
    drawPixel(x0 + 4, y0, color);
    drawPixel(x0 + 5, y0, color);
    // drawPixel(x0+6, y0 , color);
    drawPixel(x0, y0 + 1, color);
    drawPixel(x0 + 4, y0 + 1, color);
    drawPixel(x0 + 7, y0 + 1, color);
    drawPixel(x0, y0 + 2, color);
    drawPixel(x0 + 4, y0 + 2, color);
    drawPixel(x0 + 7, y0 + 2, color);
    drawPixel(x0, y0 + 3, color);
    drawPixel(x0 + 4, y0 + 3, color);
    drawPixel(x0 + 7, y0 + 3, color);
    drawPixel(x0 + 1, y0 + 4, color);
    drawPixel(x0 + 2, y0 + 4, color);
    drawPixel(x0 + 3, y0 + 4, color);
    drawPixel(x0 + 6, y0 + 4, color);
  } else if (value == 7) {
    drawPixel(x0, y0 + 7, color);
    drawPixel(x0 + 1, y0 + 7, color);
    drawPixel(x0 + 2, y0, color);
    drawPixel(x0 + 2, y0 + 1, color);
    drawPixel(x0 + 2, y0 + 2, color);
    drawPixel(x0 + 2, y0 + 3, color);
    drawPixel(x0 + 2, y0 + 7, color);
    drawPixel(x0 + 3, y0 + 4, color);
    drawPixel(x0 + 3, y0 + 7, color);
    drawPixel(x0 + 4, y0 + 5, color);
    drawPixel(x0 + 4, y0 + 6, color);
    drawPixel(x0 + 4, y0 + 7, color);
  } else if (value == 8) {
    drawPixel(x0, y0 + 1, color);
    drawPixel(x0, y0 + 2, color);
    drawPixel(x0, y0 + 3, color);
    drawPixel(x0, y0 + 5, color);
    drawPixel(x0, y0 + 6, color);
    drawPixel(x0 + 1, y0, color);
    drawPixel(x0 + 1, y0 + 4, color);
    drawPixel(x0 + 1, y0 + 7, color);
    drawPixel(x0 + 2, y0, color);
    drawPixel(x0 + 2, y0 + 4, color);
    drawPixel(x0 + 2, y0 + 7, color);
    drawPixel(x0 + 3, y0, color);
    drawPixel(x0 + 3, y0 + 4, color);
    drawPixel(x0 + 3, y0 + 7, color);
    drawPixel(x0 + 4, y0 + 1, color);
    drawPixel(x0 + 4, y0 + 2, color);
    drawPixel(x0 + 4, y0 + 3, color);
    drawPixel(x0 + 4, y0 + 5, color);
    drawPixel(x0 + 4, y0 + 6, color);
  } else if (value == 9) {
    drawPixel(x0, y0 + 1, color);
    drawPixel(x0, y0 + 4, color);
    drawPixel(x0, y0 + 5, color);
    drawPixel(x0, y0 + 6, color);
    drawPixel(x0 + 1, y0, color);
    drawPixel(x0 + 1, y0 + 3, color);
    drawPixel(x0 + 1, y0 + 7, color);
    drawPixel(x0 + 2, y0, color);
    drawPixel(x0 + 2, y0 + 3, color);
    drawPixel(x0 + 2, y0 + 7, color);
    drawPixel(x0 + 3, y0, color);
    drawPixel(x0 + 3, y0 + 3, color);
    drawPixel(x0 + 3, y0 + 7, color);
    drawPixel(x0 + 4, y0 + 1, color);
    drawPixel(x0 + 4, y0 + 2, color);
    drawPixel(x0 + 4, y0 + 3, color);
    drawPixel(x0 + 4, y0 + 4, color);
    drawPixel(x0 + 4, y0 + 5, color);
    drawPixel(x0 + 4, y0 + 6, color);
  }
}

// To display letters
void display_letter(uint8_t x, uint8_t y, char alpha, uint8_t color) {
  if (alpha == 'a' || alpha == 'A') {
    drawPixel(x, y + 1, color);
    drawPixel(x, y + 4, color);
    drawPixel(x, y + 2, color);
    drawPixel(x, y + 3, color);
    drawPixel(x + 1, y, color);
    drawPixel(x + 1, y + 1, color);
    drawPixel(x + 1, y + 2, color);
    drawPixel(x + 1, y + 3, color);
    drawPixel(x + 1, y + 4, color);
    drawPixel(x + 1, y + 5, color);
    drawPixel(x + 2, y, color);
    drawPixel(x + 2, y + 1, color);
    drawPixel(x + 2, y + 4, color);
    drawPixel(x + 2, y + 5, color);
    drawPixel(x + 3, y, color);
    drawPixel(x + 3, y + 1, color);
    drawPixel(x + 3, y + 2, color);
    drawPixel(x + 3, y + 3, color);
    drawPixel(x + 3, y + 4, color);
    drawPixel(x + 3, y + 5, color);
    drawPixel(x + 4, y, color);
    drawPixel(x + 4, y + 1, color);
    drawPixel(x + 4, y + 2, color);
    drawPixel(x + 4, y + 3, color);
    drawPixel(x + 4, y + 4, color);
    drawPixel(x + 4, y + 5, color);
    drawPixel(x + 5, y, color);
    drawPixel(x + 5, y + 1, color);
    drawPixel(x + 5, y + 4, color);
    drawPixel(x + 5, y + 5, color);
    drawPixel(x + 6, y, color);
    drawPixel(x + 6, y + 1, color);
    drawPixel(x + 6, y + 4, color);
    drawPixel(x + 6, y + 5, color);
  }

  if (alpha == 'b' || alpha == 'B') {
    drawPixel(x + 2, y + 2, color);
    drawPixel(x, y + 2, color);
    drawPixel(x + 1, y + 4, color);
    drawPixel(x + 3, y + 4, color);
    drawPixel(x, y + 3, color);
    drawPixel(x + 4, y + 3, color);
    drawPixel(x + 4, y + 2, color);
    drawPixel(x + 2, y, color);
    drawPixel(x + 2, y + 1, color);
    drawPixel(x + 2, y + 2, color);
    drawPixel(x + 2, y + 3, color);
    drawPixel(x, y + 1, color);
    drawPixel(x + 4, y + 1, color);
    drawPixel(x, y, color);
    drawPixel(x + 1, y, color);
    drawPixel(x + 2, y, color);
    drawPixel(x + 3, y, color);
    drawPixel(x + 4, y, color);
  }

  if (alpha == 'c' || alpha == 'C') {

    drawPixel(x, y + 1, color);
    drawPixel(x, y + 2, color);
    drawPixel(x, y + 3, color);
    drawPixel(x, y + 4, color);
    drawPixel(x + 1, y, color);
    drawPixel(x + 1, y + 1, color);
    drawPixel(x + 1, y + 2, color);
    drawPixel(x + 1, y + 3, color);
    drawPixel(x + 1, y + 4, color);
    drawPixel(x + 2, y, color);
    drawPixel(x + 2, y + 1, color);
    drawPixel(x + 3, y, color);
    drawPixel(x + 3, y + 1, color);
    drawPixel(x + 4, y, color);
    drawPixel(x + 4, y + 1, color);
    drawPixel(x + 5, y + 1, color);
    drawPixel(x + 5, y + 2, color);
    drawPixel(x + 5, y + 3, color);
    drawPixel(x + 5, y + 4, color);
    drawPixel(x + 5, y, color);
    drawPixel(x + 6, y + 1, color);
    drawPixel(x + 6, y + 2, color);
    drawPixel(x + 6, y + 3, color);
    drawPixel(x + 6, y + 4, color);
  }

  if (alpha == 'D') {
    drawPixel(x, y, color);
    drawPixel(x + 6, y, color);
    drawPixel(x, y + 1, color);
    drawPixel(x, y + 2, color);
    drawPixel(x, y + 3, color);
    drawPixel(x, y + 4, color);
    drawPixel(x, y + 5, color);
    drawPixel(x + 1, y + 1, color);
    drawPixel(x + 1, y + 5, color);
    drawPixel(x + 1, y, color);
    drawPixel(x + 1, y + 2, color);
    drawPixel(x + 1, y + 3, color);
    drawPixel(x + 1, y + 4, color);
    drawPixel(x + 1, y + 6, color);
    drawPixel(x + 2, y + 0, color);
    drawPixel(x + 2, y + 1, color);
    drawPixel(x + 2, y + 5, color);
    drawPixel(x + 2, y + 6, color);
    drawPixel(x + 3, y + 0, color);
    drawPixel(x + 3, y + 1, color);
    drawPixel(x + 3, y + 5, color);
    drawPixel(x + 3, y + 6, color);
    drawPixel(x + 4, y + 0, color);
    drawPixel(x + 4, y + 1, color);
    drawPixel(x + 4, y + 5, color);
    drawPixel(x + 4, y + 6, color);
    drawPixel(x + 5, y + 1, color);
    drawPixel(x + 5, y + 5, color);
    drawPixel(x + 5, y, color);
    drawPixel(x + 5, y + 2, color);
    drawPixel(x + 5, y + 3, color);
    drawPixel(x + 5, y + 4, color);
    drawPixel(x + 5, y + 6, color);
    drawPixel(x + 6, y + 1, color);
    drawPixel(x + 6, y + 2, color);
    drawPixel(x + 6, y + 3, color);
    drawPixel(x + 6, y + 4, color);
    drawPixel(x + 6, y + 5, color);
  }

  if (alpha == '&') {
    drawPixel(x, y + 2, color);
    drawPixel(x, y + 3, color);
    drawPixel(x + 1, y + 2, color);
    drawPixel(x + 1, y + 3, color);
    drawPixel(x + 1, y + 1, color);
    drawPixel(x + 1, y + 4, color);
    drawPixel(x + 2, y, color);
    drawPixel(x + 2, y + 1, color);
    drawPixel(x + 2, y + 4, color);
    drawPixel(x + 2, y + 5, color);
    drawPixel(x + 3, y + 2, color);
    drawPixel(x + 3, y + 3, color);
    drawPixel(x + 4, y + 2, color);
    drawPixel(x + 4, y + 3, color);
    drawPixel(x + 5, y, color);
    drawPixel(x + 5, y + 1, color);
    drawPixel(x + 5, y + 4, color);
    drawPixel(x + 5, y + 5, color);
    drawPixel(x + 6, y + 2, color);
    drawPixel(x + 6, y + 3, color);
    drawPixel(x + 6, y + 1, color);
    drawPixel(x + 6, y + 4, color);
    drawPixel(x + 7, y + 2, color);
    drawPixel(x + 7, y + 3, color);
    drawPixel(x + 4, y + 5, color);
    drawPixel(x + 4, y + 6, color);
    drawPixel(x + 6, y + 5, color);
    drawPixel(x + 7, y + 6, color);
  }
  if (alpha == 'e' || alpha == 'E') {
    drawPixel(x, y, color);
    drawPixel(x, y + 1, color);
    drawPixel(x, y + 2, color);
    drawPixel(x, y + 3, color);
    drawPixel(x, y + 4, color);
    drawPixel(x + 1, y, color);
    drawPixel(x + 1, y + 1, color);
    drawPixel(x + 1, y + 2, color);
    drawPixel(x + 1, y + 3, color);
    drawPixel(x + 1, y + 4, color);
    drawPixel(x + 2, y, color);
    drawPixel(x + 2, y + 1, color);
    drawPixel(x + 3, y, color);
    drawPixel(x + 3, y + 1, color);
    drawPixel(x + 3, y + 2, color);
    drawPixel(x + 3, y + 3, color);
    drawPixel(x + 3, y + 4, color);
    drawPixel(x + 4, y, color);
    drawPixel(x + 4, y + 1, color);
    drawPixel(x + 5, y + 1, color);
    drawPixel(x + 5, y + 2, color);
    drawPixel(x + 5, y + 3, color);
    drawPixel(x + 5, y + 4, color);
    drawPixel(x + 5, y, color);
    drawPixel(x + 6, y, color);
    drawPixel(x + 6, y + 1, color);
    drawPixel(x + 6, y + 2, color);
    drawPixel(x + 6, y + 3, color);
    drawPixel(x + 6, y + 4, color);
  }

  if (alpha == 'f' || alpha == 'F') {
    drawPixel(x, y, color);
    drawPixel(x, y + 1, color);
    drawPixel(x, y + 2, color);
    drawPixel(x, y + 3, color);
    drawPixel(x, y + 4, color);
    drawPixel(x + 1, y, color);
    drawPixel(x + 1, y + 1, color);
    drawPixel(x + 1, y + 2, color);
    drawPixel(x + 1, y + 3, color);
    drawPixel(x + 1, y + 4, color);
    drawPixel(x + 2, y, color);
    drawPixel(x + 2, y + 1, color);
    drawPixel(x + 3, y, color);
    drawPixel(x + 3, y + 1, color);
    drawPixel(x + 3, y + 2, color);
    drawPixel(x + 3, y + 3, color);
    drawPixel(x + 4, y, color);
    drawPixel(x + 4, y + 1, color);
    drawPixel(x + 4, y + 2, color);
    drawPixel(x + 4, y + 3, color);
    drawPixel(x + 5, y, color);
    drawPixel(x + 5, y + 1, color);
    drawPixel(x + 6, y, color);
    drawPixel(x + 6, y + 1, color);
  }
  if (alpha == 'G') {
    drawPixel(x, y + 1, color);
    drawPixel(x, y + 2, color);
    drawPixel(x, y + 3, color);
    drawPixel(x, y + 4, color);
    drawPixel(x, y + 5, color);
    drawPixel(x + 1, y, color);
    drawPixel(x + 1, y + 1, color);
    drawPixel(x + 1, y + 2, color);
    drawPixel(x + 1, y + 3, color);
    drawPixel(x + 1, y + 4, color);
    drawPixel(x + 1, y + 5, color);
    drawPixel(x + 2, y, color);
    drawPixel(x + 2, y + 1, color);
    drawPixel(x + 3, y, color);
    drawPixel(x + 3, y + 1, color);
    drawPixel(x + 3, y + 3, color);
    drawPixel(x + 3, y + 4, color);
    drawPixel(x + 3, y + 5, color);
    drawPixel(x + 4, y, color);
    drawPixel(x + 4, y + 1, color);
    drawPixel(x + 4, y + 4, color);
    drawPixel(x + 4, y + 5, color);
    drawPixel(x + 5, y, color);
    drawPixel(x + 5, y + 1, color);
    drawPixel(x + 5, y + 2, color);
    drawPixel(x + 5, y + 3, color);
    drawPixel(x + 5, y + 4, color);
    drawPixel(x + 5, y + 5, color);
    drawPixel(x + 6, y + 1, color);
    drawPixel(x + 6, y + 2, color);
    drawPixel(x + 6, y + 3, color);
    drawPixel(x + 6, y + 4, color);
    drawPixel(x + 6, y + 5, color);
  }
  if (alpha == 'g') {
    drawPixel(x, y + 1, color);
    drawPixel(x, y + 2, color);
    drawPixel(x, y + 3, color);
    drawPixel(x + 1, y, color);
    drawPixel(x + 1, y + 3, color);
    drawPixel(x + 2, y, color);
    drawPixel(x + 2, y + 2, color);
    drawPixel(x + 2, y + 3, color);
    drawPixel(x + 3, y, color);
    drawPixel(x + 3, y + 3, color);
    drawPixel(x + 4, y, color);
    drawPixel(x + 4, y + 3, color);
  }

  if (alpha == 'h' || alpha == 'H') {
    drawPixel(x, y, color);
    drawPixel(x, y + 3, color);
    drawPixel(x + 1, y, color);
    drawPixel(x + 1, y + 3, color);
    drawPixel(x + 2, y, color);
    drawPixel(x + 2, y + 1, color);
    drawPixel(x + 2, y + 2, color);
    drawPixel(x + 2, y + 3, color);
    drawPixel(x + 3, y, color);
    drawPixel(x + 3, y + 3, color);
    drawPixel(x + 4, y, color);
    drawPixel(x + 4, y + 3, color);
  }

  if (alpha == '-') {
    drawPixel(x, y, color);
    drawPixel(x, y + 1, color);
    drawPixel(x, y + 2, color);
    drawPixel(x, y + 3, color);
  }

  if (alpha == '!') {
    drawPixel(x, y, color);
    drawPixel(x + 1, y, color);
    drawPixel(x + 2, y, color);
    drawPixel(x + 4, y, color);
  }

  if (alpha == 'i' || alpha == 'I') {
    drawPixel(x, y, color);
    drawPixel(x, y + 1, color);
    drawPixel(x, y + 2, color);
    drawPixel(x, y + 3, color);
    drawPixel(x, y + 4, color);
    drawPixel(x, y + 5, color);
    drawPixel(x + 1, y, color);
    drawPixel(x + 1, y + 1, color);
    drawPixel(x + 1, y + 2, color);
    drawPixel(x + 1, y + 3, color);
    drawPixel(x + 1, y + 4, color);
    drawPixel(x + 1, y + 5, color);
    drawPixel(x + 2, y + 2, color);
    drawPixel(x + 3, y + 2, color);
    drawPixel(x + 4, y + 2, color);
    drawPixel(x + 2, y + 3, color);
    drawPixel(x + 3, y + 3, color);
    drawPixel(x + 4, y + 3, color);
    drawPixel(x + 5, y + 1, color);
    drawPixel(x + 5, y + 2, color);
    drawPixel(x + 5, y + 3, color);
    drawPixel(x + 5, y + 4, color);
    drawPixel(x + 5, y + 5, color);
    drawPixel(x + 5, y, color);
    drawPixel(x + 6, y, color);
    drawPixel(x + 6, y + 1, color);
    drawPixel(x + 6, y + 2, color);
    drawPixel(x + 6, y + 3, color);
    drawPixel(x + 6, y + 4, color);
    drawPixel(x + 6, y + 5, color);
  }

  if (alpha == 'l' || alpha == 'L') {
    drawPixel(x, y, color);
    drawPixel(x, y + 1, color); 
    drawPixel(x + 1, y, color);
    drawPixel(x + 1, y + 1, color); 
    drawPixel(x + 2, y, color);
    drawPixel(x + 2, y + 1, color); 
    drawPixel(x + 3, y, color);
    drawPixel(x + 3, y + 1, color);
    drawPixel(x + 4, y, color);
    drawPixel(x + 5, y, color);
    drawPixel(x + 5, y + 1, color);
    drawPixel(x + 5, y + 2, color);
    drawPixel(x + 5, y + 3, color);
    drawPixel(x + 5, y + 4, color); 
    drawPixel(x + 6, y, color);
    drawPixel(x + 6, y + 1, color);
    drawPixel(x + 6, y + 2, color);
    drawPixel(x + 6, y + 3, color);
    drawPixel(x + 6, y + 4, color); 
  }

  if (alpha == 'h' || alpha == 'H') {
    drawPixel(x, y, color);
    drawPixel(x, y + 3, color);
    drawPixel(x + 1, y, color);
    drawPixel(x + 1, y + 3, color);
    drawPixel(x + 2, y, color);
    drawPixel(x + 2, y + 1, color);
    drawPixel(x + 2, y + 2, color);
    drawPixel(x + 2, y + 3, color);
    drawPixel(x + 3, y, color);
    drawPixel(x + 3, y + 3, color);
    drawPixel(x + 4, y, color);
    drawPixel(x + 4, y + 3, color);
  }

  if (alpha == 'm' || alpha == 'M') {
    drawPixel(x, y, color);
    drawPixel(x, y + 4, color);
    drawPixel(x + 1, y, color);
    drawPixel(x + 1, y + 4, color);
    drawPixel(x + 2, y, color);
    drawPixel(x + 2, y + 4, color);
    drawPixel(x + 2, y + 2, color);
    drawPixel(x + 3, y, color);
    drawPixel(x + 3, y + 4, color);
    drawPixel(x + 1, y + 1, color);
    drawPixel(x + 1, y + 3, color);
    drawPixel(x + 4, y, color);
    drawPixel(x + 4, y + 4, color);
  }

  if (alpha == 'n') {
    drawPixel(x, y, color);
    drawPixel(x, y + 4, color);
    drawPixel(x + 1, y, color);
    drawPixel(x + 1, y + 4, color);
    drawPixel(x + 1, y + 1, color);
    drawPixel(x + 2, y, color);
    drawPixel(x + 2, y + 4, color);
    drawPixel(x + 2, y + 2, color);
    drawPixel(x + 3, y, color);
    drawPixel(x + 3, y + 4, color);
    drawPixel(x + 3, y + 3, color);
    drawPixel(x + 4, y, color);
    drawPixel(x + 4, y + 4, color);
  }

  if (alpha == 'N') {
    drawPixel(x, y, color);
    drawPixel(x, y + 1, color);
    drawPixel(x, y + 5, color);
    drawPixel(x, y + 6, color);
    drawPixel(x + 1, y, color);
    drawPixel(x + 1, y + 1, color);
    drawPixel(x + 1, y + 5, color);
    drawPixel(x + 1, y + 6, color);
    drawPixel(x + 2, y, color);
    drawPixel(x + 2, y + 1, color);
    drawPixel(x + 2, y + 5, color);
    drawPixel(x + 2, y + 6, color);
    drawPixel(x + 3, y, color);
    drawPixel(x + 3, y + 1, color);
    drawPixel(x + 3, y + 5, color);
    drawPixel(x + 3, y + 6, color);
    drawPixel(x + 4, y, color);
    drawPixel(x + 4, y + 1, color);
    drawPixel(x + 4, y + 5, color);
    drawPixel(x + 4, y + 6, color);
    drawPixel(x + 5, y, color);
    drawPixel(x + 5, y + 1, color);
    drawPixel(x + 5, y + 5, color);
    drawPixel(x + 5, y + 6, color);
    drawPixel(x + 6, y, color);
    drawPixel(x + 6, y + 1, color);
    drawPixel(x + 6, y + 5, color);
    drawPixel(x + 6, y + 6, color);

    drawPixel(x + 1, y + 2, color);
    drawPixel(x + 2, y + 2, color);
    drawPixel(x + 2, y + 3, color);
    drawPixel(x + 3, y + 3, color);
    drawPixel(x + 3, y + 4, color);
    drawPixel(x + 4, y + 4, color);
  }

  if (alpha == 'O') {
    drawPixel(x, y + 1, color);
    drawPixel(x, y + 2, color);
    drawPixel(x, y + 3, color);
    drawPixel(x, y + 4, color);
    drawPixel(x, y + 5, color);
    drawPixel(x + 1, y + 1, color);
    drawPixel(x + 1, y + 5, color);
    drawPixel(x + 1, y, color);
    drawPixel(x + 1, y + 2, color);
    drawPixel(x + 1, y + 3, color);
    drawPixel(x + 1, y + 4, color);
    drawPixel(x + 1, y + 6, color);
    drawPixel(x + 2, y + 0, color);
    drawPixel(x + 2, y + 1, color);
    drawPixel(x + 2, y + 5, color);
    drawPixel(x + 2, y + 6, color);
    drawPixel(x + 3, y + 0, color);
    drawPixel(x + 3, y + 1, color);
    drawPixel(x + 3, y + 5, color);
    drawPixel(x + 3, y + 6, color);
    drawPixel(x + 4, y + 0, color);
    drawPixel(x + 4, y + 1, color);
    drawPixel(x + 4, y + 5, color);
    drawPixel(x + 4, y + 6, color);
    drawPixel(x + 5, y + 1, color);
    drawPixel(x + 5, y + 5, color);
    drawPixel(x + 5, y, color);
    drawPixel(x + 5, y + 2, color);
    drawPixel(x + 5, y + 3, color);
    drawPixel(x + 5, y + 4, color);
    drawPixel(x + 5, y + 6, color);
    drawPixel(x + 6, y + 1, color);
    drawPixel(x + 6, y + 2, color);
    drawPixel(x + 6, y + 3, color);
    drawPixel(x + 6, y + 4, color);
    drawPixel(x + 6, y + 5, color);
  }

  if (alpha == 'Q') {
    drawLine(x, y + 1, x, y + 3, color);
    drawPixel(x + 1, y, color);
    drawPixel(x + 1, y + 4, color);
    drawPixel(x + 2, y, color);
    drawPixel(x + 2, y + 4, color);
    drawPixel(x + 3, y, color);
    drawPixel(x + 3, y + 4, color);
    drawLine(x + 4, y + 1, x + 4, y + 3, color);
  }

  if (alpha == 'o') {
    uint8_t p = x;
    uint8_t q = y;

    drawLine(p, q + 1, p, q + 2, color);
    drawPixel(p - 1, q, color);
    drawPixel(p - 1, q + 3, color);
    drawPixel(p - 2, q, color);
    drawPixel(p - 2, q + 3, color);
    drawLine(p - 3, q + 1, p - 3, q + 2, color);
  }

  if (alpha == 'p' || alpha == 'P') {
    drawPixel(x, y + 1, color);
    drawPixel(x, y + 2, color);
    drawPixel(x + 1, y + 3, color);
    drawPixel(x + 2, y + 2, color);
    drawPixel(x + 2, y + 1, color);
    drawLine(x, y, x + 4, y, color);
  }

  if (alpha == 'r' || alpha == 'R') {
    drawPixel(x, y + 1, color);
    drawPixel(x, y + 2, color);
    drawPixel(x, y + 3, color);
    drawPixel(x, y + 4, color);
    drawPixel(x + 1, y, color);
    drawPixel(x + 1, y + 1, color);
    drawPixel(x + 1, y + 2, color);
    drawPixel(x + 1, y + 3, color);
    drawPixel(x + 1, y + 4, color);
    drawPixel(x + 1, y + 5, color);
    drawPixel(x + 4, y + 3, color);
    drawPixel(x + 2, y, color);
    drawPixel(x + 2, y + 1, color);
    drawPixel(x + 2, y + 4, color);
    drawPixel(x + 2, y + 5, color);
    drawPixel(x + 4, y + 3, color);
    drawPixel(x + 3, y, color);
    drawPixel(x + 3, y + 1, color);
    drawPixel(x + 3, y + 2, color);
    drawPixel(x + 3, y + 3, color);
    drawPixel(x + 3, y + 4, color);
    drawPixel(x + 4, y, color);
    drawPixel(x + 4, y + 1, color);
    drawPixel(x + 4, y + 2, color);
    drawPixel(x + 4, y + 3, color);
    drawPixel(x + 5, y, color);
    drawPixel(x + 5, y + 1, color);
    drawPixel(x + 5, y + 3, color);
    drawPixel(x + 5, y + 4, color);
    drawPixel(x + 6, y, color);
    drawPixel(x + 6, y + 1, color);
    drawPixel(x + 6, y + 5, color);
    drawPixel(x + 6, y + 4, color);
  }

  if (alpha == 'S') {
    drawPixel(x, y + 1, color);
    drawPixel(x, y + 2, color);
    drawPixel(x, y + 3, color);
    drawPixel(x, y + 4, color);
    drawPixel(x + 1, y, color);
    drawPixel(x + 1, y + 1, color);
    drawPixel(x + 1, y + 2, color);
    drawPixel(x + 1, y + 3, color);
    drawPixel(x + 1, y + 4, color);
    drawPixel(x + 2, y, color);
    drawPixel(x + 2, y + 1, color);
    drawPixel(x + 3, y + 1, color);
    drawPixel(x + 3, y + 2, color);
    drawPixel(x + 3, y + 3, color);
    drawPixel(x + 3, y + 4, color);
    drawPixel(x + 4, y + 3, color);
    drawPixel(x + 4, y + 4, color);
    drawPixel(x + 5, y, color);
    drawPixel(x + 5, y + 1, color);
    drawPixel(x + 5, y + 2, color);
    drawPixel(x + 5, y + 3, color);
    drawPixel(x + 5, y + 4, color);
    drawPixel(x + 6, y, color);
    drawPixel(x + 6, y + 2, color);
    drawPixel(x + 6, y + 3, color);
    drawPixel(x + 6, y + 1, color);
  }

  if (alpha == 's') {
    drawPixel(x, y + 1, color);
    drawPixel(x, y + 2, color);
    drawPixel(x, y + 3, color);
    drawPixel(x + 1, y, color);
    drawPixel(x + 2, y + 1, color);
    drawPixel(x + 2, y + 2, color);
    drawPixel(x + 3, y + 3, color);
    drawPixel(x + 4, y + 1, color);
    drawPixel(x + 4, y + 2, color);
    drawPixel(x + 4, y, color);
  }

  if (alpha == 't' || alpha == 'T') {
    drawPixel(x, y, color);
    drawPixel(x, y + 1, color);
    drawPixel(x, y + 2, color);
    drawPixel(x, y + 4, color);
    drawPixel(x, y + 3, color);
    drawPixel(x + 1, y + 2, color);
    drawPixel(x + 2, y + 2, color);
    drawPixel(x + 3, y + 2, color);
    drawPixel(x + 4, y + 2, color);
  }

  if (alpha == 'v' || alpha == 'V') {

    drawPixel(x, y, color);
    drawPixel(x, y + 1, color);
    drawPixel(x + 1, y, color);
    drawPixel(x + 1, y + 1, color);
    drawPixel(x + 2, y + 1, color);
    drawPixel(x + 3, y + 2, color);
    drawPixel(x + 4, y + 3, color);
    drawPixel(x + 3, y + 4, color);
    drawPixel(x + 2, y + 5, color);
    drawPixel(x + 1, y + 5, color);
    drawPixel(x + 1, y + 6, color);
    drawPixel(x, y + 6, color);
    drawPixel(x, y + 5, color);

  }

  if (alpha == 'w' || alpha == 'W') {
    drawPixel(x, y, color);
    drawPixel(x, y + 4, color);
    drawPixel(x + 1, y, color);
    drawPixel(x + 1, y + 4, color);
    drawPixel(x + 2, y, color);
    drawPixel(x + 2, y + 2, color);
    drawPixel(x + 2, y + 4, color);
    drawPixel(x + 3, y, color);
    drawPixel(x + 3, y + 1, color);
    drawPixel(x + 3, y + 3, color);
    drawPixel(x + 3, y + 4, color);
    drawPixel(x + 4, y, color);
    drawPixel(x + 4, y + 4, color);
  }

  if (alpha == 'y' || alpha == 'Y') {
    drawPixel(x, y, color);
    drawPixel(x, y + 1, color);
    drawPixel(x, y + 6, color);
    drawPixel(x, y + 7, color);
    drawPixel(x + 1, y + 1, color);
    drawPixel(x + 1, y + 2, color);
    drawPixel(x + 1, y + 6, color);
    drawPixel(x + 1, y + 5, color);

    drawPixel(x + 2, y + 2, color);
    drawPixel(x + 2, y + 4, color);
    drawPixel(x + 2, y + 3, color);
    drawPixel(x + 2, y + 5, color);
    drawPixel(x + 3, y + 3, color);
    drawPixel(x + 3, y + 4, color);
    drawPixel(x + 4, y + 3, color);
    drawPixel(x + 4, y + 4, color);
    drawPixel(x + 5, y + 3, color);
    drawPixel(x + 5, y + 4, color);
    drawPixel(x + 6, y + 3, color);
    drawPixel(x + 6, y + 4, color);
  }

  if (alpha == '1') {
    drawPixel(x + 2, y + 4, color);
    drawPixel(x + 1, y + 3, color);
    drawPixel(x + 2, y + 3, color);
    drawPixel(x + 2, y + 2, color);
    drawPixel(x + 2, y + 1, color);
    drawPixel(x + 1, y, color);
    drawPixel(x + 2, y, color);
    drawPixel(x + 3, y, color);
  }

  if (alpha == '2') {
    drawPixel(x, y + 4, color);
    drawPixel(x + 1, y + 4, color);
    drawPixel(x + 2, y + 4, color);
    drawPixel(x + 3, y + 4, color);
    drawPixel(x + 4, y + 3, color);
    drawPixel(x + 1, y + 2, color);
    drawPixel(x + 2, y + 2, color);
    drawPixel(x + 3, y + 2, color);
    drawPixel(x, y + 1, color);
    drawPixel(x, y, color);
    drawPixel(x + 1, y, color);
    drawPixel(x + 2, y, color);
    drawPixel(x + 3, y, color);
    drawPixel(x + 4, y, color);
  }
}

gpio_s reset;
bool get_reset_ip(void) {
  reset = gpio__construct_as_input(GPIO__PORT_0, 11);
  bool rst = gpio0__get_level(11);
  if (!rst) {
    return 1;
  } else {
    return 0;
  }
}
