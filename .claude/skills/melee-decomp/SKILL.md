---
name: melee-decomp
description: Conventions, style guidelines, and general tips for Melee decompilation. Use whenever you are directly responsible for writing matching C code, not when you are orchestrating decompilation work performed by other agents.
---

# Decompiling Melee Functions

The first step in decompiling a function is to run decomp.py on it:
```sh
`. .venv/bin/activate && python tools/decomp.py --no-copy <function name> --globals=none --no-casts`
```
This will generate an initial guess that can be pasted into the .c source file.

decomp.py uses m2c under the hood. Understanding the various quirks of m2c is very important to decompilation work.

## Understanding m2c Behavior

If the function being decompiled has been declared in a header, m2c will use the parameter and return types of that declaration. These types are often wrong. In particular, most un-decompiled functions are declared as `UNK_RET foo(UNK_PARAMS)`, aka `s32 foo(void)`. Updating these declarations with more accurate types will yield much better m2c output.

If a struct field is being accessed, but the definition of that struct does not have a field at the necessary offset, m2c will generate an access to a (non-existent) "unk" field, e.g. `foo->unkC`. This indicates that either the definition of `foo` is inaccurate, or that `foo` is the wrong type.

When a union is accessed, m2c must guess which member to use, and usually guesses wrong. If you know which member should be used, append `--union-field <union type>:<member>` to the decomp.py command.

When a void* is accessed, m2c must guess which type to cast it to, and usually guesses wrong. If you know which type should be used, append `--void-var-type <var name>:<type>` to the decomp.py command.

m2c cannot determine when the stack should contain a struct type, such as `Vec3`; instead, it emits separate declarations for each field of the struct. When this happens, try passing `--stack-structs`, which will output the inferred stack types as a C struct. Then rewrite this struct to use better types, append it to the end of ./build/ctx.c, and rerun decomp.py.

m2c doesn't understand bitfields very well. If you see ugly-looking bit arithmetic code, check whether it's accessing a struct with bitfields; if so, you may be able to replace this code with simple bit toggles.

## Common Mismatches

### Incorrect Stack Offsets

This is caused by declaring the wrong stack variables, or declaring them in the wrong order. You can use the `PAD_STACK` macro (after the final stack variable) to adjust the total size of the stack, if necessary.

## Style Tips

- When in doubt, choose type and variable names that match the surrounding code.
- Prefix struct members with their hex offset: `/* 0x04 */ u32 x4_thing`
- Make NULL checks explicit: `if (ptr != NULL)` not `if (!ptr)`
- Use `true`/`false` instead of `1`/`0` for bool returns.
