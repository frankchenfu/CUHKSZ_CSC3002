#ifndef LIB_H
#define LIB_H

#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>

/* stanford standard library */
/*
 * Function: toUpperCase
 * Usage: string s = toUpperCase(str);
 * -----------------------------------
 * Returns a new string in which all lowercase characters have been converted
 * into their uppercase equivalents.
 */
std::string toUpperCase(std::string str);
/*
 * Function: toLowerCase
 * Usage: string s = toLowerCase(str);
 * -----------------------------------
 * Returns a new string in which all uppercase characters have been converted
 * into their lowercase equivalents.
 */
std::string toLowerCase(std::string str);
/*
 * Function: integerToString
 * Usage: string s = integerToStrin
 * --------------------------------
 * Converts an integer into the cor
 * For example, calling <code>integ
 * the string <code>"123"</code>.
 */
std::string integerToString(int n);
/*
 * Function: error
 * Usage: error(msg);
 * ------------------
 * Signals an error condition in a program by throwing an
 * <code>ErrorException</code> with the specified message.
 */
void error(std::string msg);
/*
 * Function: stringToReal
 * Usage: double d = stringToReal(str);
 * ------------------------------------
 * Converts a string representing a real number into its corresponding
 * value.  If the string is not a legal floating-point number or contains
 * extraneous characters other than whitespace, <code>stringToReal</code>
 * calls <code>error</code> with an appropriate message.
 */
double stringToReal(const std::string& str);
double stringToDouble(const std::string& str);   // alias
int stringToInteger(const std::string& str);
/*
 * Function: startsWith
 * Usage: if (startsWith(str, prefix)) ...
 * ---------------------------------------
 * Returns <code>true</code> if the string <code>str</code> starts with
 * the specified prefix, which may be either a string or a character.
 */
bool startsWith(const std::string& str, char prefix);
bool startsWith(const std::string& str, const std::string& prefix);
 /*
 * Friend function: writeQuotedString
 * Usage: writeQuotedString(outfile, str, forceQuotes);
 * ----------------------------------------------------
 * Writes the string str to outfile surrounded by double quotes, converting
 * special characters to escape sequences, as necessary.  If the optional
 * parameter forceQuotes is explicitly set to false, quotes are included
 * in the output only if necessary.
 */
void writeQuotedString(std::ostream & os, const std::string & str,
         bool forceQuotes = true);
/*
 * Friend function: stringNeedsQuoting
 * Usage: if (stringNeedsQuoting(str)) ...
 * ---------------------------------------
 * Checks whether the string needs quoting in order to be read correctly.
 */
bool stringNeedsQuoting(const std::string & str);
/*
 * Friend function: writeGenericValue
 * Usage: writeGenericValue(os, value, forceQuotes);
 * -------------------------------------------------
 * Writes a generic value to the output stream.  If that value is a string,
 * this function uses writeQuotedString to write the value.
 */
template <typename ValueType>
void writeGenericValue(std::ostream & os, const ValueType & value,
         bool forceQuotes) {
 os << value;
}
template <>
inline void writeGenericValue(std::ostream & os, const std::string & value,
          bool forceQuotes) {
 writeQuotedString(os, value, forceQuotes);
}
std::string ltrim(const std::string &s);
std::string rtrim(const std::string &s);
std::string trim(const std::string &s);
// std::string toUpperCase(std::string str) {
//  transform(str.begin(), str.end(), str.begin(), ::toupper);
//  return str;
// }

// std::string toLowerCase(std::string str) {
//  transform(str.begin(), str.end(), str.begin(), ::tolower);
//  return str;
// }



#endif // LIB_H

#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
// #include "CSC3002OJActive/assignment4/lib.h"

std::string toUpperCase(std::string str) {
   for (int i = 0; i < str.length(); i++) {
        str[i] = toupper(str[i]);
   }
   return str;
}

std::string toLowerCase(std::string str) {
   for (int i = 0; i < str.length(); i++) {
        str[i] = tolower(str[i]);
   }
   return str;
}

std::string integerToString(int n) {
    std::ostringstream stream;
    stream << n;
    return stream.str();
}

std::string longToString(long n) {
    std::ostringstream stream;
    stream << n;
    return stream.str();
}

double stringToReal(const std::string& str) {
   double db {std::stod(str)};
   return db;
}

double stringToDouble(const std::string& str) {
    return stringToReal(str);
}

int stringToInteger(const std::string& str) {
    std::istringstream stream(str);
    int value;
    stream >> value >> std::ws;
    if (stream.fail() || !stream.eof()) {
        error("stringToInteger: Illegal integer format (" + str + ")");
    }
    return value;
}

bool startsWith(const std::string& str, char prefix) {
    return str.length() > 0 && str[0] == prefix;
}

bool startsWith(const std::string& str, const std::string& prefix) {
    if (str.length() < prefix.length()) return false;
    int nChars = prefix.length();
    for (int i = 0; i < nChars; i++) {
        if (str[i] != prefix[i]) return false;
    }
    return true;
}

void writeQuotedString(std::ostream & os, const std::string & str, bool forceQuotes) {
   if (!forceQuotes && stringNeedsQuoting(str)) forceQuotes = true;
   if (forceQuotes) os << '"';
   int len = str.length();
   for (int i = 0; i < len; i++) {
      char ch = str.at(i);
      switch (ch) {
       case '\a': os << "\\a"; break;
       case '\b': os << "\\b"; break;
       case '\f': os << "\\f"; break;
       case '\n': os << "\\n"; break;
       case '\r': os << "\\r"; break;
       case '\t': os << "\\t"; break;
       case '\v': os << "\\v"; break;
       case '\\': os << "\\\\"; break;
       default:
         if (isprint(ch) && ch != '"') {
            os << ch;
         } else {
            std::ostringstream oss;
            oss << std::oct << std::setw(3) << std::setfill('0') << (int(ch) & 0xFF);
            os << "\\" << oss.str();
         }
      }
   }
   if (forceQuotes) os << '"';
}

/*
 * Implementation notes: readQuotedString and writeQuotedString
 * ------------------------------------------------------------
 * Most of the work in these functions has to do with escape sequences.
 */

static const std::string STRING_DELIMITERS = ",:)}]\n";

bool stringNeedsQuoting(const std::string & str) {
   int n = str.length();
   for (int i = 0; i < n; i++) {
      char ch = str[i];
      if (isspace(ch)) return false;
      if (STRING_DELIMITERS.find(ch) != std::string::npos) return true;
   }
   return false;
}

const std::string WHITESPACE = " \n\r\t\f\v";
 
std::string ltrim(const std::string &s)
{
    size_t start = s.find_first_not_of(WHITESPACE);
    return (start == std::string::npos) ? "" : s.substr(start);
}
 
std::string rtrim(const std::string &s)
{
    size_t end = s.find_last_not_of(WHITESPACE);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}
 
std::string trim(const std::string &s) {
    return rtrim(ltrim(s));
}


void error(std::string msg) {
   std::cout << "ERROR: " << msg << std::endl;
}
