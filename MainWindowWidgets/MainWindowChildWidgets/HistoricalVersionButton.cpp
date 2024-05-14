#include "HistoricalVersionButton.h"

// 构造函数
HistoricalVersionButton::HistoricalVersionButton(QWidget *parent)
    : QWidget{parent}
{
    // 初始化
    initMemberVariable();
    initLayout();
    initConnect();
}

// 初始化 成员变量
void HistoricalVersionButton::initMemberVariable()
{
    // 设置 按钮固定高度
    this->setFixedHeight(HISTORICALVERSIONBUTTON_HEIGHT);

    m_backgroundColor = QColor(245, 246, 249);
    m_hoverBackgroundColor = QColor(220, 234, 246);
    m_lineColor = QColor(205, 205, 205);

    // 默认 鼠标未 悬停
    m_isHover = false;
}
// 初始化 布局
void HistoricalVersionButton::initLayout()
{
    // 主布局，水平布局，左边为 标题和时间，中间为 弹簧，右边为 字数区
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(5,5,5,5);
    this->setLayout(mainLayout); // 将 主布局 设置为 控件布局

    // 标题区，垂直布局，上方为 标题标签，下方为 时间标签
    QVBoxLayout *titleLayout = new QVBoxLayout;
    titleLayout->setSpacing(0);
    titleLayout->setContentsMargins(0,0,0,0);
    mainLayout->addLayout(titleLayout);

    // 设置 标签背景颜色
    QString backgroundColorString = converRGB16HexStr(m_backgroundColor);

    // 版本标题 标签
    m_labTitle = new QLabel(this);
    m_labTitle->setProperty("WidgetStyleClass", "HistoricalVersionButton_TitleLabel");
    m_labTitle->setText("版本X");
    m_labTitle->setStyleSheet(QString("background-color: %1;").arg(backgroundColorString));
    titleLayout->addWidget(m_labTitle);

    // 创建时间 标签
    m_labBirthTime = new QLabel(this);
    m_labBirthTime->setProperty("WidgetStyleClass", "HistoricalVersionButton_BirthTimeLabel");
    m_labBirthTime->setText("YYYY-MM-DD hh:mm:ss");
    m_labBirthTime->setWordWrap(true);
    m_labBirthTime->setStyleSheet(QString("background-color: %1;").arg(backgroundColorString));
    titleLayout->addWidget(m_labBirthTime);


    // 添加弹簧
    mainLayout->addSpacerItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Fixed));

    // 字数区
    QVBoxLayout *wordCountLayout = new QVBoxLayout;
    wordCountLayout->setSpacing(0);
    wordCountLayout->setContentsMargins(0,0,0,0);
    mainLayout->addLayout(wordCountLayout);

    // 添加弹簧
    // wordCountLayout->addSpacerItem(new QSpacerItem(10,10,QSizePolicy::Fixed,QSizePolicy::Expanding));
    wordCountLayout->addSpacing(24);

    // 字数 标签
    m_labWordCount = new QLabel(this);
    m_labWordCount->setProperty("WidgetStyleClass", "HistoricalVersionButton_WordCountLabel");
    m_labWordCount->setText("X字");
    m_labWordCount->setStyleSheet(QString("background-color: %1;").arg(backgroundColorString));
    wordCountLayout->addWidget(m_labWordCount);

}
// 初始化 绑定
void HistoricalVersionButton::initConnect()
{

}

// 设置 和 获取 背景颜色
void HistoricalVersionButton::setBackgroundColor(const QColor &backgroundColor)
{
    m_backgroundColor = backgroundColor;
    update();
}
QColor HistoricalVersionButton::getBackgroundColor() const
{
    return m_backgroundColor;
}
// 设置 和 获取 鼠标悬停时的背景颜色
void HistoricalVersionButton::setHoverBackgroundColor(const QColor &hoverBackgroundColor)
{
    m_hoverBackgroundColor = hoverBackgroundColor;
    update();
}
QColor HistoricalVersionButton::getHoverBackgroundColor() const
{
    return m_hoverBackgroundColor;
}
// 设置 和 获取 线条颜色
void HistoricalVersionButton::setLineColor(const QColor &lineColor)
{
    m_lineColor = lineColor;
    update();
}
QColor HistoricalVersionButton::getLineColor() const
{
    return m_lineColor;
}

// 设置 历史版本按钮 信息
void HistoricalVersionButton::setButtonInfo(QString versionID, QString filePath, QString title, QString birthTime, int wordCount)
{
    // 设置 章节ID、版本ID、字数、备份文件路径
    m_versionID = versionID;
    m_filePath = filePath;

    // 设置 控件
    m_labTitle->setText(title);
    m_labBirthTime->setText(birthTime);
    m_labWordCount->setText(QString("%1字").arg(wordCount));

    // 更新
    update();
}


// 绘画 事件
void HistoricalVersionButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this); // 画家类对象
    painter.setRenderHint(QPainter::Antialiasing); // 抗锯齿

    painter.setPen(Qt::NoPen); // 不设置画笔
    if(m_isHover) // 鼠标悬停
    {
        painter.setBrush(QBrush(m_hoverBackgroundColor));

        // 设置 标签背景颜色
        QString hoverBackgroundColorString = converRGB16HexStr(m_hoverBackgroundColor);
        m_labTitle->setStyleSheet(QString("background-color: %1;").arg(hoverBackgroundColorString));
        m_labBirthTime->setStyleSheet(QString("background-color: %1;").arg(hoverBackgroundColorString));
        m_labWordCount->setStyleSheet(QString("background-color: %1;").arg(hoverBackgroundColorString));
    }
    else
    {
        painter.setBrush(QBrush(m_backgroundColor)); // 设置 画刷

        // 设置 标签背景颜色
        QString backgroundColorString = converRGB16HexStr(m_backgroundColor);
        m_labTitle->setStyleSheet(QString("background-color: %1;").arg(backgroundColorString));
        m_labBirthTime->setStyleSheet(QString("background-color: %1;").arg(backgroundColorString));
        m_labWordCount->setStyleSheet(QString("background-color: %1;").arg(backgroundColorString));
    }

    painter.drawRect(QRect(0,0,this->width(),this->height())); // 绘制 控件尺寸 的矩形

    // 绘制 底部 线条
    QPen pen;
    pen.setWidth(2);
    pen.setColor(m_lineColor);
    pen.setStyle(Qt::SolidLine);
    pen.setCapStyle(Qt::RoundCap);
    painter.setPen(pen);

    painter.drawLine(QPoint(0,this->height()-1),QPoint(this->width(),this->height()-1));
}

// 鼠标 双击事件
void HistoricalVersionButton::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event);

    if(m_filePath !="") // 文件路径 不为空
        emit historicalVersionButton_DoubleClicked(m_filePath); // 发送 信号
}

// 尺寸改变事件
void HistoricalVersionButton::resizeEvent(QResizeEvent *event)
{
    update();
    QWidget::resizeEvent(event);
}

// 鼠标 进入离开事件
void HistoricalVersionButton::enterEvent(QEnterEvent *event)
{
    Q_UNUSED(event);
    m_isHover = true;
    update();

    // QWidget::enterEvent(event);
}
void HistoricalVersionButton::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);
    m_isHover = false;
    update();

    // QWidget::leaveEvent(event);
}



