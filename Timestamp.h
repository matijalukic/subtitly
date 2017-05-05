#ifndef _TIMESTAMP_
#define _TIMESTAMP_
#include <iostream>
#include <string>
#include <cstdlib>
#include <regex>
#include <iomanip>
#include <sstream>
using namespace std;

class ErrTimeFormatFault {

public:
	enum ErrType {STRERR, NUMSEG};
    ErrType errorType;
    ErrTimeFormatFault(ErrType error = STRERR)
		: errorType(error) {}

	friend ostream& operator<<(ostream& outputStream, const ErrTimeFormatFault& objekat) {

		if (objekat.errorType == ErrTimeFormatFault::ErrType::STRERR) {
			return outputStream << "Format vremena u stringu je pogresan!" << endl;
		}
		return outputStream << "Format vremena je van opsega" << endl;
	}
};

class Timestamp {

private:
	//unsigned int hours, minutes, seconds, miliseconds;
	double seconds;
public:
	// Konstruktori
	Timestamp() : seconds(.0) {}
	Timestamp(double);
    Timestamp(const string&) throw (ErrTimeFormatFault);

	// Getter
	double getSeconds() const;
	const string& getStringSec() const;

	// provera
	bool isBefore(const Timestamp&) const;
	bool isAfter(const Timestamp&) const;

	// relacioni
	bool operator>(const Timestamp&) const ;
	bool operator<(const Timestamp&) const;
    bool operator==(Timestamp&);
    bool operator==(Timestamp&) const;
    bool operator==(const Timestamp&) const;
	inline bool operator!=(const Timestamp&) const;

	// sabiranje i oduzimanje
	Timestamp operator+(const Timestamp&);
	Timestamp& operator+=(const Timestamp&);
	Timestamp operator-(const Timestamp&);
	Timestamp& operator-=(const Timestamp&);

	// Ispis
    string timestampStirng() const;
	friend ostream& operator<<(ostream&, const Timestamp&);
    // Sredina vremena
	friend Timestamp middle(const Timestamp&, const Timestamp&);
};
#endif
