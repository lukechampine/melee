#include "itmewtwoshadowball.h"

#include "it/inlines.h"
#include "it/it_26B1.h"

/// #it_802C4D10

/// #it_802C4F50

/// #it_802C5000

/// #it_802C519C

/// #it_802C53F0

/// #it_802C56A0

/// #it_802C573C

/// #it_802C578C

/// #it_802C57C8

void it_802C5B0C(Item_GObj* gobj) {}

bool it_802C5B10(Item_GObj* gobj)
{
    return false;
}

/// #it_802C5B18

/// #it_802C5BF8

/// #it_802C5CF4

/// #it_802C5D64

/// #it_802C5DAC

void it_802C5DCC(Item_GObj* gobj) {}

/// #it_802C5DD0

/// #fn_802C5E18

/// #it_802C5E5C

/// #it_802C5F7C

void it_802C6094(Item_GObj* gobj)
{
    Item* ip = GET_ITEM(gobj);
    itResetVelocity(ip);
}

bool it_802C60AC(Item_GObj* gobj)
{
    return false;
}

bool it_802C60B4(Item_GObj* arg0)
{
    return true;
}

bool it_802C60BC(Item_GObj* arg0)
{
    return true;
}

bool it_802C60C4(Item_GObj* arg0)
{
    return true;
}

/// #it_802C60CC

bool it_802C6138(Item_GObj* arg0)
{
    return true;
}

/// #it_802C6140

void it_802C61D4(Item_GObj* gobj, Item_GObj* ref_gobj)
{
    it_8026B894(gobj, ref_gobj);
}

ItemStateTable it_803F7760[] = {
    { 0, it_802C57C8, it_802C5B0C, it_802C5B10 },
    { 1, it_802C5BF8, it_802C5CF4, it_802C5D64 },
    { 2, it_802C5BF8, it_802C5CF4, it_802C5D64 },
    { 3, it_802C5BF8, it_802C5CF4, it_802C5D64 },
    { 4, it_802C5BF8, it_802C5CF4, it_802C5D64 },
    { 5, it_802C5BF8, it_802C5CF4, it_802C5D64 },
    { 6, it_802C5BF8, it_802C5CF4, it_802C5D64 },
    { 7, it_802C5BF8, it_802C5CF4, it_802C5D64 },
    { 8, it_802C5BF8, it_802C5CF4, it_802C5D64 },
    { 9, it_802C5DAC, it_802C5DCC, it_802C5DD0 },
    { 1, it_802C5F7C, it_802C6094, it_802C60AC },
    { 2, it_802C5F7C, it_802C6094, it_802C60AC },
    { 3, it_802C5F7C, it_802C6094, it_802C60AC },
    { 4, it_802C5F7C, it_802C6094, it_802C60AC },
    { 5, it_802C5F7C, it_802C6094, it_802C60AC },
    { 6, it_802C5F7C, it_802C6094, it_802C60AC },
    { 7, it_802C5F7C, it_802C6094, it_802C60AC },
    { 8, it_802C5F7C, it_802C6094, it_802C60AC },
};
