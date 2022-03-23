import CPlaydate

public class Playdate {
  public static let shared = Playdate()

  var pd: PlaydateAPI!
  var system: playdate_sys!
  public private(set) var graphics: playdate_graphics!
  public private(set) var display: playdate_display!

  internal var eventHandler: ((SystemEvent) -> Void)? = nil
  internal var updateCallback: (() -> Bool)? = nil

  public func register(_ pd: PlaydateAPI) {
    self.pd = pd
    self.system = pd.system.pointee
    self.graphics = pd.graphics.pointee
    self.display = pd.display.pointee
  }
}

