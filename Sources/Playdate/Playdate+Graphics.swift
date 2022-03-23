import CPlaydate

extension Playdate {
  public class Graphics {
    let pointee: playdate_graphics
    var _backgroundColor: Color?

    init(_ pd: PlaydateAPI) {
      pointee = pd.graphics.pointee
    }
  }
}

extension Playdate.Graphics {
  /// Background color shown when the display is offset or for clearing dirty areas in the sprite system.
  ///
  public var backgroundColor: Color? {
    get { _backgroundColor }
    set {
      guard let color = newValue else {
        _backgroundColor = nil
        pointee.setBackgroundColor(kColorClear)
        return
      }
      _backgroundColor = newValue
      pointee.setBackgroundColor(.init(rawValue: UInt32(color.rawValue)))
    }
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

  /// Clears the entire display, filling it with color.
  /// 
  public func clear(with color: Color) {
    pointee.clear(LCDColor(color.rawValue))
  }

  /// Manually flushes the current frame buffer out to the display.
  /// This function is automatically called after each pass through the run loop, so there shouldn’t be any need to call it yourself.
  ///
  func display() {
    pointee.display()
  }
}
