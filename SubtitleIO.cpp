#include "SubtitleIO.h"
double SubtitleIO::framesPerSecond = 25.;

SubtitleIO::SubtitleIO(string fileName) {

    loadedSubtitle.open(fileName, ios_base::in | ios_base::out | ios_base::app);

	if (!loadedSubtitle.is_open())
		throw IOError();

}

SubtitleIO::~SubtitleIO() {
	loadedSubtitle.close();
}

// pre nego sto otvorimo
void SubtitleIO::premakeSubtitles(Subtitles& tempSubtitles) {
	
	tempSubtitles.clearTitles();
	
	if (!loadedSubtitle.is_open())
		throw IOError();
}

// Pravimo
// pravljenje i parsiranje tilova
void SubtitleIO::makeSubtitle(Subtitles& tempSubtitles) {

	premakeSubtitles(tempSubtitles);


	// sve dok nije kraj fajla
	while (!loadedSubtitle.eof()) {
		// Procitamo novi title
		Subtitle * newSubtitle = readOneSubtitle();
		if (newSubtitle)
			tempSubtitles.addBack(*newSubtitle);
		else
			failedReads++;

	}


}


// Funkcija MicroDVD sa streamom
ostream& makeMPlayerFile(ostream& outputStream, Subtitle& titleToConvert, double& prevLastTime){

    // Dohvatanje i update
    double timeAppearance = titleToConvert.getTimeOfAppearance().getSeconds() - prevLastTime;

    double timeLasts =  titleToConvert.getTimeOfRemoval().getSeconds() - titleToConvert.getTimeOfAppearance().getSeconds() ;
    prevLastTime = titleToConvert.getTimeOfRemoval().getSeconds();

    // dodajemo
    outputStream << timeAppearance << " " << timeLasts << endl;
    outputStream << titleToConvert.getString() << endl;
    return outputStream;
}

// MPlayer sa streamom
ostream& makeMicroDVDFile(ostream& outputStream, Subtitle& titleToConvert){

    int fps = SubtitleIO::framesPerSecond;
    int frameAppearance = (int)(fps * titleToConvert.getTimeOfAppearance().getSeconds() + 0.5);
    int frameRemoval = (int)(fps * titleToConvert.getTimeOfRemoval().getSeconds() + 0.5);

    outputStream << "{" << frameAppearance << "}";
    outputStream << "{" << frameRemoval<< "}";

    // dohvatamo i preuredjujemo u format za fajl
    string titleText = titleToConvert.getString();
    replace(titleText.begin(), titleText.end(), '\n', '|');

    outputStream << titleText << endl;

    return outputStream;
}


// Dohvatamo broj neprocitanih
int inline SubtitleIO::getFailedReads() const {
	return failedReads;
}
// Za cuvanje titlova
void SubtitleIO::saveTitle(string fileName, const Subtitles& subtitlesToSave, SubtitleType type) throw (IOError) {

	// uzimamo format
	string format = fileName.substr(fileName.find_last_of(".") + 1);
	transform(format.begin(), format.end(), format.begin(), ::tolower);

	bool formatError = false;

    // test
    loadedSubtitle.close();
    loadedSubtitle.open(fileName, ios_base::in | ios_base::out | ios_base::trunc);

        switch (type) {
        case SubtitleIO::SubRip:
            // Ako se ne slaze format sa tipom
            if (format != "srt"){
                formatError = true;
                break;
            }

            // Ubacujemo u fajl
            loadedSubtitle << const_cast<Subtitles&>(subtitlesToSave);

            break;

        case SubtitleIO::MicroDVD:
            if (format != "sub" && format != "txt")
            {
                formatError = true;
                break;
            }

            // upisujemo u ovom formatu u fajl
            for (const Subtitle& tempTitle : subtitlesToSave.getSubtitles()) {

                //loadedSubtitle << SubtitleIO::makeMicroDVD(const_cast<Subtitle&>(tempTitle));
                makeMicroDVDFile(loadedSubtitle, const_cast<Subtitle&>(tempTitle));
            }

            break;

        case SubtitleIO::MPlayer:
            if (format != "sub")
            {
                formatError = true;
                break;
            }

            // upisujemo u MPlayer formatu
            // upisujemo u ovom formatu u fajl
            double startTime = 0;
            for (const Subtitle& tempTitle : subtitlesToSave.getSubtitles()) {
                //loadedSubtitle << SubtitleIO::makeMPlayer(const_cast<Subtitle&>(tempTitle), startTime);
                makeMPlayerFile(loadedSubtitle, const_cast<Subtitle&>(tempTitle), startTime);
            }
            break;
        }

        // Pogresan format
        if (formatError){
            throw IOError();
        }

    loadedSubtitle.close();
    loadedSubtitle.open(fileName, ios_base::in | ios_base::out | ios_base::app);

}

// Cuvamo kao novi
void SubtitleIO::saveTitleAs(string fileName, const Subtitles& subtitlesToSave, SubtitleType type) throw (IOError) {

    // uzimamo format
    string format = fileName.substr(fileName.find_last_of(".") + 1);
    transform(format.begin(), format.end(), format.begin(), ::tolower);

    bool formatError = false;

    ofstream tempFile;
    // test
    tempFile.open(fileName, ios_base::in | ios_base::out | ios_base::trunc);

    if(tempFile.is_open()){
        switch (type) {
        case SubtitleIO::SubRip:
            // Ako se ne slaze format sa tipom
            if (format != "srt"){
                formatError = true;
                break;
            }

            // Ubacujemo u fajl
            tempFile << const_cast<Subtitles&>(subtitlesToSave);

            break;

        case SubtitleIO::MicroDVD:
            if (format != "sub" && format != "txt")
            {
                formatError = true;
                break;
            }

            // upisujemo u ovom formatu u fajl
            for (const Subtitle& tempTitle : subtitlesToSave.getSubtitles()) {

                //tempFile << SubtitleIO::makeMicroDVD(const_cast<Subtitle&>(tempTitle));
                makeMicroDVDFile(tempFile, const_cast<Subtitle&>(tempTitle));

            }

            break;

        case SubtitleIO::MPlayer:
            if (format != "sub")
            {
                formatError = true;
                break;
            }

            // upisujemo u MPlayer formatu
            // upisujemo u ovom formatu u fajl
            double startTime = 0;
            for (const Subtitle& tempTitle : subtitlesToSave.getSubtitles()) {

                //tempFile << SubtitleIO::makeMPlayer(const_cast<Subtitle&>(tempTitle), startTime);
                makeMPlayerFile(tempFile, const_cast<Subtitle&>(tempTitle), startTime);
            }
            break;
        }

        // Pogresan format
        if (formatError){
            throw IOError();
        }

    tempFile.close();

    }
    else
        throw IOError();

}

// Kovertujemo MicroDVD
string SubtitleIO::makeMicroDVD(Subtitle& titleToConvert) {

	string toReturn = "";
    int fps = SubtitleIO::framesPerSecond;
    int frameAppearance = (int)(fps * titleToConvert.getTimeOfAppearance().getSeconds() + 0.5);
    int frameRemoval = (int)(fps * titleToConvert.getTimeOfRemoval().getSeconds() + 0.5);

    toReturn.append("{" + to_string(frameAppearance) + "}");
    toReturn.append("{" + to_string(frameRemoval) + "}");

	// dohvatamo i preuredjujemo u format za fajl
	string titleText = titleToConvert.getString();
	replace(titleText.begin(), titleText.end(), '\n', '|');

	toReturn.append(titleText + "\n");

	return toReturn;
}

// Konvertujemo u MPlayer
string SubtitleIO::makeMPlayer(Subtitle& subtitleToConvert, double& prevLastTime) {
	string toReturn = "";
	// Dohvatanje i update
	double timeAppearance = subtitleToConvert.getTimeOfAppearance().getSeconds() - prevLastTime;

    double timeLasts =  subtitleToConvert.getTimeOfRemoval().getSeconds() - subtitleToConvert.getTimeOfAppearance().getSeconds() ;
    prevLastTime = subtitleToConvert.getTimeOfRemoval().getSeconds();

	// dodajemo
    toReturn.append(to_string(timeAppearance)).append(" ").append(to_string(timeLasts)).append("\n");
    toReturn.append(subtitleToConvert.getString()).append("\n");

	return toReturn;
}


