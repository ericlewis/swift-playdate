import CPlaydate

public struct DisplayList {
  public static func updateAndDrawSprites() {
    Sprite.shared.updateAndDrawSprites()
  }
}

open class Sprite {
  fileprivate static let shared = Playdate.shared.sprites!
  let ptr: OpaquePointer?

  public var position: Point {
    var x: Float = 0
    var y: Float = 0
    Sprite.shared.getPosition(ptr, &x, &y)
    return .init(x: x, y: y)
  }

  public init() {
    ptr = Sprite.shared.newSprite()
  }

  public func setImage(_ bmp: Bitmap) {
    Sprite.shared.setImage(ptr, bmp.ptr, kBitmapUnflipped)
  }

  public func move(to point: Point) {
    Sprite.shared.moveTo(ptr, point.x, point.y)
  }

  public func move(by delta: Point) {
    Sprite.shared.moveBy(ptr, delta.x, delta.y)
  }

  public func addToDisplayList() {
    Sprite.shared.addSprite(ptr)
  }

  deinit {
    Sprite.shared.freeSprite(ptr)
  }
}
