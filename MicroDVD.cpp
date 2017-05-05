#include "MicroDVD.h"

MicroDVD::MicroDVD(string fileName)
	:SubtitleIO(fileName){
    type = SubtitleIO::MicroDVD;
	try
	{
		if (!(fileName.substr(fileName.find_last_of(".") + 1) == "sub") && !(fileName.substr(fileName.find_last_of(".") + 1) == "txt"))
			throw IOError();

	}
	catch (IOError err)
	{
		cout << "File type Error" << endl;
	}


}

Subtitle * MicroDVD::readOneSubtitle() {
	
	string frameAppearance, frameRemoval, titleText, wholeLine;

	getline(loadedSubtitle, wholeLine);

	if (wholeLine.empty()) return nullptr;

	regex titleSplit("\\{([0-9]+)\\}\\{([0-9]+)\\}([^\n]+)");
	smatch titleMatch;

	regex_match(wholeLine, titleMatch, titleSplit);

	frameAppearance = titleMatch[1];
	frameRemoval = titleMatch[2];

	// pretvaranje u double
	double frameAppearanceNumber = stod(frameAppearance);
	double frameRemovalNumber = stod(frameRemoval);

	// pretvaranje u sekunde
    double secondAppearance = frameAppearanceNumber / SubtitleIO::framesPerSecond;
    double secondRemoval = frameRemovalNumber / SubtitleIO::framesPerSecond;

	// dohvatanje titla
	titleText = titleMatch[3];
	replace(titleText.begin(), titleText.end(), '|', '\n');

	return new Subtitle(Timestamp(secondAppearance), Timestamp(secondRemoval), titleText);

}

