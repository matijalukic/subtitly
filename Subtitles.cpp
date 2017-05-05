#include "Subtitles.h"

// Konstruktori
Subtitles::Subtitles(const Subtitle& firstSubtitle){
  titles.push_back(firstSubtitle);
}


// Ispis titlova
ostream&
operator<<(ostream& outputStream, Subtitles& subtitle ){
	int i = 1;
    for (Subtitle& title : subtitle.titles){
        outputStream << i++ << endl << title; // title u srt formatu
    }
    outputStream << endl;
    return outputStream;
}

// Dodavanje na kraj
Subtitles&
Subtitles::addBack(Subtitle& titleToAdd) throw(ErrSubtitle) {
  try{
    // Ako title nije posle poslednjeg titla bacamo gresku
    if(
      !titles.empty() &&
      titles.back().getTimeOfRemoval().isAfter(titleToAdd.getTimeOfRemoval()))
        throw ErrSubtitle();

      titles.push_back(titleToAdd);
  }catch(ErrSubtitle error){
    cout << "Neuspesno dodavanje, vreme titla nije regularno!" << endl;
  }

  return *this;
}

// Dohvatanje celog 
const vector<Subtitle>&
Subtitles::getSubtitles() const{
	return titles;
}


// dohvatanje
const Subtitle&
Subtitles::get(int index) const{
  try{

    if(index < 0 || index >= titles.size())
      throw ErrSubtitle();

    Subtitle& subtitleToReturn = const_cast<Subtitle&>(titles.at(index));
	return subtitleToReturn;
  }
  catch(ErrSubtitle err){
      cout << "Neregularan index!" << endl;
  }

  return titles.at(0);


}
// Dohvatanje bez converta
Subtitle&
Subtitles::get(int index) throw(ErrSubtitle){

    if(index < 0 || index >= titles.size())
      throw ErrSubtitle();

    Subtitle& subtitleToReturn = const_cast<Subtitle&>(titles.at(index));
    return subtitleToReturn;

}

// Obrisi sve titlove
void
Subtitles::clearTitles() {
	titles.clear();
}

// Broj titlova
int
Subtitles::titlesNumber() const {
	return titles.size();
}

// Obrisi title
void
Subtitles::erase(int indexToDelete){
    titles.erase(titles.begin() + indexToDelete);
}

// Obrisi titlove
void
Subtitles::erase(vector<int>& indexesToDelete) {
    // obrisanih
    int deleted = 0;
	// Prolazimo sve indexe koje treba da ubacimo
	for (auto indexToDelete : indexesToDelete) 
        erase(indexToDelete - deleted++);
}

void
Subtitles::erase(Timestamp & from, Timestamp & to){
    int index = 0;
    vector<int> indexesToDelete;

    // Citamo sve i ubacujemo indexe iz opsega
    for(auto& subtitle: titles){
        // ako je u opsegu
        if(!subtitle.getTimeOfAppearance().isBefore(from) // ne pocinje pre pocetnog vremena
           && subtitle.getTimeOfAppearance().isBefore(to) // pocinje pre kranjeg vremena
           && subtitle.getTimeOfRemoval().isAfter(from) // zavrsava se posle pocetnog vremena
           && !subtitle.getTimeOfRemoval().isAfter(to)) // ne zavrsava se pre krajnjeg vremena
            indexesToDelete.push_back(index); // dodajemo ga u vektor indexa
        // Ako smo presli opseg
        if(subtitle.getTimeOfAppearance().isAfter(to))
            break;
        index++;
    }

    erase(indexesToDelete);

}

// Ubaci title na mesto
void
Subtitles::insertAt(int indexToInsert, Subtitle& titleToInsert) throw(ErrSubtitle){
    if(indexToInsert < 0 || indexToInsert >= titlesNumber())
        throw ErrSubtitle();

    titles.insert(titles.begin() + indexToInsert, titleToInsert);
}

// Ubaci titlove na mesto
void
Subtitles::insertAt(vector<int>& positions, vector<Subtitle>& titlesToInserts) throw (ErrSubtitle){

		// za sve vektore
        int itemsAdded = 0;
		auto itPositions = positions.begin();
		auto itTitles = titlesToInserts.begin();
        for (;itPositions < positions.end() && itTitles < titlesToInserts.end(); ++itPositions, ++itTitles) {

            if(*itPositions < 0 || *itPositions >= titlesNumber())
                throw ErrSubtitle();

            titles.insert(titles.begin() + *itPositions, *itTitles);
		}

}


// Pronadji mesto za title
int
Subtitles::findPlace(const Subtitle& subToInsert) const{
    int start = 0, end = titlesNumber() - 1;
    int mid = (start + end) / 2;
    const Timestamp& newTimeApp = subToInsert.getTimeOfAppearance();

    bool equalTime = false;

    // Ako je vreme posle kraja vracamo - 1
    // Oznaka da dodajemo na kraj
    if(titles.back().getTimeOfAppearance().isBefore(newTimeApp))
        return -1;


    // logaritamska pretraga
    while(end - start > 1){
        if(titles.at(mid).getTimeOfAppearance().getSeconds() == newTimeApp.getSeconds()){
            equalTime = true;
            break;
        }
        else if(titles.at(mid).getTimeOfAppearance() < newTimeApp ){
            start = mid + 1;
        }
        else if(titles.at(mid).getTimeOfAppearance() > newTimeApp){
            end = mid - 1;
        }

        mid = (end + start) / 2;

    }

    // Ako nisu jednaka vremena vracamo krajnji na koji treba ubaciti
    if(!equalTime){
        // ako nije  ili ako je nula i nalazi se na pocetku
        if(end!=0 || (end == 0 && newTimeApp.isBefore(titles.at(end).getTimeOfAppearance())))
            return end;
        return end + 1;
    }
    // dok vreme uklanjanja nije pre mida ako ima vise istih sa vise razlicith vremena odjava
    while(!subToInsert.getTimeOfRemoval().isBefore(titles.at(mid).getTimeOfRemoval()))
        mid++;

    // ako smo do kraja dosli
    if(mid + 1 == titlesNumber())
        return -1;
// bug kad do kraja dodjemo
    return mid;

}

// Pronadji title sa vremenom
int
Subtitles::jumpTo(Timestamp& jumpTime) const{

    int index = 0;

    for(auto& sub: titles){
        if(!sub.getTimeOfAppearance().isBefore(jumpTime))
            break;

        index++;
    }

    return index;
}
