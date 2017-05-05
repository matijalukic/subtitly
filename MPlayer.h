#ifndef _MPLAYER_
#define _MPLAYER_
#include "SubtitleIO.h"
#include "Subtitles.h"

class MPlayer : public SubtitleIO{
    double diffTime;
public:
    MPlayer(string) throw(IOError);

    Subtitle * readOneSubtitle() override;

};
#endif
