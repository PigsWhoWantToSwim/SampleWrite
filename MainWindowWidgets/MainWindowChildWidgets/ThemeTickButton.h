#ifndef THEMETICKBUTTON_H
#define THEMETICKBUTTON_H

#include <QWidget>
#include "GlobalClass/TickButton.h"
#include "GlobalClass/AppEnumClass.h"

class ThemeTickButton : public TickButton
{
    Q_OBJECT
    Q_PROPERTY(QColor ThemeButtonTextColor READ getTextColor WRITE setTextColor)
public:
    explicit ThemeTickButton(QWidget *parent = nullptr);

    // 设置 获取 按钮文本
    void setText(QString text, Qt::AlignmentFlag flag = Qt::AlignCenter);
    QString getText() const;

    // 设置 获取 按钮文本颜色
    void setTextColor(const QColor &textColor);
    QColor getTextColor() const;

    // 设置 和 获取 qss主题文件路径
    void setQSSFilePath(const QString qssFilePath);
    QString getQSSFilePath() const;

    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *event) override;


private:
    void drawText(QPainter *painter); // 绘制 文字

    // 加载 主题样式表 文件
    void loadThemeStyleFile();

signals:
private:
    // 成员函数
    QString m_text; // 按钮上的文本
    Qt::AlignmentFlag m_textAlignmentFlag;
    QColor m_textColor; // 文本颜色

    QString m_qssFilePath; // qss 主题文件路径

    // QWidget interface
protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
};

#endif // THEMETICKBUTTON_H
