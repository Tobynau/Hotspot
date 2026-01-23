cmake_minimum_required(VERSION 3.16)
project(hotspot_gui LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Quick Core Qml DBus QuickControls2)

include_directories(${CMAKE_SOURCE_DIR}/src)

add_executable(hotspot-gui
  src/gui/main.cpp
  src/gui/HotspotClient.cpp
  src/gui/qml/Main.qml
)

target_link_libraries(hotspot-gui PRIVATE Qt6::Quick Qt6::Qml Qt6::Core Qt6::DBus Qt6::QuickControls2)

add_executable(hotspot-daemon
  src/privileged/daemon_main.cpp
  src/backend/daemon.cpp
  src/backend/HostapdController.cpp
  src/backend/nat/NatManager.cpp
  src/backend/linux/NetworkManagerAdapter.cpp
  src/backend/linux/DummyAdapter.cpp
)

target_link_libraries(hotspot-daemon PRIVATE Qt6::Core Qt6::DBus)

# Install rules and packaging will be added later
