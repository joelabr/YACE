#ifndef YACE_CHIP8_CPU_H
#define YACE_CHIP8_CPU_H

#include <cstdio>
#include <cstdlib>
#include <stack>

namespace YACE
{
  class Chip8;
  class CPU
  {
    public:
      CPU(Chip8& chip8);

      void execute(int cycles);
      void reset();

    private:
      Chip8& chip8;
      unsigned short opcode;

      // Stack
      std::stack<unsigned int> stack;

      // Registers
      short I;
      char V[16];
      unsigned int program_counter;

      // Opcode functions
      void handleOpcodes0x0000(unsigned short opcode);
      void handleOpcodes0x8000(unsigned short opcode);
      void handleOpcodes0xE000(unsigned short opcode);
      void handleOpcodes0xF000(unsigned short opcode);
      void opcode0x00E0(unsigned short opcode);
      void opcode0x00EE(unsigned short opcode);
      void opcode0x1NNN(unsigned short opcode);
      void opcode0x2NNN(unsigned short opcode);
      void opcode0x3XNN(unsigned short opcode);
      void opcode0x4XNN(unsigned short opcode);
      void opcode0x5XY0(unsigned short opcode);
      void opcode0x6XNN(unsigned short opcode);
      void opcode0x7XNN(unsigned short opcode);
      void opcode0x8XY0(unsigned short opcode);
      void opcode0x8XY1(unsigned short opcode);
      void opcode0x8XY2(unsigned short opcode);
      void opcode0x8XY3(unsigned short opcode);
      void opcode0x8XY4(unsigned short opcode);
      void opcode0x8XY5(unsigned short opcode);
      void opcode0x8XY6(unsigned short opcode);
      void opcode0x8XY7(unsigned short opcode);
      void opcode0x8XYE(unsigned short opcode);
      void opcode0x9XY0(unsigned short opcode);
      void opcode0xANNN(unsigned short opcode);
      void opcode0xBNNN(unsigned short opcode);
      void opcode0xCXNN(unsigned short opcode);
      void opcode0xDXYN(unsigned short opcode);
      void opcode0xEX9E(unsigned short opcode);
      void opcode0xEXA1(unsigned short opcode);
      void opcode0xFX07(unsigned short opcode);
      void opcode0xFX0A(unsigned short opcode);
      void opcode0xFX15(unsigned short opcode);
      void opcode0xFX18(unsigned short opcode);
      void opcode0xFX1E(unsigned short opcode);
      void opcode0xFX29(unsigned short opcode);
      void opcode0xFX33(unsigned short opcode);
      void opcode0xFX55(unsigned short opcode);
      void opcode0xFX65(unsigned short opcode);
  };
}

#endif
