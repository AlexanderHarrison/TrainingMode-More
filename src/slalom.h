#include "../MexTK/mex.h"
#include "events.h"

typedef struct SlalomData SlalomData;
typedef struct SlalomAssets SlalomAssets;

struct SlalomData
{
    EventDesc *event_desc;
    SlalomAssets *slalom_assets;
    u8 is_fail;        // status of the last l-cancel
    u8 is_fastfall;    // bool used to detect fastfall frame
    u8 fastfall_frame; // frame the player fastfell on
    struct
    {
        GOBJ *gobj;
        Text *text_time;
        Text *text_air;
        Text *text_scs;
        int canvas;
        float arrow_base_x; // starting X position of arrow
        float arrow_prevpos;
        float arrow_nextpos;
        int arrow_timer;
    } hud;
};

typedef struct SlalomAssets
{
    JOBJ *hud;
    void **hudmatanim; // pointer to array
};

#define LCLTEXT_SCALE 4.2
#define LCLARROW_JOBJ 7

void Slalom_HUDCamThink(GOBJ *gobj);
void Event_Exit();
