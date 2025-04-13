cmake_minimum_required(VERSION 3.19)
project(fsm LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Scxml StateMachine)

file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE HEADERS "${CMAKE_SOURCE_DIR}/src/*.h")

qt_standard_project_setup()

qt_add_executable(fsm
    ${SOURCES}
    ${HEADERS}
)

target_link_libraries(fsm PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Scxml Qt6::StateMachine)

include(GNUInstallDirs)

install(TARGETS fsm
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET fsm
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
