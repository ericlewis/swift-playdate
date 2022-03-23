import CPlaydate

public enum Color: Int {
  case black = 0
  case white
  case clear
  case xor // TODO: better name
  case pattern // TODO: should be take assc value

  init(_ lcdColor: LCDSolidColor) {
    self.init(rawValue: Int(lcdColor.rawValue))!
  }
}

public enum Language: Int {
  case english = 0
  case japanese
  case unknown

  init(_ language: PDLanguage) {
    self.init(rawValue: Int(language.rawValue))!
  }
}

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
