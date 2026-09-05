add_executable(RadiantTests RadiantTestMain.cpp)

target_link_libraries(
  RadiantTests
  PRIVATE Radiant
  PRIVATE Catch2::Catch2WithMain)

target_include_directories(RadiantTests PUBLIC ${CMAKE_SOURCE_DIR}/include)

include(CTest)
include(Catch)

catch_discover_tests(RadiantTests)
