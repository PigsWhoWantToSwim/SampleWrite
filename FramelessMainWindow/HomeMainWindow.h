#ifndef HOMEMAINWINDOW_H
#define HOMEMAINWINDOW_H

#include <QWidget>
#include <QDialog>
#include <QGridLayout>
#include "BasedFramelessMainWindow.h"
#include "MainWindowWidgets/MainWindowTitleBar/HomeTitleBar.h"
#include "MainWindowWidgets/HomeNavigationBar.h"
#include "MainWindowWidgets/HomeWorkspace.h"
#include "GlobalClass/MaskFrame.h"
#include "MainWindowDialog/ModifyNovelDialog.h"
#include "EditMainWindow.h"



class HomeMainWindow : public BasedFramelessMainWindow
{
    Q_OBJECT
public:
    explicit HomeMainWindow(QWidget *parent = nullptr);

    // QWidget interface
protected:

    // BasedFramelessMainWindow interface
protected:
    // 重写 初始化 虚函数
    virtual void initMemberVariable() override;
    virtual void initLayout() override;
    virtual void initConnect() override;

    // BasedFramelessMainWindow interface
private:

private slots:
    // 窗口状态改变
    void do_windowState_change(bool isMaximized);

    // 新建小说 处理槽函数
    void do_newNovel();
    // 编辑小说 处理槽函数
    void do_editNovel(QString novelID, QString novelTitle, QString author,
                   QString novelType, QString novelCoverPath);

    // 小说控件 左键 跳转 处理槽函数
    void do_novelWidget_clicked(QString novelID, QString novelTitle);

    // 编辑页面 返回 处理  槽函数
    void do_returnHomeMainWindow();

private:
    // 标题栏
    HomeTitleBar *m_homeTitleBar;
    // 导航栏
    HomeNavigationBar *m_homeNavigtionBar;
    // 工作区
    HomeWorkspace *m_homeWorkspace;

    // 编辑 页面
    EditMainWindow *m_editMainWindow;
};

#endif // HOMEMAINWINDOW_H
