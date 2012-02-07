YACE (Yet Another Chip8 Emulator)
=================================
YACE is a Chip8 emulator written in C++. YACE is only the emulation core and doesn't come with a Front end.

Usage
-----
YACE is supposed to be used in other projects that implements a front end for YACE.

Compiling
---------
Since YACE is only the emulation core and doesn't provide a front end, it's kind of pointless to compile it by itself. Despite this it's still possible to compile a **debug** version of YACE to view debug prints in a terminal\command line interface.

Make is needed to compile YACE using the provided *makefile*. Using a terminal\command line, navigate to the folder where YACE is located, type *make* and press enter.

###NOTE
*The provided makefile uses g++ as the compiler.*

To-do list
----------
Implement SuperChip opcodes.

YACE Front ends
---------
+ [Allegro YACE] (https://github.com/Johoel/Allegro-YACE)
