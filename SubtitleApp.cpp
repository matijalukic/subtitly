#include "SubtitleApp.h"

// Konstruktor i destruktor
SubtitleApp::SubtitleApp() 
	:fileLoader(0), loadedTitle(){
    subtitlePath = "";
}
SubtitleApp::SubtitleApp(const Subtitle& firstSuibtitle)
	: fileLoader(0), loadedTitle(firstSuibtitle){
    subtitlePath = "";
}
SubtitleApp::~SubtitleApp() {
	if (fileLoader)
		delete fileLoader;
}


// Ucitavanje fajla
void SubtitleApp::loadTitle(const string& fileName) throw(IOError) {
        // Zatvaramo stari
        if(fileLoader) delete fileLoader;

        subtitlePath = fileName;
		// uzimamo format
		string format = fileName.substr(fileName.find_last_of(".") + 1);

		// PREPOZNAVANJE FORMATA
		if (format == "srt")
			fileLoader = new SRT(fileName);
		else if (format == "sub")
			if (isMicroDVD(fileName))
				fileLoader = new MicroDVD(fileName);
			else
				fileLoader = new MPlayer(fileName);
		else if (format == "txt")
			fileLoader = new MicroDVD(fileName);
		else
			throw IOError();
		
		// pravimo title
		fileLoader->makeSubtitle(loadedTitle);



}

// Provera tipa na osnovu sadrzaja
bool SubtitleApp::isMicroDVD(const string& fileName) throw (IOError) {

	// otvaramo
	determineType.open(fileName);

	// provera da li je otvoreno
	if (!determineType.is_open())
		throw IOError();

	char firstChar = determineType.peek();

	determineType.close();
	// ako pocinje sa vititcastom onda jeste
	return firstChar == '{';


}

// Subtitle save
void SubtitleApp::saveTitle(string fileName) throw(IOError) {
    // Ako ne posotji fileLoader
    if(!fileLoader){
        string format = fileName.substr(fileName.find_last_of(".") + 1);
        transform(format.begin(), format.end(), format.begin(), ::tolower);

        // PREPOZNAVANJE FORMATA
        if (format == "srt")
            fileLoader = new SRT(fileName);
        else if (format == "sub")
            if (isMicroDVD(fileName))
                fileLoader = new MicroDVD(fileName);
            else
                fileLoader = new MPlayer(fileName);
        else if (format == "txt")
            fileLoader = new MicroDVD(fileName);
        else
            throw IOError();

    }

    SubtitleIO::SubtitleType typeErr = fileLoader->type;
    // Ako imamo ista titlova
    if (loadedTitle.titlesNumber() > 0)
        // Ako imaju ista imena
        if(fileName == subtitlePath)
            fileLoader->saveTitle(subtitlePath, loadedTitle, typeErr);
        else
            fileLoader->saveTitleAs(fileName, loadedTitle, typeErr);
    else
		throw IOError();
}
void SubtitleApp::saveTitleAs(string fileName, SubtitleIO::SubtitleType type) throw(IOError){
    // Ako imamo ista titlova
    if (loadedTitle.titlesNumber() > 0)
        // Ako imaju ista imena
        if(fileName == subtitlePath)
            fileLoader->saveTitle(subtitlePath, loadedTitle, type);
        else
            fileLoader->saveTitleAs(fileName, loadedTitle, type);
    else
        throw IOError();
}

// Dohvatamo tip
SubtitleIO::SubtitleType SubtitleApp::getType(){
    return fileLoader->type;
}
const string& SubtitleApp::getSubtitleName() const{
    return subtitlePath;
}
