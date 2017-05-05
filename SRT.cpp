#include "SRT.h"

// Konstruktor
SRT::SRT(string fileName)
  : SubtitleIO(fileName) 
{
    // changing type
    type = SubtitleIO::SubRip;
	try {
		// provera tipa
		if(!(fileName.substr(fileName.find_last_of(".") + 1) == "srt"))
			throw IOError();
		}

	catch(IOError e){
		cout << "File type error." << endl;
	}
}
Subtitle * SRT::readOneSubtitle() {

	string buffText, titleTime, titleText;
	int titleNumber;

	// prva linija broj
	getline(loadedSubtitle, buffText);
	//buffText = jumpEmptyRows;
	titleNumber = atoi(buffText.c_str()); // konvertujemo procitanu prvu liniju u broj


	loadedSubtitle.get(); // preskacemo endl

						  // vreme ulaska i izlaska
	getline(loadedSubtitle, titleTime);

	// Parsiranje vremena
	string timeAppearance, timeRemoval;
    regex timeSplit("([^\s]*) --> ([^\s]*)");
	smatch matchesResult;

	regex_match(titleTime, matchesResult, timeSplit);

	// Ako je lose procitao
	if (matchesResult.size() < 2) {
		return nullptr;
	}

	// sve dok ne naidjemo na prazan red ucitavamo i konkateniramo u string
	getline(loadedSubtitle, buffText);
	titleText.clear();
	while (!buffText.empty()) {
		titleText.append(buffText).append("\n");
		getline(loadedSubtitle, buffText);
	}

    // uklanjamo posledniji znak, novi red
    //titleText = titleText.substr(0, titleText.length() - 1);

	// prolazimo sve prazne redove do sledeceg broja
	while (!isdigit(loadedSubtitle.peek()) && buffText.empty()) {
		if (loadedSubtitle.eof())
			break;
		getline(loadedSubtitle, buffText);
	}

	timeAppearance = matchesResult[1];
	timeRemoval = matchesResult[2];

	Subtitle * resultSubtitle = new Subtitle(Timestamp(timeAppearance), Timestamp(timeRemoval), titleText);

	return resultSubtitle;

}
