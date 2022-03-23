import CPlaydate

extension Playdate {
  public class System {
    var pd: PlaydateAPI
    var pointee: playdate_sys

    init(_ pd: PlaydateAPI) {
      self.pd = pd
      pointee = pd.system.pointee
    }
  }
}

extension Playdate.System {
  public var batteryPercentage: Float {
    get {
      pointee.getBatteryPercentage()
    }
  }

  public var batteryVoltage: Float {
    get {
      pointee.getBatteryVoltage()
    }
  }

  /// Calculates the current frames per second and draws that value at x, y.
  ///
  public func drawFPS(x: Int, y: Int) {
    pointee.drawFPS(CInt(x), CInt(y))
  }

  /// Calls the log function, equivalent to print() in Lua.
  ///
  public func log(_ message: String) {
    logToConsole(withUnsafeMutablePointer(to: &pd) { $0 }, message)
  }

  /// Calls the log function, outputting an error in red to the console, then pauses execution.
  ///
  public func error(_ message: String) {
    errorToConsole(withUnsafeMutablePointer(to: &pd) { $0 }, message)
  }

  /// Replaces the default Lua run loop function with a custom update function.
  /// The update function should return a bool indicating to the system a need to update the display.
  ///
  public func setUpdateCallback(_ callback: @escaping () -> Bool) {
    Playdate.shared.updateCallback = callback
    pointee.setUpdateCallback(_update, nil)
  }
}
