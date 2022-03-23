import Foundation
import CPlaydate

public struct MenuItem {
  fileprivate let id = UUID()
  fileprivate let _ptr: OpaquePointer
  fileprivate let _func: (() -> Void)?
  fileprivate let _funcCheckmark: ((Bool) -> Void)?
  fileprivate let _funcOption: ((String) -> Void)?
  fileprivate var _options: [String] = []

  public let title: String
}

public class Menu {
  public static let shared = Menu()

  public private(set) var menuItems: [MenuItem] = []

  fileprivate let sys = Playdate.shared.system.pointee

  init() {}
}

extension Menu {

  /// Adds a new menu item to the System Menu.
  /// `title` will be the title displayed by the menu item.
  /// When invoked by the user, this menu item will:
  ///
  /// 1. Invoke your callback function.
  /// 2. Hide the System Menu.
  /// 3. Unpause your game and call eventHandler() with the `resume` SystemEvent.
  ///
  /// Your game can then present an options interface to the player, or take other action, in whatever manner you choose.
  ///
  @discardableResult
  public func addItem(_ title: String, f: @escaping () -> Void) throws -> MenuItem {
    let ptr: OpaquePointer
    switch menuItems.count {
    case 0:
      ptr = sys.addMenuItem(title, _firstMenuCallback(_:), nil)!
    case 1:
      ptr = sys.addMenuItem(title, _secondMenuCallback(_:), nil)!
    case 2:
      ptr = sys.addMenuItem(title, _thirdMenuCallback(_:), nil)!
    default:
      fatalError() // TODO: can't add more than 3, also, don't fatal.
    }
    let menuItem = MenuItem(
      _ptr: ptr,
      _func: f,
      _funcCheckmark: nil,
      _funcOption: nil,
      title: title
    )
    menuItems.append(menuItem)
    return menuItem
  }

  /// Adds a new menu item that can be checked or unchecked by the player.
  ///
  /// `title` will be the title displayed by the menu item.
  /// `isOn` should be `true` for checked, `false` for unchecked.
  ///
  /// If this menu item is interacted with while the system menu is open, callback will be called when the menu is closed.
  ///
  public func addCheckmarkItem(_ title: String, isOn: Bool, f: @escaping (Bool) -> Void) throws -> MenuItem {
    let isOnInt: Int32 = isOn ? 1 : 0
    let ptr: OpaquePointer
    switch menuItems.count {
    case 0:
      ptr = sys.addCheckmarkMenuItem(title, isOnInt, _firstMenuCallback(_:), nil)!
    case 1:
      ptr = sys.addCheckmarkMenuItem(title, isOnInt, _secondMenuCallback(_:), nil)!
    case 2:
      ptr = sys.addCheckmarkMenuItem(title, isOnInt, _thirdMenuCallback(_:), nil)!
    default:
      fatalError() // TODO: can't add more than 3, also, don't fatal.
    }
    let menuItem = MenuItem(
      _ptr: ptr,
      _func: nil,
      _funcCheckmark: f,
      _funcOption: nil,
      title: title
    )
    menuItems.append(menuItem)
    return menuItem
  }

  /// Adds a new menu item that allows the player to cycle through a set of options.
  ///
  /// `title` will be the title displayed by the menu item.
  ///
  /// options should be an array of strings representing the states this menu item can cycle through.
  /// Due to limited horizontal space, the option strings and title should be kept short for this type of menu item.
  ///
  /// If this menu item is interacted with while the system menu is open, callback will be called when the menu is closed.
  ///
  public func addOptionItem(_ title: String, options: [String], f: @escaping (String) -> Void) throws -> MenuItem {
    let count = CInt(options.count)
    var cOptions = withArrayOfCStrings(options, { $0 }).map { UnsafePointer($0) }

    let ptr: OpaquePointer
    switch menuItems.count {
    case 0:
      ptr = sys.addOptionsMenuItem(title, &cOptions, count, _firstMenuCallback(_:), nil)!
    case 1:
      ptr = sys.addOptionsMenuItem(title, &cOptions, count, _secondMenuCallback(_:), nil)!
    case 2:
      ptr = sys.addOptionsMenuItem(title, &cOptions, count, _thirdMenuCallback(_:), nil)!
    default:
      fatalError() // TODO: can't add more than 3, also, don't fatal.
    }

    let menuItem = MenuItem(
      _ptr: ptr,
      _func: nil,
      _funcCheckmark: nil,
      _funcOption: f,
      _options: options,
      title: title
    )
    menuItems.append(menuItem)
    return menuItem
  }

  /// Removes the menu item from the system menu.
  ///
  public func remove(item: MenuItem) {
    guard let idx = menuItems.firstIndex(where: { item.id == $0.id }) else {
      // TODO: warn if we miss?
      return
    }
    sys.removeMenuItem(menuItems[idx]._ptr)
    menuItems.remove(at: idx)
  }

  /// Removes all custom menu items from the system menu.
  ///
  public func removeAll() {
    sys.removeAllMenuItems()
    menuItems.removeAll()
  }
}

fileprivate func _firstMenuCallback(_ ptr: UnsafeMutableRawPointer?) {
  let item = Menu.shared.menuItems[0]
  let index = Menu.shared.sys.getMenuItemValue(item._ptr)

  if let reg = item._func {
    reg()
  } else if let check = item._funcCheckmark {
    check(index == 1 ? true : false)
  } else if let option = item._funcOption {
    option(item._options[Int(index)])
  }
}

fileprivate func _secondMenuCallback(_ ptr: UnsafeMutableRawPointer?) {
  let item = Menu.shared.menuItems[1]
  let index = Menu.shared.sys.getMenuItemValue(item._ptr)

  if let reg = item._func {
    reg()
  } else if let check = item._funcCheckmark {
    check(index == 1 ? true : false)
  } else if let option = item._funcOption {
    option(item._options[Int(index)])
  }
}

fileprivate func _thirdMenuCallback(_ ptr: UnsafeMutableRawPointer?) {
  let item = Menu.shared.menuItems[2]
  let index = Menu.shared.sys.getMenuItemValue(item._ptr)

  if let reg = item._func {
    reg()
  } else if let check = item._funcCheckmark {
    check(index == 1 ? true : false)
  } else if let option = item._funcOption {
    option(item._options[Int(index)])
  }
}

fileprivate func withArrayOfCStrings<R>(
  _ args: [String],
  _ body: ([UnsafeMutablePointer<CChar>?]) -> R
) -> R {
  let cStrings = args.map { strdup($0.cString(using: .ascii)) }

  // FIXME: there is a probable memory leak here, we should be freeing our strings i think.

  return body(cStrings)
}
