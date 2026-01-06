---
name: mismatch-db
description: Knowledge base for common objdiff mismatches. Use to interpret non-empty diffs reported by checkdiff.py.
---

# Common Causes for Match Failure

The following is a list of common reasons why your compiled C code doesn't match the target assembly. 

## Incorrect stack size

The target assembly calls `stwu` with a different offset, affecting many downstream `r1` accesses.

### Example diff:
```diff
@@ -1,41 +1,41 @@ it_3F14_Logic32_DmgReceived
 0x000000: mflr r0
 0x000004: stw r0 0x4(r1)
-0x000008: stwu r1 -0x28(r1)
+0x000008: stwu r1 -0x20(r1)
-0x00000c: stw r31 0x24(r1)
+0x00000c: stw r31 0x1c(r1)
-0x000010: stw r30 0x20(r1)
+0x000010: stw r30 0x18(r1)
-0x000014: stw r29 0x1c(r1)
+0x000014: stw r29 0x14(r1)
 0x000018: mr r29 r3
 0x00001c: lwz r31 0x2c(r3)
```

### Supporting evidence

- The `stwu r1` instruction at the beginning of the function uses the wrong offset.
- The `addi r1 r1` instruction at the end of the function uses the wrong immediate value.

### Root cause

The stack is too large or too small.

### Fix

If the stack is too large, try to reuse variables. If it is too small, use the `PAD_STACK` macro after the final stack variable.

### Example fix
```diff
     Item* ip = GET_ITEM(gobj);
     HSD_GObj* go = it_8027236C(gobj);
+    PAD_STACK(8);
```


## Copying structs field-by-field

The diff shows a sequence of loads and stores that differ in type: The target assembly uses `lwz` and `stw`, while the non-matching assembly uses `lfs` and `stfs` (or other load/store pairs).

### Example diff
```diff
@@ -1,10 +1,10 @@ it_80295748
 0x000000: lwz r3 0x2c(r3)
 0x000004: lwz r3 0xc4(r3)
-0x000008: lwz r5 0x4(r3)
+0x000008: lwz r3 0x4(r3)
-0x00000c: lwz r3 0x4(r5)
+0x00000c: lfs f0 0x4(r3)
-0x000010: lwz r0 0x8(r5)
+0x000010: stfs f0 0x0(r4)
-0x000014: stw r3 0x0(r4)
+0x000014: lfs f0 0x8(r3)
-0x000018: stw r0 0x4(r4)
+0x000018: stfs f0 0x4(r4)
-0x00001c: lwz r0 0xc(r5)
+0x00001c: lfs f0 0xc(r3)
-0x000020: stw r0 0x8(r4)
+0x000020: stfs f0 0x8(r4)
 0x000024: blr
```

### Supporting evidence

- Two or more assignments in a row, where each pair of source and destination are offset by one word
- Code involving `Vec3`

## Root cause

When an entire struct is copied, the compiler will copy it word-by-word, without regard to the type of each field. m2c doesn't recognize this, and instead generates code that copies the struct field-by-field, with instructions matching the type of each field.

### Fix

Assign the entire struct in one expression.

### Example fix
```diff
-    pos->x = attrs->x4.x;
-    pos->y = attrs->x4.y;
-    pos->z = attrs->x4.z;
+    *pos = attrs->x4;
```
