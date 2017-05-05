#ifndef _SUBTITLE_
#define _SUBTITLE_
#include "Timestamp.h"
using namespace std;

class ErrSubtitle{};
class Subtitle{

private:
  Timestamp timeAppearance, timeRemoval;
  string titleText;
  bool valid; // flag za merge
public:
  // Konstruktori
  Subtitle();
  Subtitle(Timestamp, Timestamp, string);
  Subtitle(string title) : Subtitle(Timestamp(), Timestamp(), title) {}

  // Getteri
  const Timestamp& getTimeOfAppearance() const;
  const Timestamp& getTimeOfRemoval() const;
  const string& getString() const;

  // Setteri
  Subtitle& setTimeOfRemoval(const Timestamp&);
  Subtitle& setTimeOfAppearance(const Timestamp&);
  Subtitle& setString(const string&);

  // Merge titleova
  Subtitle& merge(Subtitle&);

  // Split titlova
  Subtitle& split();
  
  // Ispis title-a 
  friend ostream& operator<<(ostream&, const Subtitle&);

  // Metode validnosti
  bool isValid() { return valid; }
  void setValid(bool v) { valid = v; }


};
#endif
