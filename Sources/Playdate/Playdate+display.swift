import CPlaydate

extension Playdate {
  public class Display {
    let pointee: playdate_display
    let graphicsPointee: playdate_graphics

    var _inverted: Bool = false
    var _refreshRate: Float = 20
    var _scale: Int = 1

    init(_ pd: PlaydateAPI) {
      pointee = pd.display.pointee
      graphicsPointee = pd.graphics.pointee
    }
  }
}

extension Playdate.Display {

  /// Returns the height of the display, taking the current scale into account;
  /// e.g., if the scale is 2, this function returns 120 instead of 240.
  ///
  public var height: Int {
    Int(pointee.getHeight())
  }

  /// Returns the width of the display, taking the current scale into account;
  /// e.g., if the scale is 2, this function returns 200 instead of 400.
  ///
  public var width: Int {
    Int(pointee.getWidth())
  }

  /// Get or set the display scale factor. Valid values for scale are 1, 2, 4, and 8.
  /// The top-left corner of the frame buffer is scaled up to fill the display;
  /// e.g., if the scale is set to 4, the pixels in rectangle [0,100] x [0,60] are drawn on the screen as 4 x 4 squares.
  ///
  public var scale: Int {
    get { _scale }
    set {
      // only allow valid scales
      if [1, 2, 4, 8].contains(newValue) {
        _scale = newValue
        pointee.setScale(UInt32(newValue))
      } else {
        Playdate.shared.system.log(
          "Warning! Attempted to set invalid display scale: \(newValue), discarding."
        )
      }
    }
  }

  /// Returns wether or not the display's drawing is inverted. Can set this value too.
  ///
  public var isInverted: Bool {
    get { _inverted }
    set {
      _inverted = newValue
      pointee.setInverted(newValue ? 1 : 0)
    }
  }

  /// Get or set the nominal refresh rate in frames per second.
  /// Default is 20 fps, the maximum rate supported by the hardware for full-frame updates.
  /// 
  public var refreshRate: Float {
    get { _refreshRate }
    set {
      _refreshRate = newValue
      pointee.setRefreshRate(newValue)
    }
  }
}

extension Playdate.Display {
  /// Manually flushes the current frame buffer out to the display.
  /// This function is automatically called after each pass through the run loop, so there shouldn’t be any need to call it yourself.
  ///
  func flush() {
    graphicsPointee.display()
  }
}
