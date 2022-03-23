# Swift :heart: Playdate

Swift package & plugin for building games that run on [Playdate's](https://play.date) [simulator](https://sdk.play.date/1.9.3/#using-playdate-simulator).

### Some things to know:
- Requires XCode 13.3+ 
- Only been tested on M1 Mac.
- Useful for exploring what swift development might be like, you cannot build for device (yet).
- This is highly experimental and likely to change - the swift wrapper is barely started.
- You will likely need to kill the simulator between runs, it doesn't seem to pick up on changes very well.

## Usage

`swift-playdate` can be used via swift-package-manager. It only requires a small bit of project structure.

- Must be a package based project with product type of dynamic.
- Must have a `Sources/Resources` folder with *at least* a valid `pdxinfo` file inside.
	- note: other assets (images, sound, lua files) are to be added here too.
- Builds are handled via SPM:
	- `swift package --disable-sandbox pdc`: creates a pdx file located at `.build/plugins/pdc/outputs/artifacts/{PACKAGE_DISPLAY_NAME}.pdx`
	- `swift package --disable-sandbox pdc --run`: creates a pdx file & opens it in the simulator
  - `--disable-sandbox` is required because we are using plugins outside their typical reach.
- Ideally swizzle the `EventCallback` in order to install an update handler. Here is an example:
```swift
import Playdate

func update() -> Bool {
  Playdate.shared.system.drawFPS(x: 0, y: 0)
  return true
}

// swizzle the default event callback provided by swift-playdate
@_dynamicReplacement(for: EventCallback(event:))
func eventCallback(event: SystemEvent) {
  if event == .initialize {
    // set our callback after initialization & before lua initialization
    Playdate.shared.system.setUpdateCallback(update)
  }
}
```
