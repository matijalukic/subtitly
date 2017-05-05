#ifndef _SRT_
#define _SRT_
#include "SubtitleIO.h"
#include "Subtitles.h"
using namespace std;


class SRT : virtual public SubtitleIO{
public:
  SRT(string);

  Subtitle * readOneSubtitle() override;

};
#endif
