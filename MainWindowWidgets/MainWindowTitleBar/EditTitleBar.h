#ifndef EDITTITLEBAR_H
#define EDITTITLEBAR_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include "GlobalClass/AppEnumClass.h"
#include "BasedMainWindowTitleBar.h"



class EditTitleBar : public BasedMainWindowTitleBar
{
    Q_OBJECT
public:
    explicit EditTitleBar(QWidget *parent = nullptr);

    // 获取 和 设置 标题栏文本
    void setTitleText(QString titleText);
    QString getTitleText() const;

    // BasedMainWindowTitleBar interface
public:
    virtual void toggleMaxRestoreButtonVisible(bool isWindowMaximized) override;

    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;

    // BasedMainWindowTitleBar interface
protected:
    virtual void initMemberVariable() override;
    virtual void initLayout() override;
    virtual void initConnect() override;


signals:



private:
    // 成员控件
    QLabel *m_labTitle;

    // 成员变量
    QString m_titleText;



};

#endif // EDITTITLEBAR_H
