#ifndef SIDEBARSETTINGPAGE_H
#define SIDEBARSETTINGPAGE_H

#include <QWidget>
#include "BasedSiderBarPage.h"
#include "GlobalClass/SwitchButton.h"

class SideBarSettingPage : public BasedSiderBarPage
{
    Q_OBJECT
public:
    explicit SideBarSettingPage(QWidget *parent = nullptr);

    // BasedSiderBarPage interface
protected:
    virtual void initMemberVariable() override;
    virtual void initLayout() override;
    virtual void initConnect() override;

signals:
    // 章节标题可见 信号
    void chapterTitleVisible_changed(bool checked);



private:
    // 成员控件
    SwitchButton *m_swbtnChapterTitleVisible;

    // 成员变量
};

#endif // SIDEBARSETTINGPAGE_H
