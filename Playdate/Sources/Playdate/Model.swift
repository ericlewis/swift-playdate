import CPlaydate

public enum SystemEvent: Int {
  case initialize = 0
  case initializeLua
  case lock
  case unlock
  case pause
  case resume
  case terminate
  case keyPressed
  case keyReleased
  case lowPower

  internal init(_ event: PDSystemEvent) {
    self.init(rawValue: Int(event.rawValue))!
  }
}

public enum DrawCommand {
  case text(String, x: Int, y: Int)
}
