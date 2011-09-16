#ifndef YACE_CHIP8_H
#define YACE_CHIP8_H

#ifdef _DEBUG_
#define print_debug(args...) (printf(args))
#else
#define print_debug(args...) (void(0))
#endif

#include <cstdio>
#include <fstream>

#include "CPU.h"

namespace YACE
{
  class Chip8
  {
    public:
      Chip8();

      enum EMU_KEYS{KEY_0 = 1, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9,
                    KEY_A, KEY_B, KEY_C, KEY_D, KEY_E, KEY_F};

      int get_cpu_cycles() {return cpu_cycles;}
      bool get_key(EMU_KEYS key) {return keys[key];}
      const char* get_video() {return (const char*)video;}
      void load_game(const char* file);
      void reset();
      void run() {while (true) step();}
      void set_cpu_cycles(int cycles) {cpu_cycles = cycles;}
      void set_key(EMU_KEYS key, bool pressed);
      void step();

      friend class CPU;

    private:
      CPU cpu;
      int cpu_cycles;
      unsigned char memory[0x1000];
      char video[0x800];

      // Timers
      unsigned int delay_timer;
      unsigned int sound_timer;

      bool keys[16];
      bool key_is_pressed;
      unsigned char last_key_pressed;

      void reset_video();
  };
}

#endif
