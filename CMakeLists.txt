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
    widget/txtmessagedialog.h
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
    widget/txtmessagedialog.cpp
)

set(QRC_FILES
    tipp10.qrc
)

add_executable(tipp10 ${SRCS} ${HDRS} ${QRC_FILES} ${WIN_RC_FILE})
target_include_directories(tipp10 PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(tipp10 Qt5::Core Qt5::Multimedia Qt5::PrintSupport
                             Qt5::Sql Qt5::Widgets
)

install(TARGETS tipp10
    RUNTIME DESTINATION bin
)

install(FILES release/tipp10v2.template
    DESTINATION share/tipp10)

if(UNIX AND NOT APPLE)
    install(FILES tipp10.desktop
        DESTINATION share/applications)

    install(FILES tipp10.png
        DESTINATION share/pixmaps)
endif()