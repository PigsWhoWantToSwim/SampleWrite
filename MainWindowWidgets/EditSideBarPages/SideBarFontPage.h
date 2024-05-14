#ifndef SIDEBARFONTPAGE_H
#define SIDEBARFONTPAGE_H

#include <QWidget>
#include <QComboBox>
#include <QFontComboBox>
#include <QColorDialog>
#include "BasedSiderBarPage.h"

class SideBarFontPage : public BasedSiderBarPage
{
    Q_OBJECT
public:
    explicit SideBarFontPage(QWidget *parent = nullptr);

signals:
    // 字体改变，发送字体
    void font_changed(TextEditArea editArea, QFont font, QColor fontColor);

    // BasedSiderBarPage interface
protected:
    virtual void initMemberVariable() override;
    virtual void initLayout() override;
    virtual void initConnect() override;

private slots:
    // 接收 字体，并发送
    void acceptFont(TextEditArea editArea);

private:
    // 章节标题区
    QFontComboBox *m_fcmbTitleFontFamily;
    QComboBox *m_cmbTitleFontSize;
    QPushButton *m_btnTitleFontColor;

    QPushButton *m_btnTitleBlod;
    QPushButton *m_btnTitleUnderline;
    QPushButton *m_btnTitleItalics;

    // 章节内容区
    QFontComboBox *m_fcmbContentFontFamily;
    QComboBox *m_cmbContentFontSize;
    QPushButton *m_btnContentFontColor;

    QPushButton *m_btnContentBlod;
    QPushButton *m_btnContentUnderline;
    QPushButton *m_btnContentItalics;
};

#endif // SIDEBARFONTPAGE_H
