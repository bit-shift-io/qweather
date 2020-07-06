project(qweather)
project(PROJECT_NAME LANGUAGES CXX)

set(QT_MIN_VERSION "5.5.0")

cmake_minimum_required(VERSION 3.5)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(UNIX)
  # Enable C++11 features for clang and gcc
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++0x")
endif()

################# Debug Flags #################

# enable debug via cmd
# cmake -DDEFINE_DEBUG=ON -DCMAKE_INSTALL_PREFIX=/usr ..

option(DEFINE_DEBUG
  "Build using debug"
  OFF)

if(DEFINE_DEBUG)
  message("Adding Debug flag...")
  add_definitions(-DDEBUG)
  set(CMAKE_BUILD_TYPE Debug)
  message("Build type is " ${CMAKE_BUILD_TYPE})
endif()

# debug, enable debugging
# https://doc.qt.io/qt-5/qtquick-debugging.html#qml-debugging-infrastructure

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DQT_QML_DEBUG ")
    message(STATUS "debug enabled")
endif()

################# Configure paths #################

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>) # /lib
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>) # /lib
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>) # /bin

################# Android #################

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

################# QT components #################

# widgets for systray qapplication
# graphical effects for drop shadow
# charts for charts
find_package(Qt5 COMPONENTS REQUIRED Core Quick Svg)

################# Files #################

set(SOURCES
    src/main.cpp
    src/database.cpp
    src/fileio.cpp
    src/forecastmodel.cpp
    src/qftp/qftp.cpp
    src/qftp/qurlinfo.cpp
    src/radarimage.cpp
    src/settings.cpp
    src/weather.cpp
)

set(HEADERS
    src/database.h
    src/fileio.h
    src/forecastmodel.h
    src/qftp/qftp.h
    src/qftp/qurlinfo.h
    src/radarimage.h
    src/settings.h
    src/weather.h
)

set(RESOURCES
    src/qml.qrc
    res/res.qrc
)

################# Target #################

qt5_add_resources(RCC_SOURCES ${RESOURCES})

if(ANDROID)
  add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS} ${RCC_SOURCES})
else()
  add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${RCC_SOURCES})
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core Qt5::Quick)
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "qweather")

################# Install #################

install(TARGETS ${PROJECT_NAME} DESTINATION bin)

if(UNIX)
  install(FILES bitshift.weather.desktop DESTINATION share/applications ) # /usr/share?
  install(FILES res/bitshift.weather.svg DESTINATION share/pixmaps ) # /usr/share?
endif()