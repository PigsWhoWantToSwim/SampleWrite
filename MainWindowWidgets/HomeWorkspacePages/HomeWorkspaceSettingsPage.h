#ifndef HOMEWORKSPACESETTINGSPAGE_H
#define HOMEWORKSPACESETTINGSPAGE_H

#include <QWidget>
#include <QGraphicsDropShadowEffect>
#include <QLabel>
#include "BasedHomeWorkspacePage.h"
#include "GlobalClass/AppEnumClass.h"
#include "GlobalClass/FlowLayout.h"
#include "MainWindowWidgets/MainWindowChildWidgets/ThemeTickButton.h"

class HomeWorkspaceSettingsPage : public BasedHomeWorkspacePage
{
    Q_OBJECT
public:
    explicit HomeWorkspaceSettingsPage(QWidget *parent = nullptr);

    // 更新容器布局
    // void updateThemeContainerLayout();

    // QWidget interface
protected:
    // 重写 显示事件，初始设置 滚动区、滚动区容器、主题页面容器宽度
    // virtual void showEvent(QShowEvent *event) override;
    // virtual void resizeEvent(QResizeEvent *event) override;


    // BasedHomeWorkspacePage interface
protected:
    // 初始化
    virtual void initMemberVariable() override;
    virtual void initLayout() override;
    virtual void initConnect() override;

private slots:
    // 主题按钮点击 处理槽函数，实现互斥功能
    void do_themeTickButton_clicked();

signals:

private:
    // 控件
    // 主题页面
    QFrame *m_frmThemeContainer;
    // 主题页面 网格布局
    FlowLayout *m_themeBtnAreaLayout;

    // 横向弹簧
    // QSpacerItem *m_horizontalSpacer;

    // 成员变量
    // 主题按钮列表
    QList<ThemeTickButton*> m_listThemeTickButtons;
    // 主题按钮区 列数
    //int m_themeBtnAreaColumeCount;




    // QWidget interface
protected:

};

#endif // HOMEWORKSPACESETTINGSPAGE_H
