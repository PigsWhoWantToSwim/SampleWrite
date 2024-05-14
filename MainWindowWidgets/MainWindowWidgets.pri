HEADERS += \
    $$PWD/EditCatalogBar.h \
    $$PWD/EditSideBar.h \
    $$PWD/EditStatusBar.h \
    $$PWD/EditToolBar.h \
    $$PWD/EditWorkspace.h \
    $$PWD/FindReplaceBar.h \
    $$PWD/HomeNavigationBar.h \
    $$PWD/HomeWorkspace.h \
    $$PWD/TextEditWorkspacePage.h

SOURCES += \
    $$PWD/EditCatalogBar.cpp \
    $$PWD/EditSideBar.cpp \
    $$PWD/EditStatusBar.cpp \
    $$PWD/EditToolBar.cpp \
    $$PWD/EditWorkspace.cpp \
    $$PWD/FindReplaceBar.cpp \
    $$PWD/HomeNavigationBar.cpp \
    $$PWD/HomeWorkspace.cpp \
    $$PWD/TextEditWorkspacePage.cpp

include ($$PWD/MainWindowTitleBar/MainWindowTitleBar.pri)
include ($$PWD/HomeWorkspacePages/HomeWorkspacePages.pri)
include ($$PWD/EditDirectoryWidget/EditDirectoryWidget.pri)
include ($$PWD/EditSideBarPages/EditSideBarPages.pri)
include ($$PWD/MainWindowChildWidgets/MainWindowChildWidgets.pri)
