#ifndef SIDEBARTHEMEPAGE_H
#define SIDEBARTHEMEPAGE_H

#include <QWidget>
#include <QGridLayout>
#include "BasedSiderBarPage.h"
#include "GlobalClass/AppEnumClass.h"
#include "GlobalClass/FlowLayout.h"
#include "MainWindowWidgets/MainWindowChildWidgets/ThemeTickButton.h"

class SideBarThemePage : public BasedSiderBarPage
{
    Q_OBJECT
public:
    explicit SideBarThemePage(QWidget *parent = nullptr);

    // 更新容器布局
    // void updateThemeContainerLayout();

    // QWidget interface
protected:
    // 重写 显示事件，在初始时 设置容器宽度与滚动区相等
    // virtual void showEvent(QShowEvent *event) override;
    // 重写 尺寸改变事件，调整小说控件布局
    // virtual void resizeEvent(QResizeEvent *event) override;

    // BasedSiderBarPage interface
protected:
    virtual void initMemberVariable() override;
    virtual void initLayout() override;
    virtual void initConnect() override;

private slots:
    // 主题按钮点击 处理槽函数，实现互斥功能
    void do_themeTickButton_clicked();

signals:

private:
    QFrame *m_frmThemeButtonContainer;
    // 主题页面 网格布局
    FlowLayout *m_themeBtnAreaLayout;
    // 横向弹簧
    // QSpacerItem *m_horizontalSpacer;

    // 勾选按钮列表
    QList<ThemeTickButton*> m_listThemeTickButtons;

    // 主题按钮区 列数
    // int m_themeBtnAreaColumeCount;

};

#endif // SIDEBARTHEMEPAGE_H
