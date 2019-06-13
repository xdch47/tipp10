cmake_minimum_required(VERSION 3.10)

project(tipp10)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

if(WIN32)
    # add application icon
    set(WIN_RC_FILE tipp10.rc)
endif()

find_package(Qt5 COMPONENTS Core Multimedia Network PrintSupport Sql Widgets REQUIRED)

set(HDRS
    def/defines.h
    def/errordefines.h
    games/abcrainwidget.h
    games/charball.h
    sql/chartablesql.h
    sql/connection.h
    sql/keyboardsql.h
    sql/lessontablesql.h
    sql/startsql.h
    sql/trainingsql.h
    widget/companylogo.h
    widget/errormessage.h
    widget/evaluationwidget.h
    widget/fingerwidget.h
    widget/helpbrowser.h
    widget/illustrationdialog.h
    widget/illustrationimage.h
    widget/keyboard.h
    widget/lessondialog.h
    widget/lessonprintdialog.h
    widget/lessonresult.h
    widget/mainwindow.h
    widget/numpad.h
    widget/progressionwidget.h
    widget/regexpdialog.h
    widget/settingsdialog.h
    widget/settingspages.h
    widget/startwidget.h
    widget/statusbar.h
    widget/tickerboard.h
    widget/trainingwidget.h
)

set(SRCS
    main.cpp
    games/abcrainwidget.cpp
    games/charball.cpp
    sql/chartablesql.cpp
    sql/keyboardsql.cpp
    sql/lessontablesql.cpp
    sql/startsql.cpp
    sql/trainingsql.cpp
    widget/companylogo.cpp
    widget/errormessage.cpp
    widget/evaluationwidget.cpp
    widget/fingerwidget.cpp
    widget/helpbrowser.cpp
    widget/illustrationdialog.cpp
    widget/illustrationimage.cpp
    widget/keyboard.cpp
    widget/lessondialog.cpp
    widget/lessonprintdialog.cpp
    widget/lessonresult.cpp
    widget/mainwindow.cpp
    widget/numpad.cpp
    widget/progressionwidget.cpp
    widget/regexpdialog.cpp
    widget/settingsdialog.cpp
    widget/settingspages.cpp
    widget/startwidget.cpp
    widget/statusbar.cpp
    widget/tickerboard.cpp
    widget/trainingwidget.cpp
)

set(QRC_FILES
    tipp10.qrc
)

set(TS_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/translations/tipp10_de.ts
    ${CMAKE_CURRENT_SOURCE_DIR}/translations/tipp10_en.ts
)

find_package(Qt5LinguistTools)
qt5_add_translation(QM_FILES ${TS_FILES})

add_executable(tipp10 ${SRCS} ${HDRS} ${QRC_FILES} ${QM_FILES} ${WIN_RC_FILE})
target_include_directories(tipp10 PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(tipp10 Qt5::Core Qt5::Multimedia Qt5::PrintSupport
                             Qt5::Sql Qt5::Widgets
)
target_compile_definitions(tipp10 PRIVATE INSTALLPREFIX="${CMAKE_INSTALL_PREFIX}")

if(WIN32)
    target_compile_definitions(tipp10 PRIVATE APP_WIN)
elseif(UNIX)
    if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
        target_compile_definitions(tipp10 PRIVATE APP_MAC)
    else()
        target_compile_definitions(tipp10 PRIVATE APP_X11)
    endif()
endif()

install(TARGETS tipp10
    RUNTIME DESTINATION bin
)

install(FILES release/tipp10v2.template
    DESTINATION share/tipp10)

install(FILES ${QM_FILES}
    DESTINATION share/tipp10/translations)

if(UNIX AND NOT APPLE)
    install(FILES tipp10.desktop
        DESTINATION share/applications)

    install(FILES tipp10.png
        DESTINATION share/pixmaps)
endif()
