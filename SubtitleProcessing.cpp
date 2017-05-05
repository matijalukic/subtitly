#include "SubtitleProcessing.h"
// Staticke promenljive
// Minimalno znakova u titlu
int SubtitleProcessing::minimalCharatersInSubtitle = 20;
// Minimalna  razlika u vremenu izmedju titlova
Timestamp SubtitleProcessing::titleTimeDifference(2);
// Maksimalno znakova u titlu
int SubtitleProcessing::maximumCharatersInSubtitle = 40;

// Konstruktor
SubtitleProcessing::SubtitleProcessing(Subtitles& addTitleToProcess)
	: subtitleProcessed(addTitleToProcess) {}


// Spajanje titlova
void SubtitleProcessing::merge(){
	// skracumjemo ime
	auto& vectorTitles = subtitleProcessed.titles;
	// skup indexa koje brisemo iz liste titlova
	vector<int> indexesToDelete;
	int indexOfNext = 1;
	int vectorTitlesSize = vectorTitles.size();

	// uzimamo pokazivac na sledeci element

    for (vector<Subtitle>::iterator it = vectorTitles.begin(); it < vectorTitles.end() - 2; it+=2) {


		// Dohvatili sadasnji i sledeci
		Subtitle& currentSubtitle = *it;
		Subtitle& nextSubtitle = *(it+1);

		// Uslov za spajanje
		if (
			(currentSubtitle.getString().length() < minimalCharatersInSubtitle || nextSubtitle.getString().length() < minimalCharatersInSubtitle)  // ako je neki title kraci od propisanog
			&&
            (const_cast<Timestamp&>(nextSubtitle.getTimeOfAppearance()) - const_cast<Timestamp&>(currentSubtitle.getTimeOfRemoval()) < titleTimeDifference) // ako razlika izmedju titlova je manja
			) 
		{
			// spajamo i dodajemo u skup iz kojeg brisemo
			currentSubtitle.merge(nextSubtitle);

			// upisujemo 
			if (indexOfNext < vectorTitlesSize)
				indexesToDelete.push_back(indexOfNext);

		}
		
		// prelazimo na sledeci
        indexOfNext+=2;
	} // end for 
	
	// ukloni
	subtitleProcessed.erase(indexesToDelete);

}

// Deljenje titlova
void SubtitleProcessing::split() {
    // skracujemo ime
        auto& vectorTitles = subtitleProcessed.titles;

        // titles splited
        int titlesSplited = 0, indexToAdd = 1;
        vector<int> positions;
        vector<Subtitle> titlesToInsert;

        for(auto& currentTitle: vectorTitles){
            try {

                // uslov za splitovanje
                if (currentTitle.getString().length() > maximumCharatersInSubtitle)
                {
                    positions.push_back(indexToAdd + titlesSplited++);
                    titlesToInsert.push_back(currentTitle.split());
                }
            }
            catch (ErrSubtitle err) {
                cout << "Los split" << endl;
            }

            indexToAdd++;
        }

        subtitleProcessed.insertAt(positions, titlesToInsert);
}

// Pomeranje uzastopnih
void
SubtitleProcessing::splitConsecutive(Timestamp & moveBy) throw (ErrSubtitle){
    auto& selectTitles = subtitleProcessed.titles;
    vector<int> selectedIndexes;
    Timestamp minimalDiff(1000); // pretpostavimo maksimalno

    int indexOfPrev = -1;
    // Za svaki title proveravamo
    for(auto& currTitle: selectTitles){
        if(indexOfPrev > -1){
            Timestamp& currApear = const_cast<Timestamp&>(currTitle.getTimeOfAppearance());
            Timestamp& prevRemov = const_cast<Timestamp&>(selectTitles.at(indexOfPrev).getTimeOfRemoval());
            // Ako nije prvi i ako je uzastopni
            if( currTitle.getTimeOfAppearance() == selectTitles.at(indexOfPrev).getTimeOfRemoval() ){
                Timestamp end = currTitle.getTimeOfRemoval();
                Timestamp start = currTitle.getTimeOfAppearance();
                // Ako je razlika manja u vremenu
                if(end - start < minimalDiff)
                    minimalDiff = end - start;

                // Dodajemo na kraj
                selectedIndexes.push_back(indexOfPrev + 1);
            }
        }

        indexOfPrev++;
    }

    // Ako bi se napravio pogrsan format vremena
    if(minimalDiff < moveBy)
        throw ErrSubtitle();

    // Svaki selektovani pomeramo za move by
    for(int index: selectedIndexes){
        Timestamp appear = selectTitles.at(index).getTimeOfAppearance();
        selectTitles.at(index).setTimeOfAppearance(appear + moveBy);
    }

}

// Pomeranje svih
void
SubtitleProcessing::shift(Timestamp& shiftBy){
    for(Subtitle& currSub: subtitleProcessed.titles){

        Timestamp newAppear = const_cast<Timestamp&>(currSub.getTimeOfAppearance()) + shiftBy;
        Timestamp newRemova = const_cast<Timestamp&>(currSub.getTimeOfRemoval()) + shiftBy;

        currSub.setTimeOfAppearance(newAppear);
        currSub.setTimeOfRemoval(newRemova);

    }

}
