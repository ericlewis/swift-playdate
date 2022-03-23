import CPlaydate

public class Playdate {
  public static let shared = Playdate()

  public private(set) var system: System!
  public private(set) var graphics: Graphics!

  public var eventHandler: ((SystemEvent) -> Void)? = nil
  public var updateCallback: (() -> Bool)? = nil

  init() {}

  public func register(_ pd: PlaydateAPI) {
    self.system = .init(pd)
    self.graphics = .init(pd)
  }

  public class System {
    var pd: PlaydateAPI
    var pointee: playdate_sys

    init(_ pd: PlaydateAPI) {
      self.pd = pd
      pointee = pd.system.pointee
    }

    public func drawFPS(x: Int, y: Int) {
      pointee.drawFPS(CInt(x), CInt(y))
    }

    public func log(_ message: String) {
      logToConsole(withUnsafeMutablePointer(to: &pd) { $0 }, message)
    }

    public func error(_ message: String) {
      errorToConsole(withUnsafeMutablePointer(to: &pd) { $0 }, message)
    }

    public func setUpdateCallback(_ callback: @escaping () -> Bool) {
      Playdate.shared.updateCallback = callback
      pointee.setUpdateCallback(_update, nil)
    }
  }

  public struct Graphics {
    let pointee: playdate_graphics

    init(_ pd: PlaydateAPI) {
      pointee = pd.graphics.pointee
    }

    public func drawText(_ text: String, x: Int, y: Int) {
      let _ = pointee.drawText(text, text.count, kUTF8Encoding, CInt(x), CInt(y))
    }

    public func draw(_ drawCommand: DrawCommand) {
      switch drawCommand {
      case .text(let string, let x, let y):
        self.drawText(string, x: x, y: y)
      }
    }
  }
}
