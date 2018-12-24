#include <stdint.h>
#include <Bitmap.h>

// Read a little-endian short word from a stream
template<typename T> inline uint16_t readShort(T &s)
{
  return (uint16_t)s.read() | (uint16_t)s.read() << 8;
}

// Read a little-endian long from a stream
template<typename T> inline uint32_t readLong(T &s)
{
  return (uint32_t)readShort(s) | (uint32_t)readShort(s) << 16;
}

enum BMP_Compression {
  BMP_NoCompression = 0,
  BMP_RLE8bpp = 1,
  BMP_RLE4bpp = 2,
  BMP_BITFIELDS = 3,
  // ... lots more methods that aren't supported :)
};

const uint8_t OFFS_DIB_HEADER = 0x0e;

// In order to support Progmem we have a wrapper class that implements the seek()
// and read() methods as inline. This allows the _displayBMP template to compile
// against a PROGMEM stored buffer, using the same source as for a BMP file object!
//
// Yes, this is kinda sorta hacky but it means we don't have separate
// code paths for PROGMEM vs SD stored BMPs. Plus we still have fast code paths for
// both.
#ifdef __AVR__
class _Progmem_wrapper {
  int base;
  int current;
public:
  inline _Progmem_wrapper(const uint8_t *base) : base((int)base),current((int)base) { }
  inline uint8_t read() { return pgm_read_byte(current++); }
  inline void read(void *buf, size_t len) {
    uint8_t *tbuf = (uint8_t *)buf;
    for(size_t i = 0; i < len; i++) // memcpy_PF not implemented in arduino's version of avrlibc
      *tbuf++ = pgm_read_byte(current++);
  }
  inline bool seek(uint32_t pos) { current=base+pos; return true; }
};
#else
// ARM version is the same, only we just treat the address like a const uint8_t*
//
// Because the ARM isn't Harvard arch this is a little inefficient, in that we're copying
// memory from one part of the address space to another sometimes. But it's not too bad as
// the implementation avoids this where possible to save RAM on the AVRs.
class _Progmem_wrapper {
  const uint8_t *base;
  const uint8_t *current;
public:
  inline _Progmem_wrapper(const uint8_t *base) : base(base),current(base) { }
  inline uint8_t read() { return *current++; }
  inline void read(void *buf, size_t len) { memcpy(buf, current, len); current += len; }
  inline boolean seek(uint32_t pos) { current=base+pos; return true; }
};

#endif



BMP_Status Bitmap::load(const uint8_t *pgm_addr) {
  _Progmem_wrapper wrapper(pgm_addr);
  return _load(wrapper);
}


template<typename SourceType> BMP_Status Bitmap::_load(SourceType &source)
{
  SourceType &f = source;
  f.seek(0);

  // File header, check magic number 'BM'
  if(f.read() != 'B' || f.read() != 'M')
    return BMP_INVALID_FORMAT;

  // Read DIB header with image properties
  f.seek(OFFS_DIB_HEADER);
  uint16_t dib_headersize = readLong(f);
  //uint16_t width, height, bpp, compression;


  bool v2header = (dib_headersize == 12); // BMPv2 header, no compression, no additional options
  if(v2header) {
    width = readShort(f);
    height = readShort(f);
    if(readShort(f) != 1)
      return BMP_UNSUPPORTED_HEADER;
    bpp = readShort(f);
    compression = BMP_NoCompression;
  }
  else {
    width = readLong(f);
    height = readLong(f);
    if(readShort(f) != 1)
      return BMP_UNSUPPORTED_HEADER;
    bpp = readShort(f);
    compression = readLong(f);
  }

  // Verify image properties from header
  if(bpp > 24)
    return BMP_TOO_MANY_COLOURS;
  if(bpp != 1 && bpp != 4 && bpp != 8 && bpp != 16 && bpp != 24)
    return BMP_UNSUPPORTED_COLOURS;

  if(!(compression == BMP_NoCompression
       || (compression == BMP_BITFIELDS && bpp == 16))) {
    return BMP_COMPRESSION_NOT_SUPPORTED;
  }

  // In case of the bitfields option, determine the pixel format. We support RGB565 & RGB555 only
  rgb565 = true;
  if(compression == BMP_BITFIELDS) {
    f.seek(0x36);
    uint16_t b = readLong(f);
    uint16_t g = readLong(f);
    uint16_t r = readLong(f);
    if(r == 0x001f && g == 0x07e0 && b == 0xf800)
      rgb565 = true;
    else if(r != 0x001f && g != 0x03e0 && b != 0x7c00)
      return BMP_UNSUPPORTED_COLOURS; // Not RGB555 either
  }

  // Find the starting offset for the data in the first row
  f.seek(0x0a);
  data_offs = readLong(f);

  // Calculate the width in bits of each row (rounded up to nearest byte)
  row_bits = (width*bpp + 7) & ~7;
  // Calculate width in bytes (4-byte boundary aligned)
  row_bytes = (row_bits/8 + 3) & ~3;

#ifdef HOOP_DEBUG
    Serial.print(F("BPP: "));
    Serial.println(bpp);
#endif

  if(bpp < 16) {
    uint16_t palette_size = 1<<bpp;
    palette = (CRGB*)malloc(sizeof(CRGB)*palette_size);
    f.seek(OFFS_DIB_HEADER + dib_headersize);
    for(uint16_t i = 0; i < palette_size; i++) {
      uint8_t pal[4];
      f.read(pal, v2header ? 3 : 4);
      palette[i].blue = pal[0]; // >> 3;
      palette[i].green = pal[1]; // >> 2;
      palette[i].red = pal[2]; // >> 3;
    }
  }
  loaded = true;
  return BMP_OK;
}

void Bitmap::next() {
  free(palette);
  loaded = false;
}

void Bitmap::displayRow(const uint8_t *pgm_addr, uint8_t tick) {
  if (!loaded) {
    if (load(pgm_addr) != BMP_OK) { return; };
  }
  _Progmem_wrapper wrapper(pgm_addr);
  return _displayRow(wrapper, tick);
}

template<typename SourceType> void Bitmap::_displayRow(SourceType &source, uint8_t tick)
{
  SourceType &f = source;
  CRGB* pixels = FastLED.leds();
  int num_pixels = FastLED.size();
  uint16_t row = tick % height;

  // Seek to the current row
  f.seek(data_offs + (row*row_bytes));

  if(bpp > 15) {
    if(bpp == 24) {
      for(uint16_t col = 0; col < width; col++) {
        CRGB px;
        px.blue = f.read();
        px.green = f.read();
        px.red = f.read();

        int r = (num_pixels/width) + 1;
        for (int i=0; i <r; i++) {
          if ((width*i)+col < num_pixels) { pixels[(width*i)+col] = px; };
        }

      }
    }
  }
  else if(bpp == 8) {
      uint8_t buf[width];
      f.read(&buf, sizeof(buf));

      for(uint16_t col = 0; col < width; col++) {
        int r = (num_pixels/width) + 1;
        for (int i=0; i <r; i++) {
          if ((width*i)+col < num_pixels) { pixels[(width*i)+col] = palette[buf[col]]; };
        }
      }

    }
  else if(bpp == 4) {
    uint8_t buf[(width+1)/2];
    f.read(&buf, sizeof(buf));
    for(uint16_t col = 0; col < width/2; col++) {
      CRGB px, px2;
      px = palette[buf[col] >> 4];
      px2 = palette[buf[col] & 0x0F];

      int r = (num_pixels/width) + 1;
      for (int i=0; i <r; i++) {
        if ((width*i)+(col*2) < num_pixels) { pixels[(width*i)+(col*2)] = px; };
        if ((width*i)+(col*2)+1 < num_pixels) { pixels[(width*i)+(col*2)+1] = px; };
      }
    }
    if(width % 2) { // Odd width, last pixel comes from here
      CRGB px;
      px = palette[buf[sizeof(buf)-1] >> 4];
    }
  }
  else if(bpp == 1) {
    const size_t row_bytes = (width+7)/8;
    uint8_t buf[row_bytes];
    f.read(&buf, row_bytes);

    uint8_t bit = 1<<7;
    uint8_t* byte = buf;
    for(uint16_t col = 0; col < width; col++) {

      int r = (num_pixels/width) + 1;
      for (int i=0; i <r; i++) {
        uint8_t p = *byte & bit ? palette[1] : palette[0];
        if ((width*i)+col < num_pixels) { pixels[(width*i)+col] = p; };
      }

      bit >>= 1;
      if(bit == 0) {
        bit = 1<<7;
        byte++;
      }
    }

  }
  return;
};
