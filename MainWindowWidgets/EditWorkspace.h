#ifndef EDITWORKSPACE_H
#define EDITWORKSPACE_H

#include <QWidget>
#include <QLabel>
#include <QStackedWidget>
#include <QPainter>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include "GlobalClass/AppEnumClass.h"
#include "GlobalClass/BlankPage.h"
#include "TextEditWorkspacePage.h"




/********************************************************************
 *************************   编辑工作区    ***************************
*********************************************************************/

class EditWorkspace : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor EditWorkspaceBackgroundColor READ getBackgroundColor WRITE setBackgroundColor)
public:
    explicit EditWorkspace(QWidget *parent = nullptr);

    // 设置 和 获取 背景颜色
    void setBackgroundColor(const QColor& backgroundColor);
    QColor getBackgroundColor() const;

public slots:
    // 工具栏 按钮 点击处理槽函数，处理撤销、重做、自动排版
    void do_editToolBarButton_clicked(EditToolBarButton toolButton, bool checked = false);

    // 双击打开 文件 处理槽函数
    void do_openChapter(QString chapterID, QString chapterTitle);

    // 已经打开章节 重命名 处理槽函数
    void do_chapterItem_renamed(QString newOpenChapterTitle);

    // 保存 当前打开的 章节
    void do_beingOpenedChapter_save();


    // 章节标题 显示隐藏 处理槽函数
    void do_chapterTitleVisible_changed(bool checked);

    // 字体改变 处理槽函数
    void do_font_changed(TextEditArea editArea, QFont font, QColor fontColor);

    // 标题对齐方式改变  处理槽函数
    void do_titleAlign_changed(Qt::Alignment flag);


    // 排版按钮 点击 处理槽函数
    void do_tbtnTypesetting_clicked();
    // 间距边距 改变，设置布局，处理槽函数
    void do_contentLayout_changed(int firstLineIndent, int wordSpacing,
                                  int lineSpacing, int leftRightMargin, int bottomMargin);



    // 初始化 历史版本信息
    void do_historicalVersionInfo_init(QString latestVersionID,int backupFileCount);

    // 恢复 历史版本 处理槽函数
    void do_historicalVersion_restore(QString filePath);

    // 撤销
    void do_tbtnUndo_clicked();
    // 重做
    void do_tbtnRedo_clicked();


    // 搜索查找内容 处理槽函数
    void do_searchText(QString text);
    // 搜索上一个查找内容 处理槽函数
    void do_searchText_previous();
    // 搜索下一个查找内容 处理槽函数
    void do_searchText_next();

    // 搜索查找内容 清空 处理槽函数
    void do_searchText_cleared();

    // 替换 当前 处理槽函数
    void do_replaceCurrent(QString replaceText);
    // 替换 全部 处理槽函数
    void do_replaceAll(QString replaceText);


    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *event) override;

private slots:
    // 关闭 文本编辑页面 处理槽函数
    void do_textEditPage_close();

private:
    // 初始化 成员函数
    void initMemberVariable();
    // 初始化 布局
    void initLayout();
    // 初始化 绑定事件
    void initConnect();

    // 绘制背景
    void drawBackground(QPainter *painter);


signals:
    // 文本框的标题 改变
    void chapterTitle_changed(QString chapterID,QString newChapterTitle);

    // 文本框内容 字数改变
    void chapterWordCount_changed(int wordCount);

    // 更新 历史版本页面
    void historicalVersionPage_update();

    // 初始化 布局 数值
    void layoutSliderValues_init(int firstLineIndent, int wordSpacing,
                                 int lineSpacing, int leftRightMargin, int bottomMargin);

    // 工作区 没有在编辑的文件，用于禁止一些按钮
    void editWorkspace_noFile();

private:
    // 成员控件
    QStackedWidget *m_stkContainer;
    BlankPage *m_blankPage;
    TextEditWorkspacePage *m_textPage;

    // 成员变量
    QColor m_backgroundColor;

    QString m_openChapterID;
};

#endif // EDITWORKSPACE_H
