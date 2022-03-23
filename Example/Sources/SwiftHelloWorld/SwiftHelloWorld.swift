import Playdate

let pd = Playdate.shared

func updateCallback() -> Bool {

  pd.graphics.clear(with: .white)
  pd.system.drawFPS(x: 0, y: 0)
  pd.graphics.draw(.text("Hello world. \(pd.system.currentTimeInterval)", x: 0, y: 20))
  pd.graphics.drawFilledRectangle(.init(x: 40, y: 40, width: 60, height: 30), color: .xor)
  pd.graphics.drawLine(from: .zero, to: .init(x: 400, y: 240), color: .xor, stroke: 5)

  return true
}

enum Fonts: String, CaseIterable {
  case first
  case second
  case third
}

@_dynamicReplacement(for: EventCallback(event:))
func eventCallback(event: SystemEvent) {
  if event == .initialize {
    pd.display.isInverted = true

    try! pd.graphics.setFont(AshevilleSans, weight: .bold, size: .pt14)

    try! Menu.shared.addCheckmarkItem("inverted", isOn: true) { isEnabled in
      pd.display.isInverted = isEnabled
    }

    try! Menu.shared.addOptionItem("font", options: Fonts.allCases) { option in
      switch Fonts(rawValue: option) {
      case .first:
        try! pd.graphics.setFont("Asheville-Sans-14-Bold")
      case .second:
        try! pd.graphics.setFont("Asheville-Sans-14-Light")
      case .third:
        try! pd.graphics.setFont("Asheville-Sans-14-Light-Oblique")
      case .none:
        break
      }
    }

    pd.system.setUpdateCallback(updateCallback)
  }
}
