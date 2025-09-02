# Calculator

A simple calculator application built with GTK+ and C++ for FinnOS.

## Features

- Basic arithmetic operations: addition, subtraction, multiplication, division
- Responsive UI with custom styling
- Smooth animations and hover effects
- Right-aligned display text
- Expression history display

## Building

Use the provided build script:

```bash
chmod +x build.sh
./build.sh
```

Or manually:

```bash
g++ main.cpp `pkg-config --cflags --libs gtk+-3.0` -o calculator
```

## Running

```bash
./calculator
```

## License

This project is licensed under the GNU General Public License v3.0 - see the LICENSE file for details.
