#ifndef _SFG_TEXTS_H
#define _SFG_TEXTS_H

/* NOTE: We don't use SFG_PROGRAM_MEMORY because that causes issues with drawing
  text (the drawing function gets a pointer and doesn't know if it's progmem or
  RAM). On Arduino these texts will simply be stored in RAM. */

static const char *SFG_menuItemTexts[] =
{
  "continue",
  "play",
  "sound",
  "look",
  "exit"
};

#define SFG_TEXT_KILLS "kills"
#define SFG_TEXT_SAVE_PROMPT "Continue..."
#define SFG_TEXT_SAVED "saved"
#define SFG_TEXT_LEVEL_COMPLETE "level clear!"

#define SFG_VERSION_STRING "1.0"
/**<
  Version numbering is following: major.minor for stable releases,
  in-development unstable versions have the version of the latest stable +
  "d" postfix, e.g. 1.0d. This means the "d" versions can actually differ even
  if they're marked the same. */


#define MAX_STORY_SIZE 500
// TODO: Should really combine these 2 bools
static uint8_t introTextLoaded = FALSE;
static uint8_t outroTextLoaded = FALSE;
static char introText[MAX_STORY_SIZE] = "";
static char outroText[MAX_STORY_SIZE] = "";

const char introTextLocation[16] = "texts/intro.txt";
const char outroTextLocation[16] = "texts/outro.txt";

static void loadText(const char* location, char* buffer)
{
	FILE* file = fopen(location, "r");
	if (file == NULL) {
		return;
	}

	if ((fgets(buffer, MAX_STORY_SIZE, file) != NULL)) {
		fclose(file);
	}

	return;
}

char* TXT_getIntroText(void)
{
	if (introTextLoaded)
	{
		return introText;
	}

	loadText(introTextLocation, introText);
	introTextLoaded = TRUE;
	return introText;
}

char* TXT_getOutroText(void)
{
	if (outroTextLoaded)
	{
		return outroText;
	}

	loadText(outroTextLocation, outroText);
	outroTextLoaded = TRUE;
	return outroText;
}

// #define SFG_MALWARE_WARNING ""

// #if SFG_OS_IS_MALWARE
//   #define SFG_MALWARE_WARNING "MALWARE OS DETECTED"
// #endif

#endif // gaurd
