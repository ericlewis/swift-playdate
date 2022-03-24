import CPlaydate

open class Bitmap {
  let ptr: OpaquePointer?

  public init(width: Int, height: Int, color: Color) {
    ptr = Graphics.pointee.newBitmap(Int32(width), Int32(height), .init(color.cValue))
  }

  public init(_ path: String) {
    ptr = Graphics.pointee.loadBitmap(path, nil) // TODO: handle error
  }

  public func clear(color: Color) {
    Graphics.pointee.clear(.init(color.cValue))
  }

  deinit {
    Graphics.pointee.freeBitmap(ptr)
  }
}
