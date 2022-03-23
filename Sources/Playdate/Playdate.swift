import CPlaydate

public class Playdate {
  public static let shared = Playdate()

  public private(set) var system: System!
  public private(set) var graphics: Graphics!
  public private(set) var display: Display!

  internal var eventHandler: ((SystemEvent) -> Void)? = nil
  internal var updateCallback: (() -> Bool)? = nil

  public func register(_ pd: PlaydateAPI) {
    self.system = .init(pd)
    self.graphics = .init(pd)
    self.display = .init(pd)
  }
}

