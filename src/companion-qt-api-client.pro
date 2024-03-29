QT       += core gui network multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    client_exceptions.cpp \
    data_types.cpp \
    main.cpp \
    main_window.cpp \
    model/abstract_api_table_model.cpp \
    model/sound_table_model.cpp \
    repository/abstract_repository.cpp \
    repository/sound_repository.cpp

HEADERS += \
    client_exceptions.h \
    data_types.h \
    main_window.h \
    model/abstract_api_table_model.h \
    model/sound_table_model.h \
    repository/abstract_repository.h \
    repository/sound_repository.h
