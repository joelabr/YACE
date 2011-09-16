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
      void opcode0x1000(unsigned short opcode);
      void opcode0x2000(unsigned short opcode);
      void opcode0x3000(unsigned short opcode);
      void opcode0x4000(unsigned short opcode);
      void opcode0x5000(unsigned short opcode);
      void opcode0x6000(unsigned short opcode);
      void opcode0x7000(unsigned short opcode);
      void opcode0x8000(unsigned short opcode);
      void opcode0x8001(unsigned short opcode);
      void opcode0x8002(unsigned short opcode);
      void opcode0x8003(unsigned short opcode);
      void opcode0x8004(unsigned short opcode);
      void opcode0x8005(unsigned short opcode);
      void opcode0x8006(unsigned short opcode);
      void opcode0x8007(unsigned short opcode);
      void opcode0x800E(unsigned short opcode);
      void opcode0x9000(unsigned short opcode);
      void opcode0xA000(unsigned short opcode);
      void opcode0xB000(unsigned short opcode);
      void opcode0xC000(unsigned short opcode);
      void opcode0xD000(unsigned short opcode);
      void opcode0xE09E(unsigned short opcode);
      void opcode0xE0A1(unsigned short opcode);
      void opcode0xF007(unsigned short opcode);
      void opcode0xF00A(unsigned short opcode);
      void opcode0xF015(unsigned short opcode);
      void opcode0xF018(unsigned short opcode);
      void opcode0xF01E(unsigned short opcode);
      void opcode0xF029(unsigned short opcode);
      void opcode0xF033(unsigned short opcode);
      void opcode0xF055(unsigned short opcode);
      void opcode0xF065(unsigned short opcode);
  };
}

#endif
