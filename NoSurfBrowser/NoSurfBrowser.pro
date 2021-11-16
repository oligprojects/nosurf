TEMPLATE = app
TARGET = NoSurfBrowser

QT += qml quick webview
QT += widgets

SOURCES += main.cpp

RESOURCES += qml.qrc

EXAMPLE_FILES += doc

macos:QMAKE_INFO_PLIST = macos/Info.plist
ios:QMAKE_INFO_PLIST = ios/Info.plist

#target.path = $$[QT_INSTALL_EXAMPLES]/webview/minibrowser
target.path = target
INSTALLS += target
