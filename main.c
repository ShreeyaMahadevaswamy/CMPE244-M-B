
#include "ff.h"
#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"
#include "acceleration.h"
#include "delay.h"
#include "task.h"

#include "board_io.h"
#include "common_macros.h"
#include "gpio.h"
#include "sj2_cli.h"
#include "uart.h"

#include "LEDmatrix.h"
#include "i2c.h"
#include "i2c_slave_init.h"
#include "lpc40xx.h"
#include "semphr.h"
#include <stdbool.h>
#include <stdint.h>

TaskHandle_t obs_gen_up = NULL;
TaskHandle_t obs_left_shift_up = NULL;
TaskHandle_t update_disp = NULL;
TaskHandle_t update_disp_2 = NULL;
TaskHandle_t final_sb = NULL;
TaskHandle_t moving_runner = NULL;
TaskHandle_t start_screen = NULL;
TaskHandle_t buzz = NULL;
TaskHandle_t collision_detect = NULL;
TaskHandle_t game_over = NULL;
SemaphoreHandle_t rst_sem;
volatile bool start;

void update2(void *p) {
  while (1) {
    update_display2();
    vTaskDelay(5);
  }
}

void update(void *p) {
  while (1) {
    update_display();
    vTaskDelay(2);
  }
}

void update_position(void *p) {
  while (1) {
    getDirection();
    vTaskDelay(100); 
  }
}

void update_colour(void *p) {
  while (1) {
    get_colour();
    vTaskDelay(100);
  }
}

void buzzer(void *p) {
  while (1) {
    buzzer_on();
    vTaskDelay(100);
  }
}

void display_title(void *p) {
  while (1) {
    xSemaphoreTake(rst_sem, 1000);
    display_game_name(1, 15);
    vTaskDelay(4000);
    vTaskSuspend(moving_runner);
    clear_display();
    vTaskDelay(500);
    vTaskSuspend(update_disp_2);
    vTaskResume(update_disp);
    clear_runner();
    vTaskResume(obs_gen_up);
    vTaskResume(obs_left_shift_up);
    xSemaphoreGive(rst_sem);
    vTaskSuspend(NULL);
  }
}

void moving_autorunner(void *p) {
  while (1) {
    autorunner_moving();
  }
}
volatile bool x = 0;
void detect_collision_task(void *p) {

  while (1) {
    x = detect_collision();
    if (x) {
      if (x) {
        delay__ms(250);
        vTaskSuspend(obs_gen_up);
        vTaskSuspend(obs_left_shift_up);
        vTaskSuspend(update_disp);

        // vTaskDelay(100);
        vTaskResume(update_disp_2);
        clear_display();
      }
      if (x) {
        vTaskResume(game_over);
        vTaskDelay(4000);
        vTaskSuspend(game_over);
        clear_display();
      }
      if (x) {
        vTaskResume(final_sb);
        vTaskDelay(4000);
        vTaskSuspend(final_sb);
        clear_display();
        
      }
    } else {
      delay__ms(50);
    }
  }
}

void obstacle_gen(void *a) {
  bool flag;
  while (1) {

    generate_obstacle_rand();
    start = 0;

    vTaskDelay(1000);
  }
}

void obstacle_shift(void *p) {
  while (1) {
    shift_obstacle_left();
    vTaskDelay(20);
  }
}

void update_scoreboard(void *p) {
  while (1) {

    update_score();
    vTaskDelay(100);
  }
}

void game_over_display(void *p) {
  while (1) {

    display_game_over(8, 10);
  }
}

void final_scoreboard(void *p) {
  while (1) {
    final_score();
  }
}

void reset_game(void *p) {
  bool rst;
  while (1) {
    rst = get_reset_ip();
    if (rst) {
      xSemaphoreTake(rst_sem, 5000);
      x = 0;
      reset_score();
      clear_display();
      reset_fn();
      vTaskSuspend(update_disp);
      vTaskSuspend(final_sb);
      vTaskSuspend(game_over);
      vTaskResume(update_disp_2);
      vTaskSuspend(obs_gen_up);
      vTaskSuspend(obs_left_shift_up);
      vTaskDelay(100);
      vTaskResume(start_screen);
      vTaskResume(moving_runner);
      vTaskDelay(500);
      xSemaphoreGive(rst_sem);
    }
    vTaskDelay(100);
  }
}

void main(void) {
  led_display_init();
  rst_sem = xSemaphoreCreateBinary();
  uart__init(UART__0, clock__get_peripheral_clock_hz(), 115200);

  xTaskCreate(display_title, "display title", 1024, NULL, PRIORITY_LOW, &start_screen);
  xTaskCreate(moving_autorunner, "moving autorunner in initial screen", 512, NULL, PRIORITY_LOW, &moving_runner);
  xTaskCreate(update2, "display  after stop", 512, NULL, PRIORITY_HIGH, &update_disp_2);
  xTaskCreate(update, "display", 2048, NULL, PRIORITY_HIGH, &update_disp);
  xTaskCreate(obstacle_gen, "obstacle generation", 1024, NULL, PRIORITY_MEDIUM, &obs_gen_up);
  xTaskCreate(obstacle_shift, "obstacle left shift", 1024, NULL, PRIORITY_MEDIUM, &obs_left_shift_up);
  xTaskCreate(update_position, "autorunner position update", 512, NULL, PRIORITY_MEDIUM, NULL);
  xTaskCreate(update_colour, "autorunner colour update", 512, NULL, PRIORITY_MEDIUM, NULL);
  xTaskCreate(detect_collision_task, "detect collision", 512, NULL, PRIORITY_MEDIUM, &collision_detect);
  xTaskCreate(update_scoreboard, "update scoreboard", 512, NULL, PRIORITY_MEDIUM, NULL);
  xTaskCreate(final_scoreboard, "final scoreboard", 512, NULL, PRIORITY_MEDIUM, &final_sb);
  xTaskCreate(reset_game, "reset task", 2048, NULL, PRIORITY_HIGH, NULL);
  xTaskCreate(game_over_display, "game over display", 1024, NULL, PRIORITY_MEDIUM, &game_over);

  vTaskSuspend(update_disp);
  vTaskSuspend(obs_gen_up);
  vTaskSuspend(obs_left_shift_up);
  vTaskSuspend(final_sb);
  vTaskSuspend(game_over);

  vTaskStartScheduler();

}
