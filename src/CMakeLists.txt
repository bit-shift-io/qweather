set(HEADER
)

set(SOURCE
    main.cpp
)

set(RESOURCE
    resources.qrc
)

qt5_add_resources(RESOURCE ${RESOURCE})
add_executable(${PROJECT_NAME} ${SOURCE} ${HEADER} ${RESOURCE})
target_link_libraries(${PROJECT_NAME} Qt5::Core  Qt5::Qml Qt5::Quick Qt5::Svg)
install(TARGETS ${PROJECT_NAME} ${KF5_INSTALL_TARGETS_DEFAULT_ARGS})
