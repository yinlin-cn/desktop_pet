QT       += core gui
QT += multimedia
QT += core
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
LIBS += -lwinmm
CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    form.cpp \
    main.cpp \
    musicgui.cpp \
    taici.cpp \
    widget.cpp \
    zidingyi.cpp

HEADERS += \
    form.h \
    musicgui.h \
    taici.h \
    widget.h \
    zidingyi.h

FORMS += \
    form.ui \
    musicgui.ui \
    taici.ui \
    widget.ui \
    zidingyi.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc \

# Windows 平台设置 EXE 图标（路径为图标文件的相对路径）
win32: RC_ICONS = biao.ico

# 设置目标文件名（不含 .exe 后缀，Qt 会自动补全）
TARGET = ELYSIA  # 自定义名称，如“爱莉宠物”或“aili_pet”

win32-msvc* {
    # Release 模式：多线程静态库 (/MT)
    Release: QMAKE_CXXFLAGS += /MT
    # Debug 模式：多线程调试静态库 (/MTd)
    Debug: QMAKE_CXXFLAGS += /MTd
    # 移除动态库相关配置
    QMAKE_CXXFLAGS -= /MD /MDd
}
