#ifndef YACE_CHIP8_H
#define YACE_CHIP8_H

#ifdef _DEBUG_
#define print_debug(args...) (printf(args))
#else
#define print_debug(args...) (void(0))
#endif

#include <cstdio>
#include <cstring>

#include "CPU.h"

namespace YACE
{
  class Chip8
  {
    public:
      Chip8();

      enum EMU_KEYS {KEY_0 = 1, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9,
                     KEY_A, KEY_B, KEY_C, KEY_D, KEY_E, KEY_F};

      enum VIDEO_MODES {CHIP8, SUPERCHIP};

      int get_cpu_cycles() {return cpu_cycles;}
      bool get_key(EMU_KEYS key) {return keys[key];}
      unsigned int get_sound_timer() {return sound_timer;}
      const char* get_video() {return (const char*)video;}
      VIDEO_MODES get_video_mode() {return video_mode;}
      void load_game(const char* file);
      void reset();
      void set_cpu_cycles(int cycles) {cpu_cycles = cycles;}
      void set_key(EMU_KEYS key, bool pressed);
      void step();

      friend class CPU;

    private:
      const int FONT_CHIP8;
      const int FONT_SUPERCHIP;

      CPU cpu;
      int cpu_cycles;
      unsigned char memory[0x1000];
      char video[0x2000];
      VIDEO_MODES video_mode;

      // Timers
      unsigned int delay_timer;
      unsigned int sound_timer;

      bool keys[16];
      bool key_is_pressed;
      unsigned char last_key_pressed;

      void setup_fonts();
      void read_font(const char* file, unsigned char* destination, int size); 
      void reset_video();
  };
}

#endif
