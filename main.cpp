/**
 * Copyright 2011 - Joel Abrahamsson
 *
 * This is only used for debugging YACE. It's not a GUI.
 */

#include <cstdio>
#include <cstdlib>
#include "include/Chip8.h"

void show_help();

int main(int argc, char **argv)
{
  using namespace YACE;

  if (argc > 1)
  {
    Chip8 chip8 = Chip8();

    chip8.load_game(argv[1]);

    if (argc > 2)
    {
      int cycles = atoi(argv[2]);

      if (cycles)
        chip8.set_cpu_cycles(cycles);
    }

    // CAUTION! Infinite loop!
    while (true)
    {
      chip8.step();

      if (std::getchar() == EOF)
        break;
    }
  }
  else
    show_help();

  return 0;
}

void show_help()
{
  printf("Usage:\n");
  printf("\tyace <file> [<cpu cycles>]\n");
}
