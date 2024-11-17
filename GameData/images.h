#ifndef _SFG_IMAGES_H
#define _SFG_IMAGES_H

#define SFG_TEXTURE_SIZE 32

#define SFG_TEXTURE_STORE_SIZE (16 + (SFG_TEXTURE_SIZE * SFG_TEXTURE_SIZE) / 2)

/**
  Color index which will in textures and sprites be considered transparent.
*/
#define SFG_TRANSPARENT_COLOR 175

/**
  Special index of an implicit texture that consists of only transparent pixels.
*/
#define SFG_TRANSPARENT_TEXTURE 255

static inline uint8_t SFG_getTexel(const uint8_t *texture, uint8_t x, uint8_t y)
{
  x &= 0x1f;
  y &= 0x1f;

  return SFG_PROGRAM_MEMORY_U8(texture +
    ((SFG_PROGRAM_MEMORY_U8(texture + 16 + (x * SFG_TEXTURE_SIZE + y) / 2) >> 
     (4 * (y % 2 == 0))) & 0x0f));
}

#define SFG_WALL_TEXTURE_COUNT 16
#define SFG_ITEM_TEXTURE_COUNT 13
#define SFG_BACKGROUND_TEXTURE_COUNT 3
#define SFG_WEAPON_TEXTURE_COUNT 6
#define SFG_EFFECT_TEXTURE_COUNT 4
#define SFG_ENEMY_TEXTURE_COUNT 23


uint8_t SFG_loadTexturesFromFile(uint8_t* buffer, const char* location, size_t size)
{         
    FILE* file = fopen(location, "rb");
    if (file != NULL)
    {
        fread(buffer, size, 1, file);
        fclose(file);

        return 1;
    }

    return 0;
}

uint8_t SFG_wallTextures[SFG_WALL_TEXTURE_COUNT * SFG_TEXTURE_STORE_SIZE];

uint8_t SFG_itemSprites[SFG_ITEM_TEXTURE_COUNT * SFG_TEXTURE_STORE_SIZE]; 

uint8_t SFG_backgroundImages[SFG_BACKGROUND_TEXTURE_COUNT * SFG_TEXTURE_STORE_SIZE];

uint8_t SFG_weaponImages[SFG_WEAPON_TEXTURE_COUNT * SFG_TEXTURE_STORE_SIZE];

uint8_t SFG_effectSprites[SFG_EFFECT_TEXTURE_COUNT * SFG_TEXTURE_STORE_SIZE];

uint8_t SFG_monsterSprites[SFG_ENEMY_TEXTURE_COUNT * SFG_TEXTURE_STORE_SIZE];

uint8_t SFG_logoImage[SFG_TEXTURE_STORE_SIZE];

uint8_t SFG_charToFontIndex(char c)
{
  if (c >= 'a' && c <= 'z')
    return c - 'a';

  if (c >= 'A' && c <= 'Z')
    return c - 'A';

  if (c >= '0' && c <= '9')
    return c - '0' + 31;

  switch (c)
  {
    case ' ': return 26; break;
    case '.': return 27; break;
    case ',': return 28; break;
    case '!': return 29; break;
    case '/': return 41; break;
    case '-': return 42; break;
    case '+': return 43; break;
    case '(': return 44; break;
    case ')': return 45; break;
    case '%': return 46; break;
    default:  return 30; break; // "?"
  }
}

#define SFG_FONT_CHARACTER_SIZE 4 ///< width (= height) of font char. in pixels

/**
  4x4 font, each character stored as 16 bits.
*/
static const uint16_t SFG_font[47] =
{
  0xfaf0, // 0 "A"
  0xfd70, // 1 "B"
  0x6990, // 2 "C"
  0xf960, // 3 "D"
  0xfd90, // 4 "E"
  0xfa80, // 5 "F"
  0x69b0, // 6 "G"
  0xf4f0, // 7 "H"
  0x9f90, // 8 "I"
  0x31f0, // 9 "J"
  0xf4b0, // 10 "K"
  0xf110, // 11 "L"
  0xfc4f, // 12 "M"
  0xf42f, // 13 "N"
  0x6996, // 14 "O"
  0xfae0, // 15 "P"
  0x69b7, // 16 "Q"
  0xfad0, // 17 "R"
  0x5da0, // 18 "S"
  0x8f80, // 19 "T"
  0xf1f0, // 20 "U"
  0xe1e0, // 21 "V"
  0xf32f, // 22 "W"
  0x9690, // 23 "X"
  0xc7c0, // 24 "Y"
  0xbd90, // 25 "Z"
  0x0000, // 26 " "
  0x0100, // 27 "."
  0x0300, // 28 ","
  0x0d00, // 29 "!"
  0x48b4, // 30 "?"
  0xf9f0, // 31 "0"
  0x9f10, // 32 "1"
  0xbdd0, // 33 "2"
  0x9da0, // 34 "3"
  0xe2f0, // 35 "4"
  0xdbb0, // 36 "5"
  0xfbb0, // 37 "6"
  0x8bc0, // 38 "7"
  0xfdf0, // 39 "8"
  0xddf0, // 40 "9"
  0x1680, // 41 "/"
  0x2220, // 42 "-"
  0x2720, // 43 "+"
  0x0690, // 44 "("
  0x0960, // 45 ")"
  0x9249  // 46 "%"
};

#endif // guard
