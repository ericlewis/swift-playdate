// swift-tools-version:5.6
// The swift-tools-version declares the minimum version of Swift required to build this package.

import PackageDescription

let package = Package(
  name: "SwiftHelloWorld",
  products: [
    .library(name: "SwiftHelloWorld", type: .dynamic, targets: ["SwiftHelloWorld"]),
  ],
  dependencies: [.package(name: "PlaydateAPI", path: "../Playdate")],
  targets: [
    .target(
      name: "SwiftHelloWorld",
      dependencies: [
        .product(name: "Playdate", package: "PlaydateAPI")
      ]
    )
  ]
)
