#ifndef HOMEWORKSPACETHEMEPAGE_H
#define HOMEWORKSPACETHEMEPAGE_H

#include <QWidget>
#include <QScrollArea>
#include <QGridLayout>
#include <QVBoxLayout>
#include <GlobalClass/AppEnumClass.h>
#include "MainWindowWidgets/MainWindowChildWidgets/ThemeTickButton.h"
#include "BasedHomeWorkspacePage.h"


#define THEMECONTAINERLAYOUT_MARGINS_LEFT 30
#define THEMECONTAINERLAYOUT_MARGINS_RIGHT 30
#define THEMECONTAINERLAYOUT_MARGINS_TOP 30
#define THEMECONTAINERLAYOUT_MARGINS_BUTTOM 30

#define THEMECONTAINERLAYOUT_HORIZONTIAL_SPACING 60
#define THEMECONTAINERLAYOUT_VERTICAL_SPACING 30




class HomeWorkspaceThemePage : public BasedHomeWorkspacePage
{
    Q_OBJECT
public:
    explicit HomeWorkspaceThemePage(QWidget *parent = nullptr);

    // 更新容器布局
    void updateContainerLayout();

    // QWidget interface
protected:
    // 重写 显示事件，在初始时 设置容器宽度与滚动区相等
    virtual void showEvent(QShowEvent *event) override;
    // 重写 尺寸改变事件，调整小说控件布局
    virtual void resizeEvent(QResizeEvent *event) override;

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
    // 容器布局
    QGridLayout *m_containerLayout;
    // 弹簧
    QSpacerItem *m_spacer;

    // 勾选按钮列表
    QList<ThemeTickButton*> m_listThemeTickButtons;

    int m_buttonColumeCount;

};

#endif // HOMEWORKSPACETHEMEPAGE_H
