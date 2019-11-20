QT       += core gui network networkauth concurrent qml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14 openssl openssl-linked

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
    anilist.cpp \
    anime.cpp \
    anitomy/anitomy.cpp \
    anitomy/element.cpp \
    anitomy/keyword.cpp \
    anitomy/parser.cpp \
    anitomy/parser_helper.cpp \
    anitomy/parser_number.cpp \
    anitomy/string.cpp \
    anitomy/token.cpp \
    anitomy/tokenizer.cpp \
    arquivos.cpp \
    confbase.cpp \
    confusuario.cpp \
    filedownloader.cpp \
    janeladeconfig.cpp \
    janelatorrent.cpp \
    leitorlistaanimes.cpp \
    logger.cpp \
    main.cpp \
    mainwindow.cpp \
    torrentinfo.cpp

HEADERS += \
    anilist.h \
    anime.h \
    anitomy/anitomy.h \
    anitomy/element.h \
    anitomy/keyword.h \
    anitomy/options.h \
    anitomy/parser.h \
    anitomy/string.h \
    anitomy/token.h \
    anitomy/tokenizer.h \
    arquivos.h \
    confbase.h \
    confusuario.h \
    filedownloader.h \
    janeladeconfig.h \
    janelatorrent.h \
    leitorlistaanimes.h \
    logger.h \
    mainwindow.h \
    torrentinfo.h

FORMS += \
    janeladeconfig.ui \
    janelatorrent.ui \
    mainwindow.ui \
    mainwindow_copy.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    visuais.qrc

DISTFILES +=
