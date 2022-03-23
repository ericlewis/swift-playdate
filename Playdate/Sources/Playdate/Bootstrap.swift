import CPlaydate

dynamic public func UpdateCallback() -> Bool { false }
dynamic public func EventCallback(event: SystemEvent) {}

func _update(_: UnsafeMutableRawPointer?) -> CInt {
  UpdateCallback() ? 1 : 0
}

@_cdecl("eventHandler")
public func eventHandler(_ playdate: PlaydateAPI, _ event: PDSystemEvent, _ arg: CInt) -> CInt {
  let event = SystemEvent(event)
  Playdate.shared.register(playdate)
  EventCallback(event: event)

  switch event {
  case .initialize:
    playdate.system.pointee.setUpdateCallback(_update, nil)
  default:
    break
  }

  return 0;
}
