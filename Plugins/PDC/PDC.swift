import Foundation
import PackagePlugin

@main
struct PDC: CommandPlugin {
  let pdxVersionString = "pdxversion=10900"

  func performCommand(context: PluginContext, arguments: [String]) throws {
    let cwd = context.pluginWorkDirectory.string
    let pwd = context.package.directory.string
    let dylib = "\(cwd)/debug/lib\(context.package.displayName).dylib"
    let artifactsPath = "\(cwd)/artifacts/"
    let pdxinfo = "\(artifactsPath)pdxinfo"
    let pdxdylib = "\(artifactsPath)pdex.dylib"
    let pdxbin = "\(artifactsPath)pdex.bin"
    let pdcOut = "\(artifactsPath)\(context.package.displayName)"
    let pdx = "\(pdcOut).pdx"
    let resources = "\(pwd)/Sources/Resources"

    // 1. build target
    try context.run("swift", ["build", "--build-path", cwd])

    // 2. clean artifacts
    try context.run("rm", ["-fr", artifactsPath])

    // 3. collect resources
    try context.run("cp", ["-fr", resources, artifactsPath])

    // 3.1 append pdxversion
    // FIXME: should use pdc to get the version.
    let infoURL = URL(fileURLWithPath: pdxinfo)
    var info = try String(data: Data(contentsOf: infoURL), encoding: .utf8)!
    info.append(pdxVersionString)
    try info.write(to: infoURL, atomically: true, encoding: .utf8)

    // 4. move dylib
    try context.run("mv", [dylib, pdxdylib])

    // 5. placeholder bin
    // TODO: not used by sim currently, but needed for device & pdc
    try context.run("touch", [pdxbin])

    // 6. run pdc
    try context.run("pdc", [artifactsPath, pdcOut])

    // 7. TODO: allow user to specify pdx location

    // 7.1 optionally open in sim.
    if arguments.contains("--run") {
      try context.run("open", [pdx])
    }

    return
  }
}

extension PluginContext {
  func run(_ command: String, _ args: [String]) throws {
    let cmd = Process()
    cmd.executableURL = URL(fileURLWithPath: try self.tool(named: command).path.string)
    cmd.arguments = args
    try cmd.run()
    cmd.waitUntilExit()
  }
}
