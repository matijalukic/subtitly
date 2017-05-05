#ifndef _SUBTITLE_IO_
#define _SUBTITLE_IO_
#include "Subtitles.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class IOError {};

class SubtitleIO {

protected:


    fstream loadedSubtitle;
    // Koliko imamo lose ocitanih
    int failedReads = 0;
    // Pre nego sto otvorimo
    void premakeSubtitles(Subtitles&);
public:
    // Broj fpsova
    static double framesPerSecond;
    // Tipovi
    enum SubtitleType { SubRip, MPlayer, MicroDVD };
    SubtitleType type = SubRip;

    SubtitleIO(string);
    virtual ~SubtitleIO();

    void makeSubtitle(Subtitles&);
    // Procitaj jednu
    virtual Subtitle * readOneSubtitle() = 0;

    int getFailedReads() const;

    void saveTitle(string, const Subtitles&, SubtitleType = SubtitleType::SubRip) throw (IOError);
    void saveTitleAs(string, const Subtitles&, SubtitleType = SubtitleType::SubRip) throw (IOError);

    // Kovertovanje zasebno
    static string makeMicroDVD(Subtitle&);
    static string makeMPlayer(Subtitle&, double&);

    friend ostream& makeMicroDVDFile(ostream&, Subtitle&);
    friend ostream& makeMPlayerFile(ostream&, Subtitle&, double&);

};

#endif
