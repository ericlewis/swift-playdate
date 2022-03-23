import Playdate

func updateCallback() -> Bool {

  Graphics.clear(with: .white)
  System.drawFPS(x: 0, y: 0)

  if System.currentButtonState == [.left, .up] {
    Graphics.draw(.text("Pressing up & left!", x: 0, y: 20))
  } else {
    Graphics.draw(.text("Time elapsed: \(System.currentTimeInterval)", x: 0, y: 20))
  }

  Graphics.drawFilledRectangle(
    Rectangle(
      x: 40,
      y: 40,
      width: 60,
      height: 30
    ),
    color: .xor
  )

  Graphics.drawLine(
    from: .zero,
    to: .init(x: 400, y: 240),
    color: .xor,
    stroke: 5
  )

  return true
}

enum Fonts: String, CaseIterable {
  case bold
  case light
  case italic
}

func initialize() {
  do {
    // set default font.
    try Graphics.setFont(AshevilleSans, weight: .bold, size: .pt14)

    // set default inversion
    Display.isInverted = true

    // create our menu items
    setupMenu()
    
  } catch {
    System.error(error.localizedDescription)
  }
}

func setupMenu() {
  try! Menu.addCheckmarkItem("inverted", isOn: true) { isEnabled in
    Display.isInverted = isEnabled
  }

  try! Menu.addCheckmarkItem("crnk snd", isOn: true) { isEnabled in
    System.isCrankSoundEnabled = isEnabled
  }

  try! Menu.addOptionItem("font", options: Fonts.allCases) { option in
    switch Fonts(rawValue: option) {
    case .bold:
      try! Graphics.setFont("Asheville-Sans-14-Bold")
    case .light:
      try! Graphics.setFont("Asheville-Sans-14-Light")
    case .italic:
      try! Graphics.setFont("Asheville-Sans-14-Light-Oblique")
    case .none:
      break
    }
  }
}

@_dynamicReplacement(for: EventCallback(event:))
func eventCallback(event: SystemEvent) {
  if event == .initialize {
    initialize()
    System.setUpdateCallback(updateCallback)
  }
}
