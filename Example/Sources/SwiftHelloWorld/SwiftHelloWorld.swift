import Playdate

@_dynamicReplacement(for: UpdateCallback)
func updateCallback() -> Bool {

  Playdate.shared.system.drawFPS(x: 0, y: 0)
  Playdate.shared.graphics.draw(.text("nice", x: 0, y: 20))

  return true
}

@_dynamicReplacement(for: EventCallback(event:))
func eventCallback(event: SystemEvent) {
  Playdate.shared.system.log("\(event)")
}
