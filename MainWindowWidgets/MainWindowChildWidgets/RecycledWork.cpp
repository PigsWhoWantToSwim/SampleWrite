#include "RecycledWork.h"

RecycledWork::RecycledWork(QWidget *parent)
    : QWidget{parent}
{
    // 初始化
    initMemberVariable();
    initLayout();
    initConnect();
}


void RecycledWork::initMemberVariable()
{
    this->setFixedHeight(RECYCLEWORK_HEIGHT);

    m_backgroundColor = QColor(245, 246, 249);
    m_hoverBackgroundColor = QColor(220, 234, 246);
    m_lineColor = QColor(205, 205, 205);

    // m_isChecked = false;
    m_workType = RecycledWorkType::Chapter;
    m_workID = "";
    m_wordCount = 0;
    m_index = -1;

    // 默认 鼠标未 悬停
    m_isHover = false;
}

void RecycledWork::initLayout()
{
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 5);
    this->setLayout(mainLayout);

    m_ckbSelect = new QCheckBox(this);
    m_ckbSelect->setProperty("WidgetStyleClass", "RecycledWork_SelectCheckBox");
    m_ckbSelect->setTristate(false); // 禁止半选
    m_ckbSelect->setChecked(false);
    m_ckbSelect->setText("");
    m_ckbSelect->setCursor(QCursor(Qt::PointingHandCursor));
    mainLayout->addWidget(m_ckbSelect);

    m_labWorkTitle = new QLabel("作品名称", this);
    m_labWorkTitle->setProperty("WidgetStyleClass", "RecycledWork_TitleLabel");
    m_labWorkTitle->setWordWrap(true);
    mainLayout->addWidget(m_labWorkTitle);

    // 作品类型
    m_labWorkType = new QLabel("类型", this);
    m_labWorkType->setProperty("WidgetStyleClass", "RecycledWork_ContentLabel");
    m_labWorkType->setWordWrap(true);
    mainLayout->addWidget(m_labWorkType);

    // 删除时间
    m_labDeleteTime = new QLabel("删除时间", this);
    m_labDeleteTime->setProperty("WidgetStyleClass", "RecycledWork_ContentLabel");
    m_labDeleteTime->setWordWrap(true);
    mainLayout->addWidget(m_labDeleteTime);

    // 剩余时间
    m_labRemaningTime = new QLabel("剩余", this);
    m_labRemaningTime->setProperty("WidgetStyleClass", "RecycledWork_ContentLabel");
    m_labRemaningTime->setWordWrap(true);
    mainLayout->addWidget(m_labRemaningTime);

    // 字数
    m_labWorkCount = new QLabel("字数", this);
    m_labWorkCount->setProperty("WidgetStyleClass", "RecycledWork_ContentLabel");
    m_labWorkCount->setWordWrap(true);
    mainLayout->addWidget(m_labWorkCount);

    // 所属的小说
    m_labBelongsNovel = new QLabel("所属作品", this);
    m_labBelongsNovel->setProperty("WidgetStyleClass", "RecycledWork_ContentLabel");
    m_labBelongsNovel->setWordWrap(true);
    mainLayout->addWidget(m_labBelongsNovel);
}

void RecycledWork::initConnect()
{
    connect(m_ckbSelect, &QCheckBox::clicked, this, [=](bool checked){
        emit recycledWork_checkStateChanged(checked);
    });
}

// 设置 和 获取 背景颜色
void RecycledWork::setBackgroundColor(const QColor &backgroundColor)
{
    m_backgroundColor = backgroundColor;
    update();
}
QColor RecycledWork::getBackgroundColor() const
{
    return m_backgroundColor;
}
// 设置 和 获取 光标悬停时的背景颜色
void RecycledWork::setHoverBackgroundColor(const QColor &hoverBackgroundColor)
{
    m_hoverBackgroundColor = hoverBackgroundColor;
    update();
}
QColor RecycledWork::getHoverBackgroundColor() const
{
    return m_hoverBackgroundColor;
}
// 设置 和 获取 线条颜色
void RecycledWork::setLineColor(const QColor &lineColor)
{
    m_lineColor = lineColor;
    update();
}
QColor RecycledWork::getLineColor() const
{
    return m_lineColor;
}

// 设置 和 获取 选中状态
void RecycledWork::setChecked(const bool checked)
{
    m_ckbSelect->setChecked(checked);
    update();
}
bool RecycledWork::getChecked() const
{
    return m_ckbSelect->isChecked();
}
// 设置 和 获取 作品ID
void RecycledWork::setWorkID(const QString workID)
{
    m_workID = workID;
}
QString RecycledWork::getWorkID() const
{
    return m_workID;
}
// 设置 和 获取 作品类型
void RecycledWork::setWorkType(const RecycledWorkType workType)
{
    m_workType = workType;
    switch (workType) {
    case RecycledWorkType::Novel:
        m_labWorkType->setText("小说");
        break;
    case RecycledWorkType::Volume:
        m_labWorkType->setText("分卷");
        break;
    case RecycledWorkType::Chapter:
        m_labWorkType->setText("章节");
        break;
    case RecycledWorkType::WorkRelated:
        m_labWorkType->setText("作品相关");
        break;
    default:
        break;
    }

    update();
}
RecycledWorkType RecycledWork::getWorkType() const
{
    return m_workType;
}
// 设置 和 获取 索引
void RecycledWork::setIndex(const int index)
{
    m_index = index;
}
int RecycledWork::getIndex() const
{
    return m_index;
}
// 设置 和 获取 作品名称
void RecycledWork::setWorkTitle(const QString workTitle)
{
    m_labWorkTitle->setText(workTitle);
    update();
}
QString RecycledWork::getWorkTitle() const
{
    return m_labWorkTitle->text();
}
// 设置 和 获取 作品删除时间
void RecycledWork::setWorkDeleteTime(const QString deleteTime)
{
    m_labDeleteTime->setText(deleteTime);
    update();
}
QString RecycledWork::getWorkDeleteTime() const
{
    return m_labDeleteTime->text();
}
// 设置 和 获取 剩余时间
void RecycledWork::setRemaningTime(const QString remaningTime)
{
    m_labRemaningTime->setText(remaningTime);
    update();
}
QString RecycledWork::getRemaningTime() const
{
    return m_labRemaningTime->text();
}
// 设置 和 获取 字数
void RecycledWork::setWordCount(const int workCount)
{
    m_wordCount = workCount;
    m_labWorkCount->setText(QString("%1字").arg(workCount));
    update();
}
int RecycledWork::getWorkCount() const
{
    return m_wordCount;
}
// 设置 和 获取 所属的小说
void RecycledWork::setBelongsNovel(const QString belongsNovel)
{
    m_labBelongsNovel->setText(belongsNovel);
    update();
}
QString RecycledWork::getBelongsNovel() const
{
    return m_labBelongsNovel->text();
}
// 设置 回收项的全部信息
void RecycledWork::setRecycledWorkInfo(const QString workID, const QString workTitle,
                                       const QString deleteTime, const QString remaningTime,
                                       const RecycledWorkType workType, const int wordCount,
                                       const QString belongsNovel, const int index)
{
    m_workID = workID;
    m_labWorkTitle->setText(workTitle);
    m_labDeleteTime->setText(deleteTime);
    m_labRemaningTime->setText(remaningTime);
    m_workType = workType;
    switch (workType) {
    case RecycledWorkType::Novel: // 小说
        m_labWorkType->setText("小说");
        break;
    case RecycledWorkType::Volume: // 小说
        m_labWorkType->setText("分卷");
        break;
    case RecycledWorkType::Chapter: // 小说
        m_labWorkType->setText("章节");
        break;
    case RecycledWorkType::WorkRelated: // 小说
        m_labWorkType->setText("作品相关");
        break;
    default:
        break;
    }
    m_wordCount = wordCount;
    m_labWorkCount->setText(QString("%1字").arg(wordCount));
    m_labBelongsNovel->setText(QString("《%1》").arg(belongsNovel));
    m_index = index;

    update();
}

// void RecycledWork::do_recycleTitleBar_checkStateChanged(bool checked)
// {
//     m_ckbSelect->setChecked(checked);
//     update();
// }

// 绘制背景
void RecycledWork::drawBackground(QPainter *painter)
{
    // 保存 画家类对象 当前坐标状态
    painter->save();
    painter->setPen(Qt::NoPen); // 边框 不设置 画笔

    if(m_isHover)
        painter->setBrush(QBrush(m_hoverBackgroundColor)); // 设置 画刷
    else
        painter->setBrush(QBrush(m_backgroundColor)); // 设置 画刷

    painter->drawRect(QRect(0,0,this->width(),this->height())); // 绘制 控件尺寸 的矩形

    // 恢复 画家类对象 上一次保存的坐标状态
    painter->restore();
}
// 绘制线条
void RecycledWork::drawLines(QPainter *painter)
{
    // 保存 画家类对象 当前坐标状态
    painter->save();

    // 绘制 左侧 顶部 右侧线条
    QPen pen;
    pen.setWidth(3);
    pen.setColor(m_lineColor);
    pen.setStyle(Qt::SolidLine);
    pen.setCapStyle(Qt::RoundCap);
    painter->setPen(pen);

    QPoint leftBottom(0,this->height());
    // QPoint leftTop(0,0);
    // QPoint rightTop(this->width(),0);
    QPoint rightBottom(this->width(),this->height());

    painter->drawLine(leftBottom, rightBottom);

    // 恢复 画家类对象 上一次保存的坐标状态
    painter->restore();
}

void RecycledWork::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event); // 不使用 事件
    //绘制准备工作
    QPainter painter(this); // 画家类 对象
    painter.setRenderHint(QPainter::Antialiasing); // 设置 抗锯齿
    //m_pixelRatio = painter.device()->devicePixelRatioF(); // 获取不同显示器的分辨率比例，防止不同分辨率下图片失真

    drawBackground(&painter); // 绘制 背景
    drawLines(&painter); // 绘制 边框
}

void RecycledWork::enterEvent(QEnterEvent *event)
{
    Q_UNUSED(event);
    m_isHover = true;
    update();
}

void RecycledWork::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);
    m_isHover = false;
    update();
}



