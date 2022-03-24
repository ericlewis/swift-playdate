# Swift :heart: Playdate

Swift package & plugin for building games that run on [Playdate's](https://play.date) [simulator](https://sdk.play.date/1.9.3/#using-playdate-simulator).

### Some things to know:
- Requires XCode 13.3+ 
- Only tested on M1 Mac.
- Useful for exploring what swift development might be like, you cannot build for device (yet).
- This is highly experimental and likely to change - the swift wrapper is barely started.
- You will likely need to kill the simulator between runs, it doesn't seem to pick up on changes very well.
- The plugin isn't very stable / smart, but it can be made to be.

## Usage

`swift-playdate` can be used via [swift-package-manager](https://github.com/apple/swift-package-manager). It only requires a small bit of project structure.

- Must be a package based project with product type of dynamic.
- Must have a `Sources/Resources` folder with *at least* a valid `pdxinfo` file inside.
	- note: other assets (images, sound, lua files) are to be added here too.
- Builds are handled via SPM:
	- `swift package --disable-sandbox pdc`: creates a pdx file located at `.build/plugins/pdc/outputs/artifacts/{PACKAGE_DISPLAY_NAME}.pdx`
	- `swift package --disable-sandbox pdc --run`: creates a pdx file & opens it in the simulator
  - `--disable-sandbox` is required because we are using plugins outside their typical reach.
- Ideally swizzle the `EventCallback` in order to install an update handler.

## Example
```swift
import Playdate

// MARK: Game Loop

func updateCallback() -> Bool {
  Graphics.clear(with: .white)
  processInput()
  DisplayList.updateAndDrawSprites()

  return true
}

func processInput() {
  let state = System.currentButtonState
  player.move(
    by: .init(
      x: state.contains(.left) ? -10 : state.contains(.right) ? 10 : 0,
      y: state.contains(.up) ? -10 : state.contains(.down) ? 10 : 0
    )
  )
}

// MARK: Setup

let image = Bitmap("images/player")
let player = Sprite()

func initialize() {
  setupPlayer()
  setupMenu()
}

func setupPlayer() {
  player.setImage(image)
  player.move(to: .init(x: 100, y: 100))
  player.addToDisplayList()
}

func setupMenu() {
  Menu.addCheckmarkItem("inverted", isOn: false) { isEnabled in
    Display.isInverted = isEnabled
  }
}

// MARK: Event Handler

@_dynamicReplacement(for: EventCallback(event:))
func eventCallback(event: SystemEvent) {
  if event == .initialize {
    initialize()
    System.setUpdateCallback(updateCallback)
  }
}
```

## Supported APIs
Below is a list of all the C apis & wether or not there is some sort of equivalent in `Playdate`.

### Display
|Status|Name|
|:-:|---|
|✅|getWidth|
|✅|getHeight|
|✅|setRefreshRate|
|✅|setInverted|
|✅|setScale|
|✅|setMosaic|
|✅|setFlipped|
|✅|setOffset|

### File
|Status|Name|
|:-:|---|
|🚧|geterr|
|🚧|stat|
|🚧|mkdir|
|🚧|unlink|
|🚧|rename|
|🚧|open|
|🚧|read|
|🚧|write|
|🚧|flush|
|🚧|tell|
|🚧|seek|

### Graphics
|Status|Name|
|:-:|---|
|✅|Clear|
|✅|setBackgroundColor|
|🚧|setStencil|
|✅|setDrawMode|
|✅|setDrawOffset|
|✅|setClipRect|
|✅|setScreenClipRect|
|✅|clearClipRect|
|✅|setLineCapStyle|
|✅|setFont|
|✅|setTextTracking|
|✅|setTextLeading|
|🚧|pushContext|
|✅|popContext|

#### Drawing
|Status|Name|
|:-:|---|
|🚧|drawBitmap|
|🚧|tileBitmap|
|✅|drawLine|
|🚧|fillTriangle|
|✅|drawRect|
|✅|fillRect|
|🚧|drawEllipse|
|🚧|fillEllipse|
|🚧|fillPolygon|
|🚧|drawScaledBitmap|
|✅|drawText|

#### Bitmap
|Status|Name|
|:-:|---|
|🚧|newBitmap|
|✅|freeBitmap|
|✅|loadBitmap|
|🚧|copyBitmap|
|🚧|loadIntoBitmap|
|🚧|getBitmapData|
|🚧|clearBitmap|
|🚧|rotatedBitmap|

#### BitmapTable
|Status|Name|
|:-:|---|
|🚧|newBitmapTable|
|🚧|freeBitmapTable|
|🚧|loadBitmapTable|
|🚧|loadIntoBitmapTable|
|🚧|getBitmapTable|

#### Font
|Status|Name|
|:-:|---|
|🚧|loadFont|
|🚧|getFontPage|
|🚧|getPageGlyph|
|🚧|getGlyphKerning|
|🚧|getTextWidth|

#### Raw Framebuffer
|Status|Name|
|:-:|---|
|🚧|getFrame|
|🚧|getDisplayFrame|
|🚧|getDebugFrame|
|🚧|copyFrameBufferBitmap|
|🚧|markUpdatedRows|
|✅|display|

### JSON
Note: it may not be worth implementing these since we have JSONDecoder.

### Lua
|Status|Name|
|:-:|---|
|🚧|addFunction|
|🚧|registerClass|
|🚧|pushFunction|
|🚧|indexMetatable|
|🚧|start|
|🚧|stop|
|🚧|getArgCount|
|🚧|getArgCount|
|🚧|argIsNil|
|🚧|getArgBool|
|🚧|getArgInt|
|🚧|getArgFloat|
|🚧|getArgString|
|🚧|getArgBytes|
|🚧|getArgObject|
|🚧|getBitmap|
|🚧|getSprite|
|🚧|pushNil|
|🚧|pushBool|
|🚧|pushInt|
|🚧|pushFloat|
|🚧|pushString|
|🚧|pushBytes|
|🚧|pushBitmap|
|🚧|pushSprite|
|🚧|pushObject|
|🚧|retainObject|
|🚧|releaseObject|
|🚧|setObjectValue|
|🚧|getObjectValue|
|🚧|callFunction|

### Scoreboards
Note: these are not documented anywhere.
|Status|Name|
|:-:|---|
|🚧|addScore|
|🚧|getPersonalBest|
|🚧|freeScore|
|🚧|getScoreboards|
|🚧|freeBoardsList|
|🚧|getScores|
|🚧|freeScoresList|

### Sound
TODO

### Sprite
|Status|Name|
|:-:|---|
|✅|newSprite|
|✅|moveTo|
|✅|moveBy|
|✅|getPosition|
|✅|addSprite|
|✅|setImage|
|✅|freeSprite|

### System
|Status|Name|
|:-:|---|
|✅|realloc|
|✅|error|
|✅|logToConsole|
|✅|error|

#### Menu
|Status|Name|
|:-:|---|
|✅|addMenuItem|
|✅|addCheckmarkMenuItem|
|✅|addOptionsMenuItem|
|✅|removeMenuItem|
|✅|removeAllMenuItems|
|❌|getMenuItemTitle|
|❌|getMenuItemValue|
|❌|setMenuItemValue|
|❌|getMenuItemUserData|
|❌|setMenuItemUserData|
|🚧|setMenuImage|

#### Miscellaneous
|Status|Name|
|:-:|---|
|✅|getCurrentTimeMilliseconds|
|✅|getSecondsSinceEpoch|
|✅|resetElapsedTime|
|✅|getElapsedTime|
|✅|getFlipped|
|✅|getReduceFlashing|
|❌|formatString|
|✅|setUpdateCallback|
|✅|drawFPS|
|✅|getBatteryPercentage|
|✅|getBatteryVoltage|
|✅|getLanguage|
|✅|setPeripheralsEnabled|
|✅|getAccelerometer|
|✅|getButtonState|
|✅|getCrankAngle|
|✅|getCrankChange|
|✅|getCrankDocked|
|✅|setAutoLockDisabled|
|✅|setCrankSoundDisabled|
