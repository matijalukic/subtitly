#include "Subtitle.h"

// Konstruktori
Subtitle::Subtitle(Timestamp appearance, Timestamp removal, string title){
  timeAppearance = appearance;
  // ako je nelegalno 2 sekunde prevoda dodajemo
  const Timestamp twoSeconds(2);
  timeRemoval = removal;
  
  if (removal.isBefore(timeAppearance))
	  timeRemoval += twoSeconds;
  
  titleText = title;
  valid = true;
}
Subtitle::Subtitle(){
  timeAppearance = Timestamp();
  timeRemoval = Timestamp();
  titleText = "";
  valid = true;
}

// dohvatanje vremena
const Timestamp& Subtitle::getTimeOfAppearance() const{
  return timeAppearance;
}
const Timestamp& Subtitle::getTimeOfRemoval() const{
  return timeRemoval;
}

// dohvatanje stringa
const string& Subtitle::getString() const{
  return titleText;
}


// Postavljanje vremena
Subtitle& Subtitle::setTimeOfRemoval(const Timestamp& newTimeOfRemoval){
  try{
    if(!newTimeOfRemoval.isAfter(timeAppearance))
      throw ErrSubtitle();
    timeRemoval = newTimeOfRemoval;

  }catch(ErrSubtitle error){
    cout << "Neregularno vreme titla uklanjanja!" << endl;
  }
 return *this;
}
Subtitle& Subtitle::setTimeOfAppearance(const Timestamp& newTimeOfAppearance){
  try{

    if(!newTimeOfAppearance.isBefore(timeRemoval))
      throw ErrSubtitle();
    timeAppearance = newTimeOfAppearance;

  }
  catch(ErrSubtitle error){
      cout << "Neregularno vreme titla pojavljivanja." << endl;
  }

  return *this;
}
// Postavljanje novog stringa
Subtitle& Subtitle::setString(const string& newText){
  titleText = newText;
  return *this;
}

// Mergovanje titleova
Subtitle& Subtitle::merge(Subtitle& secondToMerge){
	// samo ako je validan
	if (valid) {
		string newTitleText = "";
		// ako je prvi pre drugog
		if (getTimeOfAppearance().isBefore(secondToMerge.getTimeOfAppearance()))
		{
			setTimeOfRemoval(secondToMerge.getTimeOfRemoval());
			newTitleText.append(getString());
			newTitleText.append(secondToMerge.getString());
		}
		// drugi je pre prvog
		else
		{
			setTimeOfAppearance(secondToMerge.getTimeOfAppearance());
			newTitleText.append(secondToMerge.getString());
			newTitleText.append(getString());
		}

		setString(newTitleText);
	}
  return *this;
}


// Ispis subtitle
ostream& operator<<(ostream& outputStream, const Subtitle& titleToShow) {

	// Ispis u formatu .srt
	/*
		00:01:54,420 --> 00:01:56,490
		- Captain.
		- Yes, sir?

	*/
    string titleString = const_cast<string&>(titleToShow.getString());

    outputStream << titleToShow.getTimeOfAppearance() << " --> " << titleToShow.getTimeOfRemoval() << endl;

    // Ako krajnji nema novi red
    if(titleString.substr(titleString.length() - 1) != "\n")
        outputStream << titleString << endl << endl;
    else if(titleString.substr(titleString.length() - 2, 1) != "\n")
        outputStream << titleString << endl;
    // Ako nema ni jedan novi red na kraju
    else


	return outputStream;
}

// Deljenje titlova
Subtitle& Subtitle::split() {
	try {

		// Izracunavamo vreme sredine
		Timestamp middleTime = middle(timeAppearance, timeRemoval);

		// Delimo string na 2 
		int splitLinePos, splitPos;
		if ((splitLinePos = titleText.find('\n')) != string::npos)
			splitPos = splitLinePos;
		else
			splitPos = titleText.length() / 2;

		// pomeramo sredinu do prvog razmaka ako nema novog reda
		while (splitLinePos == string::npos && titleText[splitPos++] != ' ');

		// delimo stirng
		string currentTitleText, nextTitleText;
		currentTitleText = titleText.substr(0, splitPos);
		nextTitleText = titleText.substr(splitPos + 1);

		// ako smo neuspesno splitovali
		if (nextTitleText.empty()) throw ErrSubtitle();

		// Stvaramo sledeci
		Subtitle * nextSubtitle = new Subtitle(middleTime, timeRemoval, nextTitleText);

		// updatujemo trenutni
		timeRemoval = middleTime;
		titleText = currentTitleText;

		return *nextSubtitle;
	}
	catch (ErrSubtitle err) {
		throw ErrSubtitle();
	}

}
