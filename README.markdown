# PNG Image Processing CLI Utility

## Overview

This project is a command-line interface (CLI) utility for processing PNG images. The utility uses the `libpng` library to perform operations on PNG images, including drawing a square with diagonals, applying an RGB color filter, rotating a specified image area, drawing rhombus, paving and also can expand original image resolution. It is written in C and uses `getopt` for parsing command-line arguments.

## Main features

- **Draw Square with Diagonals**: Draws a square with specified coordinates, side size, line thickness, and color, with optional filling.
- **RGB Filter**: Applies a filter to set a specific color component (red, green, or blue) to a chosen value across the entire image.
- **Rotate Area**: Rotates a specified rectangular area of the image by 90, 180, or 270 degrees.
- **Image Information**: Displays metadata about the PNG image (width, height, bit depth, color type, etc.).
- **Error Handling**: Robust validation of input parameters and error reporting for memory, file, and command issues.
- **Help Menu**: Provides a detailed help message with available options and their formats.

## Requirements

- **Compiler**: GCC or any C compiler supporting C99.
- **Library**: `libpng` (version compatible with `PNG_LIBPNG_VER_STRING`).
- **Operating System**: Linux or any POSIX-compliant system (developed and tested on Linux ).
- **Dependencies**: Standard C libraries (`stdio.h`, `stdlib.h`, `string.h`, `math.h`, `stdbool.h`) and `getopt.h`.

## Installation

1. **Clone the Repository**:

   ```bash
   git clone https://github.com/fedor-serdiukov/png_processing.git
   cd png_processing
   ```

2. **Install libpng**: On a Debian-based system (e.g., Ubuntu), run:

   ```bash
   sudo apt-get install libpng-dev
   ```

3. **Compile the Program**:

   ```bash
   #on Debian-based systems
   gcc -o cw png.c -lpng
   ```

## Usage

Run the program with the following syntax:

```bash
./cw [options] --input input.png [--output output.png]
```

### Available Options

- `-h, --help`: Display the help message with all available options.
- `-i, --input <FILE_NAME>`: Specify the input PNG file (required).
- `-o, --output <FILE_NAME>`: Specify the output PNG file (default: `out.png`).
- `--info`: Display PNG file information (width, height, bit depth, etc.).
- `--squared_lines`: Draw a square with diagonals. Requires:
  - `--left_up <x.y>`: Top-left corner coordinates (e.g., `100.100`).
  - `--side_size <l>`: Side length of the square (&gt; 0).
  - `--thickness <t>`: Line thickness (&gt; 0).
  - `--color <R.G.B>`: Line color (e.g., `255.0.0` for red).
  - `--fill`: Fill the square (optional).
  - `--fill_color <R.G.B>`: Fill color (e.g., `200.200.0`).
- `--rgbfilter`: Apply an RGB filter. Requires:
  - `--component_name <C>`: Color component (`red`, `green`, or `blue`).
  - `--component_value <v>`: Component value (0–255).
- `--rotate`: Rotate a rectangular area. Requires:
  - `--left_up <x.y>`: Top-left corner coordinates.
  - `--right_down <x.y>`: Bottom-right corner coordinates.
  - `--angle <d>`: Rotation angle (90, 180, or 270 degrees).
- other functions will be described soon(rhombus, paving, expand etc.)

### Example Commands

1. **Display Help**:

   ```bash
   ./cw --help
   ```

2. **Draw a Square**:

   ```bash
   ./cw --input input.png --squared_lines --left_up 100.100 --side_size 50 --thickness 2 --color 255.0.0 --fill --fill_color 200.200.0 --output out.png
   ```

3. **Apply RGB Filter**:

   ```bash
   ./cw --input input.png --rgbfilter --component_name red --component_value 255 --output out.png
   ```

4. **Rotate an Area**:

   ```bash
   ./cw --input input.png --rotate --left_up 50.50 --right_down 150.150 --angle 90 --output out.png
   ```

5. **Show Image Info**:

   ```bash
   ./cw --input input.png --info
   ```

## Error Handling

The program defines specific error codes:

- `40`: Memory allocation failure.
- `41`: Invalid parameter type or value.
- `42`: File I/O error.
- `43`: libpng internal error.
- `44`: Invalid command-line argument.

Errors are reported to `stderr` with descriptive messages, and the program exits with the corresponding code.

## Project Structure

- `src/png.c`: Main source file.
- `README.md`: This file, providing project documentation.

## Testing

The program has been tested with various PNG images to ensure correct behavior for:

- Drawing squares with and without fill.
- Applying RGB filters to different color components.
- Rotating rectangular areas at supported angles.
- Handling edge cases (e.g., invalid coordinates, non-RGB images, or file errors).

## License

This project is licensed under the MIT License. See the `LICENSE` file for details.

## Author

Fedor Serdiukov
