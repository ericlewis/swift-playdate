# Swift & Playdate (DOES NOT COMPILE FOR DEVICE)

Use swift to build games for the Playdate simulator.

### This repo consists of two folders:
- [Example](Example): swift package that bundles up a playdate app
	- You will primarily work in this folder, it can be opened with Xcode.
	- This is also where you will be executing the build script
- [Playdate](Playdate): swift package for the C API / swift wrapper
	- Unless you are building out the swift wrapper, there is little reason mess with this.

### Some things to know:
- Requires whatever the latest Xcode is.
- This has only been tested on M1 mac.
- This is only useful for exploring what using swift with playdate might be like, you cannot build for device (yet).
- This is highly experimental and likely to change and is the swift wrapper is barely started.
- You will need to fix the symlink in `Playdate/Headers`, since they cannot be relative & I am unsure about distributing the headers (we need a better way!).
- You can compile in Xcode, but won't produce a dylib (maybe archive does?), just use the build script in `Example` for now, hopefully we can use spm plugins to automate.
- You will likely need to kill the simulator between runs, it doesn't seem to pick up on changes very well.
- I like using this command: `./build.sh; open swift.pdx`