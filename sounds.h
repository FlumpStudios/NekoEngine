#ifndef _SFG_SOUNDS_H
#define _SFG_SOUNDS_H

#define SFG_SFX_SAMPLE_COUNT 2048
#define SFG_SFX_SIZE (SFG_SFX_SAMPLE_COUNT / 2)

/**
  Gets an 8bit sound sample.
*/
#define SFG_GET_SFX_SAMPLE(soundIndex,sampleIndex) \
  ((SFG_PROGRAM_MEMORY_U8(SFG_sounds + soundIndex * SFG_SFX_SIZE \
  + sampleIndex / 2) << (4 * ((sampleIndex % 2) != 0))) & 0xf0)

#define SFG_TRACK_SAMPLES (512 * 1024)
#define SFG_TRACK_COUNT 6

/**
  Average value of each music track, can be used to correct DC offset issues if
  they appear.
*/
SFG_PROGRAM_MEMORY uint8_t SFG_musicTrackAverages[SFG_TRACK_COUNT] =
  {14,7,248,148,6,8};

struct
{ // all should be initialized to 0 by default
  uint8_t track;
  uint32_t t;      // time variable/parameter
  uint32_t t2;     // stores t squared, for better performance
  uint32_t n11t;   // stores a multiple of 11, for better performance
} SFG_MusicState;

/**
  Gets the next 8bit 8KHz music sample for the bytebeat soundtrack. This
  function is to be used by the frontend that plays music.
*/
uint8_t SFG_getNextMusicSample()
{
  if (SFG_MusicState.t >= SFG_TRACK_SAMPLES)
  {
    SFG_MusicState.track++;

    if (SFG_MusicState.track >= SFG_TRACK_COUNT)
      SFG_MusicState.track = 0;

    SFG_MusicState.t = 0;
    SFG_MusicState.t2 = 0;
    SFG_MusicState.n11t = 0;
  }

  uint32_t result;

  #define S SFG_MusicState.t // can't use "T" because of a C++ template
  #define S2 SFG_MusicState.t2
  #define N11S SFG_MusicState.n11t

  /* CAREFUL! Bit shifts in any direction by amount greater than data type
     width (32) are undefined behavior. Use % 32. */

  switch (SFG_MusicState.track) // individual music tracks
  {
    case 0:
    {
      result = S * (4 + (1 ^ S >> 10 & 5)) * (2 + (3 & S >> 16)) >> (S >> 10 & 3);

      SFG_MusicState.t2 += S;

      break;
    }

    case 1:
    {
      result = (S >> 6 | (S & 32768 ? -6 * S / 7 : (S & 65536 ? -9 * S & 100 : -9 * (S & 100)) / 10));

      break;
    }

    case 2:
    {
      result = 
        ~((((S >> ((S >> 2) % 32)) | (S >> ((S >> 5) % 32))) & 0x12) << 1) 
        | (S >> 11);

      break;
    }

    case 3:
    {
      result =
        (((((S >> ((S >> 2) % 32)) + (S >> ((S >> 7) % 32)))) & 0x3f) | (S >> 5)
        | (S >> 11)) & ((S & (32768 | 8192)) ? 0xf0 : 0x30);

      break;
    }

    case 4:
    {
      result =
        ((0x47 >> ((S >> 9) % 32)) & (S >> (S % 32))) | 
        (0x57 >> ((S >> 7) % 32)) |
        (0x06 >> ((S >> ((((N11S) >> 14) & 0x0e) % 32)) % 32));

      SFG_MusicState.n11t += 11;

      break;
    }

    case 5:
    {
      uint32_t a = S >> ((S >> 6) % 32);
      uint32_t b = 0x011121 >> (((a + S) >> 11) % 32);
      result = 
        (((S >> 9) + (S ^ (S << 1))) & (0x7f >> (((S >> 15) & 0x03) % 32))) 
        & (b + a);

      break;
    }

    default:
      result = 127;
      break;
  }

  #undef S
  #undef S2
  #undef N11S

  SFG_MusicState.t += 1;

  return result;
}

/**
  Switches the bytebeat to next music track.
*/
void SFG_nextMusicTrack()
{
  uint8_t current = SFG_MusicState.track;

  while (SFG_MusicState.track == current)
    SFG_getNextMusicSample();
}

uint8_t   SFG_sfxFromFile(uint8_t* buffer, const char* location, size_t size)
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


uint8_t SFG_sounds[SFG_SFX_SIZE * 6];

#endif // guard
