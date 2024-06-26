# Reviser

A C++ implementation of the Reversi (Othello) game.

## Installation

You need a C++ compiler that supports C++23 and CMake to build the project.
The project has been tested with, GCC 14, Clang 18 or MSVC 19.39.33523.0.

If you have a C++20 compiler, you can modify the `CMakeLists.txt` file to
require C++20 instead of C++23 and change the uses of `std::format` to,
e.g., string concatenation.

If you have these prerequisites, you can build the project by running
the following commands:

```bash
git clone https://github.com/hoelzl/Reviser.git
mkdir Build
cd Build
cmake ../Reviser
cmake --build .
```

Then run the binary created by the build process.
For Windows this is typically something like

```powershell
.\reviser-cli\Debug\reviser-cli.exe
```

For Linux it may be something like:

```bash
./reviser-cli/reviser-cli
```

## Usage

The only command line argument understood by the program is `-h` or `--human` which
will change the game from a computer-vs-computer game to a human-vs-computer
game. In this mode, the human player is always black and the computer is always white.