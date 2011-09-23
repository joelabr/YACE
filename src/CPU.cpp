#include "../include/CPU.h"
#include "../include/Chip8.h"

namespace YACE
{
  CPU::CPU(Chip8& chip8) : chip8(chip8), opcode(0), stack(), I(0), program_counter(0x200)
  {
    reset();
  }

  /*
   *  Private methods
   */
  /**
   *  Takes care of all 0x0000 opcodes.
   */
  void CPU::handleOpcodes0x0000(unsigned short opcode)
  {
    if ((opcode & 0x00FF) == 0xE0)
      opcode0x00E0(opcode);
    else if ((opcode & 0x00FF) == 0xEE)
      opcode0x00EE(opcode);
  }

  /**
   *  Takes care of all 0x8000 opcodes.
   */
  void CPU::handleOpcodes0x8000(unsigned short opcode)
  {
    switch (opcode & 0x000F)
    {
      case 0:
        opcode0x8XY0(opcode);
        break;
      case 0x1: // VX = VX | VY
        opcode0x8XY1(opcode);
        break;
      case 0x2: // VX = VX & VY
        opcode0x8XY2(opcode);
        break;
      case 0x3: // VX = VY ^ VY
        opcode0x8XY3(opcode);
        break;
      case 0x4: // VX = VX + VY, VF = Carry
        opcode0x8XY4(opcode);
        break;
      case 0x5: // VX = VX - VY, VF = !Borrow
        opcode0x8XY5(opcode);
        break;
      case 0x6: // VX = VX >> 1, VF = Carry
        opcode0x8XY6(opcode);
        break;
      case 0x7: // VX = VY - VX, VF = !Borrow
        opcode0x8XY7(opcode);
        break;
      case 0xE: // VX = VX << 1, VF = Carry
        opcode0x8XYE(opcode);
        break;
    }
    program_counter += 2;
  }

  /**
   *  Takes care of all 0xE000 opcodes.
   */
  void CPU::handleOpcodes0xE000(unsigned short opcode)
  {
    if ((opcode & 0x00FF) == 0x9E)
      opcode0xEX9E(opcode);
    else if ((opcode & 0x00FF) == 0xA1)
      opcode0xEXA1(opcode);

    program_counter += 2;
  }

  /**
   *  Takes care of all 0xF000 opcodes.
   */
  void CPU::handleOpcodes0xF000(unsigned short opcode)
  {
    switch (opcode & 0x00FF)
    {
      case 0x7: // VX = delay_timer
        opcode0xFX07(opcode);
        break;
      case 0xA: // VX = Key press
        opcode0xFX0A(opcode);
        break;
      case 0x15:  // delay_timer = VX
        opcode0xFX15(opcode);
        break;
      case 0x18:  // sound_timer = VX
        opcode0xFX18(opcode);
        break;
      case 0x1E:  // I = I + VX
        opcode0xFX1E(opcode);
        break;
      case 0x29:  // I = Location of the sprite for character in VX
        opcode0xFX29(opcode);
        break;
      case 0x33:  // I, I + 1, I + 2 = BCD of VX
        opcode0xFX33(opcode);
        break;
      case 0x55:  // Stores V0 to VX in memory starting at I
        opcode0xFX55(opcode);
        break;
      case 0x65:  // Fills V0 to VX with values from memory starting at I
        opcode0xFX65(opcode);
        break;
    }
    program_counter += 2;
  }

  /**
   *  Clears the screen.
   */
  void CPU::opcode0x00E0(unsigned short opcode)
  {
    print_debug("Clears the screen.\n");
    chip8.reset_video();
    program_counter += 2;
  }

  /**
   *  Return from subroutine.
   */
  void CPU::opcode0x00EE(unsigned short opcode)
  {
    print_debug("Returns from subroutine.\n");
    if (!stack.empty())
    {
      program_counter = stack.top() + 2;
      stack.pop();
    }
    else
      program_counter += 2;
  }

  /**
   *  Jumps to address NNN.
   */
  void CPU::opcode0x1NNN(unsigned short opcode)
  {
    int address = opcode & 0x0FFF;

    print_debug("Jump to address %X.\n", address);
    program_counter = address;
  }

  /**
   *  Calls subroutine at NNN.
   */
  void CPU::opcode0x2NNN(unsigned short opcode)
  {
    int address = opcode & 0x0FFF;

    print_debug("Call subroutine at %X.\n", address);
    if (stack.size() < 16)
    {
      stack.push(program_counter);
      program_counter = address;
    }
    else
      program_counter += 2;
  }

  /**
   *  Skips next instruction if VX == NN.
   */
  void CPU::opcode0x3XNN(unsigned short opcode)
  {
    int register_x = ((opcode & 0x0F00) >> 8);
    int value = (opcode & 0x00FF);

    print_debug("Skips next instruction if V%X [%X] == %.2X.\n", register_x, V[register_x], value);
    if (V[register_x] == value)
      program_counter += 4;
    else
      program_counter += 2;
  }

  /**
   *  Skips the next instruction if VX != NN.
   */
  void CPU::opcode0x4XNN(unsigned short opcode)
  {
    int register_x = ((opcode & 0x0F00) >> 8);
    int value = (opcode & 0x00FF);

    print_debug("Skips next instruction if V%X [%X] != %.2X.\n", register_x, V[register_x], value);
    if (V[register_x] != value)
      program_counter += 4;
    else
      program_counter += 2;
  }

  /**
   *  Skips the next instruction if VX == VY.
   */
  void CPU::opcode0x5XY0(unsigned short opcode)
  {
    int register_x = ((opcode & 0x0F00) >> 8);
    int register_y = ((opcode & 0x00F0) >> 4);

    print_debug("Skip next instruction if V%X == V%X [%X == %X].\n", register_x, register_y,
                                                                     V[register_x], V[register_y]);
    if (V[register_x] == V[register_y])
      program_counter += 4;
    else
      program_counter += 2;
  }

  /**
   *  Sets VX to NN.
   */
  void CPU::opcode0x6XNN(unsigned short opcode)
  {
    int register_x = (opcode & 0x0F00) >> 8;
    int value = opcode & 0x00FF;

    print_debug("Set V%X to %X.\n", register_x, value);
    V[register_x] = value;

    program_counter += 2;
  }

  /**
   *  Adds NN to VX.
   */
  void CPU::opcode0x7XNN(unsigned short opcode)
  {
    int register_x = (opcode & 0x0F00) >> 8;
    int value = opcode & 0x00FF;

    print_debug("Add %X to V%X.\n", value, register_x);
    V[register_x] += value;

    program_counter += 2;
  }

  /**
   *  Sets VX to the value of VY.
   */
  void CPU::opcode0x8XY0(unsigned short opcode)
  {
    int register_x = (opcode & 0x0F00) >> 8;
    int register_y = (opcode & 0x00F0) >> 4;

    print_debug("Set V%X to the value of V%X [%X].\n", register_x, register_y,
                                                        V[register_y]);
    V[register_x] = V[register_y];
  }

  /**
   *  Sets VX to VX | VY.
   */
  void CPU::opcode0x8XY1(unsigned short opcode)
  {
    int register_x = (opcode & 0x0F00) >> 8;
    int register_y = (opcode & 0x00F0) >> 4;

    print_debug("Set V%X to V%X OR V%X [%X | %X].\n", register_x, register_x, register_y,
                                            V[register_x], V[register_y]);
    V[register_x] |= V[register_y];
  }

  /**
   *  Sets VX to VX & VY.
   */
  void CPU::opcode0x8XY2(unsigned short opcode)
  {
    int register_x = (opcode & 0x0F00) >> 8;
    int register_y = (opcode & 0x00F0) >> 4;

    print_debug("Set V%X to V%X AND V%X [%X & %X].\n", register_x, register_x, register_y,
                                              V[register_x], V[register_y]);
    V[register_x] &= V[register_y];
  }

  /**
   *  Sets VX to VX ^ VY.
   */
  void CPU::opcode0x8XY3(unsigned short opcode)
  {
    int register_x = (opcode & 0x0F00) >> 8;
    int register_y = (opcode & 0x00F0) >> 4;

    print_debug("Set V%X to V%X XOR V%X [%X ^ %X].\n", register_x, register_x, register_y,
                                                        V[register_x], V[register_y]);
    V[register_x] ^= V[register_y];
  }

  /**
   *  VX = VX + VY. VF = Carry (1 if carry).
   */
  void CPU::opcode0x8XY4(unsigned short opcode)
  {
    int register_x = (opcode & 0x0F00) >> 8;
    int register_y = (opcode & 0x00F0) >> 4;

    print_debug("Set V%X to V%X + V%X [%X + %X].\n", register_x, register_x, register_y,
                                                      V[register_x], V[register_y]);
    V[0xF] = (V[register_x] + V[register_y]) > 0xFF;
    V[register_x] += V[register_y];
  }

  /**
   *  VX = VX - VY. VF = Borrow (0 if borrow).
   */
  void CPU::opcode0x8XY5(unsigned short opcode)
  {
    int register_x = (opcode & 0x0F00) >> 8;
    int register_y = (opcode & 0x00F0) >> 4;

    print_debug("Set V%X to V%X - V%X [%X - %X].\n", register_x, register_x, register_y,
                                           V[register_x], V[register_y]);
    V[0xF] = !(V[register_x] < V[register_y]);
    V[register_x] -= V[register_y];
  }

  /**
   *  VX = VX >> 1. VF = Least significant bit.
   */
  void CPU::opcode0x8XY6(unsigned short opcode)
  {
    int register_x = (opcode & 0x0F00) >> 8;

    print_debug("Shift V%X right by 1.\n", register_x);
    V[0xF] = V[register_x] & 1;
    V[register_x] >>= 1;
  }

  /**
   *  VX = VY - VX. VF = Borrow (0 if borrow).
   */
  void CPU::opcode0x8XY7(unsigned short opcode)
  {
    int register_x = (opcode & 0x0F00) >> 8;
    int register_y = (opcode & 0x00F0) >> 4;

    print_debug("Set V%X = V%X - V%X [%X - %X].\n", register_x, register_y, register_x,
                                                    V[register_y], V[register_x]);
    V[0xF] = !(V[register_y] < V[register_x]);
    V[register_x] = V[register_y] - V[register_x];
  }

  /**
   *  VX = VX << 1. VF = Most significant bit.
   */
  void CPU::opcode0x8XYE(unsigned short opcode)
  {
    int register_x = (opcode & 0x0F00) >> 8;

    print_debug("Shift V%X left by 1.\n", register_x);
    V[0xF] = V[register_x] >> 7;
    V[register_x] <<= 1;
  }

  /**
   *  Skips the next instruction if VX != VY.
   */
  void CPU::opcode0x9XY0(unsigned short opcode)
  {
    int register_x = (opcode & 0x0F00) >> 8;
    int register_y = (opcode & 0x00F0) >> 4;

    print_debug("Skip next instruction if V%X != V%X. [%X != %X]\n", register_x, register_y,
                                                                     V[register_x], V[register_y]);
    if (V[register_x] != V[register_y])
      program_counter += 4;
    else
      program_counter += 2;
  }

  /**
   *  Sets I to the address NNN.
   */
  void CPU::opcode0xANNN(unsigned short opcode)
  {
    int address = opcode & 0x0FFF;

    print_debug("Set I to the address %.3X.\n", address);
    I = address;

    program_counter += 2;
  }

  /**
   *  Jumps to the address NNN + V0.
   */
  void CPU::opcode0xBNNN(unsigned short opcode)
  {
    int address = opcode & 0x0FFF;

    print_debug("Jump to address %.3X plus V0 [%X].\n", address, V[0]);
    program_counter = V[0] + address;
  }

  /**
   *  Sets VX to a random number AND NN.
   */
  void CPU::opcode0xCXNN(unsigned short opcode)
  {
    int register_x = (opcode & 0x0F00) >> 8;
    int value = opcode & 0x00FF;

    print_debug("Set V%X to a random number AND %X.\n", register_x, value);
    V[register_x] = ((rand() % 0xFF) & 0xFF) & value;

    program_counter += 2;
  }

  /**
   *  Draws a sprite at coordinate (VX, VY).
   */
  void CPU::opcode0xDXYN(unsigned short opcode)
  {
    V[0xF] = 0;
    int pos_x = V[(opcode & 0x0F00) >> 8];
    int pos_y = V[(opcode & 0x00F0) >> 4];
    int lines = opcode & 0x000F;

    print_debug("Draw sprite at (%i, %i) [%X lines].\n", pos_x, pos_y, lines);

    char data;

    for (int y = 0; y < lines; y++)
    {
      data = chip8.memory[I + y];
      for (int x = 0; x < 8; x++)
      {
        if (data & (0x80 >> x))
        {
          int pos = (pos_x + x) + ((pos_y + y) * 64);

          if (chip8.video[pos])
            V[0xF] = 1;

          chip8.video[pos] ^= 1;
        }
      }
    }

    program_counter += 2;
  }

  /**
   *  Skips the next instruction if the key stored in VX is pressed.
   */
  void CPU::opcode0xEX9E(unsigned short opcode)
  {
    int register_x = (opcode & 0x0F00) >> 8;

    print_debug("Skip next instruction if key[%X] is pressed.\n", register_x);
    if (chip8.keys[int(V[register_x])])
      program_counter += 2;
  }

  /**
   *  Skips the next instruction if the key stored in VX isn't pressed.
   */
  void CPU::opcode0xEXA1(unsigned short opcode)
  {
    int register_x = (opcode & 0x0F00) >> 8;

    print_debug("Skip next instruction if key[%X] isn't pressed.\n", register_x);
    if (!chip8.keys[int(V[register_x])])
      program_counter += 2;
  }

  /**
   *  Sets VX to the value of the delay timer.
   */
  void CPU::opcode0xFX07(unsigned short opcode)
  {
    int register_x = (opcode & 0x0F00) >> 8;

    print_debug("Set V%X to the value of the delay timer [%X].\n", register_x, chip8.delay_timer);
    V[register_x] = chip8.delay_timer;
  }

  /**
   *  A key press is awaited, and then stored in VX.
   */
  void CPU::opcode0xFX0A(unsigned short opcode)
  {
    int register_x = (opcode & 0x0F00) >> 8;

    print_debug("Set V%X to awaited key press.\n", register_x);
    if (chip8.key_is_pressed)
    {
      V[register_x] = chip8.last_key_pressed;
      chip8.key_is_pressed = false;
    }
    else
      program_counter -= 2;
  }

  /**
   *  Sets the delay timer to VX.
   */
  void CPU::opcode0xFX15(unsigned short opcode)
  {
    int register_x = (opcode & 0x0F00) >> 8;

    print_debug("Set delay timer to V%X [%X].\n", register_x, V[register_x]);
    chip8.delay_timer = V[register_x];
  }

  /**
   *  Sets the sound timer to VX.
   */
  void CPU::opcode0xFX18(unsigned short opcode)
  {
    int register_x = (opcode & 0x0F00) >> 8;

    print_debug("Set sound timer to V%X [%X]\n", register_x, V[register_x]);
    chip8.sound_timer = V[register_x];
  }

  /**
   *  Adds VX to I.
   */
  void CPU::opcode0xFX1E(unsigned short opcode)
  {
    int register_x = (opcode & 0x0F00) >> 8;

    print_debug("Add V%X [%X] to I [%X]\n", register_x, V[register_x], I);
    I += V[register_x];
  }

  /**
   *  Sets I to the location of the sprite for the character in VX.
   */
  void CPU::opcode0xFX29(unsigned short opcode)
  {
    int register_x = (opcode & 0x0F00) >> 8;

    print_debug("Set I to the location of the sprite for the character in V%X [%X].\n", register_x, V[register_x]);
    I = V[register_x] * 5;
  }

  /**
   *  Store the BCD representation of VX.
   */
  void CPU::opcode0xFX33(unsigned short opcode)
  {
    int register_x = (opcode & 0x0F00) >> 8;

    print_debug("Stores the BCD representation of V%X [%X] in I, I+1, I+2.\n", register_x, V[register_x]);
    chip8.memory[I] = V[register_x] / 100;
    chip8.memory[I + 1] = (V[register_x] / 10) % 10;
    chip8.memory[I + 2] = V[register_x] % 10;
  }

  /**
   *  Stores V0 to VX in memory starting at address I.
   */
  void CPU::opcode0xFX55(unsigned short opcode)
  {
    int register_x = (opcode & 0x0F00) >> 8;

    print_debug("Stores V0 to V%X in memory starting at I [%X].\n", register_x, I);

    for (int i = 0; i <= register_x; i++)
      chip8.memory[I + i] = V[i];

    I += register_x + 1;
  }

  /**
   *  Fills V0 to VX with values from memory starting at address I.
   */
  void CPU::opcode0xFX65(unsigned short opcode)
  {
    int register_x = (opcode & 0x0F00) >> 8;

    print_debug("Fills V0 to V%X with values from memory starting at I [%X].\n", register_x, I);

    for (int i = 0; i <= register_x; i++)
      V[i] = chip8.memory[I + i];

    I += register_x + 1;
  }

  /*
   *  Public methods
   */
  void CPU::execute(int cycles)
  {
    for (int i = cycles; i > 0; i--)
    {
      opcode = (chip8.memory[program_counter] << 8) | chip8.memory[program_counter + 1];

      print_debug("(%.4X) ", opcode);
      switch(opcode & 0xF000)
      {
        case 0x0000:  // Clear screen | Return from a subroutine
          handleOpcodes0x0000(opcode);
          break;
        case 0x1000:  // Jump to address
          opcode0x1NNN(opcode);
          break;
        case 0x2000:  // Call subroutine
          opcode0x2NNN(opcode);
          break;
        case 0x3000:  // Skip next instruction if VX == NN
          opcode0x3XNN(opcode);
          break;
        case 0x4000:  // Skip next instruction if VX != NN
          opcode0x4XNN(opcode);
          break;
        case 0x5000:  // Skip next instruction if VX == VY
          opcode0x5XY0(opcode);
          break;
        case 0x6000:  // Set VX to NN
          opcode0x6XNN(opcode);
          break;
        case 0x7000:  // Add NN to VX
          opcode0x7XNN(opcode);
          break;
        case 0x8000:  // Bit operations
          handleOpcodes0x8000(opcode);
          break;
        case 0x9000:  // Skip next instruction if VX != VY
          opcode0x9XY0(opcode);
          break;
        case 0xA000:  // Set I to the address NNN
          opcode0xANNN(opcode);
          break;
        case 0xB000:  // Jump to address NNN plus V0
          opcode0xBNNN(opcode);
          break;
        case 0xC000:  // Sets VX to a random number AND NN
          opcode0xCXNN(opcode);
          break;
        case 0xD000:  // Draw sprite at screen locatn (reg VX, reg VY) height N
          opcode0xDXYN(opcode);
          break;
        case 0xE000:  // Skip next instruction if the key stored in VX is pressed
          handleOpcodes0xE000(opcode);
          break;
        case 0xF000:
          handleOpcodes0xF000(opcode);
          break;
        default:
          fprintf(stderr, "Unsupported opcode %X\n", opcode);
          program_counter += 2;
      }
    }
  }

  void CPU::reset()
  {
    opcode = 0;
    stack = std::stack<unsigned int>();

    // Reset V-registers
    for (int i = 0; i < 16; i++)
      V[i] = 0;

    // Reset I register
    I = 0;

    // Reset PC-register (Program Counter)
    program_counter = 0x200;
  }
}
