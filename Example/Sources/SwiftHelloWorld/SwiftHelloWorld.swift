import Playdate

func updateCallback() -> Bool {

  Graphics.clear(with: .white)
  let state = System.currentButtonState
  player.move(
    by: .init(
      x: state.contains(.left) ? -10 : state.contains(.right) ? 10 : 0,
      y: state.contains(.up) ? -10 : state.contains(.down) ? 10 : 0
    )
  )

  DisplayList.updateAndDrawSprites()
  
  return true
}

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
  Menu.addCheckmarkItem("crnk snd", isOn: true) { isEnabled in
    System.isCrankSoundEnabled = isEnabled
  }
}

@_dynamicReplacement(for: EventCallback(event:))
func eventCallback(event: SystemEvent) {
  if event == .initialize {
    initialize()
    System.setUpdateCallback(updateCallback)
  }
}
