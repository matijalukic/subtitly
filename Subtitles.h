#ifndef _SUBTITLES_
#define _SUBTITLES_
#include "Subtitle.h"
#include <vector>
#include <set>
using namespace std;
typedef vector<Subtitle> SubtitleList;

class Subtitles{
	// klasa pristupa clanovima
	friend class SubtitleProcessing;
	friend class MyFrame;
private:
  vector<Subtitle> titles;

public:
  Subtitles(){}
  Subtitles(const Subtitle&);

  // Dodavanje
  Subtitles& addBack(Subtitle&);

  // dohvatanje
  const Subtitle& get(int) const;
  Subtitle& get(int);

  // Ispis
  friend ostream& operator<<(ostream&, Subtitles&);

  // Brisanje odredjenih
  void erase(int);
  void erase(vector<int>&);
  void erase(Timestamp&, Timestamp&);

  // Ciscenje
  void clearTitles();

  // Ubacivanje na mesto
  void insertAt(int, Subtitle&) throw (ErrSubtitle);
  void insertAt(vector<int>&, SubtitleList&) throw(ErrSubtitle);

  // Broj titlova
  int titlesNumber() const;

  // Pronalazimo mesto
  int findPlace(const Subtitle&) const;
  int jumpTo(Timestamp&) const;

  // Dohvatanje vektora
  const vector<Subtitle>& getSubtitles() const;

};
#endif
