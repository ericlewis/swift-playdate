# Swift & Playdate (DOES NOT COMPILE FOR DEVICE)

Use swift to build games for the Playdate simulator.

This repo consists of two folders:
- Example, which holds the example playdate application
- Playdate, which holds the C API & Swift wrapper

Some things to know:
- This has only been tested on M1 mac.
- This expects the SDK folder to be in the default location.
- This is only useful for exploring what using swift with playdate might be like, you cannot build for device (yet).
- This is highly experimental and likely to change.
- If you are having trouble, the `C_API` folder in `Headers` is supposed to be symlinked to the one in the SDK folder, just relink.
- You can compile in Xcode, but won't produce a dylib, just use the build script in example for now, hopefully we can use spm plugins to automate.