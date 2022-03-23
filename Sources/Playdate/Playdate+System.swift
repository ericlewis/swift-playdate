import CPlaydate
import Foundation

extension Playdate {
  public class System {
    var pd: PlaydateAPI
    var pointee: playdate_sys
    var _isAutoLockEnabled: Bool = true

    init(_ pd: PlaydateAPI) {
      self.pd = pd
      pointee = pd.system.pointee
    }
  }
}

// MARK: Getters & Setters

extension Playdate.System {
  /// Returns the current language of the system.
  ///
  public var language: Language {
    Language(pointee.getLanguage())
  }

  /// Returns a value from 0-100 denoting the current level of battery charge. 0 = empty; 100 = full.
  ///
  public var batteryPercentage: Float {
    get {
      pointee.getBatteryPercentage()
    }
  }

  /// Returns the battery’s current voltage level.
  ///
  public var batteryVoltage: Float {
    pointee.getBatteryVoltage()
  }

  /// Returns the number of seconds since…​some arbitrary point in time.
  /// This should present a consistent timebase while a game is running, but the counter will be disabled when the device is sleeping.
  ///
  public var currentTimeInterval: TimeInterval {
    TimeInterval(pointee.getCurrentTimeMilliseconds()) * 0.001
  }

  /// Returns `true` if the global "flipped" system setting is set, otherwise `false`.
  ///
  public var isFlipped: Bool {
    pointee.getFlipped() == 1 ? true : false
  }

  /// Returns `true` if the global "reduce flashing" system setting is set, otherwise `false`.
  ///
  public var reducedFlashing: Bool {
    pointee.getReduceFlashing() == 1 ? true : false
  }

  /// Disables or enables the 60 second auto lock feature. When changed, the timer is reset to 60 seconds.
  ///
  public var isAutoLockEnabled: Bool {
    get { _isAutoLockEnabled }
    set {
      _isAutoLockEnabled = newValue
      pointee.setAutoLockDisabled(newValue ? 0 : 1)
    }
  }
}

// MARK: Methods

extension Playdate.System {
  /// Allocates heap space if ptr is `nil`, else reallocates the given pointer. If size is zero, frees the given pointer.
  ///
  public func realloc(ptr: UnsafeMutableRawPointer?, size: Int) -> UnsafeMutableRawPointer? {
    pointee.realloc(ptr, size)
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

extension Playdate.System {
  /// The last-read accelerometer data.
  var accelerometerValue: AccelerometerData {
    var x: Float = -1
    var y: Float = -1
    var z: Float = -1
    pointee.getAccelerometer(&x, &y, &z)
    return .init(x: x, y: y, z: z)
  }

  /// By default, the accelerometer is disabled to save (a small amount of) power.
  /// Accelerometer data is not available until the next update cycle after it’s enabled.
  ///
  func startAccelerometerUpdates() {
    pointee.setPeripheralsEnabled(kAccelerometer)
  }

  /// Disables accelerometer data output
  ///
  func stopAccelerometerUpdates() {
    pointee.setPeripheralsEnabled(kNone)
  }
}
