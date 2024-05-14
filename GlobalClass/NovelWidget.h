#ifndef NOVELWIDGET_H
#define NOVELWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QMouseEvent>
#include <QGraphicsDropShadowEffect>
#include <QFile>
#include <QDir>
#include <QMessageBox>
#include "GlobalClass/AppEnumClass.h"
#include "GlobalClass/AppDataBase.h"
#include "MainWindowWidgets/MainWindowChildWidgets/SwitchImageWhenEnteringWidget.h"


/* 小说控件 */
class NovelWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor NovelWidgetBackgroundColor READ getBackgroundColor WRITE setBackgroundColor)
    Q_PROPERTY(QColor NovelWidgetLabelKeyWordsColor READ getLabelKeyWordsColor WRITE setLabelKeyWordsColor)
public:
    explicit NovelWidget(QWidget *parent = nullptr);

    // 设置 和 获取 背景颜色
    void setBackgroundColor(const QColor &backgroundColor);
    QColor getBackgroundColor() const;

    // 设置 和 获取 标签关键字颜色
    void setLabelKeyWordsColor(const QColor &labelKeyWordsColor);
    QColor getLabelKeyWordsColor() const;


    // 获取 小说控件信息
    void setNovelID(QString novelID);
    QString getNovelID();

    void setNovelTitle(QString novelTitle);
    QString getNovelTitle();

    void setAuthor(QString author);
    QString getAuthor();

    void setNovelCoverPath(QString novelCoverPath);
    QString getNovelCoverPath();

    void setNovelType(QString novelType);
    QString getNovelType();

    void setVolumesCount(int volumesCount);
    int getVolumesCount();

    void setChaptersCount(int chaptersCount);
    int getChaptersCount();

    void setWordsCount(int wordsCount);
    int getWordsCount();

    void setIndex(int novelIndex);
    int getIndex();


    void setNovelInfo(QString novelID, QString novelTitle, QString author,
                      QString novelCoverPath, QString novelType, int volumesCount,
                      int chaptersCount, int wordsCount, int novelIndex);

    // 导出成TXT
    bool exportNovelWidgetAsTXT(QString savedFilePath);

    // QWidget interface
protected:
    // 重写鼠标按下、鼠标进入、鼠标离开
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void enterEvent(QEnterEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;

private:
    // 初始化
    void initMemberVariable();
    void initLayout();
    void initConnect();



signals:
    // 在 小说 控件上 按下左键
    void novelWidget_mouseLeft_clicked(QString novelID, QString novelTitle);


private:
    // 成员控件
    QWidget *m_widShadowEffect;
    SwitchImageWhenEnteringWidget *m_swiNovelCover;
    QLabel *m_labNovelTitle;
    QLabel *m_labAuthor;

    QLabel *m_labNovelType;
    QLabel *m_labVolumesCount;
    QLabel *m_labChaptersCount;
    QLabel *m_labWordsCount;

    // 成员变量
    QString m_novelID;
    QString m_novelTitle;
    QString m_author;
    QString m_novelCoverPath;
    QString m_novelType;

    int m_volumesCount;
    int m_chaptersCount;
    int m_wordsCount;

    int m_novelIndex;

    int m_novelCoverRectRadius; // 封面圆角半径

    QColor m_backgroundColor;
    QColor m_labelKeyWordsColor;
    QString m_strLabelKeyWordsColor;
};

#endif // NOVELWIDGET_H
