/***************************************************************************//**
 * @file main.c
 * @brief Phase 2 BG24 capture — bare-metal sl_main loop (no FreeRTOS).
 ******************************************************************************/
#include "sl_component_catalog.h"
#include "sl_main_init.h"
#include "sl_main_process_action.h"
#include "app.h"

int main(void)
{
  sl_main_init();
  app_init();

  while (1) {
    sl_main_process_action();
    app_process_action();
  }
}
