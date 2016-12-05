cmake_minimum_required(VERSION 3.6)

project(python-runner)

find_package(ECM 0.0.11 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(FeatureSummary)

find_package(Qt5 REQUIRED COMPONENTS Widgets)
find_package(KF5 REQUIRED COMPONENTS CoreAddons Solid KIO Runner I18n)

find_package(PythonLibs 3.5 REQUIRED)
find_program(PYTHON "python" REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})

find_package(Boost COMPONENTS python3 REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

add_definitions(-DQT_NO_KEYWORDS)
add_definitions(-DNDEBUG)
include(CheckCXXCompilerFlag)

# http://stackoverflow.com/a/33266748/3946766
function(enable_cxx_compiler_flag_if_supported flag)
    string(FIND "${CMAKE_CXX_FLAGS}" "${flag}" flag_already_set)
    if(flag_already_set EQUAL -1)
        check_cxx_compiler_flag("${flag}" flag_supported)
        if(flag_supported)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flag}" PARENT_SCOPE)
        endif()
    endif()
endfunction()

enable_cxx_compiler_flag_if_supported("-Wall")
enable_cxx_compiler_flag_if_supported("-Wextra")
enable_cxx_compiler_flag_if_supported("-pedantic")

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

execute_process(COMMAND ${PYTHON}-config --ldflags
                    OUTPUT_VARIABLE PYTHON_LDFLAGS
                    OUTPUT_STRIP_TRAILING_WHITESPACE
                    ERROR_QUIET)
string(STRIP "${PYTHON_LDFLAGS}" PYTHON_LDFLAGS)

get_filename_component(PYTHON_DL_NAME ${PYTHON_LIBRARIES} NAME)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/python-krunner-conf.hpp.in ${CMAKE_CURRENT_BINARY_DIR}/python-krunner-conf.hpp)

set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/sip_krunnerpart0.cpp PROPERTIES
    GENERATED TRUE
)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/sip_krunnerpart0.cpp
    PRE_BUILD
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/config.py src/sip/
    COMMAND ${PYTHON} ${CMAKE_CURRENT_SOURCE_DIR}/config.py)

add_library(plasma_runner_python SHARED src/python-krunner.cpp ${CMAKE_CURRENT_BINARY_DIR}/sip_krunnerpart0.cpp)
set_target_properties(plasma_runner_python PROPERTIES PREFIX "")
target_link_libraries(plasma_runner_python KF5::Runner KF5::KIOCore KF5::KIOWidgets KF5::I18n KF5::CoreAddons)
target_link_libraries(plasma_runner_python Qt5::Widgets)
target_link_libraries(plasma_runner_python ${Boost_LIBRARIES})
target_link_libraries(plasma_runner_python ${PYTHON_LDFLAGS})

# Install the library and .desktop file
# TODO make the qt plugin a symlink
install(TARGETS plasma_runner_python DESTINATION ${QT_PLUGINS_DIR})

install(CODE "execute_process(COMMAND ${PYTHON} ${CMAKE_CURRENT_SOURCE_DIR}/setup.py install)")

# get_cmake_property(_variableNames VARIABLES)
# foreach (_variableName ${_variableNames})
#     message(STATUS "${_variableName}=${${_variableName}}")
# endforeach()
