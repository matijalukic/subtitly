#ifndef _MICRO_DVD_
#define _MICRO_DVD_
#include "Subtitles.h"
#include "SubtitleIO.h"

class MicroDVD : public SubtitleIO {
public:
    MicroDVD(string) throw(IOError);

    void setFast(bool isFast) {
        if(isFast) framesPerSecond = 25.;
        else framesPerSecond = 23.976;
    }

    Subtitle * readOneSubtitle() override;
};
#endif
