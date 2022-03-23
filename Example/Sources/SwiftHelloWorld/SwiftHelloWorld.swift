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

func initialize() {
  do {
    setupDisplay()
    try setupFonts()
    try setupMenu()
  } catch {
    System.error(error.localizedDescription)
  }
}

func setupDisplay() {
  Display.isInverted = true
}

func setupFonts() throws {
  try Graphics.setFont(AshevilleSans, weight: .bold, size: .pt14)
}

enum Fonts: String, CaseIterable {
  case bold
  case light
  case italic
}

func setupMenu() throws {
  Menu.addCheckmarkItem("inverted", isOn: true) { isEnabled in
    Display.isInverted = isEnabled
  }

  Menu.addCheckmarkItem("crnk snd", isOn: true) { isEnabled in
    System.isCrankSoundEnabled = isEnabled
  }

  try Menu.addOptionItem("font", options: Fonts.allCases) { option in
    switch Fonts(rawValue: option) {
    case .bold:
      try Graphics.setFont("Asheville-Sans-14-Bold")
    case .light:
      try Graphics.setFont("Asheville-Sans-14-Light")
    case .italic:
      try Graphics.setFont("Asheville-Sans-14-Light-Oblique")
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
