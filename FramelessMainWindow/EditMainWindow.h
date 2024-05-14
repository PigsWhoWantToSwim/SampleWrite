#ifndef EDITMAINWINDOW_H
#define EDITMAINWINDOW_H

#include <QWidget>
#include <QSplitter>
#include "GlobalClass/AppEnumClass.h"
#include "BasedFramelessMainWindow.h"
#include "MainWindowWidgets/MainWindowTitleBar/EditTitleBar.h"
#include "MainWindowWidgets/EditToolBar.h"
#include "MainWindowWidgets/EditCatalogBar.h"
#include "MainWindowWidgets/FindReplaceBar.h"
#include "MainWindowWidgets/EditWorkspace.h"
#include "MainWindowWidgets/EditSideBar.h"
#include "MainWindowWidgets/EditStatusBar.h"

class EditMainWindow : public BasedFramelessMainWindow
{
    Q_OBJECT
public:
    // explicit EditMainWindow(QWidget *parent = nullptr);
    // explicit EditMainWindow(QString novelID, QWidget *parent = nullptr);
    explicit EditMainWindow(QString novelID, QString novelTitle, QWidget *parent = nullptr);

    // 设置 和 获取 小说ID
    void setNovelID(QString novelID);
    QString getNovelID() const;

    // 设置 和 获取 标题栏 文本本
    void setTitleText(QString titleText);
    QString getTitleText() const;

    // BasedFramelessMainWindow interface
protected:
    virtual void initMemberVariable() override;
    virtual void initLayout() override;
    virtual void initConnect() override;

private slots:
    // 窗口状态改变
    void do_windowState_change(bool isMaximized);

signals:
    // 返回 首页
    void returnHomeMainWindow();
    // 目录项 已删除
    void directioryItem_deleted();


private:
    // 成员控件
    EditTitleBar *m_editTitleBar;
    EditToolBar *m_editToolBar;

    EditCatalogBar *m_editCatalogBar; // 目录栏
    FindBar *m_findBar; // 查找栏
    ReplaceBar *m_replaceBar; // 替换栏
    EditWorkspace *m_editWorkspace; // 编辑区
    EditSideBar *m_editSideBar; // 侧边栏
    EditStatusBar *m_editStatusBar; // 状态栏

    // 成员变量
    QString m_novelID;
    QString m_novelTitle;
};

#endif // EDITMAINWINDOW_H
