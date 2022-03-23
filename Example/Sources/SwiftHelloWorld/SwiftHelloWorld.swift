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
    let _ = try! Menu.shared.addCheckmarkItem("testing2", isOn: true) { isEnabled in
      pd.system.log(isEnabled ? "on" : "off")
    }

    let _ = try! Menu.shared.addOptionItem("boo", options: ["test1", "test2", "test3"]) {
      pd.system.log($0)
    }
  }
}
