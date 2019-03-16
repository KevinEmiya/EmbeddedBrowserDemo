INCLUDEPATH += $$PWD

#CEF Settings
linux-g++ {
    CONFIG(debug, debug|release){
        LIBS += $$PWD/libs/linux-x64/debug/libcef.so \
            $$PWD/libs/linux-x64/debug/libcef_dll_wrapper.a \
    }else{
        LIBS += $$PWD/libs/linux-x64/release/libcef.so \
            $$PWD/libs/linux-x64/release/libcef_dll_wrapper.a
    }
}
win32 {
    CONFIG(debug, debug|release){
        LIBS += libs/win32/debug/libcef.lib \
            libs/win32/debug/libcef_dll_wrapper.lib
    }else{
        LIBS += libs/win32/release/libcef.lib \
            libs/win32/release/libcef_dll_wrapper.lib
    }
}

QT += core \
    gui \
    network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

HEADERS += $$PWD/src/cef/QCefDefines.h \
    $$PWD/src/cef/QCefJSDialogHandler.h \
    $$PWD/src/cef/QCefContext.h \
    $$PWD/src/cef/QCefApp.h \
    $$PWD/src/cef/QCefRenderHandler.h \
    $$PWD/src/cef/QCefV8Handler.h \
    $$PWD/src/cef/QCefView.h \
    $$PWD/src/cef/QCefClient.h \
    $$PWD/src/cef/QCefInspector.h \
    $$PWD/src/cef/QCefSslContext.h

SOURCES += $$PWD/src/cef/QCefJSDialogHandler.cpp \
    $$PWD/src/cef/QCefContext.cpp \
    $$PWD/src/cef/QCefApp.cpp \
    $$PWD/src/cef/QCefRenderHandler.cpp \
    $$PWD/src/cef/QCefV8Handler.cpp \
    $$PWD/src/cef/QCefView.cpp \
    $$PWD/src/cef/QCefClient.cpp \
    $$PWD/src/cef/QCefInspector.cpp \
    $$PWD/src/cef/QCefSslContext.cpp
