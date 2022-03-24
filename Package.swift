// swift-tools-version:5.6
// The swift-tools-version declares the minimum version of Swift required to build this package.

import PackageDescription

let package = Package(
  name: "swift-playdate",
  platforms: [.custom("Playdate", versionString: "1.0")],
  products: [
    .library(name: "Playdate", targets: ["Playdate"]),
    .plugin(name: "PDC", targets: ["PDC"])
  ],
  targets: [
    .systemLibrary(
      name: "CPlaydate"
    ),
    .target(
      name: "Playdate",
      dependencies: ["CPlaydate"]
    ),
    .plugin(
      name: "PDC",
      capability: .command(
        intent: .custom(verb: "pdc", description: "builds a pdex from resources")
      )
    ),
    .plugin(
      name: "LinkSDK",
      capability: .buildTool()
    ),
  ]
)
