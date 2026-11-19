/*
 * Copyright (c) 2015 Yoon-Ki Hong
 *
 * This file is subject to the terms and conditions of the MIT License.
 * See the file "LICENSE" in the main directory of this archive for more details.
 */

/**
 * @file Brush.h
 * @brief Abstract 2D drawing interface (dots, lines, shapes, text, and bitmaps) of the TurtleShip GUI library.
 * @author Yoon-Ki Hong
 * @date 2015
 */

#ifndef YSS_GUI_BRUSH__H_
#define YSS_GUI_BRUSH__H_

#include <stdint.h>
#include "Color.h"
#include <yss/Position.h>
#include <yss/Area.h>
#include <yss/Size.h>
#include "TurtleShipTypes.h"

class Font;

/**
 * @class Brush
 * @brief Abstract base class providing generic 2D drawing primitives on a pixel canvas.
 *
 * @details
 * `Brush` implements device-independent 2D graphics algorithms—including straight lines, outlined and
 * filled circles, triangles, rectangles, quadrangles, anti-aliased UTF-8 text rendering, and bitmap
 * clipping helpers—on top of a small set of pure virtual pixel-level operations (`drawDot()`,
 * `blendDot()`, `fillDotArray()`, `drawBitmapBase()`, `getCanvasSize()`, `getPixelCapacity()`).
 *
 * Concrete drawing surfaces such as off-screen frame buffers (e.g. `FrameBufferRgb565LE`,
 * `FrameBufferRgb565BE`) or direct TFT LCD drivers (e.g. `BrushTftLcdRgb565LE`,
 * `Touch_LCD_Shield_for_Arduino_2_8_inch`) derive from `Brush` and implement the pixel-format or
 * hardware-specific operations.
 *
 * Text strings are rendered using a `Font` (such as `CodeFont`) registered through `setFont()`, with
 * 4-bit per-pixel alpha blending handled via `blendDot()`.
 *
 * ### Example Usage in User Application
 * @code
 * #include <bsp.h>
 * #include <font/Ubuntu_20_B.h>
 *
 * void drawCloseButton(Area area)
 * {
 *     lcd.lock();
 *
 *     // Off-screen rendering on a FrameBufferRgb565LE (which inherits Brush)
 *     brush.setSize(area);
 *     brush.setBackgroundColor(0x80, 0x80, 0x80);
 *     brush.clear();
 *
 *     brush.setBrushColor(0xA0, 0xA0, 0xA0);
 *     brush.fillRectangular({3, 3, 24, 34});
 *
 *     brush.setFont(Font_Ubuntu_20_B);
 *     brush.setBrushColor(0x00, 0x00, 0x00);
 *     brush.drawString(Brush::ALIGN_CENTER_MID, "X");
 *
 *     // Blit the rendered off-screen buffer onto the LCD (which also inherits Brush)
 *     lcd.drawBitmap(area.getPosition(), brush.getBitmap());
 *
 *     // Direct line drawing on the LCD surface
 *     lcd.setBrushColor({0xFF, 0x00, 0x00});
 *     lcd.drawLine({10, 10}, {100, 80});
 *
 *     lcd.unlock();
 * }
 * @endcode
 */
class Brush
{
public:
	/**
	 * @enum align_t
	 * @brief Text alignment flags used by `drawString(align_t, const char*)`.
	 *
	 * @details
	 * Bits `[2:0]` encode horizontal alignment (`0x01`: left, `0x02`: center, `0x04`: right),
	 * and bits `[5:3]` encode vertical alignment (`0x08`: top, `0x10`: middle, `0x20`: bottom).
	 */
	typedef enum
	{
		ALGIN_LEFT_TOP = (0x01 | 0x08),		///< Left aligned, top aligned.
		ALGIN_LEFT_MID = (0x01 | 0x10),		///< Left aligned, vertically centered.
		ALGIN_LEFT_BOT = (0x01 | 0x20),		///< Left aligned, bottom aligned.

		ALIGN_CENTER_TOP = (0x02 | 0x08),	///< Horizontally centered, top aligned.
		ALIGN_CENTER_MID = (0x02 | 0x10),	///< Horizontally and vertically centered.
		ALIGN_CENTER_BOT = (0x02 | 0x20),	///< Horizontally centered, bottom aligned.

		ALIGN_RIGHT_TOP = (0x04 | 0x08),	///< Right aligned, top aligned.
		ALIGN_RIGHT_MID = (0x04 | 0x10),	///< Right aligned, vertically centered.
		ALIGN_RIGHT_BOT = (0x04 | 0x20)		///< Right aligned, bottom aligned.
	}align_t;

	/**
	 * @brief Constructs a `Brush` instance with no font attached (`mFont = nullptr`).
	 * @param[in] none
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2015
	 * @warning None.
	 * @note None.
	 * @pre None.
	 * @post None.
	 * @deprecated None.
	 */
	Brush(void);

	/**
	 * @brief Draws a single pixel at `(x, y)` using the current brush (foreground) color.
	 * @param[in] x X coordinate of the pixel on the canvas.
	 * @param[in] y Y coordinate of the pixel on the canvas.
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2015
	 * @warning None.
	 * @note None.
	 * @pre None.
	 * @post None.
	 * @deprecated None.
	 */
	virtual void drawDot(int16_t x, int16_t y) __attribute__((optimize("-O1"))) = 0;

	/**
	 * @brief Alpha-blends the current brush color with the existing pixel at `(x, y)`.
	 * @param[in] x     X coordinate of the pixel on the canvas.
	 * @param[in] y     Y coordinate of the pixel on the canvas.
	 * @param[in] alpha Blending weight of the brush color (`0x00`: transparent, `0xFF`: opaque).
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2015
	 * @warning None.
	 * @note Primarily used by `drawChar()` to render 4-bit anti-aliased font glyphs.
	 * @pre None.
	 * @post None.
	 * @deprecated None.
	 */
	virtual void blendDot(int16_t x, int16_t y, uint8_t alpha) __attribute__((optimize("-O1"))) = 0;
	
	/**
	 * @brief Sets the current brush (foreground) color from a `Color` object.
	 * @param[in] color New foreground brush color.
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2015
	 * @warning None.
	 * @note None.
	 * @pre None.
	 * @post None.
	 * @deprecated None.
	 */
	virtual void setBrushColor(Color color) __attribute__((optimize("-O1"))) = 0;

	/**
	 * @brief Sets the current brush (foreground) color from individual RGBA components.
	 * @param[in] red   Red channel intensity (`0` ~ `255`).
	 * @param[in] green Green channel intensity (`0` ~ `255`).
	 * @param[in] blue  Blue channel intensity (`0` ~ `255`).
	 * @param[in] alpha Alpha channel intensity (`0` ~ `255`, default `0xFF`: opaque).
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2015
	 * @warning None.
	 * @note None.
	 * @pre None.
	 * @post None.
	 * @deprecated None.
	 */
	virtual void setBrushColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 0xFF) __attribute__((optimize("-O1"))) = 0;

	/**
	 * @brief Gets the current brush (foreground) color.
	 * @param[in] none
	 * @return Current foreground `Color` object.
	 * @author Yoon-Ki Hong
	 * @date 2015
	 * @warning None.
	 * @note None.
	 * @pre None.
	 * @post None.
	 * @deprecated None.
	 */
	virtual Color getBrushColor() __attribute__((optimize("-O1"))) = 0;

	/**
	 * @brief Sets the background color used by `clear()` and `clearRectangular()`.
	 * @param[in] color New background color.
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2015
	 * @warning None.
	 * @note None.
	 * @pre None.
	 * @post None.
	 * @deprecated None.
	 */
	virtual void setBackgroundColor(Color color) __attribute__((optimize("-O1"))) = 0;

	/**
	 * @brief Sets the background color from individual RGBA components.
	 * @param[in] red   Red channel intensity (`0` ~ `255`).
	 * @param[in] green Green channel intensity (`0` ~ `255`).
	 * @param[in] blue  Blue channel intensity (`0` ~ `255`).
	 * @param[in] alpha Alpha channel intensity (`0` ~ `255`, default `0xFF`: opaque).
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2015
	 * @warning None.
	 * @note None.
	 * @pre None.
	 * @post None.
	 * @deprecated None.
	 */
	virtual void setBackgroundColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 0xFF) __attribute__((optimize("-O1"))) = 0;

	/**
	 * @brief Gets the current background color.
	 * @param[in] none
	 * @return Current background `Color` object.
	 * @author Yoon-Ki Hong
	 * @date 2015
	 * @warning None.
	 * @note None.
	 * @pre None.
	 * @post None.
	 * @deprecated None.
	 */
	virtual Color getBackgroundColor() __attribute__((optimize("-O1"))) = 0;

	/**
	 * @brief Draws a straight line between two points using the current brush color.
	 * @param[in] p1 Start point `(x, y)` of the line.
	 * @param[in] p2 End point `(x, y)` of the line.
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2015
	 * @warning None.
	 * @note None.
	 * @pre None.
	 * @post None.
	 * @deprecated None.
	 */
	void drawLine(Position p1, Position p2) __attribute__((optimize("-O1")));

	/**
	 * @brief Draws the outline of a circle using the current brush color.
	 * @param[in] pos    Center coordinate of the circle.
	 * @param[in] radius Radius of the circle in pixels.
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2015
	 * @warning Nothing is drawn if the radius `radius` is less than 3 pixels.
	 * @note None.
	 * @pre None.
	 * @post None.
	 * @deprecated None.
	 */
	void drawCircle(Position pos, uint16_t radius) __attribute__((optimize("-O1")));

	/**
	 * @brief Draws a filled circle using the current brush color.
	 * @param[in] pos    Center coordinate of the circle.
	 * @param[in] radius Radius of the circle in pixels.
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2015
	 * @warning Nothing is drawn if the radius `radius` is less than 3 pixels.
	 * @note None.
	 * @pre None.
	 * @post None.
	 * @deprecated None.
	 */
	void fillCircle(Position pos, uint16_t radius) __attribute__((optimize("-O1")));

	/**
	 * @brief Draws the wireframe outline of a triangle connecting three vertices.
	 * @param[in] p1 First vertex coordinate.
	 * @param[in] p2 Second vertex coordinate.
	 * @param[in] p3 Third vertex coordinate.
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2015
	 * @warning None.
	 * @note None.
	 * @pre None.
	 * @post None.
	 * @deprecated None.
	 */
	void drawTriangle(Position p1, Position p2, Position p3) __attribute__((optimize("-O1")));

	/**
	 * @brief Fills the interior of a triangle defined by three vertices with the current brush color.
	 * @param[in] p1 First vertex coordinate.
	 * @param[in] p2 Second vertex coordinate.
	 * @param[in] p3 Third vertex coordinate.
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2015
	 * @warning None.
	 * @note None.
	 * @pre None.
	 * @post None.
	 * @deprecated None.
	 */
	void fillTriangle(Position p1, Position p2, Position p3) __attribute__((optimize("-O1")));

	/**
	 * @brief Draws the outline of a rectangle with the current brush color.
	 * @param[in] rect Rectangular area (top-left position and width/height).
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2015
	 * @warning None.
	 * @note None.
	 * @pre None.
	 * @post None.
	 * @deprecated None.
	 */
	void drawRectangular(Area rect) __attribute__((optimize("-O1")));

	/**
	 * @brief Fills a rectangular area with the current brush color.
	 * @param[in] rect Rectangular area (top-left position and width/height).
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2015
	 * @warning None.
	 * @note None.
	 * @pre None.
	 * @post None.
	 * @deprecated None.
	 */
	void fillRectangular(Area rect) __attribute__((optimize("-O1")));

	/**
	 * @brief Fills a four-vertex quadrangle with the current brush color.
	 * @param[in] p1 First vertex coordinate.
	 * @param[in] p2 Second vertex coordinate.
	 * @param[in] p3 Third vertex coordinate.
	 * @param[in] p4 Fourth vertex coordinate.
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2015
	 * @warning None.
	 * @note Decomposes the quadrangle into two filled triangles: `(p1, p2, p4)` and `(p2, p3, p4)`.
	 * @pre None.
	 * @post None.
	 * @deprecated None.
	 */
	void fillQuadrangle(Position p1, Position p2, Position p3, Position p4) __attribute__((optimize("-O1")));

	/**
	 * @brief Fills a rectangular area with the current background color.
	 * @param[in] rect Rectangular area (top-left position and width/height) to clear.
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2015
	 * @warning None.
	 * @note None.
	 * @pre None.
	 * @post None.
	 * @deprecated None.
	 */
	void clearRectangular(Area rect) __attribute__((optimize("-O1")));

	/**
	 * @brief Fills the entire canvas with the current brush (foreground) color.
	 * @param[in] none
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2015
	 * @warning None.
	 * @note None.
	 * @pre None.
	 * @post None.
	 * @deprecated None.
	 */
	void fill() __attribute__((optimize("-O1")));

	/**
	 * @brief Clears the entire canvas by filling it with the current background color.
	 * @param[in] none
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2015
	 * @warning None.
	 * @note None.
	 * @pre None.
	 * @post None.
	 * @deprecated None.
	 */
	void clear() __attribute__((optimize("-O1")));

	/**
	 * @brief Attaches a `Font` object to be used for subsequent character and string rendering.
	 * @param[in] font Reference to the `Font` object.
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2015
	 * @warning Only the address of `font` is stored; the referenced `Font` instance must remain valid
	 *          throughout its use by this `Brush`.
	 * @note None.
	 * @pre None.
	 * @post None.
	 * @deprecated None.
	 */
	void setFont(Font &font) __attribute__((optimize("-O1")));

	/**
	 * @brief Gets the pointer to the currently attached `Font`.
	 * @param[in] none
	 * @return Pointer to the active `Font`, or `nullptr` if no font has been configured.
	 * @author Yoon-Ki Hong
	 * @date 2015
	 * @warning None.
	 * @note None.
	 * @pre None.
	 * @post None.
	 * @deprecated None.
	 */
	Font* getFont() __attribute__((optimize("-O1")));

	/**
	 * @brief Renders a single UTF-8 character at the specified position using the current font and brush color.
	 * @param[in] pos  Top-left coordinate of the character cell on the canvas.
	 * @param[in] utf8 Packed UTF-8 character code (as returned by `Font::getUtf8()`).
	 * @return Horizontal advance width in pixels to the next character position (`0` if no font or glyph is available).
	 * @author Yoon-Ki Hong
	 * @date 2015
	 * @warning None.
	 * @note None.
	 * @pre A valid `Font` must be attached via `setFont()`.
	 * @post None.
	 * @deprecated None.
	 */
	uint8_t drawChar(Position pos, uint32_t utf8) __attribute__((optimize("-O1")));

	/**
	 * @brief Renders a null-terminated UTF-8 string starting at the specified top-left position.
	 * @param[in] pos Top-left coordinate of the first character on the canvas.
	 * @param[in] str Null-terminated UTF-8 string to draw.
	 * @return Final cursor `Position` immediately following the last drawn character.
	 * @author Yoon-Ki Hong
	 * @date 2015
	 * @warning None.
	 * @note None.
	 * @pre A valid `Font` must be attached via `setFont()`.
	 * @post None.
	 * @deprecated None.
	 */
	Position drawString(Position pos, const char *str) __attribute__((optimize("-O1")));

	/**
	 * @brief Renders a null-terminated UTF-8 string aligned within the canvas bounds.
	 * @param[in] align Alignment flag (`align_t`) specifying horizontal and vertical placement on the canvas.
	 * @param[in] str   Null-terminated UTF-8 string to draw.
	 * @return Final cursor `Position` immediately following the last drawn character.
	 * @author Yoon-Ki Hong
	 * @date 2015
	 * @warning None.
	 * @note None.
	 * @pre A valid `Font` must be attached via `setFont()`.
	 * @post None.
	 * @deprecated None.
	 */
	Position drawString(align_t align, const char *str) __attribute__((optimize("-O1")));

	/**
	 * @brief Calculates the bounding `Size` (width and height) required to render a UTF-8 string with the current font.
	 * @param[in] str Null-terminated UTF-8 string to measure.
	 * @return Bounding `Size` in pixels (`0 x 0` if no font is set or the string is empty).
	 * @author Yoon-Ki Hong
	 * @date 2015
	 * @warning None.
	 * @note None.
	 * @pre None.
	 * @post None.
	 * @deprecated None.
	 */
	Size calculateStringArea(const char *str) __attribute__((optimize("-O1")));

	/**
	 * @brief Draws a bitmap image at the given canvas position.
	 * @param[in] pos    Top-left coordinate on the canvas where the bitmap will be placed.
	 * @param[in] bitmap Bitmap descriptor (`bitmap_t`) containing dimensions, pixel format, and pixel data.
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2015
	 * @warning None.
	 * @note None.
	 * @pre None.
	 * @post None.
	 * @deprecated None.
	 */
	void drawBitmap(Position pos, const bitmap_t bitmap) __attribute__((optimize("-O1")));

	/**
	 * @brief Draws a bitmap image clipped to a specific destination rectangle on the canvas.
	 * @param[in] canvasRect Destination clipping area on the canvas.
	 * @param[in] bitmapPos  Top-left coordinate of the bitmap in canvas coordinates.
	 * @param[in] bitmap     Bitmap descriptor (`bitmap_t`) to draw.
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2015
	 * @warning None.
	 * @note None.
	 * @pre None.
	 * @post None.
	 * @deprecated None.
	 */
	void drawBitmap(Area canvasRect, Position bitmapPos, const bitmap_t bitmap) __attribute__((optimize("-O1")));

	/**
	 * @brief Draws a bitmap image clipped to a destination rectangle within a canvas of the specified size.
	 * @param[in] canvasSize Explicit canvas dimensions.
	 * @param[in] canvasRect Destination clipping area on the canvas.
	 * @param[in] bitmapPos  Top-left coordinate of the bitmap in canvas coordinates.
	 * @param[in] bitmap     Bitmap descriptor (`bitmap_t`) to draw.
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2015
	 * @warning None.
	 * @note Declared as an overload prototype; use `drawBitmap(Area, Position, const bitmap_t)` or `drawBitmapBase()`.
	 * @pre None.
	 * @post None.
	 * @deprecated None.
	 */
	void drawBitmap(Size canvasSize, Area canvasRect, Position bitmapPos, const bitmap_t bitmap) __attribute__((optimize("-O1")));

	/**
	 * @brief Computes the visible portion of the source area (set by `setCalculatorSource()`) clipped to the canvas bounds.
	 * @param[in] none
	 * @return Clipped `Area` that lies strictly within the canvas.
	 * @author Yoon-Ki Hong
	 * @date 2015
	 * @warning None.
	 * @note None.
	 * @pre `setCalculatorSource()` must be called prior to invoking this method.
	 * @post Updates `mSrcOffset` with the linear pixel offset to the top-left visible source pixel.
	 * @deprecated None.
	 */
	Area calculateValidArea() __attribute__((optimize("-O1")));

	/**
	 * @brief Checks whether any edge of the source area (set by `setCalculatorSource()`) extends outside the canvas.
	 * @param[in] none
	 * @return `true` if the source area partially or completely exceeds the canvas boundaries; `false` otherwise.
	 * @author Yoon-Ki Hong
	 * @date 2015
	 * @warning None.
	 * @note None.
	 * @pre `setCalculatorSource()` must be called prior to invoking this method.
	 * @post None.
	 * @deprecated None.
	 */
	bool isOutsideCanvas() __attribute__((optimize("-O1")));

	/**
	 * @brief Initializes the internal clipping calculator with a source area on the canvas.
	 * @param[in] src Source area (position and size) to test and clip against the current canvas dimensions.
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2015
	 * @warning None.
	 * @note None.
	 * @pre None.
	 * @post Caches the source geometry and current canvas dimensions, and resets `mSrcOffset` to `0`.
	 * @deprecated None.
	 */
	void setCalculatorSource(Area src) __attribute__((optimize("-O1")));

protected:
	Font *mFont;	///< Pointer to the active `Font` used for text rendering (`nullptr` if not set).

	/**
	 * @brief Internal state variables for the canvas clipping calculator (`setCalculatorSource()` / `calculateValidArea()`).
	 *
	 * - `mSrcWidth`, `mSrcHeight`: Width and height of the source area in pixels.
	 * - `mSrcX`, `mSrcY`: Top-left coordinates of the source area on the canvas.
	 * - `mSrcOffset`: Linear pixel offset into the source buffer for the first visible pixel after clipping.
	 * - `mCanvasWidth`, `mCanvasHeight`: Cached width and height of the canvas in pixels.
	 */
	int32_t mSrcWidth, mSrcHeight, mSrcX, mSrcY, mSrcOffset, mCanvasWidth, mCanvasHeight;
	
	/**
	 * @brief Checks whether a bitmap area intersects a destination clipping area within the canvas.
	 * @param[in] canvasSize    Total dimensions of the canvas.
	 * @param[in] canvasDesArea Destination clipping area on the canvas.
	 * @param[in] bitmapArea    Bounding area of the bitmap on the canvas.
	 * @return `true` if `canvasDesArea` starts within the canvas and overlaps `bitmapArea`; `false` otherwise.
	 * @author Yoon-Ki Hong
	 * @date 2015
	 * @warning None.
	 * @note None.
	 * @pre None.
	 * @post None.
	 * @deprecated None.
	 */
	bool checkDrawingAble(Size &canvasSize, Area &canvasDesArea, Area &bitmapArea);

	/**
	 * @brief Calculates the linear pixel offset to the first visible pixel in the source bitmap buffer.
	 * @param[in] canvasSize    Total dimensions of the canvas (unused).
	 * @param[in] canvasDesArea Destination clipping area on the canvas.
	 * @param[in] bitmapArea    Bounding area of the bitmap on the canvas.
	 * @return Linear offset in pixels from the start of the bitmap buffer.
	 * @author Yoon-Ki Hong
	 * @date 2015
	 * @warning None.
	 * @note None.
	 * @pre None.
	 * @post None.
	 * @deprecated None.
	 */
	uint32_t calculateSrcFrameBufferOffset(Size &canvasSize, Area &canvasDesArea, Area &bitmapArea);

	/**
	 * @brief Calculates the clipped horizontal width of a bitmap inside a destination area.
	 * @param[in] canvasSize    Total dimensions of the canvas (unused).
	 * @param[in] canvasDesArea Destination clipping area on the canvas.
	 * @param[in] bitmapArea    Bounding area of the bitmap on the canvas.
	 * @return Visible bitmap width in pixels.
	 * @author Yoon-Ki Hong
	 * @date 2015
	 * @warning None.
	 * @note None.
	 * @pre None.
	 * @post None.
	 * @deprecated None.
	 */
	uint16_t calculateSrcWidth(Size &canvasSize, Area &canvasDesArea, Area &bitmapArea);

	/**
	 * @brief Calculates the clipped vertical height of a bitmap inside a destination area.
	 * @param[in] canvasSize    Total dimensions of the canvas (unused).
	 * @param[in] canvasDesArea Destination clipping area on the canvas.
	 * @param[in] bitmapArea    Bounding area of the bitmap on the canvas.
	 * @return Visible bitmap height in pixels.
	 * @author Yoon-Ki Hong
	 * @date 2015
	 * @warning None.
	 * @note None.
	 * @pre None.
	 * @post None.
	 * @deprecated None.
	 */
	uint16_t calculateSrcHeight(Size &canvasSize, Area &canvasDesArea, Area &bitmapArea);

	/**
	 * @brief Calculates the linear pixel offset in the canvas frame buffer where bitmap drawing starts.
	 * @param[in] canvasSize    Total dimensions of the canvas.
	 * @param[in] canvasDesArea Destination clipping area on the canvas (unused).
	 * @param[in] bitmapArea    Bounding area of the bitmap on the canvas.
	 * @return Linear offset in pixels from the start of the canvas frame buffer.
	 * @author Yoon-Ki Hong
	 * @date 2015
	 * @warning None.
	 * @note None.
	 * @pre None.
	 * @post None.
	 * @deprecated None.
	 */
	uint32_t calculateDesFrameBufferOffset(Size &canvasSize, Area &canvasDesArea, Area &bitmapArea);

	/**
	 * @brief Clips a 16-bit (2 bytes per pixel) source area against a destination clipping area.
	 * @param[in]     des         Destination clipping area.
	 * @param[in,out] src         Source area to clip; modified in-place to the overlapping region.
	 * @param[in,out] frameBuffer Pointer to the 16-bit source pixel pointer; advanced to the first visible pixel.
	 * @return `true` if `src` and `des` overlap and drawing should proceed; `false` otherwise.
	 * @author Yoon-Ki Hong
	 * @date 2015
	 * @warning None.
	 * @note None.
	 * @pre None.
	 * @post None.
	 * @deprecated None.
	 */
	bool calculate2BytesPixelDrawingInfo(Area &des, Area &src, uint16_t **frameBuffer);

	/**
	 * @brief Fills a rectangular region on the canvas with a specified color using `fillDotArray()`.
	 * @param[in] x      Top-left X coordinate of the rectangle.
	 * @param[in] y      Top-left Y coordinate of the rectangle.
	 * @param[in] width  Width of the rectangle in pixels.
	 * @param[in] height Height of the rectangle in pixels.
	 * @param[in] color  Fill color.
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2015
	 * @warning None.
	 * @note None.
	 * @pre None.
	 * @post None.
	 * @deprecated None.
	 */
	virtual void fillRectBase(int16_t x, int16_t y, uint16_t width, uint16_t height, Color color) __attribute__((optimize("-O1")));

	/**
	 * @brief Device-specific implementation for blitting a bitmap clipped to a destination area.
	 * @param[in] canvasSize       Total dimensions of the canvas.
	 * @param[in] canvasDesArea    Destination clipping area on the canvas.
	 * @param[in] bitmapDrawingPos Top-left coordinate of the bitmap on the canvas.
	 * @param[in] bitmap           Bitmap descriptor (`bitmap_t`) to draw.
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2015
	 * @warning None.
	 * @note None.
	 * @pre None.
	 * @post None.
	 * @deprecated None.
	 */
	virtual void drawBitmapBase(Size canvasSize, Area canvasDesArea, Position bitmapDrawingPos, const bitmap_t bitmap) __attribute__((optimize("-O1"))) = 0;

	/**
	 * @brief Device-specific implementation for blitting a bitmap at the given canvas position.
	 * @param[in] pos    Top-left coordinate of the bitmap on the canvas.
	 * @param[in] bitmap Bitmap descriptor (`bitmap_t`) to draw.
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2015
	 * @warning None.
	 * @note None.
	 * @pre None.
	 * @post None.
	 * @deprecated None.
	 */
	virtual void drawBitmapBase(Position pos, const bitmap_t bitmap) __attribute__((optimize("-O1"))) = 0;

	/**
	 * @brief Fills a contiguous run of pixels in the canvas buffer with the specified color.
	 * @param[in] offset Byte offset in the canvas buffer where the run begins (`pixelIndex * getPixelCapacity()`).
	 * @param[in] count  Number of consecutive pixels to fill.
	 * @param[in] color  Fill color.
	 * @return none
	 * @author Yoon-Ki Hong
	 * @date 2015
	 * @warning None.
	 * @note None.
	 * @pre None.
	 * @post None.
	 * @deprecated None.
	 */
	virtual void fillDotArray(uint32_t offset, uint32_t count, Color color) __attribute__((optimize("-O1"))) = 0;

	/**
	 * @brief Returns the storage size of a single pixel in bytes.
	 * @param[in] none
	 * @return Bytes per pixel (e.g. `2` for RGB565 or ARGB1555).
	 * @author Yoon-Ki Hong
	 * @date 2015
	 * @warning None.
	 * @note None.
	 * @pre None.
	 * @post None.
	 * @deprecated None.
	 */
	virtual uint8_t getPixelCapacity() __attribute__((optimize("-O1"))) = 0;

	/**
	 * @brief Returns the current drawable dimensions of the canvas.
	 * @param[in] none
	 * @return Canvas `Size` (width and height in pixels).
	 * @author Yoon-Ki Hong
	 * @date 2015
	 * @warning None.
	 * @note None.
	 * @pre None.
	 * @post None.
	 * @deprecated None.
	 */
	virtual Size getCanvasSize() __attribute__((optimize("-O1"))) = 0;
};

#endif
