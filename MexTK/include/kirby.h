#ifndef MEX_H_KIRBY
#define MEX_H_KIRBY

#include "structs.h"
#include "datatypes.h"
#include "obj.h"
#include "gx.h"
#include "effects.h"
#include "match.h"
#include "collision.h"

#define Kirby_GetStateID(state) (state + 400) // if you need to check for an m-ex copy state, start at 400

struct FtVarKirby
{
    int charVar1;           // 0x222c
    int charVar2;           // 0x2230
    int charVar3;           // 0x2234
    int copy_index;         // 0x2238
    JOBJ *copy_jobj;        // 0x223c
    FtParts dobj_lookup;    // 0x2240
    FtPartsVis ftparts_vis; // 0x2250
};

void AS_CaptureWaitKirbyItem(GOBJ *item);
void AS_CaptureWaitKirby(GOBJ *);
void Kirby_ResetCopyCharVars(GOBJ *fighter, int ft_kind);
void Kirby_ExecuteAbilityLossFunction(GOBJ *fighter, int ft_kind);

#endif
