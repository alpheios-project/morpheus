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

	const char* langAttr = "";
	if (cur_lang() == GREEK)
		langAttr = " xml:lang=\"grc\"";
	else if (cur_lang() == LATIN)
		langAttr = " xml:lang=\"lat\"";

	/* start word */
	if (nanals > 0)
	{
		fprintf(fout, "<word>\n");
		fprintf(fout, "<form%s>%s</form>\n", langAttr, rawword_of(gkword));
	}

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
				fprintf(fout, "<hdwd%s>%s</hdwd>\n", langAttr, curlem);

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

	/* calculate language attribute for tags */
	const char* langAttr = "";
	if (cur_lang() == GREEK)
		langAttr = " xml:lang=\"grc\"";
	else if (cur_lang() == LATIN)
		langAttr = " xml:lang=\"lat\"";

	/* calculate term (stem + suffix) to display */
	char	stem[BUFSIZ];
	char	suffix[BUFSIZ];
	int		stemlen = 0;
	int		suffixlen = 0;
	*stem = '\0';
	*suffix = '\0';

	/* build stem from preverb, aug1, stem, with colons between pieces */
	const char*	part = preverb_of(analysis);
	if (part && *part)
	{
		strncat(stem, part, BUFSIZ);
		stemlen = strlen(stem);
	}
	/* aug1's containing > seem to indicate form changes already present */
	/* in other parts; those without represent a new piece */
	part = aug1_of(analysis);
	if (part && *part && !strchr(part, '>'))
	{
		if (stemlen > 0)
			strcat(stem, ":");
		strncat(stem, part, BUFSIZ - stemlen);
		stemlen = strlen(stem);
	}
	part = stem_of(analysis);
	if (part && *part)
	{
		if (stemlen > 0)
			strcat(stem, ":");
		strncat(stem, part, BUFSIZ - stemlen);
		stemlen = strlen(stem);
	}

	/* build suffix from suffix and endstring */
	part = suffix_of(analysis);
	if (part && *part)
	{
		strncat(suffix, part, BUFSIZ);
		suffixlen = strlen(suffix);
	}
	part = endstring_of(analysis);
	if (part && *part)
	{
		if (suffixlen > 0)
			strcat(suffix, ":");
		strncat(suffix, part, BUFSIZ - suffixlen);
		suffixlen = strlen(suffix);
	}

	/* get case(s), initialize ptrs to first case */
	word_form	wf = forminfo_of(analysis);
	const char*	caseNames = alpheiosMorphLookup(alpheiosCaseNames, case_of(wf));
	const char*	nextCase;
	const char*	endCase;
	if (caseNames)
	{
		nextCase = caseNames;
		endCase = strchr(nextCase, '/');
		if (!endCase)
			endCase = nextCase + strlen(nextCase);
	}
	else
	{
		nextCase = endCase = "";
	}

	/* for each case (using empty string if none exist) */
	while (nextCase)
	{
		/* get gender(s), initialize ptrs to first gender */
		const char*	genderNames = alpheiosMorphLookup(alpheiosGenderNames,
													  gender_of(wf));
		const char*	nextGender;
		const char*	endGender;
		if (genderNames)
		{
			nextGender = genderNames;
			endGender = strchr(nextGender, '/');
			if (!endGender)
				endGender = nextGender + strlen(nextGender);
		}
		else
		{
			nextGender = endGender = "";
		}

		/* for each gender (using empty string if none exist) */
		while (nextGender)
		{
			fprintf(fout, "<infl>\n");

			/* put out term */
			fprintf(fout, "<term%s>", langAttr);
			if (stemlen > 0)
				fprintf(fout, "<stem>%s</stem>", stem);
			if (suffixlen > 0)
				fprintf(fout, "<suff>%s</suff>", suffix);
			fprintf(fout, "</term>\n");

			/* put out part of speech */
			alpheiosDumpPartOfSpeech(analysis, fout);

			/* dump case and gender (if any) and other morphological info */
			int caseLen = endCase - nextCase;
			int	genderLen = endGender - nextGender;
			if (caseLen)
			{
				fprintf(fout,
						"<case>%*.*s</case>\n",
						caseLen,
						caseLen,
						nextCase);
			}
			if (genderLen)
			{
				fprintf(fout,
						"<gend>%*.*s</gend>\n",
						genderLen,
						genderLen,
						nextGender);
			}
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

			fprintf(fout, "</infl>\n");

			/* advance to next gender */
			if (*endGender == '/')
			{
				nextGender = endGender + 1;
				endGender = strchr(nextGender, '/');
				if (!endGender)
					endGender = nextGender + strlen(nextGender);
			}
			else
			{
				nextGender = NULL;
			}
		}

		/* advance to next case */
		if (*endCase == '/')
		{
			nextCase = endCase + 1;
			endCase = strchr(nextCase, '/');
			if (!endCase)
				endCase = nextCase + strlen(nextCase);
		}
		else
		{
			nextCase = NULL;
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
