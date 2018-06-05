#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */

#define LRESET	PTCD_PTCD5

#define GLCD_CS1     PTBD_PTBD1	  // Chip Selection 1GLCD_CS2     PIN_B1   // Chip Selection 2
#define GLCD_CS2     PTBD_PTBD0   // Chip Selection 2
#define GLCD_RST     PTBD_PTBD2	  // RESET
#define GLCD_RS      PTBD_PTBD3	 //Data or Instruction input
#define GLCD_E       PTBD_PTBD4   // Enable

const unsigned char ra[64][64]={
		{0,	1,	2,	3,	4,	5,	6,	7,	8,	9,	10,	11,	12,	13,	14,	15,	16,	17,	18,	19,	20,	21,	22,	23,	24,	25,	26,	27,	28,	29,	30,	31,	32,	33,	34,	35,	36,	37,	38,	39,	40,	41,	42,	43,	44,	45,	46,	47,	48,	49,	59,	60,	61,	62,	63,	64	}	,
		{1,	0,	1,	2,	3,	4,	5,	6,	7,	8,	9,	10,	11,	12,	13,	14,	15,	16,	17,	18,	19,	20,	21,	22,	23,	24,	25,	26,	27,	28,	29,	30,	31,	32,	33,	34,	35,	36,	37,	38,	39,	40,	41,	42,	43,	44,	45,	46,	47,	48,	58,	59,	60,	61,	62,	63	}	,
		{2,	1,	0,	2,	3,	4,	5,	6,	7,	8,	9,	10,	11,	12,	13,	14,	15,	16,	17,	18,	19,	20,	21,	22,	23,	24,	25,	26,	27,	28,	29,	30,	31,	32,	33,	34,	35,	36,	37,	38,	39,	40,	41,	42,	43,	44,	45,	46,	47,	48,	58,	59,	60,	61,	62,	63	}	,
		{3,	2,	2,	0,	2,	4,	5,	6,	7,	8,	9,	10,	11,	12,	13,	14,	15,	16,	17,	18,	19,	20,	21,	22,	23,	24,	25,	26,	27,	28,	29,	30,	31,	32,	33,	34,	35,	36,	37,	38,	39,	40,	41,	42,	43,	44,	45,	46,	47,	48,	58,	59,	60,	61,	62,	63	}	,
		{4,	3,	3,	2,	0,	3,	4,	5,	6,	8,	9,	10,	11,	12,	13,	14,	15,	16,	17,	18,	19,	20,	21,	22,	23,	24,	25,	26,	27,	28,	29,	30,	31,	32,	33,	34,	35,	36,	37,	38,	39,	40,	41,	42,	43,	44,	45,	46,	47,	48,	58,	59,	60,	61,	62,	63	}	,
		{5,	4,	4,	4,	3,	0,	3,	4,	6,	7,	8,	9,	10,	12,	13,	14,	15,	16,	17,	18,	19,	20,	21,	22,	23,	24,	25,	26,	27,	28,	29,	30,	31,	32,	33,	34,	35,	36,	37,	38,	39,	40,	41,	42,	43,	44,	45,	46,	47,	48,	58,	59,	60,	61,	62,	63	}	,
		{6,	5,	5,	5,	4,	3,	0,	3,	5,	6,	8,	9,	10,	11,	12,	13,	14,	15,	16,	18,	19,	20,	21,	22,	23,	24,	25,	26,	27,	28,	29,	30,	31,	32,	33,	34,	35,	36,	37,	38,	39,	40,	41,	42,	43,	44,	45,	46,	47,	48,	58,	59,	60,	61,	62,	63	}	,
		{7,	6,	6,	6,	5,	4,	3,	0,	3,	5,	7,	8,	9,	10,	12,	13,	14,	15,	16,	17,	18,	19,	20,	21,	22,	24,	25,	26,	27,	28,	29,	30,	31,	32,	33,	34,	35,	36,	37,	38,	39,	40,	41,	42,	43,	44,	45,	46,	47,	48,	58,	59,	60,	61,	62,	63	}	,
		{8,	7,	7,	7,	6,	6,	5,	3,	0,	4,	6,	7,	8,	10,	11,	12,	13,	15,	16,	17,	18,	19,	20,	21,	22,	23,	24,	25,	26,	27,	28,	29,	30,	32,	33,	34,	35,	36,	37,	38,	39,	40,	41,	42,	43,	44,	45,	46,	47,	48,	58,	59,	60,	61,	62,	63	}	,
		{9,	8,	8,	8,	8,	7,	6,	5,	4,	0,	4,	6,	7,	9,	10,	12,	13,	14,	15,	16,	17,	18,	20,	21,	22,	23,	24,	25,	26,	27,	28,	29,	30,	31,	32,	33,	34,	35,	36,	37,	38,	40,	41,	42,	43,	44,	45,	46,	47,	48,	58,	59,	60,	61,	62,	63	}	,
		{10,9,	9,	9,	9,	8,	8,	7,	6,	4,	0,	4,	6,	8,	9,	11,	12,	13,	14,	16,	17,	18,	19,	20,	21,	22,	24,	25,	26,	27,	28,	29,	30,	31,	32,	33,	34,	35,	36,	37,	38,	39,	40,	41,	42,	43,	44,	45,	46,	47,	58,	59,	60,	61,	62,	63	}	,
		{11,10,	10,	10,	10,	9,	9,	8,	7,	6,	4,	0,	4,	6,	8,	10,	11,	12,	14,	15,	16,	17,	19,	20,	21,	22,	23,	24,	25,	26,	27,	28,	30,	31,	32,	33,	34,	35,	36,	37,	38,	39,	40,	41,	42,	43,	44,	45,	46,	47,	57,	58,	60,	61,	62,	63	}	,
		{12,11,	11,	11,	11,	10,	10,	9,	8,	7,	6,	4,	0,	5,	7,	9,	10,	12,	13,	14,	16,	17,	18,	19,	20,	21,	23,	24,	25,	26,	27,	28,	29,	30,	31,	32,	33,	35,	36,	37,	38,	39,	40,	41,	42,	43,	44,	45,	46,	47,	57,	58,	59,	60,	61,	62	}	,
		{13,12,	12,	12,	12,	12,	11,	10,	10,	9,	8,	6,	5,	0,	5,	7,	9,	10,	12,	13,	15,	16,	17,	18,	20,	21,	22,	23,	24,	25,	27,	28,	29,	30,	31,	32,	33,	34,	35,	36,	37,	38,	39,	40,	42,	43,	44,	45,	46,	47,	57,	58,	59,	60,	61,	62	}	,
		{14,13,	13,	13,	13,	13,	12,	12,	11,	10,	9,	8,	7,	5,	0,	5,	7,	9,	11,	12,	14,	15,	16,	18,	19,	20,	21,	23,	24,	25,	26,	27,	28,	29,	30,	32,	33,	34,	35,	36,	37,	38,	39,	40,	41,	42,	43,	44,	45,	46,	57,	58,	59,	60,	61,	62	}	,
		{15,14,	14,	14,	14,	14,	13,	13,	12,	12,	11,	10,	9,	7,	5,	0,	5,	8,	9,	11,	13,	14,	16,	17,	18,	20,	21,	22,	23,	24,	25,	27,	28,	29,	30,	31,	32,	33,	34,	36,	37,	38,	39,	40,	41,	42,	43,	44,	45,	46,	57,	58,	59,	60,	61,	62	}	,
		{16,15,	15,	15,	15,	15,	14,	14,	13,	13,	12,	11,	10,	9,	7,	5,	0,	5,	8,	10,	12,	13,	15,	16,	17,	19,	20,	21,	22,	24,	25,	26,	27,	28,	30,	31,	32,	33,	34,	35,	36,	37,	38,	39,	40,	42,	43,	44,	45,	46,	56,	57,	58,	59,	60,	61	}	,
		{17,16,	16,	16,	16,	16,	15,	15,	15,	14,	13,	12,	12,	10,	9,	8,	5,	0,	5,	8,	10,	12,	13,	15,	16,	18,	19,	20,	22,	23,	24,	25,	27,	28,	29,	30,	31,	32,	33,	35,	36,	37,	38,	39,	40,	41,	42,	43,	44,	45,	56,	57,	58,	59,	60,	61	}	,
		{18,17,	17,	17,	17,	17,	16,	16,	16,	15,	14,	14,	13,	12,	11,	9,	8,	5,	0,	6,	8,	10,	12,	14,	15,	17,	18,	20,	21,	22,	24,	25,	26,	27,	28,	30,	31,	32,	33,	34,	35,	36,	37,	39,	40,	41,	42,	43,	44,	45,	56,	57,	58,	59,	60,	61	}	,
		{19,18,	18,	18,	18,	18,	18,	17,	17,	16,	16,	15,	14,	13,	12,	11,	10,	8,	6,	0,	6,	8,	11,	12,	14,	16,	17,	19,	20,	21,	23,	24,	25,	26,	28,	29,	30,	31,	32,	34,	35,	36,	37,	38,	39,	40,	41,	42,	44,	45,	55,	56,	57,	59,	60,	61	}	,
		{20,19,	19,	19,	19,	19,	19,	18,	18,	17,	17,	16,	16,	15,	14,	13,	12,	10,	8,	6,	0,	6,	9,	11,	13,	15,	16,	18,	19,	21,	22,	23,	24,	26,	27,	28,	29,	31,	32,	33,	34,	35,	36,	38,	39,	40,	41,	42,	43,	44,	55,	56,	57,	58,	59,	60	}	,
		{21,20,	20,	20,	20,	20,	20,	19,	19,	18,	18,	17,	17,	16,	15,	14,	13,	12,	10,	8,	6,	0,	6,	9,	11,	13,	15,	16,	18,	20,	21,	22,	24,	25,	26,	28,	29,	30,	31,	32,	34,	35,	36,	37,	38,	39,	40,	42,	43,	44,	55,	56,	57,	58,	59,	60	}	,
		{22,21,	21,	21,	21,	21,	21,	20,	20,	20,	19,	19,	18,	17,	16,	16,	15,	13,	12,	11,	9,	6,	0,	6,	9,	11,	13,	15,	17,	18,	20,	21,	23,	24,	25,	27,	28,	29,	30,	32,	33,	34,	35,	36,	38,	39,	40,	41,	42,	43,	54,	55,	56,	57,	59,	60	}	,
		{23,22,	22,	22,	22,	22,	22,	21,	21,	21,	20,	20,	19,	18,	18,	17,	16,	15,	14,	12,	11,	9,	6,	0,	6,	9,	12,	14,	15,	17,	19,	20,	22,	23,	25,	26,	27,	28,	30,	31,	32,	33,	35,	36,	37,	38,	39,	40,	42,	43,	54,	55,	56,	57,	58,	59	}	,
		{24,23,	23,	23,	23,	23,	23,	22,	22,	22,	21,	21,	20,	20,	19,	18,	17,	16,	15,	14,	13,	11,	9,	6,	0,	7,	10,	12,	14,	16,	18,	19,	21,	22,	24,	25,	26,	28,	29,	30,	32,	33,	34,	35,	36,	38,	39,	40,	41,	42,	53,	54,	56,	57,	58,	59	}	,
		{25,24,	24,	24,	24,	24,	24,	24,	23,	23,	22,	22,	21,	21,	20,	20,	19,	18,	17,	16,	15,	13,	11,	9,	7,	0,	7,	10,	12,	14,	16,	18,	19,	21,	23,	24,	25,	27,	28,	29,	31,	32,	33,	34,	36,	37,	38,	39,	40,	42,	53,	54,	55,	56,	57,	58	}	,
		{26,25,	25,	25,	25,	25,	25,	25,	24,	24,	24,	23,	23,	22,	21,	21,	20,	19,	18,	17,	16,	15,	13,	12,	10,	7,	0,	7,	10,	12,	14,	16,	18,	20,	21,	23,	24,	26,	27,	29,	30,	31,	32,	34,	35,	36,	37,	39,	40,	41,	52,	54,	55,	56,	57,	58	}	,
		{27,26,	26,	26,	26,	26,	26,	26,	25,	25,	25,	24,	24,	23,	23,	22,	21,	20,	20,	19,	18,	16,	15,	14,	12,	10,	7,	0,	7,	10,	13,	15,	17,	18,	20,	22,	23,	25,	26,	28,	29,	30,	32,	33,	34,	36,	37,	38,	39,	40,	52,	53,	54,	55,	56,	58	}	,
		{28,27,	27,	27,	27,	27,	27,	27,	26,	26,	26,	25,	25,	24,	24,	23,	22,	22,	21,	20,	19,	18,	17,	15,	14,	12,	10,	7,	0,	7,	10,	13,	15,	17,	19,	21,	22,	24,	25,	27,	28,	29,	31,	32,	33,	35,	36,	37,	38,	40,	51,	53,	54,	55,	56,	57	}	,
		{29,28,	28,	28,	28,	28,	28,	28,	27,	27,	27,	26,	26,	25,	25,	24,	24,	23,	22,	21,	21,	20,	18,	17,	16,	14,	12,	10,	7,	0,	7,	10,	13,	15,	17,	19,	21,	22,	24,	26,	27,	28,	30,	31,	33,	34,	35,	36,	38,	39,	51,	52,	53,	54,	55,	57	}	,
		{30,29,	29,	29,	29,	29,	29,	29,	28,	28,	28,	27,	27,	27,	26,	25,	25,	24,	24,	23,	22,	21,	20,	19,	18,	16,	14,	13,	10,	7,	0,	7,	11,	13,	16,	18,	19,	21,	23,	24,	26,	27,	29,	30,	32,	33,	34,	36,	37,	38,	50,	51,	53,	54,	55,	56	}	,
		{31,30,	30,	30,	30,	30,	30,	30,	29,	29,	29,	28,	28,	28,	27,	27,	26,	25,	25,	24,	23,	22,	21,	20,	19,	18,	16,	15,	13,	10,	7,	0,	7,	11,	13,	16,	18,	20,	21,	23,	25,	26,	28,	29,	31,	32,	33,	35,	36,	37,	50,	51,	52,	53,	54,	55	}	,
		{32,31,	31,	31,	31,	31,	31,	31,	30,	30,	30,	30,	29,	29,	28,	28,	27,	27,	26,	25,	24,	24,	23,	22,	21,	19,	18,	17,	15,	13,	11,	7,	0,	8,	11,	14,	16,	18,	20,	22,	24,	25,	27,	28,	30,	31,	33,	34,	35,	37,	49,	50,	51,	53,	54,	55	}	,
		{33,32,	32,	32,	32,	32,	32,	32,	32,	31,	31,	31,	30,	30,	29,	29,	28,	28,	27,	26,	26,	25,	24,	23,	22,	21,	20,	18,	17,	15,	13,	11,	8,	0,	8,	11,	14,	16,	18,	20,	22,	24,	25,	27,	29,	30,	32,	33,	34,	36,	48,	50,	51,	52,	53,	54	}	,
		{34,33,	33,	33,	33,	33,	33,	33,	33,	32,	32,	32,	31,	31,	30,	30,	30,	29,	28,	28,	27,	26,	25,	25,	24,	23,	21,	20,	19,	17,	16,	13,	11,	8,	0,	8,	11,	14,	16,	19,	21,	22,	24,	26,	27,	29,	30,	32,	33,	35,	48,	49,	50,	51,	53,	54	}	,
		{35,34,	34,	34,	34,	34,	34,	34,	34,	33,	33,	33,	32,	32,	32,	31,	31,	30,	30,	29,	28,	28,	27,	26,	25,	24,	23,	22,	21,	19,	18,	16,	14,	11,	8,	0,	8,	12,	14,	17,	19,	21,	23,	24,	26,	28,	29,	31,	32,	34,	47,	48,	49,	51,	52,	53	}	,
		{36,35,	35,	35,	35,	35,	35,	35,	35,	34,	34,	34,	33,	33,	33,	32,	32,	31,	31,	30,	29,	29,	28,	27,	26,	25,	24,	23,	22,	21,	19,	18,	16,	14,	11,	8,	0,	8,	12,	15,	17,	19,	21,	23,	25,	27,	28,	30,	31,	33,	46,	48,	49,	50,	51,	52	}	,
		{37,36,	36,	36,	36,	36,	36,	36,	36,	35,	35,	35,	35,	34,	34,	33,	33,	32,	32,	31,	31,	30,	29,	28,	28,	27,	26,	25,	24,	22,	21,	20,	18,	16,	14,	12,	8,	0,	8,	12,	15,	17,	19,	21,	23,	25,	27,	28,	30,	32,	45,	47,	48,	49,	50,	52	}	,
		{38,37,	37,	37,	37,	37,	37,	37,	37,	36,	36,	36,	36,	35,	35,	34,	34,	33,	33,	32,	32,	31,	30,	30,	29,	28,	27,	26,	25,	24,	23,	21,	20,	18,	16,	14,	12,	8,	0,	8,	12,	15,	17,	20,	22,	24,	25,	27,	29,	30,	45,	46,	47,	48,	50,	51	}	,
		{39,38,	38,	38,	38,	38,	38,	38,	38,	37,	37,	37,	37,	36,	36,	36,	35,	35,	34,	34,	33,	32,	32,	31,	30,	29,	29,	28,	27,	26,	24,	23,	22,	20,	19,	17,	15,	12,	8,	0,	8,	12,	15,	18,	20,	22,	24,	26,	27,	29,	44,	45,	46,	48,	49,	50	}	,
		{40,39,	39,	39,	39,	39,	39,	39,	39,	38,	38,	38,	38,	37,	37,	37,	36,	36,	35,	35,	34,	34,	33,	32,	32,	31,	30,	29,	28,	27,	26,	25,	24,	22,	21,	19,	17,	15,	12,	8,	0,	9,	12,	15,	18,	20,	22,	24,	26,	28,	43,	44,	46,	47,	48,	49	}	,
		{41,40,	40,	40,	40,	40,	40,	40,	40,	40,	39,	39,	39,	38,	38,	38,	37,	37,	36,	36,	35,	35,	34,	33,	33,	32,	31,	30,	29,	28,	27,	26,	25,	24,	22,	21,	19,	17,	15,	12,	9,	0,	9,	12,	15,	18,	20,	22,	24,	26,	42,	43,	45,	46,	47,	49	}	,
		{42,41,	41,	41,	41,	41,	41,	41,	41,	41,	40,	40,	40,	39,	39,	39,	38,	38,	37,	37,	36,	36,	35,	35,	34,	33,	32,	32,	31,	30,	29,	28,	27,	25,	24,	23,	21,	19,	17,	15,	12,	9,	0,	9,	13,	16,	18,	21,	23,	25,	41,	42,	44,	45,	46,	48	}	,
		{43,42,	42,	42,	42,	42,	42,	42,	42,	42,	41,	41,	41,	40,	40,	40,	39,	39,	39,	38,	38,	37,	36,	36,	35,	34,	34,	33,	32,	31,	30,	29,	28,	27,	26,	24,	23,	21,	20,	18,	15,	12,	9,	0,	9,	13,	16,	18,	21,	23,	40,	41,	43,	44,	46,	47	}	,
		{44,43,	43,	43,	43,	43,	43,	43,	43,	43,	42,	42,	42,	42,	41,	41,	40,	40,	40,	39,	39,	38,	38,	37,	36,	36,	35,	34,	33,	33,	32,	31,	30,	29,	27,	26,	25,	23,	22,	20,	18,	15,	13,	9,	0,	9,	13,	16,	19,	21,	39,	40,	42,	43,	45,	46	}	,
		{45,44,	44,	44,	44,	44,	44,	44,	44,	44,	43,	43,	43,	43,	42,	42,	42,	41,	41,	40,	40,	39,	39,	38,	38,	37,	36,	36,	35,	34,	33,	32,	31,	30,	29,	28,	27,	25,	24,	22,	20,	18,	16,	13,	9,	0,	9,	13,	16,	19,	38,	39,	41,	42,	44,	45	}	,
		{46,45,	45,	45,	45,	45,	45,	45,	45,	45,	44,	44,	44,	44,	43,	43,	43,	42,	42,	41,	41,	40,	40,	39,	39,	38,	37,	37,	36,	35,	34,	33,	33,	32,	30,	29,	28,	27,	25,	24,	22,	20,	18,	16,	13,	9,	0,	9,	13,	16,	36,	38,	40,	41,	43,	44	}	,
		{47,46,	46,	46,	46,	46,	46,	46,	46,	46,	45,	45,	45,	45,	44,	44,	44,	43,	43,	42,	42,	42,	41,	40,	40,	39,	39,	38,	37,	36,	36,	35,	34,	33,	32,	31,	30,	28,	27,	26,	24,	22,	21,	18,	16,	13,	9,	0,	9,	13,	35,	37,	38,	40,	41,	43	}	,
		{48,47,	47,	47,	47,	47,	47,	47,	47,	47,	46,	46,	46,	46,	45,	45,	45,	44,	44,	44,	43,	43,	42,	42,	41,	40,	40,	39,	38,	38,	37,	36,	35,	34,	33,	32,	31,	30,	29,	27,	26,	24,	23,	21,	19,	16,	13,	9,	0,	9,	34,	36,	37,	39,	40,	42	}	,
		{49,48,	48,	48,	48,	48,	48,	48,	48,	48,	47,	47,	47,	47,	46,	46,	46,	45,	45,	45,	44,	44,	43,	43,	42,	42,	41,	40,	40,	39,	38,	37,	37,	36,	35,	34,	33,	32,	30,	29,	28,	26,	25,	23,	21,	19,	16,	13,	9,	0,	32,	34,	36,	37,	39,	41	}	,
		{50,49,	49,	49,	49,	49,	49,	49,	49,	49,	48,	48,	48,	48,	48,	47,	47,	47,	46,	46,	45,	45,	44,	44,	43,	43,	42,	42,	41,	40,	40,	39,	38,	37,	36,	35,	34,	33,	32,	31,	30,	28,	27,	25,	23,	21,	19,	17,	14,	9,	31,	33,	34,	36,	38,	39	}	,
		{51,50,	50,	50,	50,	50,	50,	50,	50,	50,	50,	49,	49,	49,	49,	48,	48,	48,	47,	47,	46,	46,	46,	45,	45,	44,	43,	43,	42,	41,	41,	40,	39,	38,	38,	37,	36,	35,	34,	32,	31,	30,	28,	27,	25,	24,	22,	19,	17,	14,	29,	31,	33,	35,	36,	38	}	,
		{52,51,	51,	51,	51,	51,	51,	51,	51,	51,	51,	50,	50,	50,	50,	49,	49,	49,	48,	48,	48,	47,	47,	46,	46,	45,	45,	44,	43,	43,	42,	41,	40,	40,	39,	38,	37,	36,	35,	34,	33,	31,	30,	29,	27,	26,	24,	22,	20,	17,	27,	29,	31,	33,	35,	37	}	,
		{53,52,	52,	52,	52,	52,	52,	52,	52,	52,	52,	51,	51,	51,	51,	50,	50,	50,	49,	49,	49,	48,	48,	47,	47,	46,	46,	45,	45,	44,	43,	42,	42,	41,	40,	39,	38,	37,	36,	35,	34,	33,	32,	30,	29,	28,	26,	24,	22,	20,	25,	28,	30,	32,	34,	35	}	,
		{54,53,	53,	53,	53,	53,	53,	53,	53,	53,	53,	52,	52,	52,	52,	51,	51,	51,	50,	50,	50,	49,	49,	48,	48,	47,	47,	46,	46,	45,	44,	44,	43,	42,	41,	41,	40,	39,	38,	37,	36,	35,	33,	32,	31,	29,	28,	26,	24,	22,	23,	26,	28,	30,	32,	34	}	,
		{55,54,	54,	54,	54,	54,	54,	54,	54,	54,	54,	53,	53,	53,	53,	52,	52,	52,	51,	51,	51,	50,	50,	49,	49,	48,	48,	47,	47,	46,	46,	45,	44,	44,	43,	42,	41,	40,	39,	38,	37,	36,	35,	34,	33,	31,	30,	28,	26,	24,	21,	23,	26,	28,	30,	32	}	,
		{56,55,	55,	55,	55,	55,	55,	55,	55,	55,	55,	54,	54,	54,	54,	53,	53,	53,	53,	52,	52,	51,	51,	51,	50,	50,	49,	49,	48,	47,	47,	46,	45,	45,	44,	43,	42,	42,	41,	40,	39,	38,	37,	35,	34,	33,	31,	30,	28,	27,	18,	21,	24,	26,	28,	30	}	,
		{57,56,	56,	56,	56,	56,	56,	56,	56,	56,	56,	55,	55,	55,	55,	54,	54,	54,	54,	53,	53,	52,	52,	52,	51,	51,	50,	50,	49,	49,	48,	47,	47,	46,	45,	44,	44,	43,	42,	41,	40,	39,	38,	37,	36,	34,	33,	32,	30,	29,	15,	18,	21,	24,	26,	29	}	,
		{58,57,	57,	57,	57,	57,	57,	57,	57,	57,	57,	56,	56,	56,	56,	56,	55,	55,	55,	54,	54,	54,	53,	53,	52,	52,	51,	51,	50,	50,	49,	49,	48,	47,	46,	46,	45,	44,	43,	42,	42,	41,	40,	38,	37,	36,	35,	33,	32,	31,	10,	15,	18,	21,	24,	27	}	,
		{59,58,	58,	58,	58,	58,	58,	58,	58,	58,	58,	57,	57,	57,	57,	57,	56,	56,	56,	55,	55,	55,	54,	54,	53,	53,	52,	52,	51,	51,	50,	50,	49,	48,	48,	47,	46,	45,	45,	44,	43,	42,	41,	40,	39,	38,	36,	35,	34,	32,	0,	10,	15,	19,	22,	24	}	,
		{60,59,	59,	59,	59,	59,	59,	59,	59,	59,	59,	58,	58,	58,	58,	58,	57,	57,	57,	56,	56,	56,	55,	55,	54,	54,	54,	53,	53,	52,	51,	51,	50,	50,	49,	48,	48,	47,	46,	45,	44,	43,	42,	41,	40,	39,	38,	37,	36,	34,	10,	0,	11,	15,	19,	22	}	,
		{61,60,	60,	60,	60,	60,	60,	60,	60,	60,	60,	60,	59,	59,	59,	59,	58,	58,	58,	57,	57,	57,	56,	56,	56,	55,	55,	54,	54,	53,	53,	52,	51,	51,	50,	49,	49,	48,	47,	46,	46,	45,	44,	43,	42,	41,	40,	38,	37,	36,	15,	11,	0,	11,	15,	19	}	,
		{62,61,	61,	61,	61,	61,	61,	61,	61,	61,	61,	61,	60,	60,	60,	60,	59,	59,	59,	59,	58,	58,	57,	57,	57,	56,	56,	55,	55,	54,	54,	53,	53,	52,	51,	51,	50,	49,	48,	48,	47,	46,	45,	44,	43,	42,	41,	40,	39,	37,	19,	15,	11,	0,	11,	15	}	,
		{63,62,	62,	62,	62,	62,	62,	62,	62,	62,	62,	62,	61,	61,	61,	61,	60,	60,	60,	60,	59,	59,	59,	58,	58,	57,	57,	56,	56,	55,	55,	54,	54,	53,	53,	52,	51,	50,	50,	49,	48,	47,	46,	46,	45,	44,	43,	41,	40,	39,	22,	19,	15,	11,	0,	11	}};

const unsigned char inicio [] = {
0x00, 0x00, 0x00, 0xF8, 0xF8, 0xF8, 0xF8, 0x38, 0x38, 0x38, 0xF8, 0xF8, 0xF0, 0xE0, 0x00, 0x00,
0x00, 0x00, 0xE0, 0xF8, 0xF8, 0x78, 0xF8, 0xF8, 0xC0, 0x00, 0x00, 0x00, 0x00, 0xF8, 0xF8, 0xF8,
0xF8, 0x38, 0x38, 0x78, 0xF8, 0xF0, 0xE0, 0xC0, 0x00, 0x00, 0x00, 0xC0, 0xF8, 0xF8, 0x78, 0xF8,
0xF8, 0xC0, 0x00, 0x00, 0x00, 0x00, 0xF8, 0xF8, 0xF8, 0xF8, 0x38, 0x38, 0x38, 0xF8, 0xF8, 0xF0,
0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xF0, 0xF0, 0xF0,
0xF0, 0xF0, 0xE0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
0xC0, 0xC0, 0xC0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x07, 0x0F, 0x3F, 0xFF, 0xFF, 0xFB, 0xE1, 0x80, 0xC0,
0xF8, 0xFF, 0xFF, 0x3F, 0x3B, 0x38, 0x3B, 0x3F, 0xFF, 0xFE, 0xF0, 0x80, 0x00, 0xFF, 0xFF, 0xFF,
0xFF, 0xE0, 0xE0, 0xE0, 0xF9, 0xFF, 0x7F, 0x1F, 0x80, 0xF0, 0xFE, 0xFF, 0x3F, 0x3B, 0x38, 0x39,
0x3F, 0xFF, 0xFF, 0xF8, 0xC0, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x07, 0x0F, 0x1F, 0x7F, 0xFF, 0xFB,
0xE1, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x1F, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0xC0, 0x87,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 0xC0, 0x00, 0x00, 0xE0, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x80, 0xFF, 0x87, 0x0E, 0x18, 0x30, 0xE1, 0xFF, 0x00, 0x88, 0xFB, 0x80, 0x00, 0x78,
0xC0, 0x84, 0x88, 0x00, 0x78, 0x84, 0x84, 0x78, 0x30, 0x00, 0xFF, 0xFF, 0x00, 0x60, 0xE4, 0x54,
0xF8, 0x80, 0x40, 0x98, 0xB4, 0x7C, 0x00, 0x00, 0x00, 0x00, 0x81, 0xFF, 0x81, 0x11, 0x0F, 0x06,
0x40, 0xE8, 0x54, 0xFC, 0xF0, 0x00, 0xD8, 0xB4, 0xB4, 0x60, 0x00, 0xFF, 0x84, 0x00, 0x88, 0xF8,
0x88, 0x08, 0x60, 0xE0, 0x54, 0xF8, 0x80, 0x00, 0xF8, 0xF8, 0x08, 0x0C, 0xF8, 0x80, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xF0, 0xF8, 0xF8, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0x7F, 0x1F, 0x9F, 0xCF, 0xCF, 0xCF, 0xEC, 0xE4, 0xE7, 0xE7, 0xE7, 0xE7, 0xF7,
0xF3, 0xF3, 0xF1, 0xF0, 0xF0, 0xF0, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0xE0, 0xE0, 0x20, 0x20, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x20, 0xE0, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xF0, 0xE0, 0x07, 0x0F, 0xCF, 0xCF, 0xCF, 0xCF, 0xCF, 0xEF, 0xE7, 0xEF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x1F, 0x1F, 0x00, 0x03, 0x00, 0x12, 0x1F, 0x12, 0x01, 0x18, 0x18, 0x15, 0x1F,
0x1E, 0x00, 0x1E, 0x1F, 0x01, 0x03, 0x1F, 0x00, 0x0C, 0x1E, 0x11, 0x01, 0x00, 0x12, 0x1F, 0x10,
0x00, 0x17, 0x05, 0x1D, 0x08, 0x0C, 0x1E, 0x11, 0x01, 0x00, 0x0E, 0x1F, 0x01, 0x11, 0x1F, 0x00,
0x00, 0x00, 0x00, 0x00, 0x1F, 0x10, 0x00, 0x00, 0x10, 0x04, 0x1F, 0x11, 0x11, 0x1F, 0x04, 0x00,
0xFF, 0x12, 0x01, 0x13, 0x0E, 0x00, 0x0E, 0x15, 0x15, 0x07, 0x00, 0x13, 0x19, 0x0D, 0x03, 0x11,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x77, 0xE7, 0xF7, 0xF3, 0xF9, 0xF8,
0xF9, 0xF9, 0xF9, 0xF9, 0xFC, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x08, 0xF8, 0xF8, 0x08, 0x00, 0xC0, 0x40, 0x40, 0xC0, 0x80, 0x08, 0xF8, 0xC8, 0x40,
0xC0, 0xC0, 0x00, 0x00, 0x00, 0xC0, 0xC0, 0x00, 0x40, 0xC0, 0xC0, 0x40, 0xC0, 0x80, 0x00, 0x00,
0x00, 0x00, 0xE0, 0x10, 0x08, 0x08, 0x08, 0x88, 0x98, 0x80, 0x00, 0xC0, 0x50, 0x08, 0xC0, 0x80,
0x40, 0xC0, 0xC0, 0x40, 0xC0, 0x80, 0x40, 0xC0, 0xC0, 0x00, 0x80, 0xC0, 0x00, 0xC0, 0x80, 0x40,
0x01, 0x81, 0xC0, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x1F, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3F, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x20, 0x1F, 0x0F, 0x00, 0x01, 0x03, 0x04, 0x04, 0x03, 0x01, 0x04, 0x07, 0x04, 0x00,
0x07, 0x07, 0x00, 0x07, 0x07, 0x02, 0x07, 0x04, 0x04, 0x07, 0x04, 0x00, 0x07, 0x07, 0x00, 0x00,
0x00, 0x00, 0x01, 0x03, 0x04, 0x04, 0x04, 0x06, 0x03, 0x00, 0x00, 0x07, 0x04, 0x04, 0x03, 0x01,
0x04, 0x07, 0x04, 0x04, 0x07, 0x07, 0x00, 0x07, 0x07, 0x00, 0x03, 0x06, 0x04, 0x04, 0x00, 0x04,
0x06, 0x05, 0x04, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03,
0x03, 0x07, 0x07, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F,
0x0F, 0x07, 0x07, 0x07, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


const char tabla[1024] = {     
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0xC0, 0x40, 0x60, 0x60, 0x20, 0x30, 0x10, 0x10, 0x18, 0x18,
		0x08, 0x08, 0x0C, 0x0C, 0x04, 0x04, 0x04, 0x04, 0x06, 0x06, 0x06, 0x06, 0x02, 0x02, 0x02, 0xFE,
		0xFE, 0x02, 0x02, 0x02, 0x02, 0x06, 0x06, 0x06, 0x06, 0x04, 0x04, 0x04, 0x04, 0x0C, 0x08, 0x08,
		0x08, 0x18, 0x10, 0x10, 0x30, 0x20, 0x20, 0x60, 0x40, 0xC0, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0x60, 0x20, 0x10, 0x18, 0x08, 0x0C,
		0x06, 0x02, 0x03, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0xC0, 0xC0, 0xC0, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0xC0,
		0xC1, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0xC0, 0xC0, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x03, 0x03, 0x06,
		0x04, 0x0C, 0x18, 0x10, 0x30, 0x60, 0x40, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
		0xC0, 0x60, 0x30, 0x18, 0x3C, 0x66, 0xC3, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x80, 0xC0, 0x40, 0x60, 0x30, 0x10, 0x18, 0x08, 0x0C, 0x04, 0x06, 0x06, 0x02, 0x03,
		0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F,
		0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01,
		0x03, 0x02, 0x02, 0x06, 0x04, 0x0C, 0x08, 0x18, 0x10, 0x30, 0x20, 0x60, 0xC0, 0x80, 0x80, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81, 0xC3, 0x66, 0x3C, 0x18, 0x30, 0x60, 0xC0,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x60, 0x38, 0x0C, 0x06, 0x03,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x80, 0xC0, 0x60, 0x30, 0x78, 0xCC, 0x86,
		0x02, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0x40,
		0x60, 0x60, 0x20, 0x30, 0x10, 0x10, 0x18, 0x18, 0x08, 0x08, 0x08, 0x0C, 0x0C, 0x0C, 0x0C, 0xFC,
		0xFC, 0x0C, 0x0C, 0x0C, 0x0C, 0x08, 0x08, 0x08, 0x08, 0x18, 0x10, 0x10, 0x30, 0x20, 0x20, 0x60,
		0x40, 0xC0, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03,
		0x86, 0xCC, 0x78, 0x30, 0x60, 0xC0, 0x80, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x01, 0x03, 0x0E, 0x18, 0x70, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x78, 0x1C, 0x07, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0xC0, 0x70, 0x18, 0x0C, 0x07, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x03, 0x02, 0x00, 0x80, 0xC0, 0x60, 0xF0, 0x98, 0x0C, 0x04, 0x06, 0x03, 0x01, 0x01, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x83,
		0x83, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x01, 0x03, 0x02, 0x06, 0x0C, 0x98, 0xF0, 0x70, 0x60, 0xC0, 0x00, 0x02, 0x03,
		0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x06, 0x1C, 0x30, 0xE0, 0x80, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x0E, 0x38, 0xE0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0xE0, 0x3C, 0x0F, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0xC0, 0x70, 0x1C, 0x07, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x70,
		0x18, 0x0E, 0x03, 0x01, 0x00, 0x00, 0x00, 0x01, 0x03, 0x06, 0x04, 0x00, 0x00, 0x80, 0xC0, 0xE0,
		0x30, 0x18, 0x08, 0x0C, 0x04, 0x06, 0x02, 0x03, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x3F,
		0x3F, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x03, 0x03, 0x02, 0x06, 0x04, 0x0C, 0x18, 0x10,
		0xF0, 0xE0, 0xC0, 0x80, 0x00, 0x04, 0x06, 0x03, 0x01, 0x00, 0x00, 0x00, 0x01, 0x03, 0x06, 0x1C,
		0x30, 0xE0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x0E, 0x38, 0xE0,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x1E, 0xF0, 0x80, 0x00, 0x00,
		0x00, 0xE0, 0xFE, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x3E,
		0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x3C, 0x07, 0x01, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x70, 0x1C, 0x06, 0x03, 0x01, 0x00, 0x01,
		0x03, 0x06, 0x0C, 0x08, 0x00, 0x00, 0x80, 0xC0, 0x60, 0x20, 0x30, 0x10, 0x10, 0x18, 0x08, 0xF8,
		0xF8, 0x08, 0x18, 0x18, 0x10, 0x30, 0x20, 0x60, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x0C, 0x06, 0x03,
		0x01, 0x00, 0x00, 0x01, 0x07, 0x0C, 0x38, 0xE0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x03, 0x1E, 0xF0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x1F, 0xF8, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x1F, 0xF8, 0x00,
		0x00, 0xFF, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xFC, 0xBF, 0x80,
		0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xFE, 0x87, 0x80, 0x80, 0x80, 0x80,
		0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xE0, 0xFE, 0x83, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
		0x80, 0x80, 0x80, 0xF0, 0xBC, 0x87, 0x81, 0x81, 0x83, 0x86, 0x8C, 0x98, 0x80, 0xC0, 0x60, 0x27,
		0x37, 0x20, 0x60, 0xC8, 0x98, 0x8C, 0x86, 0x83, 0x81, 0x81, 0x83, 0x8E, 0xF8, 0x80, 0x80, 0x80,
		0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x81, 0x9F, 0xF8, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
		0x80, 0x80, 0x80, 0x80, 0x80, 0xFF, 0xF0, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
		0x80, 0x83, 0xFF, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xBF, 0xFE
};

unsigned long TIEMPOX;
unsigned int dato;
unsigned char bandera=0;
unsigned char x1,r1,y1;
unsigned char M[512];

//-------Funciones LCD---------
void tiempo(long t){
	unsigned long k;
	for(k=0;k<t;k++);
	return;	
}

void lcd_pulso(){
   GLCD_E=1;       // Pulse the enable pin
   tiempo(2);
   GLCD_E=0;
   tiempo(2);
   return;
}

void lcd_comando(char data2){
   GLCD_RS=0;
   PTED=data2;
   lcd_pulso();
   return;
}

void lcd_dato(char data1){
   GLCD_RS=1;
   PTED=data1;
   lcd_pulso();
   return;
}
void lcd_chip(char b){
	if(b==1){
    GLCD_CS1=0;      // Reset the chip select lines
    GLCD_CS2=1;
}else{
	 GLCD_CS1=1;      // Reset the chip select lines
	 GLCD_CS2=0;
}
   return;
}


void lcd_init(){
         
     GLCD_RST=1;	//Reinicia LCD
     tiempo(2000);
     GLCD_RST=0;
     tiempo(2000);
     GLCD_RST=1;
     tiempo(50000);

     lcd_chip(0); //Primera Mitad Pantalla
     lcd_comando(0x3E);
     lcd_comando(0xC0);// Linea de desplegado inicial en cero
     lcd_comando(0xB8);// Y=0
     lcd_comando(0x40);// X=0
     lcd_comando(0x3F);
     
     lcd_chip(1);// Segunda Mitad Pantalla
     lcd_comando(0x3E);//Apaga el display
     lcd_comando(0xC0);// Linea de desplegado inicial en cero
     lcd_comando(0xB8);// Y=0
     lcd_comando(0x40);// X=0
     lcd_comando(0x3F);
     return;
}
void lcd_inicio(){
   unsigned char cont,w;
   unsigned int index=0;//llega a 1024
   for(cont=0;cont<8;cont++){
   lcd_chip(0);//Primera Mitad
   lcd_comando(0b10111000| cont);//Pagina X seleccionada
   lcd_comando(0b11000000);//Linea 0
   for(w=0;w<64;w++){
   lcd_dato(inicio[index]);
   index++;
   }
   lcd_chip(1);//Primera Mitad
   lcd_comando(0b10111000| cont);//Pagina X seleccionada
   lcd_comando(0b11000000);//Linea 0
   for(w=0;w<64;w++){
   lcd_dato(inicio[index]);
   index++;
   }
   }
   return;
}


void lcd_radar(){
   unsigned char cont,w;
   unsigned int index=0;//llega a 1024
   for(cont=0;cont<8;cont++){
   lcd_chip(0);//Primera Mitad
   lcd_comando(0b10111000| cont);//Pagina X seleccionada
   lcd_comando(0b11000000);//Linea 0
   for(w=0;w<64;w++){
   lcd_dato(tabla[index]);
   index++;
   }
   lcd_chip(1);//Primera Mitad
   lcd_comando(0b10111000| cont);//Pagina X seleccionada
   lcd_comando(0b11000000);//Linea 0
   for(w=0;w<64;w++){
   lcd_dato(tabla[index]);
   index++;
   }
   }
   return;
}

void lcd_clear(){
	char x, y,aux;
	GLCD_RS=0;
   for(y=0;y<8;y++){   
      lcd_chip(0);//Primera Mitad
      aux=0b10111000| y;
      lcd_comando(aux);//Pagina X seleccionada
      lcd_comando(0b11000000);//Linea 0
      for(x=0;x<64;x++){
         lcd_dato(0x00);
      }
      lcd_chip(1);//Segunda Mitad
      aux=0b10111000| y;
      lcd_comando(aux);//Pagina X seleccionada
      lcd_comando(0b11000000);//Linea 0
      for(x=0;x<64;x++){
         lcd_dato(0b00000000);//Se incrementan las posiciones y automaticamente
      }
   }
   return;
}



//---------Funciones motor--------------

void motor(void){
	if (TPM2C1V==4694){
		bandera=1;
	}
	if(TPM2C1V==1400){
		bandera=0;
	}
	if (bandera==1){
		TPM2C1V=TPM2C1V-18;
		dato=dato-1;
	}
	else{
		TPM2C1V=TPM2C1V+18;
		dato=dato+1;
	}
  } 


void vaciar(void){
	int p;
	for(p=0;p<512;p++){
	 M[p]=0b00000000;//Se incrementan las posiciones y automaticamente
	 }
	return;
}
void Gdato(void){
	//unsigned char temp;
	//unsigned int temp1;
	r1=(unsigned char)(64*TIEMPOX)/46920;//Obtengo el radio 64->4mts
	x1=(unsigned char)(127*(dato)/183);//128 max
	y1=63-(unsigned char)ra[r1,x1]*63/64;//maximo valor 64
	/*if(temp==0){
		N[temp1]=0b00000001;
	}else if(temp==1){
		N[temp1]=0b00000010;
	}else if(temp==2){
		N[temp1]=0b00000100;
	}else if(temp==3){
		N[temp1]=0b00001000;
	}else if(temp==4){
		N[temp1]=0b00010000;
	}else if(temp==5){
		N[temp1]=0b00100000;
	}else if(temp==6){
		N[temp1]=0b01000000;
	}else{ 	
		N[temp1]=0b10000000;
	}*/
	return;
}

void main(void) 
{
	
	SOPT1 = 0;
	MCGTRM = 0xAA; 
	MCGC1 = 6;
	while(MCGSC_LOCK == 0){}; 
	PTEDD=0xFF;//Salidas para datos LCD
	PTBDD=0xFF;//Salidas para control LCD
	PTCDD=0b00100000;
	PTFDD=0b00100001;
	PTFPE=0b00000010;
	
	LRESET=1;
	tiempo(500);
	LRESET=0;
	
	 lcd_init();//Inicializa LCD
	 //lcd_clear();//Limpia la lcd 
	 lcd_inicio();
	 tiempo(50000);
	 tiempo(50000);
	 tiempo(50000);
	 lcd_radar();// PONE IMAGEN DEL RADAR
	
	TPM1SC=0b00001010; //TIM1 Pre*4, Int. deshabilitada, TIM deshabilitado
	TPM1MOD=50000;
	TPM1C2SC=0b00101000;
	TPM1C2V=20;//pulso para
	
	TPM1C3SC=0b00001100; 
	
	TPM2SC = 0b00001010; //TIM2 Pre*4, Int. deshabilitada, TIM deshabilitado
	TPM2MOD=40000;   
	TPM2C1SC=0b00101000;
	TPM2C1V=1400;//pulso para
	
	dato=0;
	
	TPM1SC_CLKSA = 1;
	TPM2SC_CLKSA = 1;
	EnableInterrupts;//CLI
	
	for(;;) 
	{
		TPM1C3SC=0b00000100;
		while(TPM1C3SC_CH3F==0);
		TPM1C3SC_CH3F=1;
		
		TPM1CNT=0;
		
		TPM1C3SC=0b00001000;
		while(TPM1C3SC_CH3F==0);
		TPM1C3SC_CH3F=1;
		
		TIEMPOX=TPM1C3V;
		Gdato();
		motor();
	}
}
