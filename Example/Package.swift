// swift-tools-version:5.6

import PackageDescription

let package = Package(
  name: "SwiftHelloWorld",
  products: [
    .library(
      name: "SwiftHelloWorld",
      targets: ["SwiftHelloWorld"]
    ),
  ],
  dependencies: [
    .package(name: "swift-playdate", path: "../")
  ],
  targets: [
    .target(
      name: "SwiftHelloWorld",
      dependencies: [
        .product(name: "Playdate", package: "swift-playdate")
      ]
    )
  ]
)
