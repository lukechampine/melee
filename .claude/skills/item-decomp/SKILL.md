---
name: item-decomp
description: Conventions, style guidelines, and general tips for item-related code. Use whenever decompiling item-related functions.
---

# Decompiling Item-Related Functions

Most item functions take `Item_GObj* gobj` as their first parameter, and return `void` or `bool`.

Most item functions involve accessing `gobj->user_data` and casting it to a `Item*`. The idiomatic way to write this is `Item* ip = GET_ITEM(gobj)`.

Each item typically has an associated set of variables. These will be defined in ./src/melee/it/itCharItems.h in a struct named `it<name>_ItemVars`. You may need to define a new struct for your item, or modify the existing definition. The struct is accessed via the `ip->xDD4_itemVar` union. The decompiler cannot automatically infer which union field is being referred to, and usually guesses wrong. To fix this, pass `--union-field Item_ItemVars:<correct field>` to decomp.py (see example below).

Some items also have associated attributes. These will be defined in ./src/melee/it/itCommonItems.h in a struct named `it<name>Attributes`. You may need to define a new struct for your item, or modify the existing definition. The struct is accessed via the `ip->xC4_article_data->x4_specialAttributes` void* pointer. The decompiler cannot automatically infer the type of void* pointers, so the resulting code is usually wrong. To fix this, pass `--void-var-type <var>:it<name>Attributes` to decomp.py (see example below).

## Decompilation Process Examples

### Simple function

Generate initial guess:
```sh
. .venv/bin/activate && python ./tools/decomp.py it_802E1C4C
```
Output:
```c
void it_802E1C4C(HSD_GObj* arg0)
{
    void* temp_r6;

    temp_r6 = arg0->user_data;
    temp_r6->unk44 = 0.0f;
    temp_r6->unk40 = 0.0f;
    Item_80268E5C(arg0, 1, ITEM_ANIM_UPDATE);
}
```

decomp.py is defaulting to `HSD_GObj` because itklap.h declares the function with unknown types. Update the declaration:
```diff
--- itklap.h
+++ itklap.h
@@ -14,7 +14,7 @@
 /* 2E1968 */ bool itKlap_UnkMotion1_Anim(Item_GObj* gobj);
 /* 2E1970 */ void itKlap_UnkMotion1_Phys(Item_GObj* gobj);
 /* 2E19FC */ bool itKlap_UnkMotion1_Coll(Item_GObj* gobj);
-/* 2E1C4C */ UNK_RET it_802E1C4C(UNK_PARAMS);
+/* 2E1C4C */ void it_802E1C4C(Item_GObj* gobj);
 /* 2E1C84 */ UNK_RET it_802E1C84(UNK_PARAMS);
 /* 2E1D24 */ bool itKlap_UnkMotion2_Anim(Item_GObj* gobj);
 /* 2E1D2C */ void itKlap_UnkMotion2_Phys(Item_GObj* gobj);
```

Now decomp.py produces:
```c
void it_802E1C4C(Item_GObj* gobj)
{
    Item* temp_r6;

    temp_r6 = gobj->user_data;
    temp_r6->x40_vel.y = 0.0f;
    temp_r6->x40_vel.x = 0.0f;
    Item_80268E5C((HSD_GObj* ) gobj, 1, ITEM_ANIM_UPDATE);
}
```

Rewrite to use idiomatic names and macros, and remove the unnecessary cast:
```c
void it_802E1C4C(Item_GObj* gobj)
{
    Item* ip = GET_ITEM(gobj);
    ip->x40_vel.y = 0.0f;
    ip->x40_vel.x = 0.0f;
    Item_80268E5C(gobj, 1, ITEM_ANIM_UPDATE);
}
```

Compile and check match:
```sh
. .venv/bin/activate && python tools/checkdiff.py it_802E1C4C
```
Output:
```diff
--- itklap.o
+++ itklap.o
```

The diff is empty; 100% match!

### Item Variables and Attributes

Generate initial guess:
```sh
. .venv/bin/activate && python ./tools/decomp.py itLinkarrow_UnkMotion1_Phys
```
Output:
```c
void itLinkarrow_UnkMotion1_Phys(Item_GObj* arg0)
{
    Item* temp_r5;
    f32 var_f1;

    temp_r5 = arg0->user_data;
    temp_r5->xDD4_itemVar.bombhei.xDEC = temp_r5->pos.x;
    temp_r5->xDD4_itemVar.bombhei.xDF0 = temp_r5->pos.y;
    temp_r5->xDD4_itemVar.bombhei.xDF4 = (bitwise s32) temp_r5->pos.z;
    var_f1 = temp_r5->xC4_article_data->x4_specialAttributes->unk1C;
    if (var_f1 < 0.0f) {
        var_f1 = -var_f1;
    }
    temp_r5->x40_vel.y -= var_f1;
}
```

decomp.py is defaulting to the `bombhei` field, which is wrong. The correct type is `itLinkArrow_ItemVars`; in the `Item_ItemVars` union, that field is named `linkarrow`. Force decomp.py to use this field:
```sh
. .venv/bin/activate && python ./tools/decomp.py itLinkarrow_UnkMotion1_Phys --union-field Item_ItemVars:linkarrow
```
Output:
```c
void itLinkarrow_UnkMotion1_Phys(Item_GObj* arg0)
{
    Item* temp_r5;
    f32 var_f1;

    temp_r5 = arg0->user_data;
    temp_r5->xDD4_itemVar.linkarrow.x18.x = temp_r5->pos.x;
    temp_r5->xDD4_itemVar.linkarrow.x18.y = temp_r5->pos.y;
    temp_r5->xDD4_itemVar.linkarrow.x18.z = temp_r5->pos.z;
    var_f1 = temp_r5->xC4_article_data->x4_specialAttributes->unk1C;
    if (var_f1 < 0.0f) {
        var_f1 = -var_f1;
    }
    temp_r5->x40_vel.y -= var_f1;
}
```

The function also accesses `xC4_article_data->x4_specialAttributes`. Unfortunately, `--void-var-type` cannot be used here, because there is no intermediate variable to reference. Make the edit manually instead, adding an `attr` variable:
```c
void itLinkarrow_UnkMotion1_Phys(Item_GObj* arg0)
{
    Item* temp_r5;
    itLinkArrowAttributes* attr = ip->xC4_article_data->x4_specialAttributes;
    f32 var_f1;

    temp_r5 = arg0->user_data;
    temp_r5->xDD4_itemVar.linkarrow.x18.x = temp_r5->pos.x;
    temp_r5->xDD4_itemVar.linkarrow.x18.y = temp_r5->pos.y;
    temp_r5->xDD4_itemVar.linkarrow.x18.z = temp_r5->pos.z;
    var_f1 = attr->x1C;
    if (var_f1 < 0.0f) {
        var_f1 = -var_f1;
    }
    temp_r5->x40_vel.y -= var_f1;
}
```

Compile and check match:
```sh
. .venv/bin/activate && python tools/checkdiff.py itLinkarrow_UnkMotion1_Phys
```
Output:
```diff
--- itlinkarrow.o
+++ itlinkarrow.o
```

Another perfect match! Stopping here is acceptable, but the code could be made more idiomatic. Rewrite the variable names, and use the `GET_ITEM` macro:
```c
void itLinkarrow_UnkMotion1_Phys(Item_GObj* gobj)
{
    Item* ip = GET_ITEM(gobj);
    itLinkArrowAttributes* attr = ip->xC4_article_data->x4_specialAttributes;
    f32 var_f1;

    ip->xDD4_itemVar.linkarrow.x18.x = ip->pos.x;
    ip->xDD4_itemVar.linkarrow.x18.y = ip->pos.y;
    ip->xDD4_itemVar.linkarrow.x18.z = ip->pos.z;
    var_f1 = attr->x1C;
    if (var_f1 < 0.0f) {
        var_f1 = -var_f1;
    }
    ip->x40_vel.y -= var_f1;
}
```

The sequence of x, y, and z assignments indicate that a Vec3 is being copied field-by-field. Combine them into one assignment:
```c
void itLinkarrow_UnkMotion1_Phys(Item_GObj* gobj)
{
    Item* ip = GET_ITEM(gobj);
    itLinkArrowAttributes* attr = ip->xC4_article_data->x4_specialAttributes;
    f32 var_f1;

    ip->xDD4_itemVar.linkarrow.x18 = ip->pos;
    var_f1 = attr->x1C;
    if (var_f1 < 0.0f) {
        var_f1 = -var_f1;
    }
    ip->x40_vel.y -= var_f1;
}
```

Finally, make use of the `ABS` macro, which also removes the need for `var_f1`:
```c
void itLinkarrow_UnkMotion1_Phys(Item_GObj* gobj)
{
    Item* ip = GET_ITEM(gobj);
    itLinkArrowAttributes* attr = ip->xC4_article_data->x4_specialAttributes;
    f32 var_f1;

    ip->xDD4_itemVar.linkarrow.x18 = ip->pos;
    ip->x40_vel.y -= ABS(attr->x1C);
}
```

Check that the function still matches:
```sh
. .venv/bin/activate && python tools/checkdiff.py itLinkarrow_UnkMotion1_Phys
```
Output:
```diff
--- itlinkarrow.o
+++ itlinkarrow.o
```

All done!
