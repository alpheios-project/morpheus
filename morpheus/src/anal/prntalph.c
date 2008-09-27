#include <gkstring.h>
#include "prntalph.h"

void		alpheiosDumpWord(gk_word* gkword, FILE* fout);
void		alpheiosDumpAnalysis(gk_analysis* analysis, FILE* fout);
void		alpheiosDumpPartOfSpeech(gk_analysis* analysis, FILE* fout);
void		alpheiosDumpMorphology(word_form a_wf, FILE* a_fout);
void		alpheiosDumpFlag(const char* a_tag,
							 const char* a_label,
							 const MorphEntry* a_table,
							 long a_flags,
							 FILE* a_fout);
const char*	alpheiosMorphLookup(const MorphEntry* a_table, long a_flags);

/* print out info on a word */
int	alpheiosPrintWord(gk_word* gkword, PrntFlags prntflags, FILE* fout)
{
	int	nanals = totanal_of(gkword);
	SortAnals(analysis_of(gkword), nanals);

	if (prntflags & PERSEUS_FORMAT)
	{
		alpheiosDumpWord(gkword, fout);
		return nanals;
	}

	return 0;
}

/* dump out info on a single word */
void	alpheiosDumpWord(gk_word* gkword, FILE* fout)
{
	int nanals = totanal_of(gkword);
	int goodanals = GoodAnals(gkword,0);
	char curlem[MAXWORDSIZE];
	*curlem = '\0';

	/* start word */
	if (nanals > 0)
		fprintf(fout, "<word>\n");

	/* for each analysis */
	gk_analysis* nxtAnalysis = analysis_of(gkword);
	gk_analysis* endAnalysis = nxtAnalysis + nanals;
	for (; nxtAnalysis != endAnalysis; ++nxtAnalysis)
	{
		/* if there are no good analyses or this is a good one */
		/* (lemma does not contain hyphen) */
		if (!goodanals || !strchr(lemma_of(nxtAnalysis), '-'))
		{
			/* if this is a new lemma */
			if (strcmp(curlem, lemma_of(nxtAnalysis)))
			{
				/* terminate last entry */
				if (*curlem)
					fprintf(fout, "</entry>\n");

				strcpy(curlem, lemma_of(nxtAnalysis));

				/* start new entry */
				fprintf(fout, "<entry>\n");

				/* put out info on lemma */
				fprintf(fout, "<dict>\n");
				fprintf(fout, "<hdwd>%s</hdwd>\n", curlem);

				/* put out part of speech for first instance */
				/* as part of speech for lemma */
				/* (should we be doing this at all?) */
				/* alpheiosDumpPartOfSpeech(nxtAnalysis, fout); */

				fprintf(fout, "</dict>\n");
			}

			alpheiosDumpAnalysis(nxtAnalysis, fout);
		}
		continue;
	}

	/* terminate last entry */
	if (*curlem)
		fprintf(fout, "</entry>\n");

	/* terminate word */
	if (nanals > 0)
		fprintf(fout, "</word>\n");
}

void			alpheiosDumpAnalysis(
gk_analysis*	analysis,
FILE*			fout)
{
	/*
		Note: The lookup tables for gender and case may
		return a multi-valued string (with values separated by "/")
		so we need to iterate and produce an inflection element for
		each combination of gender and case.
	*/

	/* get case(s), copying to local storage because strtok_r overwrites */
	word_form	wf = forminfo_of(analysis);
	char		caseNames[BUFSIZ];
	const char*	nextCase = alpheiosMorphLookup(alpheiosCaseNames, case_of(wf));
	if (nextCase)
		strncpy(caseNames, nextCase, BUFSIZ);
	else
		*caseNames = '\0';

	/* for each case (using empty string if none exist) */
	char*	saveCase;
	nextCase = strtok_r(caseNames, "/", &saveCase);
	if (nextCase == NULL)
		nextCase = "";
	for (; nextCase; nextCase = strtok_r(NULL, "/", &saveCase))
	{
		/* get gender(s), copying to local storage */
		char		genderNames[BUFSIZ];
		const char*	nextGender = alpheiosMorphLookup(alpheiosGenderNames,
													 gender_of(wf));
		if (nextGender)
			strncpy(genderNames, nextGender, BUFSIZ);
		else
			*genderNames = '\0';

		/* for each gender (using empty string if none exist) */
		char*	saveGender;
		nextGender = strtok_r(genderNames, "/", &saveGender);
		if (nextGender == NULL)
			nextGender = "";
		for (; nextGender; nextGender = strtok_r(NULL, "/", &saveGender))
		{
			fprintf(fout, "<infl>\n");

			/* put out term */
			fprintf(fout, "<term");
			if (cur_lang() == GREEK)
				fprintf(fout, " xml:lang=\"grc\"");
			else if (cur_lang() == LATIN)
				fprintf(fout, " xml:lang=\"la\"");
			fprintf(fout, ">");

			char	term[BUFSIZ];
			strncpy(term, workword_of(analysis), BUFSIZ);

			/* find start of stem and suffix in term */
			/* last underscore in term separates them */
			char*	stem = term;
			char*	suffix = strrchr(term, '_');
			if (suffix)
			{
				/* if no char or non-alpha after last underscore */
				/* there's no suffix */
				if (!*++suffix || !isalpha(*suffix))
					suffix = NULL;

				/* if there is a suffix, null-terminate the stem */
				if (suffix)
					suffix[-1] = '\0';
			}

			/* sometimes term has multiple underscores - remove them */
			const char*	in = stem;
			char*		out = stem;
			for (; *in; ++in)
			{
				if (*in != '_')
					*out++ = *in;
			}
			*out = '\0';

			/* put out parts and terminate term element */
			if (*stem)
				fprintf(fout, "<stem>%s</stem>", stem);
			if (suffix)
				fprintf(fout, "<suff>%s</suff>", suffix);
			fprintf(fout, "</term>\n");

			/* put out part of speech */
			alpheiosDumpPartOfSpeech(analysis, fout);

			/* dump case and gender (if any) and other morphological info */
			if (*nextCase)
				fprintf(fout, "<case>%s</case>\n", nextCase);
			if (*nextGender)
				fprintf(fout, "<gend>%s</gend>\n", nextGender);
			alpheiosDumpMorphology(wf, fout);

			/* other info: geographic region, dialect */
			alpheiosDumpFlag("geo",
							 NULL,
							 alpheiosGeoNames,
							 geogregion_of(analysis),
							 fout);
			alpheiosDumpFlag("note",
							 "Dialect: ",
							 alpheiosDialectNames,
							 dialect_of(analysis),
							 fout);

/*
			fprintf(fout,
					"<note>Stem: %o</note>\n",
					analysis->gs_steminfo);
*/

			fprintf(fout, "</infl>\n");
		}
	}
}

/* dump part of speech */
void	alpheiosDumpPartOfSpeech(gk_analysis* analysis, FILE* fout)
{
	/* check various part of speech forms */
	const char*	pofs = NULL;
	if (Is_participle(analysis))
	{
		pofs = "verb participle";
	}
	else if (Is_nounform(analysis) || Is_adjform(analysis))
	{
		pofs = "noun";
	}
  	else if (Is_verbform(analysis))
	{
		pofs = "verb";
	}

	/* check stemtype and adjust part of speech */
	/* Note: check for "_adj" must precede test for "pron" */
	const char*	stemType = NameOfStemtype(stemtype_of(analysis));
	if (strstr(stemType, "_adj"))
		pofs = "adjective";
	else if (strstr(stemType, "pron") ||
			 !strcmp(stemType, "indef") ||
			 !strcmp(stemType, "relative"))
		pofs = "pronoun";
	else if (!strcmp(stemType, "adverb") ||
			 !strcmp(stemType, "article") ||
			 !strcmp(stemType, "particle"))
		pofs = stemType;
	else if (!strcmp(stemType, "conj"))
		pofs = "conjunction";
	else if (!strcmp(stemType, "exclam"))
		pofs = "exclamation";
	else if (!strcmp(stemType, "indecl"))
		pofs = "irregular";
	else if (!strcmp(stemType, "prep"))
		pofs = "preposition";

	/* if part of speech found */
	if (pofs)
	{
		fprintf(fout, "<pofs>%s</pofs>\n", pofs);

		/* if noun, look for declension */
		if (strcmp(pofs, "noun") == 0)
		{
			alpheiosDumpFlag("decl",
							 NULL,
							 alpheiosDeclNames,
							 stemtype_of(analysis),
							 fout);
		}
	}
}

/* dump morphological values (except case and gender) */
void	alpheiosDumpMorphology(word_form a_wf, FILE* a_fout)
{
	alpheiosDumpFlag("comp",
					 NULL,
					 alpheiosComparisonNames,
					 degree_of(a_wf),
					 a_fout);
	alpheiosDumpFlag("mood", NULL, alpheiosMoodNames, mood_of(a_wf), a_fout);
	alpheiosDumpFlag("num", NULL, alpheiosNumberNames, number_of(a_wf), a_fout);
	alpheiosDumpFlag("pers",
					 NULL,
					 alpheiosPersonNames,
					 person_of(a_wf),
					 a_fout);
	alpheiosDumpFlag("tense", NULL, alpheiosTenseNames, tense_of(a_wf), a_fout);
	alpheiosDumpFlag("voice", NULL, alpheiosVoiceNames, voice_of(a_wf), a_fout);
}

void				alpheiosDumpFlag(
const char*			a_tag,
const char*			a_label,
const MorphEntry*	a_table,
long				a_flags,
FILE*				a_fout)
{
	const char*	name = alpheiosMorphLookup(a_table, a_flags);
	if (name && strlen(name) > 0)
		fprintf(a_fout,
				"<%s>%s%s</%s>\n",
				a_tag,
				(a_label ? a_label : ""),
				name,
				a_tag);
}

const char*	alpheiosMorphLookup(const MorphEntry* a_table, long a_flags)
{
	if (!a_flags || !a_table)
		return NULL;

	const MorphEntry*	nextEntry;
	for (nextEntry = a_table; nextEntry->d_flags != 0; ++nextEntry)
	{
		if ((nextEntry->d_flags & a_flags) == a_flags)
			return nextEntry->d_name;
	}

	return NULL;
}
