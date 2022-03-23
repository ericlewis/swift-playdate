import CPlaydate

extension Playdate {
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
