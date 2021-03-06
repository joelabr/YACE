#include "../include/Chip8.h"

namespace YACE
{
  Chip8::Chip8() : FONT_CHIP8(0x109), FONT_SUPERCHIP(0x159), cpu(CPU(*this)), cpu_cycles(400), delay_timer(0), sound_timer(0), key_is_pressed(false), last_key_pressed(KEY_0)
  {
    reset();
    setup_fonts();
  }

  /*
   *  Private methods
   */

  /**
   *  Resets video.
   */
  void Chip8::reset_video()
  {
    video_mode = CHIP8;
    std::memset(video, 0, 0x2000);
  }

  /**
   *  Setup fonts
   */
  void Chip8::setup_fonts()
  {
    // Setup Chip8-font (5-byte font)
    read_font("chip8.font", memory + FONT_CHIP8, 0x50);

    // Setup SuperChip-font (10-byte font)
    read_font("superchip.font", memory + FONT_SUPERCHIP, 0xA0);
  }

  /**
   *  Reads a font file.
   */
  void Chip8::read_font(const char* file, unsigned char* destination, int size)
  {
    // Open font file
    FILE* font = fopen(file, "rb");

    if (font)
    {
      // Read font to destination
      fread(destination, 1, size, font);
      fclose(font);
    }
  }

  /*
   * Public methods
   */
  /**
   *  Loads a game into memory.
   */
  void Chip8::load_game(const char* file)
  {
    FILE* input = fopen(file, "rb");
    if (input)
    {
      fseek(input, 0, SEEK_END);
      int length = ftell(input);
      fseek(input, 0, SEEK_SET);

      fread(&memory[0x200], 1, length, input);
      fclose(input);
    }
    else
      throw "Couldn't open specified file!";
  }

  /**
    * Resets emulator to known values.
    */
  void Chip8::reset()
  {
    using std::memset;

    // Reset CPU
    cpu.reset();

    reset_video();

    // Reset interpreter memory
    memset(memory, 0, FONT_CHIP8);

    // Reset program memory
    memset(memory + 0x200, 0, 0x800);

    // Reset keys
    memset(keys, 0, 16);
  }

  /**
   *  Sets key state of given key
   */
  void Chip8::set_key(EMU_KEYS key, bool pressed)
  {
    keys[key - 1] = pressed;

    if (pressed)
    {
      key_is_pressed = true;
      last_key_pressed = key - 1;
    }
  }

  /**
   * Steps the emulator.
   */
  void Chip8::step()
  {
    cpu.execute(cpu_cycles);

    if (delay_timer > 0)
      delay_timer--;

    if (sound_timer > 0)
      sound_timer--;
  }
}
