#ifndef MEX_H_1P
#define MEX_H_1P

#include "structs.h"
#include "datatypes.h"

struct ClassicLineupDesc
{
    s16 stage_index; // 0x0
    s8 ckind[5];     // 0x2, read @ 801b2bf0
};

struct ClassicLineup // array of these @
{
    u8 index;                      // 0x0, 13 is terminator @ 801b2c8c
    u8 flags;                      // 0x1, 0x08 corresponds to array1, 0x02 corresponds to array2, and 0x01 corresponds to array3, 0 corresponds to array4, 0x80 will refer to the below value (x2), 0x20 always uses lineupdesc @ 803ddf88 (looks like master hand)
    u8 x2;                         // 0x2, enum check for flag 0x80, 1 will use lineupdesc @ 804d4318, 2 will use lineupdesc @ 804d4320, 3 will use 804d4328
    u16 x4;                        // 0x4
    u16 x6;                        // 0x6
    u8 x8;                         // 0x8
    ClassicLineupDesc *lineupdesc; // 0xc, checked constantly in ClassicMode_DefineStagesAndCharValue
};

// variables
static ClassicLineup *stc_clsc_lineup = 0x803ddec8;
static u8 *stc_clsc_shuffledorder1 = 0x80490900; // 9 elements
static u8 *stc_clsc_shuffledorder2 = 0x804908f4; // 10 elements
static u8 *stc_clsc_shuffledorder3 = 0x804908d4; // 27 elements
static u8 *stc_clsc_shuffledorder4 = 0x804908ac; // 38 elements

// functions
ClassicLineupDesc *ClassicMode_GetNextNonDuplicateLineupDesc(ClassicLineupDesc *lineupdesc, s8 *shuffled_order, ClassicLineup *final_lineup); // finds and returns a ptr to the first lineupdesc without a character that matches p1. checks in the order of shuffled order

#endif
