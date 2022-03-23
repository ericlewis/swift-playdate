import Playdate

let pd = Playdate.shared

func updateCallback() -> Bool {

  pd.graphics.clear(with: .white)
  pd.system.drawFPS(x: 0, y: 0)
  pd.graphics.draw(.text("Hello world. \(pd.system.currentTimeInterval)", x: 0, y: 20))

  return true
}

@_dynamicReplacement(for: EventCallback(event:))
func eventCallback(event: SystemEvent) {
  if event == .initialize {
    pd.system.setUpdateCallback(updateCallback)
    pd.display.isInverted = true
    let _ = try! Menu.shared.addCheckmarkItem("Inverted", isOn: true) { isEnabled in
      pd.display.isInverted = isEnabled
    }

    let _ = try! Menu.shared.addOptionItem("boo", options: ["test1", "test2", "test3", "test4"]) { _ in

    }
  }
}
