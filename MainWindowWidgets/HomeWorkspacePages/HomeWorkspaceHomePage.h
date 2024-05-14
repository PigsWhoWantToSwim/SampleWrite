#ifndef HOMEWORKSPACEHOMEPAGE_H
#define HOMEWORKSPACEHOMEPAGE_H

#include <QWidget>
#include <QScrollArea>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QEvent>
#include <QThread>
#include <QMessageBox>
#include <QMenu>
#include <QCoreApplication>
#include <QFileDialog>
#include "BasedHomeWorkspacePage.h"
#include "GlobalClass/AppEnumClass.h"
#include "GlobalClass/FlowLayout.h"
#include "GlobalClass/NovelWidget.h"
#include "GlobalClass/AppDataBase.h"




class HomeWorkspaceHomePage : public BasedHomeWorkspacePage
{
    Q_OBJECT
public:
    explicit HomeWorkspaceHomePage(QWidget *parent = nullptr);



    // 更新 页面
    // void updatePage();

public:
    // 添加小说
    void do_novelAdded(QString novelID);
    // 编辑小说
    void do_novelEdited(QString novelID);
    // 小说已恢复
    void do_updateHomePage();

    // QWidget interface
protected:


    // BasedHomeWorkspacePage interface
protected:
    // 初始化
    virtual void initMemberVariable() override;
    virtual void initLayout() override;
    virtual void initConnect() override;

private slots:
    // 在 空白处（容器）上 右键 弹出 自定义菜单，处理槽函数
    void do_blank_customContextMenuRequested(const QPoint &pos);
    // 在 小说控件 上 右键 弹出 自定义菜单，处理槽函数
    void do_novelWidget_customContextMenuRequested(const QPoint &pos);

private:
    // 读取 数据库
    void readDataBase();
    // 更新 数据库
    void updateDataBase();
    // 更新索引
    void updateIndex();


    // 删除一本小说
    void deleteNovelWidget(NovelWidget* novelWidget);

    // 释放 旧小说 内存
    // void removeAllNovelWidgetsFromLayout();


signals:
    // 新建 小说 信号
    void newNovel();
    // 编辑 小说 信号
    void editNovel(QString novelID, QString novelTitle, QString author,
                   QString novelType, QString novelCoverPath);

    // 删除小说信号
    void deleteNovel(QString novelID);

    // 小说 控件 被点击
    void novelWidget_clicked(QString novelID ,QString novelTitle);

private:
    // 控件
    // 容器布局
    FlowLayout *m_containerLayout;

    // 小说控件 列表
    QList<NovelWidget*> m_listNovelWidgets;

    // 成员变量


};

#endif // HOMEWORKSPACEHOMEPAGE_H
