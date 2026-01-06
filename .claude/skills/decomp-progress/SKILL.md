---
name: decomp-progress
description: Track progress when decompiling multiple Melee functions. Use before you start working on a batch of functions so that you can give the user a nice summary when you're finished.
---

# Usage

Before doing decompilation work, run this command. It will ensure that project currently compiles, and will also generate a baseline to compare against later.
```sh
ninja baseline
```

When you have finished decompiling a batch of functions, run:
```sh
ninja changes_all
```
This will report statistics on the new matches, which you can include in your response. 
