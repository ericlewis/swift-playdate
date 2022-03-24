import CPlaydate

open class Bitmap {
  let ptr: OpaquePointer?

  public var size: Size {
    var width: Int32 = 0
    var height: Int32 = 0
    Graphics.pointee.getBitmapData(ptr, &width, &height, nil, nil, nil)
    return .init(width: Float(width), height: Float(height))
  }

  public init(width: Int, height: Int, color: Color) {
    ptr = Graphics.pointee.newBitmap(Int32(width), Int32(height), .init(color.cValue))
  }

  public init(_ path: String) {
    ptr = Graphics.pointee.loadBitmap(path, nil) // TODO: handle error
  }

  init(_ ptr: OpaquePointer?) {
    self.ptr = ptr
  }

  public func clear(color: Color) {
    Graphics.pointee.clear(.init(color.cValue))
  }

  public func copy() -> Bitmap {
    Bitmap(Graphics.pointee.copyBitmap(ptr))
  }

  public func rotate(_ degress: Float, scale: Point = .init(x: 1, y: 1)) {
    // TODO: implement
  }

  deinit {
    Graphics.pointee.freeBitmap(ptr)
  }
}
