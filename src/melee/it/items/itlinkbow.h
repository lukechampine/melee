#ifndef GALE01_2AF1A4
#define GALE01_2AF1A4

#include "ft/forward.h"
#include "it/forward.h"

#include "ft/chara/ftLink/ftLk_SpecialN.h"
#include "it/items/types.h"

/* 2AF1A4 */ HSD_GObj* it_802AF1A4(f32, Fighter_GObj*, Vec3*, Fighter_Part,
                                   int);
/* 2AF298 */ void it_802AF298(Item_GObj*);
/* 2AF304 */ void it_802AF304(Item_GObj*);
/* 2AF434 */ void it_802AF434(Item_GObj*);
/* 2AF920 */ void it_802AF920(Item_GObj*, Item_GObj*);
/* 3F6E98 */ extern ItemStateTable it_803F6E98[];

/* 2AF64C */ bool it_802AF64C(Item_GObj* gobj);
/* 2AF844 */ void it_802AF844(Item_GObj* gobj);
/* 2AF848 */ bool it_802AF848(Item_GObj* gobj);
/* 2AF90C */ bool it_802AF90C(Item_GObj* gobj);
/* 2AF914 */ void it_802AF914(Item_GObj* gobj);
/* 2AF918 */ bool it_802AF918(Item_GObj* gobj);
#endif
