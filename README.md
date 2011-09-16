YACE (Yet Another Chip8 Emulator)
=================================
YACE is a Chip8 emulator written in C++. YACE is only the emulation core and doesn't come with a GUI.

Usage
-----
YACE is supposed to be used in other projects that implements a GUI for YACE.

Compiling
---------
Since YACE is only the emulation core and doesn't provide a GUI, it's kind of pointless to compile it by itself. Despite this it's still possible to compile a **debug** version of YACE to view debug prints in a terminal\command line interface.

Make is needed to compile YACE using the provided *makefile*. Using a terminal\command line, navigate to the folder where YACE is located, type *make* and press enter.

**NOTE**
*The provided makefile uses g++ as the compiler.*

YACE GUIs
---------
+ Allegro YACE (Link coming...)

To-do list
----------
+ Work out bug(s) corrupting parts of graphics in some games (ex. in Space Invaders there's some text that's corrupted).
