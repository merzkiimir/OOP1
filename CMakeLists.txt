cmake_minimum_required(VERSION 3.19)
project(kyrs LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(kyrs
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui

    src/contact.cpp
    src/number.cpp
    src/vectnmb.cpp
    src/vctcontact.cpp
    src/smt.cpp
    src/contactdialog.h src/contactdialog.cpp
    src/searchdialog.h src/searchdialog.cpp






)

target_include_directories(kyrs PRIVATE src)

target_link_libraries(kyrs
    PRIVATE
        Qt::Core
        Qt::Widgets
)

include(GNUInstallDirs)

install(TARGETS kyrs
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET kyrs
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
