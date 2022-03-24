import CPlaydate

open class Bitmap {
  let ptr: OpaquePointer?
  
  public init(_ path: String) {
    ptr = Playdate.shared.graphics.loadBitmap(path, nil) // TODO: handle error
  }

  deinit {
    Playdate.shared.graphics.freeBitmap(ptr)
  }
}
