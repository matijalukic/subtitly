#include "MPlayer.h"

MPlayer::MPlayer(string fileName)
	:SubtitleIO(fileName){
	diffTime = 0.;
    type = SubtitleIO::MPlayer;
	try
	{
		// provera tipa
		if (!(fileName.substr(fileName.find_last_of(".") + 1) == "sub"))
			throw IOError();
	}
	catch (IOError err)
	{
		cout << "File type error" << endl;
	}


}

Subtitle * MPlayer::readOneSubtitle() {

	string timesLine, titleText;
	getline(loadedSubtitle, timesLine);

	regex timeSplit("(\\d+.{0,1}\\d*)[\\s](\\d+.{0,1}\\d*)");
	smatch timesRegex;

	regex_match(timesLine, timesRegex, timeSplit);

	// Ako smo podelili na manje od 2 string
	if (timesRegex.size() < 2)
		return nullptr;

	// vremena
	double timeAppearance, timeRemoval;

	// dodajemo procitani 
	diffTime += stod(timesRegex[1]);
	// belezimo
	timeAppearance = diffTime;

	// dodajemo drugi procitani
	diffTime += stod(timesRegex[2]);
	timeRemoval = diffTime;


	string textToAppend;
	getline(loadedSubtitle, textToAppend);

	// dok nije prazan
	while (!textToAppend.empty()) {
		titleText.append(textToAppend).append("\n");
		getline(loadedSubtitle, textToAppend);
	}

	// prolazimo sve prazne redove
	while (!isdigit(loadedSubtitle.peek()) && textToAppend.empty()) {
		if (loadedSubtitle.eof())
			break;
		getline(loadedSubtitle, textToAppend);
	}


	return new Subtitle(Timestamp(timeAppearance), Timestamp(timeRemoval), titleText);


}

