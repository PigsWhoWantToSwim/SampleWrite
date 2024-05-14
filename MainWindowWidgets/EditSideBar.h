#ifndef EDITSIDEBAR_H
#define EDITSIDEBAR_H

#include <QWidget>
#include <QStackedWidget>
#include <QVBoxLayout>
#include "GlobalClass/AppEnumClass.h"
#include "EditSideBarPages/SideBarThemePage.h"
#include "EditSideBarPages/SideBarFontPage.h"
#include "EditSideBarPages/SideBarLayoutPage.h"
#include "EditSideBarPages/SideBarHistoricalPage.h"
#include "EditSideBarPages/SideBarSettingPage.h"


class EditSideBar : public QWidget
{
    Q_OBJECT
public:
    explicit EditSideBar(QWidget *parent = nullptr);

public slots:
    // 工具按钮 点击处理 槽函数，实现页面切换
    void do_editToolBarButton_clicked(EditToolBarButton toolButton, bool checked = false);

    // 双击 打开文章 处理槽函数
    void do_openChapter(QString openChapterID, QString openChapterTitle);

    // 更新 历史版本页面 处理槽函数
    void do_historicalVersionPage_update();

    // 初始化 布局 数值
    void do_layoutSliderValues_init(int firstLineIndent, int wordSpacing,
                                    int lineSpacing, int leftRightMargin, int bottomMargin);

    // 关闭文本编辑编辑页
    void do_editWorkspace_noFile();

private:
    // 初始化 成员函数
    void initMemberVariable();
    // 初始化 布局
    void initLayout();
    // 初始化 绑定事件
    void initConnect();


signals:
    // 关闭 侧边栏 页面、
    void editSideBarPages_close(EditToolBarButton toolButton);

    // 发送 字体页面 字体变化
    void fontPage_font_changed(TextEditArea editArea, QFont font, QColor fontColor);

    // 标题显示隐藏 状态变化
    void layout_titleVisible_changed(bool checked);
    // 发送 标题对齐方式 变化
    void layoutPage_titleAlign_changed(Qt::Alignment flag);
    // 发送 间距边距数值 改变
    void contentLayout_changed(int firstLineIndent, int wordSpacing,
                               int lineSpacing, int leftRightMargin, int bottomMargin);

    // 初始化 历史版本信息
    void historicalVersionInfo_init(QString latestVersionID,int backupFileCount);

    // 恢复历史版本 信号
    void historicalVersion_restore(QString filePath);

    // 章节标题可见 信号
    void chapterTitleVisible_changed(bool checked);

private:
    // 成员控件
    QStackedWidget *m_stkContainer; // 栈容器
    SideBarThemePage *m_themePage;
    SideBarFontPage *m_fontPage;
    SideBarLayoutPage *m_layoutPage;
    SideBarHistoricalPage *m_historicalPage;
    SideBarSettingPage *m_settingPage;
};

#endif // EDITSIDEBAR_H
