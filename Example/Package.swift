// swift-tools-version:5.6

import PackageDescription

let package = Package(
  name: "SwiftHelloWorld",
  products: [
    .library(
      name: "SwiftHelloWorld",
      type: .dynamic,
      targets: ["SwiftHelloWorld"]
    ),
  ],
  dependencies: [
    .package(name: "Playdate", path: "../Playdate")
  ],
  targets: [
    .target(
      name: "SwiftHelloWorld",
      dependencies: [
        .product(name: "Playdate", package: "Playdate")
      ]
    )
  ]
)
