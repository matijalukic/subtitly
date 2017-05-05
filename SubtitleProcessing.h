#ifndef _SUBTITLE_PROCESS_
#define _SUBTITLE_PROCESS_
#include "Subtitles.h"
#include <algorithm>
#include <iterator>
using namespace std;

class ErrProcess {};

class SubtitleProcessing {

public:
    Subtitles& subtitleProcessed;

    SubtitleProcessing(Subtitles&);
    SubtitleProcessing(const SubtitleProcessing&) = delete;
    SubtitleProcessing(SubtitleProcessing&&) = delete;

    // Uslovi za procese
    static int minimalCharatersInSubtitle;
    static Timestamp titleTimeDifference;
    static int maximumCharatersInSubtitle;

    // Spajanje
    void merge();

    // Deljenje ako ima vise znakova
    void split();

    // Pomeranje uzastopnih
    void splitConsecutive(Timestamp&) throw(ErrSubtitle);

    // Shiftovanje
    void shift(Timestamp&);
};
#endif
