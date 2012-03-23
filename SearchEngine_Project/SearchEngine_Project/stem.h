
/*******************************   stem.h   ***********************************

   Purpose:    Implementation of the Porter stemming algorithm documented 
               in: Porter, M.F., "An Algorithm For Suffix Stripping," 
               Program 14 (3), July 1980, pp. 130-137.

   Provenance: Written by B. Frakes and C. Cox, 1986.
               Changed by C. Fox, 1990.
                  - made measure function a DFA
                  - restructured structs
                  - renamed functions and variables
                  - restricted function and variable scopes
               Changed by C. Fox, July, 1991.
                  - added ANSI C declarations 
                  - branch tested to 90% coverage

   Notes:      This code will make little sense without the the Porter
               article.  The stemming function converts its input to
               lower case.
**/

/************************   Standard Include Files   *************************/

#ifndef _STEMMER_
#define _STEMMER_

#define _StemSuffix "_"
#define _StemSuffixLen 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <string>

using namespace std;
#include<map>
typedef std::map<std::string,int> dic;
/*****************************************************************************/
/*****************   Private Defines and Data Structures   *******************/

#define _FALSE                         0
#define _TRUE                          1
#define EOS                         '\0'

#define IsVowel(c)        ('a'==(c)||'e'==(c)||'i'==(c)||'o'==(c)||'u'==(c))

#define _LAMBDA ""

//char * _LAMBDA = "";

//int ContainsVowel(char  * w);
//int RemoveAnE(char * w);
//int AddAnE(char * w);

#define _ContainsVowel 1
#define _RemoveAnE 2
#define _AddAnE 3


typedef struct {
           int id;                 /* returned if rule fired */
           char *old_end;          /* suffix replaced */
           char *new_end;          /* suffix replacement */
           int old_offset;         /* from end of word to start of suffix */
           int new_offset;         /* from beginning to end of new suffix */
           int min_root_size;      /* min root word size for replacement */
	   int func;		   /* call which function */
           /*int (*condition)(char * w);  /* the replacement test function*/
           } RuleList;


/******************************************************************************/
/*****************   Initialized Private Data Structures   ********************/

// #define _NULL NULL
#define _NULL 0

static RuleList step1a_rules[] =
           {
             101,  "sses",      "ss",    3,  1, -1,  _NULL,
             102,  "ies",       "i",     2,  0, -1,  _NULL,
             103,  "ss",        "ss",    1,  1, -1,  _NULL,
             104,  "s",         _LAMBDA,  0, -1, -1,  _NULL,
             000,  NULL,        NULL,    0,  0,  0,  _NULL,
           };

static RuleList step1b_rules[] =
           {
             105,  "eed",       "ee",    2,  1,  0,  _NULL,
             106,  "ed",        _LAMBDA,  1, -1, -1,  _ContainsVowel,
             107,  "ing",       _LAMBDA,  2, -1, -1,  _ContainsVowel,
             000,  NULL,        NULL,    0,  0,  0,  _NULL,
           };

static RuleList step1b1_rules[] =
           {
             108,  "at",        "ate",   1,  2, -1,  _NULL,
             109,  "bl",        "ble",   1,  2, -1,  _NULL,
             110,  "iz",        "ize",   1,  2, -1,  _NULL,
             111,  "bb",        "b",     1,  0, -1,  _NULL,
             112,  "dd",        "d",     1,  0, -1,  _NULL,
             113,  "ff",        "f",     1,  0, -1,  _NULL,
             114,  "gg",        "g",     1,  0, -1,  _NULL,
             115,  "mm",        "m",     1,  0, -1,  _NULL,
             116,  "nn",        "n",     1,  0, -1,  _NULL,
             117,  "pp",        "p",     1,  0, -1,  _NULL,
             118,  "rr",        "r",     1,  0, -1,  _NULL,
             119,  "tt",        "t",     1,  0, -1,  _NULL,
             120,  "ww",        "w",     1,  0, -1,  _NULL,
             121,  "xx",        "x",     1,  0, -1,  _NULL,
             122,  _LAMBDA,      "e",    -1,  0, -1,  _AddAnE,
             000,  NULL,        NULL,    0,  0,  0,  _NULL,
             };

static RuleList step1c_rules[] =
           {
             123,  "y",         "i",      0,  0, -1,  _ContainsVowel,
             000,  NULL,        NULL,    0,  0,  0,  _NULL,
           };

static RuleList step2_rules[] =
           {
             203,  "ational",   "ate",   6,  2,  0,  _NULL,
             204,  "tional",    "tion",  5,  3,  0,  _NULL,
             205,  "enci",      "ence",  3,  3,  0,  _NULL,
             206,  "anci",      "ance",  3,  3,  0,  _NULL,
             207,  "izer",      "ize",   3,  2,  0,  _NULL,
             208,  "abli",      "able",  3,  3,  0,  _NULL,
             209,  "alli",      "al",    3,  1,  0,  _NULL,
             210,  "entli",     "ent",   4,  2,  0,  _NULL,
             211,  "eli",       "e",     2,  0,  0,  _NULL,
             213,  "ousli",     "ous",   4,  2,  0,  _NULL,
             214,  "ization",   "ize",   6,  2,  0,  _NULL,
             215,  "ation",     "ate",   4,  2,  0,  _NULL,
             216,  "ator",      "ate",   3,  2,  0,  _NULL,
             217,  "alism",     "al",    4,  1,  0,  _NULL,
             218,  "iveness",   "ive",   6,  2,  0,  _NULL,
             219,  "fulnes",    "ful",   5,  2,  0,  _NULL,
             220,  "ousness",   "ous",   6,  2,  0,  _NULL,
             221,  "aliti",     "al",    4,  1,  0,  _NULL,
             222,  "iviti",     "ive",   4,  2,  0,  _NULL,
             223,  "biliti",    "ble",   5,  2,  0,  _NULL,
             000,  NULL,        NULL,    0,  0,  0,  _NULL,
           };

static RuleList step3_rules[] =
           {
             301,  "icate",     "ic",    4,  1,  0,  _NULL,
             302,  "ative",     _LAMBDA,  4, -1,  0, _NULL,
             303,  "alize",     "al",    4,  1,  0,  _NULL,
             304,  "iciti",     "ic",    4,  1,  0,  _NULL,
             305,  "ical",      "ic",    3,  1,  0,  _NULL,
             308,  "ful",       _LAMBDA,  2, -1,  0, _NULL,
             309,  "ness",      _LAMBDA,  3, -1,  0, _NULL,
             000,  NULL,        NULL,    0,  0,  0,  _NULL,
           };

static RuleList step4_rules[] =
           {
             401,  "al",        _LAMBDA,  1, -1,  1,  _NULL,
             402,  "ance",      _LAMBDA,  3, -1,  1,  _NULL,
             403,  "ence",      _LAMBDA,  3, -1,  1,  _NULL,
             405,  "er",        _LAMBDA,  1, -1,  1,  _NULL,
             406,  "ic",        _LAMBDA,  1, -1,  1,  _NULL,
             407,  "able",      _LAMBDA,  3, -1,  1,  _NULL,
             408,  "ible",      _LAMBDA,  3, -1,  1,  _NULL,
             409,  "ant",       _LAMBDA,  2, -1,  1,  _NULL,
             410,  "ement",     _LAMBDA,  4, -1,  1,  _NULL,
             411,  "ment",      _LAMBDA,  3, -1,  1,  _NULL,
             412,  "ent",       _LAMBDA,  2, -1,  1,  _NULL,
             423,  "sion",      "s",     3,  0,  1,  _NULL,
             424,  "tion",      "t",     3,  0,  1,  _NULL,
             415,  "ou",        _LAMBDA,  1, -1,  1,  _NULL,
             416,  "ism",       _LAMBDA,  2, -1,  1,  _NULL,
             417,  "ate",       _LAMBDA,  2, -1,  1,  _NULL,
             418,  "iti",       _LAMBDA,  2, -1,  1,  _NULL,
             419,  "ous",       _LAMBDA,  2, -1,  1,  _NULL,
             420,  "ive",       _LAMBDA,  2, -1,  1,  _NULL,
             421,  "ize",       _LAMBDA,  2, -1,  1,  _NULL,
             000,  NULL,        NULL,    0,  0,  0,  _NULL,
           };

static RuleList step5a_rules[] =
           {
             501,  "e",         _LAMBDA,  0, -1,  1,  _NULL,
             502,  "e",         _LAMBDA,  0, -1, -1,  _RemoveAnE,
             000,  NULL,        NULL,    0,  0,  0,  _NULL,
           };

static RuleList step5b_rules[] =
           {
             503,  "ll",        "l",     1,  0,  1,  _NULL,
             000,  NULL,        NULL,    0,  0,  0,  _NULL,
           };

class stemmer {

  public:
        //char LAMBDA[1] = "";        /* the constant empty string */
        char *end;                  /* pointer to the end of the word */
	char tmpstr[1000];

/*****************************************************************************/
/********************   Private Function Declarations   **********************/
        int WordSize( char *word );
        int ContainsVowel( char *word );
        int EndsWithCVC( char *word );
        int AddAnE( char *word );
        int RemoveAnE( char *word );
        int ReplaceEnd( char *word, RuleList *rule );
	bool stemming(char * W);
	void Stem0(char * W);
	void Stem(char * W);
	void Stem(std::string & W);
	void StemS(char * W);
	inline void MarkStem(char * w);
	bool Stemmed(char *W); // has suffix
	inline int CallCondition(RuleList * r, char * w);
};

#endif _STEMMER_

