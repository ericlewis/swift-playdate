import Playdate

let pd = Playdate.shared

func updateCallback() -> Bool {

  pd.system.drawFPS(x: 0, y: 0)
  pd.graphics.draw(.text("Hello world. \(pd.system.batteryPercentage)", x: 0, y: 20))

  return true
}

@_dynamicReplacement(for: EventCallback(event:))
func eventCallback(event: SystemEvent) {
  if event == .initialize {
    pd.system.setUpdateCallback(updateCallback)
  }
}
