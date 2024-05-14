#include "ThemeTickButton.h"

ThemeTickButton::ThemeTickButton(QWidget *parent)
    : TickButton{parent}
{
    // 设置 控件属性
    this->setCursor(QCursor(Qt::PointingHandCursor)); // 设置 光标形状

    m_text = "";
    m_textAlignmentFlag = Qt::AlignLeft;
    m_textColor = QColor(255, 255, 255);
    m_qssFilePath = ":/theme/default.qss";
}

void ThemeTickButton::setText(QString text, Qt::AlignmentFlag flag)
{
    m_text = text;
    m_textAlignmentFlag = flag;
    update();
}

QString ThemeTickButton::getText() const
{
    return m_text;
}

void ThemeTickButton::setTextColor(const QColor &textColor)
{
    m_textColor = textColor;
    update();
}

QColor ThemeTickButton::getTextColor() const
{
    return m_textColor;
}

void ThemeTickButton::setQSSFilePath(const QString qssFilePath)
{
    m_qssFilePath = qssFilePath;
}

QString ThemeTickButton::getQSSFilePath() const
{
    return m_qssFilePath;
}

void ThemeTickButton::drawText(QPainter *painter)
{
    // 保存 画家类对象 当前坐标状态
    painter->save();

    // 设置 画笔颜色
    painter->setPen(m_textColor);

    // 文本矩形 粗略高度
    int textRectHeight = this->height()/6;

    // 中文字号VS英文字号(磅)VS像素值的对应关系：
    // 八号＝5磅(5pt) ==(5/72)*96=6.67 =6px
    // 七号＝5.5磅 ==(5.5/72)*96=7.3 =7px

    int fontSize = (textRectHeight * 1.0 * 72) / 96; // 通过高度像素值计算字号（粗略）
    setFont(QFont("Microsoft Yahei", fontSize)); // 设置 字体

    // 居中绘制文本
    QFontMetrics metrics = painter->fontMetrics(); // 获取字体信息
    int stringHeight = metrics.ascent() + metrics.descent(); // 真正的字符串高度（不算 line gap）

    int textMargin, x, y;

    textMargin = (this->height()/2 - stringHeight)/2; // 计算 边距
    if(m_buttonStyle == TickButtonStyle::rectButton || m_buttonStyle == TickButtonStyle::rectImageButton) // 矩形按钮
        x = this->rect().left() + textMargin; // 计算 文本矩形 左上角 x坐标
    else if(m_buttonStyle == TickButtonStyle::circleButton || m_buttonStyle == TickButtonStyle::circleImageButton)
    {
        // 获取 较小的边
        int minEdge = this->width() > this->height() ? this->height() : this->width();
        int offset_X = (this->width() - minEdge)/2; // 偏移

        x = this->rect().left() + textMargin + offset_X; // 计算 文本矩形 左上角 x坐标
    }

    y = this->rect().bottom() - textMargin - stringHeight; // 计算 文本矩形 左上角 y坐标  // + metrics.ascent();


    // 文本矩形的几何尺寸
    QRect textRect(x, y, this->width()-textMargin*2, stringHeight);

    // 绘制 文字，左对齐
    painter->drawText(textRect, m_textAlignmentFlag, m_text);

    // 恢复 画家类对象 上一次保存的坐标状态
    painter->restore();
}

// 加载 主题样式表 文件
void ThemeTickButton::loadThemeStyleFile()
{
    // 加载样式表
    QFile QSSFile(m_qssFilePath);

    if(QSSFile.open(QFile::ReadOnly))
    {
        qApp->setStyleSheet(QSSFile.readAll());
    }
    QSSFile.close();

    saveCurrentTheme(m_qssFilePath);
}

void ThemeTickButton::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event); // 不使用 事件
    if(!m_checked) // 当前未被勾选
    {
        m_checked = !m_checked;
        emit themeButton_clicked(); // 发送 按钮状态 改变信号
        emit themeButton_clicked(m_checked);

        loadThemeStyleFile();
    }

    update();

    QWidget::mousePressEvent(event);
}

void ThemeTickButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event); // 不使用 事件
    //绘制准备工作
    QPainter painter(this); // 画家类 对象
    m_pixelRatio = painter.device()->devicePixelRatioF(); // 获取不同显示器的分辨率比例，防止不同分辨率下图片失真
    painter.setRenderHint(QPainter::Antialiasing); // 设置 抗锯齿

    // 判断 按钮 风格
    if(m_buttonStyle == TickButtonStyle::rectImageButton || m_buttonStyle == TickButtonStyle::circleImageButton) // 图片按钮
        drawImage(&painter); //绘制图片
    else
        drawBackground(&painter); // 绘制背景

    //绘制文字
    drawText(&painter);

    if(m_checked) // 被 勾选
    {
        drawBorder(&painter); // 绘制 边框
        drawTick(&painter); // 绘制 勾号
    }
}
