#ifndef MEX_H_COLOR
#define MEX_H_COLOR

#include "gx.h"
#include "structs.h"
#include "datatypes.h"

/*** Structs ***/

struct ColAnimDesc // in PlCo
{
    u8 *cmd_data; // 0x0
    u8 priority;  // 0x4
    u8 x5;        // 0x5, forget what this does
};
struct ColorOverlay
{
    int timer;                      // 0x0
    int pri;                        // 0x4  this colanims priority, lower = will persist
    void *ptr1;                     // 0x8
    int loop;                       // 0xc
    void *ptr2;                     // 0x10
    int x14;                        // 0x14
    void *alloc;                    // 0x18
    int x1c;                        // 0x1c
    int x20;                        // 0x20
    int x24;                        // 0x24
    int colanim;                    // 0x28, id for the color animation in effect
    GXColor hex;                    // 0x2C
    float color_red;                // 0x30
    float color_green;              // 0x34
    float color_blue;               // 0x38
    float color_alpha;              // 0x3C
    float colorblend_red;           // 0x40
    float colorblend_green;         // 0x44
    float colorblend_blue;          // 0x48
    float colorblend_alpha;         // 0x4C
    GXColor light_color;            // 0x50
    float light_red;                // 0x54
    float light_green;              // 0x58
    float light_blue;               // 0x5C
    float light_alpha;              // 0x60
    float lightblend_red;           // 0x64
    float lightblend_green;         // 0x68
    float lightblend_blue;          // 0x6c
    float lightblend_alpha;         // 0x70
    float light_angle;              // 0x74
    float light_unk;                // 0x78
    unsigned char color_enable : 1; // 0x7c, 0x80
    unsigned char flag2 : 1;        // 0x7c, 0x40
    unsigned char light_enable : 1; // 0x7c, 0x20
    unsigned char flag4 : 1;        // 0x7c, 0x10
    unsigned char flag5 : 1;        // 0x7c, 0x08
    unsigned char flag6 : 1;        // 0x7c, 0x04
    unsigned char flag7 : 1;        // 0x7c, 0x02
    unsigned char flag8 : 1;        // 0x7c, 0x01
};

void ColAnim_Apply(ColorOverlay *col, void *colanim_data, int colanim_index, int r6);
void ColAnim_Disable(ColorOverlay *col);

#endif