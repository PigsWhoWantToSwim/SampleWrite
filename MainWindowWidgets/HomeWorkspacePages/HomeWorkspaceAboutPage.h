#ifndef HOMEWORKSPACEABOUTPAGE_H
#define HOMEWORKSPACEABOUTPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QDesktopServices>
#include "BasedHomeWorkspacePage.h"

#define HOMEABOUTPAGE_MAINLAYOUT_MARGIN_LEFT 0
#define HOMEABOUTPAGE_MAINLAYOUT_MARGIN_RIGHT 0
#define HOMEABOUTPAGE_MAINLAYOUT_MARGIN_TOP 0
#define HOMEABOUTPAGE_MAINLAYOUT_MARGIN_BOTTOM 0

class HomeWorkspaceAboutPage : public BasedHomeWorkspacePage
{
    Q_OBJECT
public:
    explicit HomeWorkspaceAboutPage(QWidget *parent = nullptr);

    // 设置 和 获取 软件版本
    void setAPPVersion(QString APPVersion);
    QString getAPPVersion();
    // 设置 和 获取 软件版本
    void setDeveloperInfo(QString developerInfo);
    QString getDeveloperInfo();
    // 设置 和 获取 软件项目地址
    void setAPPProjectURL(QString URL);
    QString getAPPProjectURL();

    // BasedHomeWorkspacePage interface
protected:
    // 初始化
    virtual void initMemberVariable() override;
    virtual void initLayout() override;
    virtual void initConnect() override;

private slots:
    void do_btnAPPUpdate_clicked();

signals:

private:
    // 成员控件
    QPushButton *m_btnAPPUpdate;

    // 成员变量
    QString m_APPVersion;
    QString m_developerInfo;
    QString m_APPProjectURL;
};

#endif // HOMEWORKSPACEABOUTPAGE_H
