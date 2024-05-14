#ifndef HOMEWORKSPACE_H
#define HOMEWORKSPACE_H

#include <QWidget>
#include <QStackedWidget>
#include <QVBoxLayout>

#include "Globalclass/AppEnumClass.h"
#include "GlobalClass/BlankPage.h"
#include "HomeWorkspacePages/HomeWorkspaceHomePage.h"
#include "HomeWorkspacePages/HomeWorkspaceRecyclePage.h"
#include "HomeWorkspacePages/HomeWorkspaceSettingsPage.h"
#include "HomeWorkspacePages/HomeWorkspaceAboutPage.h"

class HomeWorkspace : public QWidget
{
    Q_OBJECT
public:
    explicit HomeWorkspace(QWidget *parent = nullptr);

public slots:
    // 标题栏 设置关于按钮点击 处理槽函数
    void do_homeTitleBar_clicked(TitleBarClickedButton clickedButton);
    // 页面改变 处理槽函数
    void do_HomeWorkspacePage_changed(HomeMainWindowPage page);

    // 添加小说
    void do_novelAdded(QString novelID);
    // 编辑小说
    void do_novelEdited(QString novelID);

    // 更新 首页
    void do_homePage_update();

    // 目录栏 删除项 处理槽函数
    void do_directioryItem_deleted();

    // QWidget interface
protected:
    // virtual void resizeEvent(QResizeEvent *event) override;

private:
    // 初始化 成员函数
    void initMemberVariable();
    // 初始化 布局
    void initLayout();
    // 初始化 绑定事件
    void initConnect();

signals:
    // 新建 小说 信号
    void newNovel();
    // 修改 小说 信号
    void editNovel(QString novelID, QString novelTitle, QString author,
                   QString novelType, QString novelCoverPath);
    // 删除小说信号
    void deleteNovel(QString novelID);

    void novelWidget_clicked(QString novelID, QString novelTitle);



private:
    // 成员控件
    QStackedWidget *m_stkContainer; // 栈容器

    HomeWorkspaceHomePage *m_homePage;
    BlankPage *m_findPage;
    HomeWorkspaceRecyclePage *m_recyclePage;
    HomeWorkspaceSettingsPage *m_settingsPage;
    // QWidget *m_themePage;
    HomeWorkspaceAboutPage *m_aboutPage;

    // 成员变量


};

#endif // HOMEWORKSPACE_H
