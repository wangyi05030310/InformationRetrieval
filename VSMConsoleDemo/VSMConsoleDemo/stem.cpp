/*******************************   stem.cpp   *********************************

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
#include "stem.h"
#include <algorithm>
#include <string>

using namespace std;

//int AddAnE(char * w);
//int RemoveAnE(char * w);
//int ContainsVowel(char * w);

/*****************************************************************************/
/*****************   Private Defines and Data Structures   *******************/
/*****************************************************************************/
/********************   Private Function Declarations   **********************/

/*FN***************************************************************************

       WordSize( word )

   Returns: int -- a weird count of word size in adjusted syllables

   Purpose: Count syllables in a special way:  count the number 
            vowel-consonant pairs in a word, disregarding initial 
            consonants and final vowels.  The letter "y" counts as a
            consonant at the beginning of a word and when it has a vowel
            in front of it; otherwise (when it follows a consonant) it
            is treated as a vowel.  For example, the WordSize of "cat" 
            is 1, of "any" is 1, of "amount" is 2, of "anything" is 3.

   Plan:    Run a DFA to compute the word size

   Notes:   The easiest and fastest way to compute this funny measure is
            with a finite state machine.  The initial state 0 checks
            the first letter.  If it is a vowel, then the machine changes
            to state 1, which is the "last letter was a vowel" state.
            If the first letter is a consonant or y, then it changes
            to state 2, the "last letter was a consonant state".  In
            state 1, a y is treated as a consonant (since it follows
            a vowel), but in state 2, y is treated as a vowel (since
            it follows a consonant.  The result counter is incremented
            on the transition from state 1 to state 2, since this
            transition only occurs after a vowel-consonant pair, which
            is what we are counting.
**/

int stemmer::WordSize(char * word)	// char *word;   /* in: word having its WordSize taken */
{
	register int result;   /* WordSize of the word */
	register int state;    /* current state in machine */

	result = 0;
	state = 0;
                 
	/* Run a DFA to compute the word size */
	while ( EOS != *word )
	{
		switch ( state )
        {
		case 0:
			{
				state = (IsVowel(*word)) ? 1 : 2;
				break;
			}
		case 1:
			{
				state = (IsVowel(*word)) ? 1 : 2;
				if ( 2 == state )	
				{
					result++;
				}
				break;
			}
         case 2: 
			 {
				 state = (IsVowel(*word) || ('y' == *word)) ? 1 : 2;
				 break;
			 }
		}
		word++;
	}
	return( result );
} /* WordSize */

/*FN**************************************************************************

       ContainsVowel( word )

   Returns: int -- TRUE (1) if the word parameter contains a vowel,
            _FALSE (0) otherwise.

   Purpose: Some of the rewrite rules apply only to a root containing
            a vowel, where a vowel is one of "aeiou" or y with a
            consonant in front of it.

   Plan:    Obviously, under the definition of a vowel, a word contains
            a vowel iff either its first letter is one of "aeiou", or
            any of its other letters are "aeiouy".  The plan is to
            test this condition.

   Notes:   None
**/

int stemmer::ContainsVowel( char * word )	/* in: buffer with word checked */
{
	if ( EOS == *word )
	{
		return( _FALSE );
	}
	else
	{
		return( IsVowel(*word) || (NULL != strpbrk(word+1,"aeiouy")) );
	}
} /* ContainsVowel */

/*FN**************************************************************************

       EndsWithCVC( word )

   Returns: int -- TRUE (1) if the current word ends with a
            consonant-vowel-consonant combination, and the second
            consonant is not w, x, or y, _FALSE (0) otherwise.

   Purpose: Some of the rewrite rules apply only to a root with
            this characteristic.

   Plan:    Look at the last three characters.

   Notes:   None
**/

int stemmer::EndsWithCVC(char * word )	/* in: buffer with the word checked */
{
	int length;         /* for finding the last three characters */

	if ( (length = strlen(word)) < 2 )
	{
		return( _FALSE );
	}
	else
    {
		end = word + length - 1;
		return(    (NULL == strchr("aeiouwxy",*end--))      /* consonant */
				&& (NULL != strchr("aeiouy",  *end--))        /* vowel */
				&& (NULL == strchr("aeiou",   *end  )) );   /* consonant */
	}
} /* EndsWithCVC */

/*FN**************************************************************************

       AddAnE( word )

   Returns: int -- _TRUE (1) if the current word meets special conditions
            for adding an e.

   Purpose: Rule 122 applies only to a root with this characteristic.

   Plan:    Check for size of 1 and a consonant-vowel-consonant ending.

   Notes:   None
**/

int stemmer::AddAnE( char * word )
{
	return( (1 == WordSize(word)) && EndsWithCVC(word) );
} /* AddAnE */

/*FN**************************************************************************

       RemoveAnE( word )

   Returns: int -- _TRUE (1) if the current word meets special conditions
            for removing an e.

   Purpose: Rule 502 applies only to a root with this characteristic.

   Plan:    Check for size of 1 and no consonant-vowel-consonant ending.

   Notes:   None
**/

int stemmer::RemoveAnE( char * word )
{
	return( (1 == WordSize(word)) && !EndsWithCVC(word) );
} /* RemoveAnE */

/*FN**************************************************************************

       ReplaceEnd( word, rule )

   Returns: int -- the id for the rule fired, 0 is none is fired

   Purpose: Apply a set of rules to replace the suffix of a word

   Plan:    Loop through the rule set until a match meeting all conditions
            is found.  If a rule fires, return its id, otherwise return 0.
            Connditions on the length of the root are checked as part of this
            function's processing because this check is so often made.

   Notes:   This is the main routine driving the stemmer.  It goes through
            a set of suffix replacement rules looking for a match on the
            current suffix.  When it finds one, if the root of the word
            is long enough, and it meets whatever other conditions are
            required, then the suffix is replaced, and the function returns.
**/

inline  int stemmer::CallCondition(RuleList * r, char * w)
{
   int i = 1;

   switch(r->func)
   {
   case _ContainsVowel:
	   {
		   i = ContainsVowel(w);
		   break;
	   }
   case _RemoveAnE:
	   {
		   i = RemoveAnE(w);
		   break;
	   }
   case _AddAnE:
	   {
		   i = AddAnE(w);
		   break;
	   }
   case _NULL:
   default:
	   break;
   }
   return i;
}

int stemmer::ReplaceEnd(char * word, RuleList *rule )
	/* in/out: buffer with the stemmed word */
	/* in: data structure with replacement rules */
{
	register char *ending;   /* set to start of possible stemmed suffix */
	char tmp_ch;             /* save replaced character when testing */

	while ( 0 != rule->id )
	{
		ending = end - rule->old_offset;
		if ( word <= ending )
			if ( 0 == strcmp(ending,rule->old_end) )
			{
				tmp_ch = *ending;
				*ending = EOS;
				if ( rule->min_root_size < WordSize(word) )
					//if ( !rule->condition || (*rule->condition)(word) )
					if (CallCondition( rule, word))
					{
						(void)strcat( word, rule->new_end );
						end = ending + rule->new_offset;
						break;
					}
					*ending = tmp_ch;
			}
			rule++;
	}
	return( rule->id );
} /* ReplaceEnd */

/*****************************************************************************/
/*********************   Public Function Declarations   **********************/

/*FN***************************************************************************

       Stem( word )

   Returns: int -- _FALSE (0) if the word contains non-alphabetic
characters
            and hence is not stemmed, _TRUE (1) otherwise

   Purpose: Stem a word

   Plan:    Part 1: Check to ensure the word is all alphabetic
            Part 2: Run through the Porter algorithm
            Part 3: Return an indication of successful stemming

   Notes:   This function implements the Porter stemming algorithm, with
            a few additions here and there.  See:

               Porter, M.F., "An Algorithm For Suffix Stripping,"
               Program 14 (3), July 1980, pp. 130-137.

            Porter's algorithm is an ad hoc set of rewrite rules with
            various conditions on rule firing.  The terminology of
            "step 1a" and so on, is taken directly from Porter's
            article, which unfortunately gives almost no justification
            for the various steps.  Thus this function more or less
            faithfully refects the opaque presentation in the article.
            Changes from the article amount to a few additions to the
            rewrite rules;  these are marked in the RuleList data
            structures with comments.
**/

inline void stemmer::MarkStem(char * w)
{
	if (w == NULL) return;
	int s = strlen(w)-1;
	if (s < 0) return;
	w[s] = (char) toupper(w[s]);
}

void stemmer::Stem0(char * w)
{

	strcpy(tmpstr,w);
	if (stemming(tmpstr) == _TRUE && (strlen(tmpstr) > 0))
	{
		strcpy(w, tmpstr);
		MarkStem(w);
	}
}

bool stemmer::Stemmed(char * w)
{
	if (w == NULL)
	{
		return false;
	}
	else 
	{
		int sw = strlen(w);
		if (sw == 0) return false;
		sw--;

		if ((w[sw] >= 'A') && (w[sw] <= 'Z'))
		{
			return true;
		}
		else 
		{
			return false;
		}
		//if (sw < _StemSuffixLen) return false;
		//char * r = w + sw - _StemSuffixLen;
		//if (strcmp(r, _StemSuffix) == 0) return true;
		//else return false;
	}
}

void stemmer::StemS(char * w) {
	if (w == NULL)
	{
		return;
	}
	else 
	{
		Stem0(w);
	}
}

void stemmer::Stem(char * w) {

	if (w == NULL) return;
	if (*w == '\0') return;
	strcpy(tmpstr, w);
	//int wlen = strlen(w);
	Stem0(w);
	//MarkStem(w);
	//if (strcmp(w, tmpstr) != 0) 
	//strcat(w,_StemSuffix);
	//cout << "Stemmed: " << w << endl;
}

void stemmer::Stem(std::string & w) {
	if (w.size() == 0) return;
	strcpy(tmpstr, (char *) w.c_str());
	Stem0(tmpstr);
	//if (strcmp((char *) w.c_str(), tmpstr) != 0) {
	w = tmpstr;
	//MarkStem((char *) w.c_str());
	//w += _StemSuffix;
	//}
}

bool stemmer::stemming( char * word )
   /* in/out: the word stemmed */
{
   int rule;    /* which rule is fired in replacing an end */

            /* Part 1: Check to ensure the word is all alphabetic */
   for ( end = word; *end != EOS; end++ )
      if ( !isalpha(*end) ) return( _FALSE );
      else *end = tolower( *end );
   end--;

                /*  Part 2: Run through the Porter algorithm */
   (void)ReplaceEnd( word, step1a_rules );
   rule = ReplaceEnd( word, step1b_rules );
   if ( (106 == rule) || (107 == rule) )
      (void)ReplaceEnd( word, step1b1_rules );
   (void)ReplaceEnd( word, step1c_rules );

   (void)ReplaceEnd( word, step2_rules );

   (void)ReplaceEnd( word, step3_rules );

   (void)ReplaceEnd( word, step4_rules );

   (void)ReplaceEnd( word, step5a_rules );
   (void)ReplaceEnd( word, step5b_rules );

           /* Part 3: Return an indication of successful stemming */
   return( _TRUE );
} /* Stem */
