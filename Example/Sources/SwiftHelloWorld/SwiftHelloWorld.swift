import Playdate

// MARK: Game Loop

func updateCallback() -> Bool {
  Graphics.clear(with: .white)
  processInputs()
  DisplayList.updateAndDrawSprites()
  
  return true
}

func processInputs() {
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
  player.move(to: .init(x: image.size.width, y: image.size.height))
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
