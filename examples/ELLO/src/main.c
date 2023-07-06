
#include "main.h"
#include "emu/bios/ROM_82.h"

#ifdef APPLE

// INFO
// https://github.com/Wiz-IO/wizio-pico-examples/tree/main/arduino/pico-apple-2/src
// https://github.com/Wiz-IO/pravetz-8mz

extern void cpu6502_reset(void);
extern int cpu6502_run(void);

uint8_t basic_mem[0x10000];

void cpu6502_dump(uint32_t pc, uint32_t a, uint32_t x, uint32_t y, uint32_t sp, uint32_t sr) {}

uint8_t cpu6502_load(uint16_t addr) { return 0; }

void cpu6502_store(uint16_t addr, uint8_t data) {}

#endif

int main(void)
{
  board_config();
  printf("SETUP\n"); // @ ELLO TX1

#if APPLE
  memset(basic_mem, 0, 0x10000);
  memcpy(&basic_mem[ROM_82_ADDRESS], ROM_82, ROM_82_SIZE);
  cpu6502_reset();
  cpu6502_run();
#endif

  while (1)
  {
    HAL_GPIO_TogglePin(RED_GPIO_Port, RED_Pin);
    HAL_Delay(1000);
    printf("LOOP ");
  }
}
