/* $Id: pairdef.h 64730 2012-05-23 01:44:39Z twu $ */
#ifndef PAIRDEF_INCLUDED
#define PAIRDEF_INCLUDED
#include "bool.h"
#include "genomicpos.h"

typedef enum {GOOD,BAD} State_T;

#define T Pair_T
struct T {
  int querypos;
  Genomicpos_T genomepos;
  int refquerypos;
  int aapos;

  int queryjump;		/* Used only for gaps */
  int genomejump;		/* Used only for gaps */

  int aaphase_g;		/* If 0, then marks start of codon.  If -1, there is no aa here. */
  int aaphase_e;		/* If 0, then marks start of codon.  If -1, there is no aa here. */

  int dynprogindex;		/* 0 if not generated by dynprog; <0 if contains intron; else > 0 */

  char cdna;
  char comp;
  char genome;
  char aa_g;			/* Genomic aa */
  char aa_e;			/* EST aa */

  bool gapp;			/* True if comp is in a big gap (from genomic perspective):
                                   >])([<#= (but not '-' or '~'). */
  bool knowngapp;		/* Known intron */

  bool extraexonp;
  bool shortexonp;

  State_T state;
  State_T vstate_good;
  State_T vstate_bad;

  bool protectedp;		/* Protected against trimming, because found using splicetrie */
  bool disallowedp;		/* Used when novelsplicingp is false and intron is not allowed */

  double donor_prob;		/* Set for an intron gap */
  double acceptor_prob;
  bool end_intron_p;
};

#undef T
#endif
