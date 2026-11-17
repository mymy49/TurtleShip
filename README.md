# TurtleShip

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![Doxygen Build](https://github.com/mymy49/TurtleShip/actions/workflows/doxygen.yml/badge.svg)](https://github.com/mymy49/TurtleShip/actions)
[![Doxygen API Docs](https://img.shields.io/badge/docs-API%20Reference-brightgreen.svg)](https://mymy49.github.io/TurtleShip/)

Lightweight, high-performance 2D graphics engine and embedded GUI framework designed for **yss OS** on resource-constrained microcontrollers.

---

## 1. Overview & Key Features

**TurtleShip** is the core graphics and user interface library of **yss OS**. It is architected specifically for embedded systems with limited RAM, featuring device-independent 2D vector primitives, dynamic off-screen frame buffers, anti-aliased typography, and a retained-mode hierarchical widget toolkit.

### Key Highlights
- **Device-Independent 2D Drawing Engine (`Brush`)**:
  - Implements standard 2D graphics primitives: dots, lines (Bresenham's algorithm), rectangles, quadrangles, triangles, circles (outline and filled).
  - Fast clipping calculations via `BitmapDrawingCalculator` to ensure drawing commands stay strictly within valid canvas boundaries.
  - Multi-alignment text rendering (`ALIGN_CENTER_MID`, `ALIGN_LEFT_TOP`, `ALIGN_RIGHT_BOT`, etc.).
- **Anti-Aliased Typography (`Font`, `CodeFont`)**:
  - High-quality UTF-8 proportional and monospaced font rendering.
  - 4-bit per-pixel sub-pixel alpha blending (`blendDot()`) for smooth glyph anti-aliasing against any background color.
- **Flexible FrameBuffer Architecture (`FrameBuffer`)**:
  - Off-screen drawing surfaces supporting dynamic memory allocation (`malloc(capacity)`).
  - Native support for multiple color formats and endianness configurations:
    - `FrameBufferRgb565LE`: Little-Endian 16-bit RGB565 (standard for ARM Cortex-M).
    - `FrameBufferRgb565BE`: Big-Endian 16-bit RGB565.
    - `FrameBufferArgb1555LE` / `FrameBufferArgb1555`: 16-bit ARGB with 1-bit alpha channel.
  - Partial redraw and blitting (`drawBitmap()`) to transfer only modified areas to the display, drastically reducing SPI/parallel bus bottlenecks.
- **Comprehensive Color Abstraction (`Color`)**:
  - Unified color manipulation with native support for `ColorRgb565LE`, `ColorRgb565BE`, `ColorRgb888`, `ColorBgr565LE`, and `ColorBgr565BE`.
  - Bitwise conversion utilities and alpha blending calculations.
- **Hierarchical Widget Toolkit (`Object`, `Container`, `Frame`, `Button`, `Label`)**:
  - Retained-mode UI component system managing geometry (`Position`, `Size`, `Area`), visibility, and dirty region invalidation.
  - Interactive touch and pointer event routing (`push`, `drag`, `up`).

---

## 2. System Specifications & Hardware Environment

### Supported Targets & Tools
- **Target Microcontrollers & Development Boards**:
  - Nuvoton NuMicro® M251 series (e.g., **NuMaker-M251KG** / Cortex®-M23).
  - Any 32-bit MCU supported by the **yss OS** platform (ARM Cortex-M0+/M3/M4/M7/M23/M33, etc.).
- **Supported Display Interfaces**:
  - TFT LCDs via SPI, 8080/6800 parallel interfaces, or RGB interfaces (e.g., ST7789V, ILI9341, ST7735).
  - Demonstrations available on the **2.8-inch TFT Touch Shield Rev2.1** (240x320 ST7789V display).
- **Required Software & Toolchains**:
  - Toolchain: ARM GNU Toolchain or CrossWorks for ARM (v5.x or higher).
    - **Note on CrossWorks for ARM**: CrossWorks for ARM does not natively support the Nuvoton M251 series. You must install the custom [CrossworksForARM_Nuvoton_M251 Support Package](https://github.com/mymy49/CrossworksForARM_Nuvoton_M251) before building M251 projects.
  - Operating System: [yss OS](https://github.com/mymy49/yss) (embedded RTOS & HAL).
  - Language: C++11 or higher.

---

## 3. Installation & Project Setup

### Cloning with Submodules
To clone a project containing TurtleShip as a submodule:
```bash
git clone --recursive https://github.com/mymy49/NuMaker-M251KG_example.git
```

To add TurtleShip as a submodule to your project:
```bash
git submodule add https://github.com/mymy49/TurtleShip.git module/TurtleShip
```

### Prerequisites: Nuvoton M251 Support Package for CrossWorks for ARM
CrossWorks for ARM does not natively provide device support for Nuvoton NuMicro® M251 microcontrollers in its default package manager. A custom device support package must be installed beforehand:

1. Clone or download the [CrossworksForARM_Nuvoton_M251](https://github.com/mymy49/CrossworksForARM_Nuvoton_M251) repository.
2. Open the package project file (`.hzp`) in CrossWorks for ARM and build it (`F7` or `Build -> Build Project`) to generate the package file (`.hzq`).
3. In CrossWorks for ARM, navigate to **Tools -> Packages -> Manually Install Packages...** and select the generated `.hzq` package file.
4. Once installed, M251 target device definitions, CMSIS headers, and project templates will be fully available in CrossWorks for ARM.

### Build and Firmware Flashing
- **CrossWorks for ARM**:
  1. Ensure the [CrossworksForARM_Nuvoton_M251](https://github.com/mymy49/CrossworksForARM_Nuvoton_M251) package has been installed.
  2. Open the project solution file (e.g., `Example.hzp`).
  3. Select target configuration (`Example THUMB Release` or `Example THUMB Debug`).
  4. Build solution via `F7` or `Build -> Build Solution`.
  5. Connect the board via Nu-Link ICE and flash with `Ctrl+D` (`Target -> Download`).
- **Command Line (pyOCD / OpenOCD)**:
  ```bash
  # Flash prebuilt binary using pyocd
  pyocd flash -t m251kg firmware.bin
  ```

---

## 4. Quick Start & Example Code

Below is a typical real-world workflow using `FrameBufferRgb565LE` as an off-screen brush to compose UI elements and blit them to a TFT LCD display under yss OS:

### 1. FrameBuffer Allocation (`bsp.cpp`)
```cpp
#include <bsp.h>
#include <TurtleShip.h>

// Global off-screen drawing buffer instance
FrameBufferRgb565LE brush;

void initializeGraphics(void)
{
    // Allocate memory for up to 5,000 pixels (10,000 bytes for RGB565)
    brush.malloc(5000);
}
```

### 2. Off-Screen Rendering & Display Blitting
```cpp
#include <bsp.h>
#include <font/Ubuntu_20_B.h>

void drawCustomCard(Area area, const char *title)
{
    // Acquire mutex lock on display driver
    lcd.lock();

    // 1. Resize off-screen canvas to widget dimensions and clear background
    brush.setSize(area);
    brush.setBackgroundColor(0x20, 0x20, 0x20); // Dark grey
    brush.clear();

    // 2. Draw border and inner panel
    brush.setBrushColor(0x40, 0x40, 0x40);
    brush.fillRectangular({2, 2, area.width - 4, area.height - 4});

    // 3. Draw geometric shapes
    brush.setBrushColor(0x00, 0xFF, 0x80); // Accent color
    brush.fillCircle({12, 12}, 6);

    // 4. Render anti-aliased text
    brush.setFont(Font_Ubuntu_20_B);
    brush.setBrushColor(0xFF, 0xFF, 0xFF); // White text
    brush.drawString(Brush::ALIGN_CENTER_MID, title);

    // 5. Transfer rendered off-screen buffer to the target LCD area
    lcd.drawBitmap(area.getPosition(), brush.getBitmap());

    // Release mutex lock
    lcd.unlock();
}
```

### 3. Direct Screen Drawing
Display drivers (such as `Touch_LCD_Shield_for_Arduino_2_8_inch` or `BrushTftLcdRgb565LE`) inherit from `Brush`, enabling direct vector rendering onto the screen:
```cpp
lcd.lock();

// Direct line drawing on LCD surface
lcd.setBrushColor({0xFF, 0x00, 0x00});
lcd.drawLine({10, 10}, {200, 10});

// Direct rectangle drawing
lcd.setBrushColor({0x00, 0x00, 0xFF});
lcd.drawRectangular({20, 30, 100, 50});

lcd.unlock();
```

---

## 5. Documentation & References

- **Doxygen API Reference**: [https://mymy49.github.io/TurtleShip/](https://mymy49.github.io/TurtleShip/)
- **Doxygen Main Page**: This `README.md` file serves as the main page of the generated Doxygen documentation (`USE_MDFILE_AS_MAINPAGE = README.md`).
- **Related Modules**:
  - [ST7789V](https://github.com/mymy49/ST7789V): Hardware display driver module utilizing TurtleShip.
  - [yss OS](https://github.com/mymy49/yss): Underlying real-time operating system and HAL library.

---

## 6. License & Author

- **Author**: Yoon-Ki Hong
- **License**: MIT License. See [LICENSE](LICENSE) for full terms and conditions.
