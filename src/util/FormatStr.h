//
// File:   FormatStr.h
// Author: richi
//
// Created on August 12, 2008, 6:27 PM
//

#ifndef _FORMATSTR_H
#define	_FORMATSTR_H

#include <string>
#include <sstream>
/*
//#include <tchar.h>

#ifndef TCHAR
 #define TCHAR char*
#endif
// TCHAR versions of "std::string", etc.
typedef std::basic_string<TCHAR> tstring;
typedef std::basic_ostringstream<TCHAR> tostringstream;
*/
typedef std::basic_ostringstream<char> tostringstream;

/////////////////////////////////////////////////////////////////////////=
////
// "FormatStr" class. Use this class to format a "tstring" on the fly with
// streaming arguments. It's basically the C++ answer to "sprintf()" style
// argument lists. Instead of passing a format string and variable number
// of arguments however, just construct a temporary as seen in the examples
// below and stream the same arguments you would normally pass to any
// "std::ostream" object via the << operator. The result can then be
// implicitly converted to a "tstring" via the "tstring()" operator so
// you can effectively use it wherever you would use a "tstring" (frequently
// as a "const tstring &" argument for instance - see example 2 below).
//
// Example 1
// ---------
//
// // Results in "Bill Gates is worth 50 billion dollars"
// tstring String =3D FormatStr() << _T("Bill Gates is worth ") << 50 << _T(" billion dollars");
//
// Example 2 (as above but pass to a function on the fly)
// ---------
//
// void YourFunc(const tstring &String)
// {
// // ...
// }
//
// YourFunc(FormatStr() << _T("Bill Gates is worth ") << 50 << _T(" billion dollars");
//
/////////////////////////////////////////////////////////////////////////=
////

class FormatStr
{
public:
    FormatStr() {}
    FormatStr(std::locale loc) { m_Stream.imbue(loc); }
    ////////////////////////////////////////////
    // IMPORTANT: Don't make T a reference arg
    // under VC6 (unfortunately). Some later
    // versions of VC6 have a bug and will
    // choke on it (infamous "internal compiler"
    // error).
    ////////////////////////////////////////////
    template <typename T>
    FormatStr &operator<<(const T &Whatever)
    {
        m_Stream << Whatever;
        return *this;
    }

    operator std::string () const
    {
		std::string str = m_Stream.str();
        return str;
    }
private:
    tostringstream m_Stream;
};

#endif	/* _FORMATSTR_H */

