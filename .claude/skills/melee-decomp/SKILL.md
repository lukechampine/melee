---
name: melee-decomp
description: Decompile individual Melee functions. Use whenever you are decompiling Melee assembly into matching C code.
---

# Decompiling Melee Functions

## Workflow

### 1. Setup

If they were not already provided to you, locate the .c and .h files that will contain the decompiled source for this function using `rg <function_name>`. Usually, there will be a `/// #function_name` placeholder comment in the .c file, and a declaration in the .h file. After locating the file, check if you have any Skills relating to it (such as `item-decomp` for item-related functions).

Run decomp.py to get an initial guess:
```sh
`. .venv/bin/activate && python tools/decomp.py --no-copy <function name> --globals=none --no-casts`
```

Paste the output into the .c file. If there's a `/// #function_name` comment, replace it. Otherwise, append to the end of the file.

### 2. Iterate

Compile and diff against the original:
```sh
. .venv/bin/activate && python tools/checkdiff.py <function_name>
```

An empty diff means a perfect match: You're done!

More likely, you will see a mismatch. Use the diff to reason about why, then edit your code and run checkdiff.py again. Also be sure to record your thought process in llm_decomp.log, for later use.

### 3. Finish Up

Success means ONE thing only: the compiled output matches the original assembly byte-for-byte (or instruction-for-instruction, depending on project standards). Close is not good enough. 99% is failure. As such, failure is quite likely, particularly for large functions. You have a maximum of 5 serious attempts to achieve a match. Each attempt should be meaningfully different based on what you learned from the previous failure.

If you do not succeed after 5 attempts, give up. You do not need to respond with detailed information about the failure; you will be recording your process in llm_decomp.log, so repeating it in your response would be redundant. Just note the fact that you failed, and that more details are available in llm_decomp.log.

After verifying that you have a 100% match, do a final style check to ensure that your function follows conventions and uses appropriate idioms. Of course, preserving the 100% match is of paramount importance, so only make changes that don't break the match, and revert to the 100% matching version if you accidentally break it.

## Understanding m2c Behavior

decomp.py uses m2c under the hood. Understanding the various quirks of m2c is very important to decompilation work.

If the function being decompiled has been declared in a header, m2c will use the parameter and return types of that declaration. These types are often wrong. In particular, most un-decompiled functions are declared as `UNK_RET foo(UNK_PARAMS)`, aka `s32 foo(void)`. Updating these declarations with more accurate types will yield much better m2c output.

If a struct field is being accessed, but the definition of that struct does not have a field at the necessary offset, m2c will generate an access to a (non-existent) "unk" field, e.g. `foo->unkC`. This indicates that either the definition of `foo` is inaccurate, or that `foo` is the wrong type.

When a union is accessed, m2c must guess which member to use, and usually guesses wrong. If you know which member should be used, append `--union-field <union type>:<member>` to the decomp.py command.

When a void* is accessed, m2c must guess which type to cast it to, and usually guesses wrong. If you know which type should be used, append `--void-field-type <Struct.field>:<type>` to the decomp.py command.

m2c cannot determine when the stack should contain a struct type, such as `Vec3`; instead, it emits separate declarations for each field of the struct. When this happens, try passing `--stack-structs`, which will output the inferred stack types as a C struct. Then rewrite this struct to use better types, append it to the end of ./build/ctx.c, and rerun decomp.py.

m2c cannot distinguish between aliases for the same type. For example, it often generates `s32` instead of `bool`, and `Point3d` instead of `Vec3`. Look at surrounding code to infer which alias to use.

Some types have alternate definitions for m2c, using `#ifdef M2C`. This enables better type resolution, but can also result in code that doesn't compile. For example, `Item_GObj` is normally equivalent to `HSD_GObj`, but has an alternate definition where the `user_data` field is an `Item*` instead of `void*`. This can cause m2c to generate expressions like `user_data->xC4_article_data`, which does not compile in an actual build because `user_data` is really `void*`.

m2c doesn't understand bitfields very well. If you see ugly-looking bit arithmetic code, check whether it's accessing a struct with bitfields; if so, you may be able to replace this code with simple bit toggles.

## Style Tips

- When in doubt, choose type and variable names that match the surrounding code.
- Prefix struct members with their hex offset: `/* 0x04 */ u32 x4_thing`
- Make NULL checks explicit: `if (ptr != NULL)` not `if (!ptr)`
- Use `true`/`false` instead of `1`/`0` for bool returns.
