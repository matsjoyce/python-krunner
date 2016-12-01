cmake_minimum_required(VERSION 3.6)

project(example-homefiles)

find_package(ECM 0.0.11 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

include(KDEInstallDirs)

install(FILES plasma-runner-homefiles-py.desktop DESTINATION ${SERVICES_INSTALL_DIR})
