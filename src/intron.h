/* $Id: intron.h 41614 2011-06-22 22:57:46Z twu $ */
#ifndef INTRON_INCLUDED
#define INTRON_INCLUDED
#include "bool.h"
#include "genomicpos.h"
#include "iit-read.h"
#include "chrnum.h"

/* Pieces for logical AND */
#define LEFT_GT  0x21		/* 100001 */
#define LEFT_GC	 0x10		/* 010000 */
#define LEFT_AT  0x08		/* 001000 */
#define LEFT_CT  0x06		/* 000110 */

#define RIGHT_AG 0x30		/* 110000 */
#define RIGHT_AC 0x0C		/* 001100 */
#define RIGHT_GC 0x02		/* 000010 */
#define RIGHT_AT 0x01		/* 000001 */

/* Intron types.  Results of logical AND of dinucleotide pairs.  Note
   that forward is > 0x04 and reverse is <= 0x04 */
#define GTAG_FWD 0x20		/* 100000 GT-AG */
#define GCAG_FWD 0x10		/* 010000 GC-AG */
#define ATAC_FWD 0x08		/* 001000 AT-AC */
#define GTAG_REV 0x04		/* 000100 CT-AC */
#define GCAG_REV 0x02		/* 000010 CT-GC */
#define ATAC_REV 0x01		/* 000001 GT-AT */
#define NONINTRON 0x00


extern int
Intron_type (char left1, char left2, char right2, char right1, int cdna_direction
#ifdef INTRON_HELP
	     , IIT_T splicesites_iit, int *splicesites_divint_crosstable,
	     int donor_typeint, int acceptor_typeint, Chrnum_T chrnum,
	     Genomicpos_T leftgenomepos, Genomicpos_T rightgenomepos,
	     Genomicpos_T chrpos, bool watsonp, int genomiclength
#endif
);
extern char *
Intron_type_string (int introntype);

extern char *
Intron_left_dinucl_string (int dinucl);
extern char *
Intron_right_dinucl_string (int dinucl);

extern bool
Intron_canonical_fwd_p (char donor1, char donor2, char acceptor2, char acceptor1);
extern bool
Intron_canonical_rev_p (char donor1, char donor2, char acceptor2, char acceptor1);
extern bool
Intron_gcag_fwd_p (char donor1, char donor2, char acceptor2, char acceptor1);
extern bool
Intron_gcag_rev_p (char donor1, char donor2, char acceptor2, char acceptor1);
extern bool
Intron_atac_fwd_p (char donor1, char donor2, char acceptor2, char acceptor1);
extern bool
Intron_atac_rev_p (char donor1, char donor2, char acceptor2, char acceptor1);

#endif
