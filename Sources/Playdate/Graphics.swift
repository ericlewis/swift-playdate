import CPlaydate

public let Graphics = _Graphics.shared

public class _Graphics {
  public static let shared = _Graphics()
  
  let pointee: playdate_graphics = Playdate.shared.graphics
  var _backgroundColor: Color?
}

extension _Graphics {
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
      pointee.setBackgroundColor(.init(rawValue: color.cValue))
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
  public func display() {
    pointee.display()
  }

  /// Sets the font to use in subsequent drawText calls.
  ///
  public func setFont<F: Font>(_ font: F, weight: F.Weight, size: F.Size) throws {
    let font = pointee.loadFont("Asheville-Sans-14-Bold", nil) // TODO: handle error
    pointee.setFont(font)
  }

  /// Sets the font to use in subsequent drawText calls.
  ///
  public func setFont(_ path: String) throws {
    let font = pointee.loadFont(path, nil) // TODO: handle error
    pointee.setFont(font)
  }

  /// Sets the mode used for drawing bitmaps.
  /// Note that text drawing uses bitmaps, so this affects how fonts are displayed as well.
  ///
  public func setDrawMode(_ mode: DrawMode) {
    pointee.setDrawMode(mode.cMode)
  }

  /// Clears the current clip rectangle
  ///
  public func clearClipRectangle() {
    pointee.clearClipRect()
  }

  /// Sets the end cap style used in the line drawing functions.
  ///
  public func setLineCapStyle(_ style: LineCapStyle) {
    pointee.setLineCapStyle(style.cStyle)
  }

  /// Sets the tracking to use when drawing text.
  ///
  public func setTextTracking(_ tracking: Int) {
    pointee.setTextTracking(Int32(tracking))
  }

  /// Sets the leading adjustment (added to the leading specified in the font) to use when drawing text.
  ///
  public func setTextLeading(_ adjustment: Int) {
    pointee.setTextLeading(Int32(adjustment))
  }

  /// Sets the current clip rect in screen coordinates.
  ///
  public func setScreenClip(_ bounds: Rectangle) {
    pointee.setScreenClipRect(
      Int32(bounds.x),
      Int32(bounds.y),
      Int32(bounds.width),
      Int32(bounds.height)
    )
  }

  /// Sets the current clip rect, using world coordinates—​that is, the given rectangle will be translated by the current drawing offset.
  /// The clip rect is cleared at the beginning of each update.
  ///
  public func setClip(_ bounds: Rectangle) {
    pointee.setClipRect(
      Int32(bounds.x),
      Int32(bounds.y),
      Int32(bounds.width),
      Int32(bounds.height)
    )
  }

  /// Pops a context off the stack (if any are left), restoring the drawing settings from before the context was pushed.
  ///
  public func popContext() {
    pointee.popContext()
  }
}

extension _Graphics {
  /// Draws a width by height rect at x, y.
  ///
  public func drawRectangle(_ bounds: Rectangle, color: Color) {
    pointee.drawRect(
      Int32(bounds.x),
      Int32(bounds.y),
      Int32(bounds.width),
      Int32(bounds.height),
      .init(color.cValue)
    )
  }

  /// Draws a filled width by height rect at x, y.
  ///
  public func drawFilledRectangle(_ bounds: Rectangle, color: Color) {
    pointee.fillRect(
      Int32(bounds.x),
      Int32(bounds.y),
      Int32(bounds.width),
      Int32(bounds.height),
      .init(color.cValue)
    )
  }

  /// Draws a line from start coordinates to end coordinates with a stroke width.
  ///
  public func drawLine(from p1: Point, to p2: Point, color: Color, stroke width: Int = 1) {
    pointee.drawLine(
      Int32(p1.x),
      Int32(p1.y),
      Int32(p2.x),
      Int32(p2.y),
      Int32(width),
      .init(color.cValue)
    )
  }

  /// Draws the bitmap with its upper-left corner at location x, y, using the given flip orientation.
  ///
  public func drawBitmap(_ bitmap: Bitmap, at location: Point) {
    // TODO: expose flip
    pointee.drawBitmap(bitmap.ptr, .init(location.x), .init(location.y), kBitmapUnflipped)
  }

  /// Draws the bitmap scaled to xscale and yscale with its upper-left corner at location x, y.
  /// Note that flip is not available when drawing scaled bitmaps but negative scale values will achieve the same effect.
  ///
  public func drawScaledBitmap(_ bitmap: Bitmap, at location: Point, scale: Point = .init(x: 1, y: 1)) {
    // TODO: expose flip
    pointee.drawScaledBitmap(bitmap.ptr, .init(location.x), .init(location.y), .init(scale.x), .init(scale.y))
  }
}
