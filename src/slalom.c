#include "slalom.h"
static char nullString[] = " ";

// Main Menu
static char **SlOptions_Toggle[] = {"On", "Off"};
static EventOption SlOptions_Main[] = {
    // HUD
    {
        .option_kind = OPTKIND_STRING,              // the type of option this is; menu, string list, integers list, etc
        .value_num = sizeof(SlOptions_Toggle) / 4,  // number of values for this option
        .option_val = 0,                            // value of this option
        .menu = 0,                                  // pointer to the menu that pressing A opens
        .option_name = "HUD",                       // pointer to a string
        .desc = "Toggle visibility of the HUD.",    // string describing what this option does
        .option_values = SlOptions_Toggle,          // pointer to an array of strings
        .onOptionChange = 0,
    },
    // Help
    {
        .option_kind = OPTKIND_FUNC,                                                                                                                                                                                       // the type of option this is; menu, string list, integers list, etc
        .value_num = 0,                                                                                                                                                                                                    // number of values for this option
        .option_val = 0,                                                                                                                                                                                                   // value of this option
        .menu = 0,                                                                                                                                                                                                         // pointer to the menu that pressing A opens
        .option_name = "Help",                                                                                                                                                                                             // pointer to a string
        .desc = "TODO dash between slalom poles",
        .option_values = 0,                                                                                                                                                                                                // pointer to an array of strings
        .onOptionChange = 0,
    },
    // Exit
    {
        .option_kind = OPTKIND_FUNC,                     // the type of option this is; menu, string list, integers list, etc
        .value_num = 0,                                  // number of values for this option
        .option_val = 0,                                 // value of this option
        .menu = 0,                                       // pointer to the menu that pressing A opens
        .option_name = "Exit",                           // pointer to a string
        .desc = "Return to the Event Selection Screen.", // string describing what this option does
        .option_values = 0,                              // pointer to an array of strings
        .onOptionChange = 0,
        .onOptionSelect = Event_Exit,
    },
};
static EventMenu LabMenu_Main = {
    .name = "Slalom Minigame",                                  // the name of this menu
    .option_num = sizeof(SlOptions_Main) / sizeof(EventOption), // number of options this menu contains
    .scroll = 0,                                                // runtime variable used for how far down in the menu to start
    .state = 0,                                                 // bool used to know if this menu is focused, used at runtime
    .cursor = 0,                                                // index of the option currently selected, used at runtime
    .options = &SlOptions_Main,                                 // pointer to all of this menu's options
    .prev = 0,                                                  // pointer to previous menu, used at runtime
};

// Init Function
void Event_Init(GOBJ *gobj)
{
    SlalomData *event_data = gobj->userdata;
    EventDesc *event_desc = event_data->event_desc;
    GOBJ *hmn = Fighter_GetGObj(0);
    FighterData *hmn_data = hmn->userdata;

    // theres got to be a better way to do this...
    event_vars = *event_vars_ptr;

    // get event assets
    event_data->slalom_assets = File_GetSymbol(event_vars->event_archive, "slalom");

    // create HUD
    Slalom_Init(event_data);

    return;
}
// Think Function
void Event_Think(GOBJ *event)
{
    SlalomData *event_data = event->userdata;

    // get fighter data
    GOBJ *hmn = Fighter_GetGObj(0);
    FighterData *hmn_data = hmn->userdata;
    //GOBJ *cpu = Fighter_GetGObj(1);
    //FighterData *cpu_data = cpu->userdata;
    HSD_Pad *pad = PadGet(hmn_data->player_controller_number, PADGET_ENGINE);


    Slalom_Think(event_data, hmn_data);

    return;
}
void Event_Exit()
{
    Match *match = MATCH;

    // end game
    match->state = 3;

    // cleanup
    Match_EndVS();

    // unfreeze
    HSD_Update *update = HSD_UPDATE;
    update->pause_develop = 0;
    return;
}

// L-Cancel functions
void Slalom_Init(SlalomData *event_data)
{
    // create hud cobj
    GOBJ *hudcam_gobj = GObj_Create(19, 20, 0);
    ArchiveInfo **ifall_archive = 0x804d6d5c;
    COBJDesc ***dmgScnMdls = File_GetSymbol(*ifall_archive, 0x803f94d0);
    COBJDesc *cam_desc = dmgScnMdls[1][0];
    COBJ *hud_cobj = COBJ_LoadDesc(cam_desc);
    // init camera
    GObj_AddObject(hudcam_gobj, R13_U8(-0x3E55), hud_cobj);
    GOBJ_InitCamera(hudcam_gobj, Slalom_HUDCamThink, 7);
    hudcam_gobj->cobj_links = 1 << 18;

    GOBJ *hud_gobj = GObj_Create(0, 0, 0);
    event_data->hud.gobj = hud_gobj;
    // Load jobj
    JOBJ *hud_jobj = JOBJ_LoadJoint(event_data->slalom_assets->hud);
    GObj_AddObject(hud_gobj, 3, hud_jobj);
    GObj_AddGXLink(hud_gobj, GXLink_Common, 18, 80);

    // create text canvas
    int canvas = Text_CreateCanvas(2, hud_gobj, 14, 15, 0, 18, 81, 19);
    event_data->hud.canvas = canvas;

    // init text
    Text **text_arr = &event_data->hud.text_time;
    for (int i = 0; i < 3; i++)
    {

        // Create text object
        Text *hud_text = Text_CreateText(2, canvas);
        text_arr[i] = hud_text;
        hud_text->kerning = 1;
        hud_text->align = 1;
        hud_text->use_aspect = 1;

        // Get position
        Vec3 text_pos;
        JOBJ *text_jobj;
        JOBJ_GetChild(hud_jobj, &text_jobj, 2 + i, -1);
        JOBJ_GetWorldPosition(text_jobj, 0, &text_pos);

        // adjust scale
        Vec3 *scale = &hud_jobj->scale;
        // text scale
        hud_text->scale.X = (scale->X * 0.01) * LCLTEXT_SCALE;
        hud_text->scale.Y = (scale->Y * 0.01) * LCLTEXT_SCALE;
        hud_text->aspect.X = 165;

        // text position
        hud_text->trans.X = text_pos.X + (scale->X / 4.0);
        hud_text->trans.Y = (text_pos.Y * -1) + (scale->Y / 4.0);

        // dummy text
        Text_AddSubtext(hud_text, 0, 0, "-");
    }

    // TODO delete arrow from dat
    JOBJ *arrow_jobj;
    JOBJ_GetChild(hud_jobj, &arrow_jobj, LCLARROW_JOBJ, -1);
    JOBJ_SetFlags(arrow_jobj, JOBJ_HIDDEN);

    return 0;
}
void Slalom_Think(SlalomData *event_data, FighterData *hmn_data)
{
    JOBJ *hud_jobj = event_data->hud.gobj->hsd_object;

    // update HUD anim
    JOBJ_AnimAll(hud_jobj);

    return;
}
void Slalom_HUDCamThink(GOBJ *gobj)
{

    // if HUD enabled and not paused
    if ((SlOptions_Main[0].option_val == 0) && (Pause_CheckStatus(1) != 2))
    {
        CObjThink_Common(gobj);
    }

    return;
}


// Initial Menu
EventMenu *Event_Menu = &LabMenu_Main;
