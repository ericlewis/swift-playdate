# Swift & Playdate (DOES NOT COMPILE FOR DEVICE)

Use swift to build games for the Playdate simulator.

### This repo consists of two folders:
- [Example](Example): swift package that bundles up a playdate app
	- You will primarily work in this folder, it can be opened with Xcode.
	- This is also where you will be executing the build script
- [Playdate](Playdate): swift package for the C API / swift wrapper
	- Unless you are building out the swift wrapper, there is little reason mess with this.

### Some things to know:
- Requires XCode 13.3+ 
- Only been tested on M1 Mac.
- Useful for exploring what swift development might be like, you cannot build for device (yet).
- This is highly experimental and likely to change - the swift wrapper is barely started.
- You will likely need to kill the simulator between runs, it doesn't seem to pick up on changes very well.

## Usage

`Playdate` can be used via swift-package-manager. It only requires a small bit of project structure.

- Must have a `Sources/Resources` folder with *at least* a valid `pdxinfo` file inside.
	- note: other assets (images, sound, lua files) are to be added here too.
- Ideally swizzle the `EventCallback` in order to install your update handler.
- Builds can be controlled via spm package commands, in the root of your project:
	- `swift package pdc`: creates a pdx file located at `.build/plugs/pdc/outputs/artifacts/{PACKAGE_DISPLAY_NAME}.px`
	- `swift package pdc --run`: creates a pdx file & opens it in the simulator