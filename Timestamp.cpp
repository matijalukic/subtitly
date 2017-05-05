#include "Timestamp.h"

// Konstruktor na osnovu double-a
Timestamp::Timestamp(double sekundi)
	: seconds(sekundi)
{}

double Timestamp::getSeconds() const { return seconds;  }
const string& Timestamp::getStringSec() const { return to_string(seconds); }
// ucitavanje na osnovu stringa
Timestamp::Timestamp(const string& newTime) throw(ErrTimeFormatFault) {
    regex formatTime("([0-9]{1,3})\:([0-9]{2})\:([0-9]{2})\,([0-9]{3})");
	smatch numbers;

    regex_match(newTime, numbers, formatTime);
		// Provera da li je validan datum
        if (numbers.size() < 3) {
			throw ErrTimeFormatFault(ErrTimeFormatFault::ErrType::STRERR);
        }

		// sati
		long sati = stoul(numbers[1]);
		// minuti
		long minuti = stoul(numbers[2]);
		// sekundi
		long sekundi = stoul(numbers[3]);
		// milisekundi
		long milisekundi = stoul(numbers[4]);

		seconds = sekundi + minuti * 60 + sati * 3600 + milisekundi*0.001;

}
// Ispis vremena
ostream& operator<<(ostream& outputStream, const Timestamp& time) {

	return outputStream 
		<< setfill('0') << setw(2) << ((int)time.seconds) / 3600 << ":"
		<< setfill('0') << setw(2) << (((int)time.seconds) / 60) % 60 << ":"
		<< setfill('0') << setw(2) << ((int)time.seconds) % 60 << ","
        << setfill('0') << setw(3) <<  ((int) (time.seconds * 1000)) % 1000;

}

// Ispis u string
string Timestamp::timestampStirng() const{
    char toReturn[12];

    int hours = ((int)seconds) / 3600;
    int minutes = ((int)seconds / 60) % 60;
    int secs = (int)seconds % 60;
    int miliseconds = ((int)(seconds * 1000)) % 1000;

    sprintf(toReturn, "%.2d:%.2d:%.2d,%.3d", hours, minutes, secs, miliseconds);

    return string(toReturn);
}


// operator >
bool Timestamp::operator>(const Timestamp& compare) const  {
	return seconds > compare.seconds;
}

// operator <
bool Timestamp::operator<(const Timestamp& compare) const  {
	return seconds < compare.seconds;
}

// da li je pre
bool Timestamp::isBefore(const Timestamp& compare) const{
		return *this < compare;
}

// da li je posle
bool Timestamp::isAfter(const Timestamp& compare) const{
		return *this > compare;
}

// da li su isti
bool Timestamp::operator==(Timestamp& compare){ // ako su do 3 decimale jednake
    return ((int) seconds * 1000) == ((int) compare.seconds * 1000);
}
bool Timestamp::operator==(Timestamp& compare) const { // ako su do 3 decimale jednake
    return ((int) seconds * 1000) == ((int) compare.seconds * 1000);
}
bool Timestamp::operator==(const Timestamp& compare) const { // ako su do 3 decimale jednake
	return ((int) seconds * 1000) == ((int) compare.seconds * 1000);
}
bool Timestamp::operator!=(const Timestamp& compare) const {
	return !(*this == compare);
}

// Sabiranje
Timestamp Timestamp::operator+(const Timestamp& add) {
	Timestamp result;
	result.seconds = seconds + add.seconds;
	return result;
}
Timestamp& Timestamp::operator+=(const Timestamp& add) { 
	*this = *this + add;  return *this; 
}

// Oduzimanje
Timestamp Timestamp::operator-(const Timestamp& sub) {
	Timestamp result;
	result.seconds = seconds - sub.seconds;
	return result;
}
Timestamp& Timestamp::operator-=(const Timestamp& sub){ 
*this = *this - sub; return *this; }

// Vraca sredinu izmedju dva vremena
Timestamp middle(const Timestamp& startTime, const Timestamp& endTime) {
	double middleSecond = (startTime.seconds + endTime.seconds) / 2;

	return Timestamp(middleSecond);
}
