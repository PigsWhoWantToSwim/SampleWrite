#ifndef TEXTEDITWORKSPACEPAGE_H
#define TEXTEDITWORKSPACEPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QTextEdit>
#include <QDir>
#include <QFile>
#include <QPainter>
#include <QTextDocument>
#include <QTextBlock>
#include <QTextBlockFormat>
#include <QMessageBox>
#include <QRegularExpression>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QTimer>
#include <QMenu>
#include <QShortcut>
#include "GlobalClass/AppEnumClass.h"
#include "GlobalClass/AppDataBase.h"

class TextEditWorkspacePage : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor TextEditWorkspacePageBackgroundColor READ getBackgroundColor WRITE setBackgroundColor)
public:
    // 构造函数
    explicit TextEditWorkspacePage(QWidget *parent = nullptr);
    // 析构函数
    ~TextEditWorkspacePage();

    // 设置 和 获取 背景颜色
    void setBackgroundColor(const QColor& backgroundColor);
    QColor getBackgroundColor() const;


    // 打开 文件
    void openChapterFile(QString chapterID, QString chapterTitle);
    // 关闭 文件
    void closeChapterFile();

    // 设置 章节新标题
    void setLineEditNewTitle(QString newTitle);

    // 字体 相关
    // 设置标题字体（包含加粗、斜体、下划线），字体颜色
    void setTitleFont(QFont newTitleFont,QColor newTitleFontColor);
    // 设置内容字体（包含加粗、斜体、下划线），字体颜色
    void setContentFont(QFont newContentFont,QColor newContentFontColor);

    // 布局 相关
    // 设置 章节标题 显示或隐藏
    void setLineEditTitleVisible(bool checked);
    // 标题对齐 改变 处理槽函数
    void setTitleAlignment(Qt::Alignment flag);

    // 设置布局
    void setContentLayout(int firstLineIndent, int wordSpacing,
                          int lineSpacing, int leftRightMargin, int bottomMargin);


    // 设置 初始历史版本信息
    void setHistoricalVersionInfo(QString latestVersionID,int backupFileCount);

    // 保存 主文件
    void saveAsMainChapterFile();

    // 撤销
    void undo();
    // 重做
    void redo();

    // 自动排版
    void autoTypeSetting();

public:
    // 搜索查找内容 处理槽函数
    void searchText(QString text);

    // 搜索上一个查找内容 处理槽函数
    void searchPreviousText();
    // 搜索下一个查找内容 处理槽函数
    void SearchNextText();

    // 搜索查找内容 清空 处理槽函数
    void clearSearchText();

    // 替换 当前 处理槽函数
    void replaceCurrent(QString replaceText);
    // 替换 全部 处理槽函数
    void replaceAll(QString replaceText);

    // 恢复 历史版本 处理槽函数
    void restoreHistoricalVersion(QString filePath);

    // QWidget interface
protected:
    // 绘画事件
    virtual void paintEvent(QPaintEvent *event) override;

private:
    // 初始化 成员函数
    void initMemberVariable();
    // 初始化 布局
    void initLayout();
    // 初始化 绑定事件
    void initConnect();

    // 设置 首行缩进
    void setFirstLineIndent(int value);
    // 设置 字间距
    void setWordSpacing(int value);
    // 设置 行间距
    void setLineSpacing(int value);
    // 设置 标题和内容 左右边距
    void setLeftRightMargin(int value);
    // 设置 内容底部边距
    void setContentBottomMargin(int value);

    // 获取 文本框内容 字数
    int getTextEditWordCount();

    // 保存 备份文件
    void saveAsBackupChapterFile();

    // 绘制背景
    void drawBackground(QPainter *painter);

private slots:
    // 正文内容文本框 右键 弹出自定义菜单 处理槽函数
    void do_tedContent_customContextMenuRequested(const QPoint &pos);

signals:
    // 章节标题 改变 信号
    void chapterTitle_changed(QString chapterID,QString newChapterTitle);

    // 当前搜索游标 改变
    // void currentCursor_changed(QTextCursor currentCursor);

    // 正文字数改变
    void chapterWordCount_changed(int wordCount);

    // 更新 历史版本页面
    void updateHistoricalVersionPage();

    // 关闭 文本编辑页面
    void textEditPage_close();

    // 初始化 布局 数值
    void contentLayoutValues_init(int firstLineIndent, int wordSpacing,
                                  int lineSpacing, int leftRightMargin, int bottomMargin);

private:
    // 成员控件
    QLineEdit *m_ledTitle; // 标题 输入框
    QTextEdit *m_tedContent; // 内容文本编辑器   
    QTimer *m_timer; // 定时器，用于定时 保存主文件和备份文件

    // 成员变量
    int m_firstLineIndent;
    int m_wordSpacing;
    int m_lineSpacing;
    int m_leftRightMargin;
    int m_bottomMargin;

    // 默认字体
    QFont m_fontTitle;
    QFont m_fontContent;

    // 打开的章节ID
    QString m_openChapterID;
    // 最新备份文件版本
    QString m_latestVersionID;
    // 备份文件数量
    int m_backupFileCount;

    // 要搜索的内容，用于取消搜索后的高亮
    QString m_searchText;
    // 游标列表 记录 与要搜索内容匹配的内容 所在的 游标
    QList<QTextCursor> m_matchedCursorList;
    // 当前游标索引，用于记录当前 游标位置
    int m_currentCursorIndex;

    QColor m_backgroundColor;
};

#endif // TEXTEDITWORKSPACEPAGE_H
