#include "../include/CPU.h"
#include "../include/Chip8.h"

namespace YACE
{
  CPU::CPU(Chip8& chip8) : chip8(chip8), opcode(0), stack(), I(0), program_counter(0x1FF)
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
        opcode0x8000(opcode);
        break;
      case 0x1: // VX = VX | VY
        opcode0x8001(opcode);
        break;
      case 0x2: // VX = VX & VY
        opcode0x8002(opcode);
        break;
      case 0x3: // VX = VY ^ VY
        opcode0x8003(opcode);
        break;
      case 0x4: // VX = VX + VY, VF = Carry
        opcode0x8004(opcode);
        break;
      case 0x5: // VX = VX - VY, VF = !Borrow
        opcode0x8005(opcode);
        break;
      case 0x6: // VX = VX >> 1, VF = Carry
        opcode0x8006(opcode);
        break;
      case 0x7: // VX = VY - VX, VF = !Borrow
        opcode0x8007(opcode);
        break;
      case 0xE: // VX = VX << 1, VF = Carry
        opcode0x800E(opcode);
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
      opcode0xE09E(opcode);
    else if ((opcode & 0x00FF) == 0xA1)
      opcode0xE0A1(opcode);

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
        opcode0xF007(opcode);
        break;
      case 0xA: // VX = Key press
        opcode0xF00A(opcode);
        break;
      case 0x15:  // delay_timer = VX
        opcode0xF015(opcode);
        break;
      case 0x18:  // sound_timer = VX
        opcode0xF018(opcode);
        break;
      case 0x1E:  // I = I + VX
        opcode0xF01E(opcode);
        break;
      case 0x29:  // I = Location of the sprite for character in VX
        opcode0xF029(opcode);
        break;
      case 0x33:  // I, I + 1, I + 2 = BCD of VX
        opcode0xF033(opcode);
        break;
      case 0x55:  // Stores V0 to VX in memory starting at I
        opcode0xF055(opcode);
        break;
      case 0x65:  // Fills V0 to VX with values from memory starting at I
        opcode0xF065(opcode);
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
  void CPU::opcode0x1000(unsigned short opcode)
  {
    print_debug("Jump to address %X.\n", opcode & 0x0FFF);
    program_counter = (opcode & 0xFFF) - 1;
  }

  /**
   *  Calls subroutine at NNN.
   */
  void CPU::opcode0x2000(unsigned short opcode)
  {
    print_debug("Call subroutine at %X.\n", opcode & 0x0FFF);
    if (stack.size() < 16)
    {
      stack.push(program_counter);
      program_counter = (opcode & 0xFFF) - 1;
    }
    else
      program_counter += 2;
  }

  /**
   *  Skips next instruction if VX == NN.
   */
  void CPU::opcode0x3000(unsigned short opcode)
  {
    print_debug("Skips next instruction if VX == NN.\n");
    if (V[((opcode & 0x0F00) >> 8)] == (opcode & 0x00FF))
      program_counter += 4;
    else
      program_counter += 2;
  }

  /**
   *  Skips the next instruction if VX != NN.
   */
  void CPU::opcode0x4000(unsigned short opcode)
  {
    print_debug("Skips next instruction if VX != NN.\n");
    if (V[((opcode & 0x0F00) >> 8)] != (opcode & 0x00FF))
      program_counter += 4;
    else
      program_counter += 2;
  }

  /**
   *  Skips the next instruction of VX == VY.
   */
  void CPU::opcode0x5000(unsigned short opcode)
  {
    print_debug("Skip next instruction if VX == VY.\n");
    if (V[((opcode & 0x0F00) >> 8)] == V[((opcode & 0x00F0) >> 4)])
      program_counter += 4;
    else
      program_counter += 2;
  }

  /**
   *  Sets VX to NN.
   */
  void CPU::opcode0x6000(unsigned short opcode)
  {
    print_debug("Set V%X to %X\n", ((opcode & 0x0F00) >> 8), opcode & 0x00FF);
    V[((opcode & 0x0F00) >> 8)] = opcode & 0x00FF;

    program_counter += 2;
  }

  /**
   *  Adds NN to VX.
   */
  void CPU::opcode0x7000(unsigned short opcode)
  {
    print_debug("Add NN to VX.\n");
    V[((opcode & 0x0F00) >> 8)] += opcode & 0x00FF;

    program_counter += 2;
  }

  /**
   *  Sets VX to the value of VY.
   */
  void CPU::opcode0x8000(unsigned short opcode)
  {
    print_debug("Set V%X to the value of V%X.\n", ((opcode & 0x0F00) >> 8),
                                             ((opcode & 0x00F0) >> 4));
    V[((opcode & 0x0F00) >> 8)] = V[((opcode & 0x00F0) >> 4)];
  }

  /**
   *  Sets VX to VX | VY.
   */
  void CPU::opcode0x8001(unsigned short opcode)
  {
    print_debug("Set VX to VX OR VY.\n");
    V[((opcode & 0x0F00) >> 8)] |= V[((opcode & 0x00F0) >> 4)];
  }

  /**
   *  Sets VX to VX & VY.
   */
  void CPU::opcode0x8002(unsigned short opcode)
  {
    print_debug("Set VX to VX AND VY.\n");
    V[((opcode & 0x0F00) >> 8)] &= V[((opcode & 0x00F0) >> 4)];
  }

  /**
   *  Sets VX to VX ^ VY.
   */
  void CPU::opcode0x8003(unsigned short opcode)
  {
    print_debug("Set VX to VX XOR VY.\n");
    V[((opcode & 0x0F00) >> 8)] ^= V[((opcode & 0x00F0) >> 4)];
  }

  /**
   *  VX = VX + VY. VF = Carry (1 if carry).
   */
  void CPU::opcode0x8004(unsigned short opcode)
  {
    print_debug("Set VX to VX + VY.\n");
    V[0xF] = (V[((opcode & 0x0F00) >> 8)] + V[((opcode & 0x00F0) >> 4)]) > 0xFF;
    V[((opcode & 0x0F00) >> 8)] += V[((opcode & 0x00F0) >> 4)];
  }

  /**
   *  VX = VX - VY. VF = Borrow (0 if borrow).
   */
  void CPU::opcode0x8005(unsigned short opcode)
  {
    print_debug("Set VX to VX - VY.\n");
    V[0xF] = !(V[((opcode & 0x0F00) >> 8)] < V[((opcode & 0x00F0) >> 4)]);
    V[((opcode & 0x0F00) >> 8)] -= V[((opcode & 0x00F0) >> 4)];
  }

  /**
   *  VX = VX >> 1. VF = Least significant bit.
   */
  void CPU::opcode0x8006(unsigned short opcode)
  {
    print_debug("Shift VX right by 1.\n");
    V[0xF] = V[((opcode & 0x0F00) >> 8)] & 0x1;
    V[((opcode & 0x0F00) >> 8)] >>= 1;
  }

  /**
   *  VX = VY - VX. VF = Borrow (0 if borrow).
   */
  void CPU::opcode0x8007(unsigned short opcode)
  {
    print_debug("Set VX = VY - VX.\n");
    V[0xF] = !(V[((opcode & 0x00F0) >> 4)] < V[((opcode & 0x0F00) >> 8)]);
    V[((opcode & 0x0F00) >> 8)] = V[((opcode & 0x00F0) >> 4)] -
                                  V[((opcode & 0x0F00) >> 8)];
  }

  /**
   *  VX = VX << 1. VF = Most significant bit.
   */
  void CPU::opcode0x800E(unsigned short opcode)
  {
    print_debug("Shift VX left by 1.\n");
    V[0xF] = V[((opcode & 0x0F00) >> 8)] & 0x80;
    V[((opcode & 0x0F00) >> 8)] <<= 1;
  }

  /**
   *  Skips the next instruction if VX != VY.
   */
  void CPU::opcode0x9000(unsigned short opcode)
  {
    print_debug("Skip next instruction if VX != VY.\n");
    if (V[((opcode & 0x0F00) >> 8)] != V[((opcode & 0x00F0) >> 4)])
      program_counter += 4;
    else
      program_counter += 2;
  }

  /**
   *  Sets I to the address NNN.
   */
  void CPU::opcode0xA000(unsigned short opcode)
  {
    print_debug("Set I to the address NNN.\n");
    I = (opcode & 0x0FFF) - 1;

    program_counter += 2;
  }

  /**
   *  Jumps to the address NNN + V0.
   */
  void CPU::opcode0xB000(unsigned short opcode)
  {
    print_debug("Jump to address NNN plus V0.\n");
    program_counter = V[0] + (opcode & 0x0FFF) - 1;
  }

  /**
   *  Sets VX to a random number AND NN.
   */
  void CPU::opcode0xC000(unsigned short opcode)
  {
    print_debug("Set VX to a random number AND NN.\n");
    V[((opcode & 0x0F00) >> 8)] = ((rand() % 0xFF) & 0xFF) & (opcode & 0x00FF);

    program_counter += 2;
  }

  /**
   *  Draws a sprite at coordinate (VX, VY).
   */
  void CPU::opcode0xD000(unsigned short opcode)
  {
    print_debug("Draw sprite at screen location.\n");

    V[0xF] = 0;
    int pos_x = V[(opcode & 0x0F00) >> 8];
    int pos_y = V[(opcode & 0x00F0) >> 4];
    int lines = opcode & 0x000F;
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
  void CPU::opcode0xE09E(unsigned short opcode)
  {
    print_debug("Skip next instruction if key is pressed.\n");
    if (chip8.keys[int(V[((opcode & 0x0F00) >> 8)])])
      program_counter += 2;
  }

  /**
   *  Skips the next instruction if the key stored in VX isn't pressed.
   */
  void CPU::opcode0xE0A1(unsigned short opcode)
  {
    print_debug("Skip next instruction if key isn't pressed.\n");
    if (!chip8.keys[int(V[((opcode & 0x0F00) >> 8)])])
      program_counter += 2;
  }

  /**
   *  Sets VX to the value of the delay timer.
   */
  void CPU::opcode0xF007(unsigned short opcode)
  {
    print_debug("Set VX to the value of the delay timer.\n");
    V[((opcode & 0x0F00) >> 8)] = chip8.delay_timer;
  }

  /**
   *  A key press is awaited, and then stored in VX.
   */
  void CPU::opcode0xF00A(unsigned short opcode)
  {
    print_debug("Set VX to awaited key press.\n");
    if (chip8.key_is_pressed)
    {
      V[((opcode & 0x0F00) >> 8)] = chip8.last_key_pressed;
      chip8.key_is_pressed = false;
    }
    else
      program_counter -= 2;
  }

  /**
   *  Sets the delay timer to VX.
   */
  void CPU::opcode0xF015(unsigned short opcode)
  {
    print_debug("Set delay timer to VX.\n");
    chip8.delay_timer = V[((opcode & 0x0F00) >> 8)];
  }

  /**
   *  Sets the sound timer to VX.
   */
  void CPU::opcode0xF018(unsigned short opcode)
  {
    print_debug("Set sound timer to VX\n");
    chip8.sound_timer = V[((opcode & 0x0F00) >> 8)];
  }

  /**
   *  Adds VX to I.
   */
  void CPU::opcode0xF01E(unsigned short opcode)
  {
    print_debug("Add VX to I\n");
    I += V[((opcode & 0x0F00) >> 8)];
  }

  /**
   *  Sets I to the location of the sprite for the character in VX.
   */
  void CPU::opcode0xF029(unsigned short opcode)
  {
    print_debug("Set I to the location of the sprite for the character in VX.\n");
    I = V[((opcode & 0x0F00) >> 8)] * 5;
  }

  /**
   *  Store the BCD representation of VX.
   */
  void CPU::opcode0xF033(unsigned short opcode)
  {
    print_debug("Stores the BCD representation of VX in I, I+1, I+2.\n");
    chip8.memory[I] = V[((opcode & 0x0F00) >> 8)] / 100;
    chip8.memory[I + 1] = (V[((opcode & 0x0F00) >> 8)] / 10) % 10;
    chip8.memory[I + 2] = V[((opcode & 0x0F00) >> 8)] % 10;
  }

  /**
   *  Stores V0 to VX in memory starting at address I.
   */
  void CPU::opcode0xF055(unsigned short opcode)
  {
    print_debug("Stores V0 to VX in memory starting at I.\n");
    for (int i = 0; i < ((opcode & 0x0F00) >> 8); i++)
      chip8.memory[I + i] = V[i];

    I += ((opcode & 0x0F00) >> 8) + 1;
  }

  /**
   *  Fills V0 to VX with values from memory starting at address I.
   */
  void CPU::opcode0xF065(unsigned short opcode)
  {
    print_debug("Fills V0 to VX with values from memory starting at I.\n");
    for (int i = 0; i < ((opcode & 0x0F00) >> 8); i++)
      V[i] = chip8.memory[I + i];

    I += ((opcode & 0x0F00) >> 8) + 1;
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
          opcode0x1000(opcode);
          break;
        case 0x2000:  // Call subroutine
          opcode0x2000(opcode);
          break;
        case 0x3000:  // Skip next instruction if VX == NN
          opcode0x3000(opcode);
          break;
        case 0x4000:  // Skip next instruction if VX != NN
          opcode0x4000(opcode);
          break;
        case 0x5000:  // Skip next instruction if VX == VY
          opcode0x5000(opcode);
          break;
        case 0x6000:  // Set VX to NN
          opcode0x6000(opcode);
          break;
        case 0x7000:  // Add NN to VX
          opcode0x7000(opcode);
          break;
        case 0x8000:  // Bit operations
          handleOpcodes0x8000(opcode);
          break;
        case 0x9000:  // Skip next instruction if VX != VY
          opcode0x9000(opcode);
          break;
        case 0xA000:  // Set I to the address NNN
          opcode0xA000(opcode);
          break;
        case 0xB000:  // Jump to address NNN plus V0
          opcode0xB000(opcode);
          break;
        case 0xC000:  // Sets VX to a random number AND NN
          opcode0xC000(opcode);
          break;
        case 0xD000:  // Draw sprite at screen locatn (reg VX, reg VY) height N
          opcode0xD000(opcode);
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
    program_counter = 0x1FF;
  }
}
