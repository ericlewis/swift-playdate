import CPlaydate

dynamic public func EventCallback(event: SystemEvent) {}

func _update(_ : UnsafeMutableRawPointer?) -> CInt {
  guard let update = Playdate.shared.updateCallback?() else {
    return 0
  }
  return update ? 1 : 0
}

@_cdecl("eventHandler")
public func eventHandler(_ playdate: PlaydateAPI, _ event: PDSystemEvent, _ arg: CInt) -> CInt {
  let event = SystemEvent(event)
  Playdate.shared.register(playdate)
  EventCallback(event: event)
  return 0;
}
