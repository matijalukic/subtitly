#ifndef _SUBTITLE_APP_
#define _SUBTITLE_APP_
#include "Subtitles.h"
#include "SubtitleIO.h"
#include "SRT.h"
#include "MicroDVD.h"
#include "MPlayer.h"
#include "SubtitleProcessing.h"

using namespace std;

class SubtitleApp{
private:
    SubtitleIO* fileLoader;
    ifstream determineType;
    string subtitlePath;
public:
    Subtitles loadedTitle;

    SubtitleApp();
    SubtitleApp(const Subtitle&);
    ~SubtitleApp();

    void loadTitle(const string&);
    void saveTitle(string) throw (IOError);
    void saveTitleAs(string, SubtitleIO::SubtitleType = SubtitleIO::SubRip) throw (IOError);

    bool isMicroDVD(const string&);
    SubtitleIO::SubtitleType getType();
    const string& getSubtitleName() const;
};
#endif
