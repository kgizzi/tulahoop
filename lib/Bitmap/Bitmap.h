/*

  Bitmap.h - Support for Bitmap Images

  Based on code from Freetronics FTOLED Library:
  https://github.com/freetronics/FTOLED

*/
#ifndef BITMAP_H_
#define BITMAP_H_

#include <FastLED.h>
#include "progmem_compat.h"

#ifndef __SD_H__
class File; /* Provided by SD library except sometimes on Yun, see comment at top of FTOLED_BMP.cpp */
#endif

// Status code returned by displayBMP()
enum BMP_Status {
  BMP_OK = 0,                          // Image Displayed OK
  BMP_INVALID_FORMAT = 1,              // Not a BMP file
  BMP_UNSUPPORTED_HEADER = 2,          // Has a rare BMP header format (not v2 or v3)
  BMP_TOO_MANY_COLOURS = 3,            // Is a 32bpp image (not supported)
  BMP_COMPRESSION_NOT_SUPPORTED = 4,   // Uses RLE or another unsupported compression format
  BMP_UNSUPPORTED_COLOURS = 5,         // Uses non-standard colour format
  BMP_ORIGIN_OUTSIDE_IMAGE = 6         // You've asked to crop a section that doesn't exist in the image
};


struct OLED_Colour
{
  byte red   : 5;
  byte green : 6;
  byte blue  : 5;
};

class Bitmap {

public:
  BMP_Status load(const uint8_t *pgm_addr);
  void next();
  void displayRow(const uint8_t *pgm_addr, uint8_t tick);

  uint16_t getWidth() { return width; };
  uint16_t getHeight() { return height; };
  uint16_t getDepth() { return bpp; };


protected:
  uint16_t width, height, bpp, compression;
  uint32_t data_offs;
  uint16_t row_bits, row_bytes;
  //OLED_Colour *palette;
  CRGB *palette;
  bool rgb565;

  bool loaded = false;
  //uint8_t *f;

  template<typename SourceType> BMP_Status _load(SourceType &source);
  template<typename SourceType> void _displayRow(SourceType &source, uint8_t tick);
};

#endif
