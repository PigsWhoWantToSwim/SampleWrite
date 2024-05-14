#ifndef SIDEBARHISTORICALPAGE_H
#define SIDEBARHISTORICALPAGE_H

#include <QWidget>
#include <QScrollArea>
#include <QMessageBox>
#include "BasedSiderBarPage.h"
#include "MainWindowWidgets/MainWindowChildWidgets/HistoricalVersionButton.h"
#include "GlobalClass/AppDataBase.h"
#include "MainWindowDialog/RestoreHistoricalVersionDialog.h"
#include "GlobalClass/MaskFrame.h"

class SideBarHistoricalPage : public BasedSiderBarPage
{
    Q_OBJECT
public:
    explicit SideBarHistoricalPage(QWidget *parent = nullptr);
    // 析构函数
    ~SideBarHistoricalPage();

    // 更新 历史版本按钮区
    void updateButtonArea();

    // 将历史版本 内容清空
    void clearHistoricalPage();

public slots:
    // 目录节点 双击 处理槽函数 重新设置 历史按钮
    void do_openChapter(QString openChapterID);

signals:
    // 初始化 历史版本信息
    void initHistoricalVersionInfo(QString latestVersionID, int backupFileCount);

    // 恢复历史版本
    void restoreVersion(QString filePath);


    // BasedSiderBarPage interface
protected:
    virtual void initMemberVariable() override;
    virtual void initLayout() override;
    virtual void initConnect() override;

private:
    // 读取 数据库数据
    void readSQLDate();

    // 释放 旧历史版本按钮 内存，为新打开文章的历史版本做准备
    void deleteAllHistoricalVersionButton();


private slots:
    // 历史版本按钮 双击事件 处理槽函数
    void do_historicalVersionButton_DoubleClicked(QString filePath);


private:
    // 成员控件
    QScrollArea *m_scrollArea; // 滚动区
    QFrame *m_frmScrollAreaContainer; // 滚动区内用于放置控件的容器
    QVBoxLayout *m_historicalVersionBtnLayout; // 历史版本按钮 布局
    QSpacerItem *m_spacer; // 弹簧

    // 成员变量
    QString m_chapterID; // 章节ID
    QList<HistoricalVersionButton*> m_listHistoricalVersionBtn; // 历史版本按钮 列表
};

#endif // SIDEBARHISTORICALPAGE_H
