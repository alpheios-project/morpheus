#include <libfiles.h>

typedef struct
{
	const char*	d_name;
	long		d_flags;
} MorphEntry;

typedef struct
{
	const char*	d_name;
	const char*	d_value;
} AttributeEntry;

/*
	Tables mapping flags for morphological categories to textual names
	Note: Entries are tested by ANDing flags with table entries, and
	some entries correspond to flag combinations.  Entries are tested
	in order until a match is found, so entries with multiple flag
	values must appear in the table after values with fewer flags.
 */

/*
#define	SUBMASK	0777
MorphEntry	alpheiosPofsNames[] =
{
	{"adjective",	ADJSTEM|SUBMASK},
	{"noun",		NOUNSTEM|SUBMASK},
	{"verb",		PPARTMASK},
	{"verb",		VERBSTEM},
	{"numeral",		NUMERAL},
	{"preposition",	PREPOSITION},
	{"article",		ARTICLE},
	{"pronoun",		PRONOUN},
	{"pronoun",		INDEF_PRON},
	{"pronoun",		PERS_PRON},
	{"pronoun",		REL_PRON},
	{"pronoun",		INDEF_REL_PRON},
	{"particle",	PARTICLE},
	{"conjunction",	CONJUNCT},
	{"adverb",		ADVERB},
	{NULL,			0}
};
*/

AttributeEntry	alpheiosPofsOrder[] =
{
	{"noun",		"5"},
	{"adjective",	"4"},
	{"verb",		"3"},
	{"adverb",		"2"},
	{"preposition",	"1"},
	{NULL,			" "}
};

MorphEntry	alpheiosDeclNames[] =
{
	{"1st",	DECL1},
	{"2nd",	DECL2},
	{"3rd",	DECL3},
	{"4th",	DECL4},
	{"5th",	DECL5},
	{"1st &amp; 2nd", DECL1|DECL2},
	{"1st &amp; 3rd", DECL1|DECL3},
	{NULL,	0}
};

MorphEntry	alpheiosCaseNames[] =
{
	{"nominative",						NOMINATIVE},
	{"genitive",						GENITIVE},
	{"ablative",						ABLATIVE},
	{"dative",							DATIVE},
	{"accusative",						ACCUSATIVE},
	{"vocative",						VOCATIVE},
	{"genitive/dative",					GENITIVE|DATIVE},
	{"ablative/dative",					ABLATIVE|DATIVE},
	{"nominative/accusative",			NOMINATIVE|ACCUSATIVE},
	{"nominative/vocative",				NOMINATIVE|VOCATIVE},
	{"nominative/vocative/accusative",	NOMINATIVE|VOCATIVE|ACCUSATIVE},
	{NULL,								0}
};

AttributeEntry	alpheiosCaseOrder[] =
{
	{"nominative",	"7"},
	{"genitive",	"6"},
	{"dative",		"5"},
	{"accusative",	"4"},
	{"ablative",	"3"},
	{"locative",	"2"},
	{"vocative",	"1"},
	{NULL,			" "}
};

MorphEntry	alpheiosComparisonNames[] =
{
	{"comparative",	COMPARATIVE},
	{"superlative",	SUPERLATIVE},
	{NULL,			0}
};

MorphEntry	alpheiosGenderNames[] =
{
	{"masculine",					MASCULINE},
	{"feminine",					FEMININE},
	{"neuter",						NEUTER},
	{"adverbial",					ADVERBIAL},
	{"masculine/neuter",			MASCULINE|NEUTER},
	{"masculine/feminine",			MASCULINE|FEMININE},
	{"masculine/feminine/neuter",	MASCULINE|FEMININE|NEUTER},
	{"common",						MASCULINE|FEMININE|NEUTER},
	{NULL,							0}
};

MorphEntry	alpheiosMoodNames[] = {
	{"indicative",	INDICATIVE},
	{"subjunctive",	SUBJUNCTIVE},
	{"imperative",	IMPERATIVE},
	{"supine",		SUPINE},
	{"optative",	OPTATIVE},
	{"infinitive",	INFINITIVE},
	{"participle",	PARTICIPLE},
	{"conditional",	CONDITIONAL},
	{"gerundive",	GERUNDIVE},
	{NULL,			0}
};

MorphEntry	alpheiosNumberNames[] =
{
	{"singular",	SINGULAR},
	{"plural",		PLURAL},
	{"dual",		DUAL},
	{NULL,			0}
};

MorphEntry	alpheiosPersonNames[] =
{
	{"1st",	PERS1},
	{"2nd",	PERS2},
	{"3rd",	PERS3},
	{NULL,	0}
};

MorphEntry	alpheiosTenseNames[] =
{
	{"present",			PRESENT},
	{"future",			FUTURE},
	{"aorist",			AORIST},
	{"perfect",			PERFECT},
	{"imperfect",		IMPERF},
	{"pluperfect",		PLUPERF},
	{"future perfect",	FUTPERF},
	{"past absolute",	PASTABSOLUTE},
	{NULL,				0}
};

MorphEntry	alpheiosVoiceNames[] =
{
	{"active",			ACTIVE},
	{"passive",			PASSIVE},
	{"middle",			MIDDLE},
	{"mediopassive",	MEDIO_PASS},
	{"deponent",		ACTIVE|MIDDLE},
	{NULL,				0}
};

MorphEntry	alpheiosStemNames[] =
{
	{"pp_pr",		PP_PR},
	{"pp_fu",		PP_FU},
	{"pp_ao",		PP_AO},
	{"pp_pf",		PP_PF},
	{"pp_pp",		PP_PP},
	{"pp_ap",		PP_AP},
	{"pp_fp",		PP_FP},
	{"pp_p4",		PP_SU},
	{"pp_va",		PP_VA},
	{"pp_vn",		PP_VN},
	{"verbstem",	VERBSTEM},
	{"indecl",		INDECL},
	{"adj3",		ADJSTEM|DECL3},
	{"noun1",		NOUNSTEM|DECL1},
	{"noun2",		NOUNSTEM|DECL2},
	{"noun3",		NOUNSTEM|DECL3},
	{"noun4",		NOUNSTEM|DECL4},
	{"noun5",		NOUNSTEM|DECL5},
	{"prim_deriv",	VERBSTEM|PRIM_CONJ},
	{"reg_deriv",	VERBSTEM|REG_CONJ},
	{"adj1",		ADJSTEM|DECL1|DECL2},
	{"adj2",		ADJSTEM|DECL1|DECL2},
	{"indecl1",		INDECL|NOUNSTEM|DECL1},
	{"indecl2",		INDECL|NOUNSTEM|DECL2},
	{"indecl3",		INDECL|NOUNSTEM|DECL3},
	{"pron3",		INDECL|NOUNSTEM|DECL3},
	{"pron1",		INDECL|NOUNSTEM|DECL1|DECL2},
	{NULL,			0}
};

MorphEntry	alpheiosFlagNames[] =
{
	{"syll_augment",	SYLL_AUGMENT},
	{"comp_only",		COMP_ONLY},
	{"not_in_comp",		NOT_IN_COMPOSITION},
	{"enclitic",		ENCLITIC},
	{"proclitic",		PROCLITIC},
	{"iterative",		ITERATIVE},
	{"ant_acc",			ANT_ACC},
	{"stem_acc",		STEM_ACC},
	{"pen_acc",			STEM_ACC},
	{"suff_acc",		SUFF_ACC},
	{"ult_acc",			SUFF_ACC},
	{"rec_acc",			REC_ACC},
	{"needs_acc",		NEEDS_ACCENT},
	{"contracted",		CONTRACTED},
	{"uncontr_end",		UNCONTR_END},
	{"uncontracted",	UNCONTR_END},
	{"uncontr_stem",	UNCONTR_STEM},
	{"pers_name",		PERS_NAME},
	{"prevb_augment",	PREVB_AUGMENT},
	{"double_augment",	DOUBLE_AUGMENT},
	{"no_comp",			NO_COMP},
	{"irreg_comp",		IRREG_COMP},
	{"irreg_superl",	IRREG_SUPERL},
	{"short_pen",		SHORT_PEN},
	{"long_pen",		LONG_PEN},
	{"r_e_i_alpha",		R_E_I_ALPHA},
	{"unaugmented",		UNAUGMENTED},
	{"apocope",			APOCOPE},
	{"has_augment",		HAS_AUGMENT},
	{"nu_movable",		NU_MOVABLE},
	{"interv_s_to_h",	INTERV_S_TO_H},
	{"poetic",			POETIC},
	{"dissimilation", 	DISSIMILATION},
	{"metathesis", 		METATHESIS},
	{"elide_preverb",	ELIDE_PREVERB},
	{"root_preverb",	ROOT_PREVERB},
	{"diminutive", 		DIMINUTIVE},
	{"early", 			EARLY},
	{"late", 			LATE},
	{"rare", 			RARE},
	{"raw_preverb", 	RAW_PREVERB},
	{"short_subj", 		SHORT_SUBJ},
	{"unasp_preverb", 	UNASP_PREVERB},
	{"redupl", 			REDUPL},
	{"attic_redupl", 	ATTIC_REDUPL},
	{"is_deriv", 		IS_DERIV},
	{"no_redupl", 		NO_REDUPL},
	{"n_infix", 		N_INFIX},
	{"syncope", 		SYNCOPE},
	{"impersonal", 		IMPERSONAL},
	{"indeclform",		INDECLFORM},
	{"needs_rbreath",	NEEDS_RBREATH},
	{"no_circumflex",	NO_CIRCUMFLEX},
	{"causal",			CAUSAL},
	{"intrans",			INTRANS},
	{"tmesis",			TMESIS},
	{"raw_sonant",		RAW_SONANT},
	{"prodelision",		PRODELISION},
	{"frequentative",	FREQUENTAT},
	{"desiderative",	DESIDERATIVE},
	{"impersonal",		IMPERSONAL},
	{"later",			LATER},
	{"double_redupl",	DOUBLE_REDUPL},
	{"pres_redupl",		PRES_REDUPL},
	{"ends_in_digamma",	ENDS_IN_DIGAMMA},
	{"geog_name",		GEOG_NAME},
	{"doubled_cons",	DOUBLED_CONS},
	{"iota_intens",		IOTA_INTENS},
	{"sig_to_ci",		SIG_TO_CI},
	{"short_eis",		SHORT_EIS},
	{"pros_to_poti",	PROS_TO_POTI},
	{"pros_to_proti",	PROS_TO_PROTI},
	{"meta_to_peda",	META_TO_PEDA},
	{"upo_to_upai",		UPO_TO_UPAI},
	{"para_to_parai",	PARA_TO_PARAI},
	{"uper_to_upeir",	UPER_TO_UPEIR},
	{"en_to_eni",		EN_TO_ENI},
	{"a_priv",			A_PRIV},
	{"a_copul",			A_COPUL},
	{"metrical_long",	METRICAL_LONG},
	{NULL,				0}
};

/* dialects */
MorphEntry	alpheiosDialectNames[] =
{
	{"Attic",				ATTIC},
	{"epic",				EPIC},
	{"Homeric",				HOMERIC},
	{"non-Homeric epic",	NON_HOMERIC_EPIC},
	{"Doric",				DORIC},
	{"Ionic",				IONIC},
	{"Aeolic",				AEOLIC},
	{"paradigm form",		PARADIGM},
	{"all",					ALL_DIAL},
	{"need_not_aug",		HOMERIC},
	{"prose",				PROSE},
/*
	{"eo_ou_dial",			ATTIC},
	{"laconian",			LACONIAN},
*/
	{"Ionic/Homeric",		IONIC|HOMERIC},
/*
	{"eo_eu_dial",			IONIC|HOMERIC},
	{"no_contr_fut",		HOMERIC|IONIC},
	{"a_no_contr",			HOMERIC|DORIC},
	{"ee_ee_dial",			(~ATTIC)},
	{"eo_eo_dial",			(~ATTIC)},
	{"eou_eou_dial",		(~ATTIC)},
	{"ew_ew_dial",			(~ATTIC)},
	{"un_contr",			(~ATTIC)},
*/
	{NULL,					0}
};

/* Geographical Regions */
MorphEntry	alpheiosGeoNames[] =
{
	{"Phocis",		PHOCIS},
	{"Locris",		LOCRIS},
	{"Elis",	  	ELIS},
	{"Laconia",		LACONIA},
	{"Heraclea",	HERACLEA},
	{"Megarid",		MEGARID},
	{"Argolid",		ARGOLID},
	{"Rhodes",		RHODES},
	{"Cos",			COS},
	{"Thera",		THERA},
	{"Cyrene",		CYRENE},
	{"Crete",		CRETE},
	{"Arcadia",		ARCADIA},
	{"Cyprus",		CYPRUS},
	{"Boeotia",		BOEOTIA},
	{NULL,			0}
};
