cmake_minimum_required(VERSION 3.6)

project(pyrunner)

find_package(ECM 0.0.11 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

include(KDEInstallDirs)
include(KDECMakeSettings)
# include(KDECompilerSettings)
include(FeatureSummary)

find_package(Qt5 REQUIRED COMPONENTS Widgets)
find_package(KF5 REQUIRED COMPONENTS CoreAddons Solid KIO Runner I18n)

find_package(PythonLibs 3.5 REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})

find_package(Boost COMPONENTS python3 REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

add_definitions(-DQT_NO_KEYWORDS)

include_directories(src)

set(example_SRCS src/pyrunner.cpp)

add_library(plasma_runner_pyrunner MODULE ${example_SRCS})
target_link_libraries(plasma_runner_pyrunner KF5::Runner KF5::KIOCore KF5::KIOWidgets KF5::I18n KF5::CoreAddons)
target_link_libraries(plasma_runner_pyrunner Qt5::Widgets)
target_link_libraries(plasma_runner_pyrunner ${Boost_LIBRARIES})
target_link_libraries(plasma_runner_pyrunner ${PYTHON_LIBRARIES})


# https://github.com/lxde/lxqt-qtplugin/blob/master/src/CMakeLists.txt
get_target_property(QT_QMAKE_EXECUTABLE ${Qt5Core_QMAKE_EXECUTABLE} IMPORTED_LOCATION)
if(NOT QT_QMAKE_EXECUTABLE)
    message(FATAL_ERROR "qmake is not found.")
endif()

# execute the command "qmake -query QT_INSTALL_PLUGINS" to get the path of plugins dir.
execute_process(COMMAND ${QT_QMAKE_EXECUTABLE} -query QT_INSTALL_PLUGINS
    OUTPUT_VARIABLE QT_PLUGINS_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
if(QT_PLUGINS_DIR)
    message(STATUS "Qt5 plugin directory:" "${QT_PLUGINS_DIR}")
else()
    message(FATAL_ERROR "Qt5 plugin directory cannot be detected.")
endif()


# Install the library and .desktop file
install(TARGETS plasma_runner_pyrunner DESTINATION ${QT_PLUGINS_DIR})
install(FILES plasma-runner-pyrunner-a.desktop DESTINATION ${SERVICES_INSTALL_DIR})
install(FILES plasma-runner-pyrunner-b.desktop DESTINATION ${SERVICES_INSTALL_DIR})
#
# get_cmake_property(_variableNames VARIABLES)
# foreach (_variableName ${_variableNames})
#     message(STATUS "${_variableName}=${${_variableName}}")
# endforeach()
