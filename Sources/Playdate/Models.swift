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

  internal var cValue: UInt32 {
    UInt32(self.rawValue)
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

public struct Rectangle {
  public let x: Int
  public let y: Int
  public let width: Int
  public let height: Int

  public init(
  x: Int,
  y: Int,
  width: Int,
  height: Int
  ) {
    self.x = x
    self.y = y
    self.width = width
    self.height = height
  }
}

public enum LineCapStyle {
  case butt
  case square
  case round

  internal var cStyle: LCDLineCapStyle {
    switch self {
    case .butt:
      return kLineCapStyleButt
    case .square:
      return kLineCapStyleSquare
    case .round:
      return kLineCapStyleRound
    }
  }
}

public enum DrawMode {
  case copy
  case whiteTransparent
  case blackTransparent
  case fillWhite
  case fillBlack
  case xor
  case xnor
  case inverted

  internal var cMode: LCDBitmapDrawMode {
    switch self {
    case .copy:
      return kDrawModeCopy
    case .whiteTransparent:
      return kDrawModeWhiteTransparent
    case .blackTransparent:
      return kDrawModeBlackTransparent
    case .fillWhite:
      return kDrawModeFillWhite
    case .fillBlack:
      return kDrawModeFillBlack
    case .xor:
      return kDrawModeXOR
    case .xnor:
      return kDrawModeNXOR
    case .inverted:
      return kDrawModeInverted
    }
  }
}

public protocol Font {
  associatedtype Weight
  associatedtype Size
  func path(_ weight: Weight, size: Size) -> String
}

public struct _Roobert: Font {
  public enum Weight: String {
    case bold
  }
  public enum Size: Int {
    case pt10 = 10
  }
  public func path(_ weight: Weight, size: Size) -> String {
    "Roobert-\(size.rawValue)-\(weight.rawValue)"
  }
}

public let Roobert = _Roobert()

public struct _AshevilleSans: Font {
  public enum Weight: String {
    case bold
    case light
    case lightOblique = "Light-Oblique"
  }
  public enum Size: Int {
    case pt14 = 14
    case pt24 = 24
  }
  public init() {}
  public func path(_ weight: Weight, size: Size) -> String {
    "Asheville-Sans-\(size.rawValue)-\(weight.rawValue)"
  }
}

public let AshevilleSans = _AshevilleSans()

public struct AccelerometerData {
  public let x: Float
  public let y: Float
  public let z: Float
}

public enum Axis {
  case horizontal
  case vertical
  case both
}

public struct Point {
  public static let zero = Point(x: 0, y: 0)

  public let x: Float
  public let y: Float

  public init(x: Float, y: Float) {
    self.x = x
    self.y = y
  }
}

public struct Button: OptionSet {
  public let rawValue: Int

  public init(rawValue: Int) {
    self.rawValue = rawValue
  }

  public static let left    = Button(rawValue: 1 << 0)
  public static let right   = Button(rawValue: 1 << 1)
  public static let up      = Button(rawValue: 1 << 2)
  public static let down    = Button(rawValue: 1 << 3)
  public static let b       = Button(rawValue: 1 << 4)
  public static let a       = Button(rawValue: 1 << 5)
}
