import CPlaydate

class Playdate {
  static let shared = Playdate()
  
  var pd: PlaydateAPI!
  var system: playdate_sys!
  var graphics: playdate_graphics!
  var display: playdate_display!
  var sprites: playdate_sprite!

  var updateCallback: (() -> Bool)? = nil

  func register(_ pd: PlaydateAPI) {
    self.pd = pd
    self.system = pd.system.pointee
    self.graphics = pd.graphics.pointee
    self.display = pd.display.pointee
    self.sprites = pd.sprite.pointee
  }
}

