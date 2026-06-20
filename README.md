# Conway's Game of Life (Native C++)

A high-performance graphical implementation of Conway's Game of Life built using pure, native C++ and the Windows API (`<windows.h>`). This implementation utilizes double-buffering techniques via the Windows GDI to ensure smooth, flicker-free rendering of cellular automata generations.

## 🚀 Features
- **Zero External Dependencies:** Built entirely using native Win32 API functions.
- **Double-Buffered Rendering:** Uses custom memory device contexts (`HDC`) to prevent screen flickering during rapid grid state updates.
- **Dynamic Seeding:** Automatically generates a randomized grid configuration upon initialization.

## 🛠️ Compilation and Setup
To compile this project locally using GCC/G++ on Windows, navigate to your source directory and execute the following linking command:

```bash
g++ rep.cpp -o game.exe -lgdi32
