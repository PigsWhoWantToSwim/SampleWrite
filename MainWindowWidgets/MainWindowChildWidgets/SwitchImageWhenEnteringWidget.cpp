#include "SwitchImageWhenEnteringWidget.h"

SwitchImageWhenEnteringWidget::SwitchImageWhenEnteringWidget(QWidget *parent)
    : QWidget{parent}
{
    // 鼠标进入离开标志
    m_isEnter = false;

    // 正常显示的背景
    m_normalBgPath = "";
    m_normalBgPix = QPixmap(this->size());
    m_normalBgPix.fill(QColor(255, 255, 255));

    // 鼠标进入时的背景
    m_enterBgPath = "";
    m_enterBgPix = QPixmap(this->size());
    m_enterBgPix.fill(QColor(0, 0, 0, 120));

    // 圆角半径
    m_rectRadius = NOVELWIDGET_COVER_RECTRADIUS;
}

// 设置 是否  进入
void SwitchImageWhenEnteringWidget::setEnter(bool isEnter)
{
    m_isEnter = isEnter;
    update();
}
// 获取 是否  进入
bool SwitchImageWhenEnteringWidget::getEnter()
{
    return m_isEnter;
}

// 设置 和 获取 正常显示时鼠标进入时背景的路径及图片
void SwitchImageWhenEnteringWidget::setNormalBackgroundPath(QString pixmapPath)
{
    m_normalBgPath = pixmapPath;
    m_normalBgPix = scalePixmapFillRect(pixmapPath, this->size());
    update();
}
void SwitchImageWhenEnteringWidget::setNormalBackgroundPixmap(QPixmap pixmap)
{
    m_normalBgPix = scalePixmapFillRect(pixmap, this->size());
    update();;
}
void SwitchImageWhenEnteringWidget::setEnterBackgroundPath(QString pixmapPath)
{
    m_enterBgPath = pixmapPath;
    m_enterBgPix = scalePixmapFillRect(pixmapPath, this->size());
    update();
}
void SwitchImageWhenEnteringWidget::setEnterBackgroundPixmap(QPixmap pixmap)
{
    m_enterBgPix = scalePixmapFillRect(pixmap, this->size());
}


QString SwitchImageWhenEnteringWidget::getNormalBackgroundPath()
{
    return m_normalBgPath;
}
QPixmap SwitchImageWhenEnteringWidget::getNormalBackgroundPixmap()
{
    return m_normalBgPix;
}
QString SwitchImageWhenEnteringWidget::getEnterBackgroundPath()
{
    return m_enterBgPath;
}
QPixmap SwitchImageWhenEnteringWidget::getEnterBackgroundPixmap()
{
    return m_enterBgPix;
}


// 绘制 正常显示背景
void SwitchImageWhenEnteringWidget::drawNormalBackground(QPainter *painter)
{
    // 保存 画家类对象 当前坐标状态
    painter->save();
    painter->setPen(Qt::NoPen); // 边框 不设置 画笔

    painter->drawPixmap(this->rect(), m_normalBgPix);

    // 恢复 画家类对象 上一次保存的坐标状态
    painter->restore();
}
// 绘制 绘制鼠标进入显示背景
void SwitchImageWhenEnteringWidget::drawEnterBackground(QPainter *painter)
{
    // 保存 画家类对象 当前坐标状态
    painter->save();
    painter->setPen(Qt::NoPen); // 边框 不设置 画笔

    painter->drawPixmap(this->rect(), m_enterBgPix);

    // 恢复 画家类对象 上一次保存的坐标状态
    painter->restore();
}

// 缩放图片 填充一整个矩形尺寸
QPixmap SwitchImageWhenEnteringWidget::scalePixmapFillRect(QString pixmapPath, QSize pixmapSize)
{
    QPixmap originalPixmap = QPixmap(pixmapPath);
    return scalePixmapFillRect(originalPixmap, pixmapSize);
}
QPixmap SwitchImageWhenEnteringWidget::scalePixmapFillRect(QPixmap pixmap, QSize pixmapSize)
{
    /************************************************************
     * 图片缩放是 按高度对齐 还是 按宽度对齐 ?                     *
     * 1.缩放法                                                 *
     *  将图片按比例缩放，使图片宽度与按钮宽度相同(使高度相同同理)    *
     *  如果缩放后的图片高度小于按钮高度，则说明按宽度对齐缩放能完全  *
     *  显示图片，反之按高度对齐缩放。                             *
     * 2.比例法                                                 *
     *  分别计算按钮和图片的宽高比(高宽比同理)，如果图片宽高比大于    *
     *  按钮宽高比，则按宽度对齐缩放，反之按高度对齐。               *
     * 3.面积法(比例法变形)                                      *
     * 如果(图片宽度 * 按钮高度) > (按钮宽度 * 图片高度)，则说明按高 *
     * 度对齐缩放时，图片宽度大于按钮宽度，应该按宽度对齐缩放，反之说  *
     * 明按高度对齐缩放时，图片宽度小于按钮宽度，能完全显示图片，应该  *
     * 按高度对齐缩放。                                           *
    ************************************************************/

    // 图片原始尺寸
    int pixWidth = pixmap.width();
    int pixHeight = pixmap.height();

    // 矩形缩放后尺寸
    int rectScaledWidth = pixWidth;
    int rectScaledHeight = pixHeight;

    // 缩放比
    double scaledRate;

    // 在原始图片上进行裁剪的起始坐标
    int start_X = 0;
    int start_Y = 0;

    // 面积法
    if (pixWidth * pixmapSize.height() > pixHeight * pixmapSize.width())
    {
        // 此时，按宽度对齐缩放能全部显示图片，但不能完全填充矩形
        // 故应该按高度对齐，此时缩放后图片宽度大于矩形，需进行裁剪
        scaledRate = double(pixmapSize.height()) / double(pixHeight); // 缩放比

        rectScaledWidth = int(pixmapSize.width() / scaledRate);

        start_X = (pixWidth - rectScaledWidth)/2;
    }
    else
    {
        // 此时，按高度对齐缩放能全部显示图片，但不能完全填充矩形
        // 故应该按宽度对齐，此时缩放后图片高度大于矩形，需进行裁剪
        scaledRate = double(pixmapSize.width()) / double(pixWidth); // 缩放比

        rectScaledHeight = int(pixmapSize.height() / scaledRate);

        start_Y = (pixHeight - rectScaledHeight)/2;
    }

    // 进行 裁剪，截取中间
    QPixmap scaledPix = pixmap.copy(start_X, start_Y, rectScaledWidth, rectScaledHeight);

    // 对裁剪后的图片 进行 缩放，缩放到 填充整个矩形
    scaledPix = scaledPix.scaled(QSize(pixmapSize.width(), pixmapSize.height()),
                                 Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    // 返回 裁剪后的图片
    return scaledPix;
}

// 获取 裁剪圆角矩形的路径
QPainterPath SwitchImageWhenEnteringWidget::getClipRectRadiusPath()
{
    // 绘制 裁剪区路径
    QPainterPath clipPath; // 裁剪路径，用于制作圆角图片
    // 移动到 左上角圆弧  与 上方直线 相切的点
    clipPath.moveTo(this->rect().left()+m_rectRadius, this->rect().top());
    // 绘制 左上角圆弧，在一个正方形内，从90°开始，逆时针绘制90°的圆弧
    clipPath.arcTo(QRect(this->rect().left(), this->rect().top(), m_rectRadius, m_rectRadius), 90, 90);
    // 绘制 左侧直线
    clipPath.lineTo(this->rect().left(),this->rect().bottom()-m_rectRadius);
    // 绘制 左下角圆弧
    clipPath.arcTo(QRect(this->rect().left(), this->rect().bottom()-m_rectRadius, m_rectRadius, m_rectRadius), 180, 90);
    // 绘制 下方直线
    clipPath.lineTo(this->rect().right()-m_rectRadius,this->rect().bottom());
    // 绘制 右下角圆弧
    clipPath.arcTo(QRect(this->rect().right()-m_rectRadius, this->rect().bottom()-m_rectRadius, m_rectRadius, m_rectRadius), 270, 90);
    // 绘制 右侧直线
    clipPath.lineTo(this->rect().right(), this->rect().top()+m_rectRadius);
    // 绘制 右上角圆弧，在一个正方形内，从0°开始，逆时针绘制90°的圆弧
    clipPath.arcTo(QRectF(this->rect().right()-m_rectRadius, this->rect().top(), m_rectRadius, m_rectRadius), 0, 90);
    // 绘制 上方直线
    clipPath.lineTo(this->rect().left()+m_rectRadius,this->rect().top());

    return clipPath;
}


// 重写绘画事件 绘制背景
void SwitchImageWhenEnteringWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event); // 不使用 事件
    //绘制准备工作
    QPainter painter(this); // 画家类 对象
    painter.setRenderHint(QPainter::Antialiasing); // 设置 抗锯齿

    // 设置 裁剪路径（裁剪掉外围）
    painter.setClipPath(getClipRectRadiusPath());

    // 绘制背景
    drawNormalBackground(&painter);
    if(m_isEnter)
        drawEnterBackground(&painter);
}

