set(SOURCE
    main.cpp
    )

qt5_add_resources(RESOURCES resources.qrc)
add_executable(qweather ${SOURCE} ${RESOURCES})
target_link_libraries(qweather Qt5::Core  Qt5::Qml Qt5::Quick Qt5::Svg)
install(TARGETS qweather ${KF5_INSTALL_TARGETS_DEFAULT_ARGS})
