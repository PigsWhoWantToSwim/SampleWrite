#include "ImageCropperWidget.h"

ImageCropperWidget::ImageCropperWidget(QWidget *parent)
    : QWidget{parent}
{
    // 初始化
    initMemberVariable();
}
ImageCropperWidget::ImageCropperWidget(int width, int height, QWidget *parent)
    : QWidget{parent}
{
    this->setFixedSize(width, height); // 设置固定宽高

    // 初始化
    initMemberVariable();
}

// 初始化 成员变量
void ImageCropperWidget::initMemberVariable()
{
   // this->setAlignment(Qt::AlignCenter); // 设置居中
    this->setMouseTracking(true); // 开启鼠标追踪

    m_borderPenWidth = IMAGECROPPERWIDGET_PEN_WIDTH;
    m_borderPenColor = QColor(255, 255, 255);
    m_controlPointColor = QColor(51, 153, 255);

    m_controlPointSize = IMAGECROPPERWIDGET_CONTROLPOINT_SIZE;

    m_cropBoxMinWidth = IMAGECROPPERWIDGET_CONTROLPOINT_SIZE * 3;
    m_cropBoxMiHeight = IMAGECROPPERWIDGET_CONTROLPOINT_SIZE * 4;

    m_scaledRate = 1.0;
    m_opacity = 0.6;

    m_cursorPosInCropBox = ImageCropBoxPosition::CROP_BOX_OUTSIZD;

    m_isLeftBtnPressed = false;
    m_isCursorPosCalculated = false;

}

// 设置 背景颜色
void ImageCropperWidget::setBackgroundColor(const QColor &backgroundColor)
{
    m_backgroundColor = backgroundColor;
    update();
}
// 获取 背景颜色
QColor ImageCropperWidget::getBackgroundColor() const
{
    return m_backgroundColor;
}
// 设置 边框颜色
void ImageCropperWidget::setBorderColor(const QColor &borderColor)
{
    m_borderPenColor = borderColor;
    update();
}
// 获取 边框颜色
QColor ImageCropperWidget::getBorderColor() const
{
    return m_borderPenColor;
}
// 设置 裁剪框宽度
void ImageCropperWidget::setBorderWidth(const double borderWidth)
{
    m_borderPenWidth = borderWidth;
    update();
}
// 获取 裁剪框宽度
double ImageCropperWidget::getBorderWidth() const
{
    return m_borderPenWidth;
}
// 设置 控制点颜色
void ImageCropperWidget::setControlPointColor(const QColor &controlPointColor)
{
    m_controlPointColor = controlPointColor;
    update();
}
// 获取 控制点颜色
QColor ImageCropperWidget::getControlPointColor() const
{
    return m_controlPointColor;
}
// 设置 控制点尺寸
void ImageCropperWidget::setControlPointSize(const double controlPointSize)
{
    m_controlPointSize = controlPointSize;
    update();
}
// 获取 控制点尺寸
double ImageCropperWidget::getControlPointSize() const
{
    return m_controlPointSize;
}
// 设置 透明度
void ImageCropperWidget::setOpacity(const double opacity)
{
    m_opacity = opacity;
    update();
}
// 获取 透明度
double ImageCropperWidget::getOpacity() const
{
    return m_opacity;
}

// 设置要裁剪的图片
void ImageCropperWidget::setPixmapToCrop(QPixmap croppedPixmap)
{
    m_pixReal = croppedPixmap;

    // 获取原始图片的宽高
    int pixWidth = m_pixReal.width();
    int pixHeight = m_pixReal.height();

    // 获取截取标签的宽高
    int widgetWidth = this->width();
    int widgetHeight = this->height();

    // 缩放后图片的宽高
    int scaledPixmapWidth = widgetWidth;
    int scaledPixmapHeight = widgetHeight;

    // 裁剪框 右上角坐标
    int start_X = 0;
    int start_Y = 0;

    /************************************************************
     * 图片缩放是 按高度对齐 还是 按宽度对齐 ?                     *
     * 1.缩放法                                                 *
     *  将图片按比例缩放，使图片宽度与裁剪控件宽度相同(使高度相同同  *
     *  理)。如果缩放后的图片高度小于按钮高度，则说明按宽度对齐缩放  *
     *  能完全显示图片，反之按高度对齐缩放。                       *
     * 2.比例法                                                  *
     *  分别计算按钮和图片的宽高比(高宽比同理)，如果图片宽高比大于   *
     *  按钮宽高比，则按宽度对齐缩放，反之按高度对齐。              *
     * 3.面积法(比例法变形)                                       *
     * 如果(图片宽度 * 按钮高度) > (按钮宽度 * 图片高度)，则说明按高 *
     * 度对齐缩放时，图片宽度大于按钮宽度，应该按宽度对齐缩放，反之说 *
     * 明按高度对齐缩放时，图片宽度小于按钮宽度，能完全显示图片，应该 *
     * 按高度对齐缩放。                                           *
    *************************************************************/

    // 使用 面积法
    if (pixWidth * widgetHeight < widgetWidth * pixHeight)
    {
        // 使用按高度对齐 能完全显示图片，且缩放后的图片宽度要小于等于截取控件的宽度
        m_scaledRate = widgetHeight / double(pixHeight); // 缩放比
        // 经缩放后，图片的宽高
        scaledPixmapWidth = int(pixWidth * m_scaledRate);

        start_X = (widgetWidth - scaledPixmapWidth)/2;
    }
    else
    {
        // 使用按宽度对齐 能完全显示图片，且缩放后的图片高度要小于等于截取控件的高度
        m_scaledRate = widgetWidth / double(pixWidth); // 缩放比
        // 经缩放后，图片的宽高
        scaledPixmapHeight = int(pixHeight * m_scaledRate);

        start_Y = (widgetHeight - scaledPixmapHeight)/2;
    }

    // 控件上 整张大图的尺寸
    m_recDisplayedPixmap.setRect(start_X, start_Y,scaledPixmapWidth, scaledPixmapHeight);

    // 将原始图片 缩放成 临时图片 （控件上的整张大图）
    m_pixShow = m_pixReal.scaled(scaledPixmapWidth, scaledPixmapHeight,
                                           Qt::KeepAspectRatio, Qt::SmoothTransformation);

    // 重新设置截取框的位置
    resetCropBoxPosition();

    update();

}
// 获取截取后的图片
QPixmap ImageCropperWidget::getCroppedPixmap()
{
    // 获得真实的截取框 距 真实图片左侧和上侧 的距离
    int startX = int((m_recCropBox.left() - m_recDisplayedPixmap.left()) / m_scaledRate);
    int startY = int((m_recCropBox.top() - m_recDisplayedPixmap.top()) / m_scaledRate);

    // 获取真实截取框的尺寸
    int realCroppedWidth = int(m_recCropBox.width() / m_scaledRate);
    int realCroppedHeight = int(m_recCropBox.height() / m_scaledRate);

    // 创建图片对象，并设置图片尺寸
    QPixmap croppedPixmap(realCroppedWidth, realCroppedHeight);

    // 使用 copy()函数 从原图片 复制要截取的部分
    croppedPixmap = m_pixReal.copy(startX, startY, realCroppedWidth, realCroppedHeight);

    return croppedPixmap;
}

// 重新设置截取框位置
void ImageCropperWidget::resetCropBoxPosition()
{
    // 获取截取标签的尺寸
    int widgetWidth = this->width();
    int widgetHeight = this->height();

    // 获取临时图片的尺寸
    int tempPixmapWidth = m_pixShow.width();
    int tempPixmapHeight = m_pixShow.height();

    // 获取临时图片宽高中较小的一条边，并缩小到原来的3/4，作为截取框的初始高
    int cropBoxHeight = int((tempPixmapWidth < tempPixmapHeight ? tempPixmapWidth : tempPixmapHeight) * 3.0 / 4.0);
    int cropBoxWidth = cropBoxHeight * 3.0 / 4.0;

    // 设置截取框尺寸，并居中处理
    m_recCropBox.setRect((widgetWidth - cropBoxWidth) / 2,
                                  (widgetHeight - cropBoxHeight) / 2,
                                  cropBoxWidth, cropBoxHeight);
}

// 两点是否相近
bool ImageCropperWidget::areTwoPointsClose(const QPoint &point1, const QPoint &point2,int effectiveDistance)
{
    // 只要 point1 在 以point2为中心，边长为CONTROL_POINT_SIZE的正方形的内部或边上，即为真
    return (abs(point1.x()-point2.x()) * 2 <= effectiveDistance) &&
           (abs(point1.y()-point2.y()) * 2 <= effectiveDistance);
}
// 获取一个点在截取框里的坐标
ImageCropBoxPosition ImageCropperWidget::getOrientationInCropBoxRect(const QPoint &point)
{
    // 判断point靠近哪一个控制点
    if (areTwoPointsClose(point, QPoint(m_recCropBox.right(), m_recCropBox.center().y()), IMAGECROPPERWIDGET_CONTROLPOINT_SIZE))
        return ImageCropBoxPosition::CROP_BOX_RIGHT; // 靠近右侧控制点
    if (areTwoPointsClose(point, QPoint(m_recCropBox.left(), m_recCropBox.center().y()), IMAGECROPPERWIDGET_CONTROLPOINT_SIZE))
        return ImageCropBoxPosition::CROP_BOX_LEFT; // 靠近左侧控制点
    if (areTwoPointsClose(point, QPoint(m_recCropBox.center().x(), m_recCropBox.top()), IMAGECROPPERWIDGET_CONTROLPOINT_SIZE))
        return ImageCropBoxPosition::CROP_BOX_TOP; // 靠近顶侧控制点
    if (areTwoPointsClose(point, QPoint(m_recCropBox.center().x(), m_recCropBox.bottom()), IMAGECROPPERWIDGET_CONTROLPOINT_SIZE))
        return ImageCropBoxPosition::CROP_BOX_BOTTOM; // 靠近底侧控制点

    if (areTwoPointsClose(point, m_recCropBox.topRight(), IMAGECROPPERWIDGET_CONTROLPOINT_SIZE))
        return ImageCropBoxPosition::CROP_BOX_TOP_RIGHT; // 靠近右上角控制点
    if (areTwoPointsClose(point, m_recCropBox.topLeft(), IMAGECROPPERWIDGET_CONTROLPOINT_SIZE))
        return ImageCropBoxPosition::CROP_BOX_TOP_LEFT; // 靠近左上角控制点
    if (areTwoPointsClose(point,m_recCropBox.bottomRight(), IMAGECROPPERWIDGET_CONTROLPOINT_SIZE))
        return ImageCropBoxPosition::CROP_BOX_BOTTOM_RIGHT; // 靠近右下角控制点
    if (areTwoPointsClose(point,m_recCropBox.bottomLeft(), IMAGECROPPERWIDGET_CONTROLPOINT_SIZE))
        return ImageCropBoxPosition::CROP_BOX_BOTTOM_LEFT; // 靠近左下角控制点

    // 在截取框内部
    if (m_recCropBox.contains(point, true))
        return ImageCropBoxPosition::CROP_BOX_INSIDE;

    // 在截取框外部
    return ImageCropBoxPosition::CROP_BOX_OUTSIZD;
}
// 改变光标形状
void ImageCropperWidget::changeCursor()
{
    switch (m_cursorPosInCropBox) // 根据光标在截取框的位置
    {
    case ImageCropBoxPosition::CROP_BOX_OUTSIZD: // 光标在截取框外部
        setCursor(Qt::ArrowCursor); // 设置光标为普通箭头
        break;
    case ImageCropBoxPosition::CROP_BOX_INSIDE: // 在截取框内部
        setCursor(Qt::SizeAllCursor); // 设置光标为十字箭头
        break;

    case ImageCropBoxPosition::CROP_BOX_TOP: // 光标在截取框顶部
    case ImageCropBoxPosition::CROP_BOX_BOTTOM: // 光标在截取框底部
        setCursor(Qt::SizeVerCursor);
        break;

    case ImageCropBoxPosition::CROP_BOX_LEFT: // 光标在截取框左侧
    case ImageCropBoxPosition::CROP_BOX_RIGHT: // 光标在截取框右侧
        setCursor(Qt::SizeHorCursor);  // 光标为 水平 双向箭头
        break;

    case ImageCropBoxPosition::CROP_BOX_TOP_LEFT: // 光标在截取框左上角
    case ImageCropBoxPosition::CROP_BOX_BOTTOM_RIGHT: // 光标在截取框右下角
        setCursor(Qt::SizeFDiagCursor); // 光标为 左上右下 双向箭头
        break;

    case ImageCropBoxPosition::CROP_BOX_TOP_RIGHT: // 光标在截取框右上角
    case ImageCropBoxPosition::CROP_BOX_BOTTOM_LEFT: // 光标在截取框左下角
        setCursor(Qt::SizeBDiagCursor); // 光标为 右上左下 双向箭头
        break;

    default:
        break;
    }
}

// 绘制 空白背景
void ImageCropperWidget::drawBlankBackground(QPainter *painter)
{
    // 保存 画家类对象 当前坐标状态
    painter->save();
    painter->setPen(Qt::NoPen); // 边框 不设置 画笔

    QPixmap blankPix(":/image/blank background - gray.png");
    int pixWidth = blankPix.width() / 8;
    int pixHeight = blankPix.height() / 8;
    blankPix = blankPix.scaled(QSize(pixWidth, pixHeight), Qt::KeepAspectRatio, Qt::SmoothTransformation);

    painter->drawTiledPixmap(this->rect(),blankPix);

    QPen pen;
    pen.setWidth(5);
    pen.setColor(QColor(0, 0, 0));

    painter->setPen(pen);
    painter->drawRect(this->rect());

    // 恢复 画家类对象 上一次保存的坐标状态
    painter->restore();
}
// 绘制 图片
void ImageCropperWidget::drawPixmapBackground(QPainter *painter)
{
    // 保存 画家类对象 当前坐标状态
    painter->save();
    painter->setPen(Qt::NoPen); // 边框 不设置 画笔

    // 绘制图片
    painter->drawPixmap(m_recDisplayedPixmap, m_pixShow);

    // 恢复 画家类对象 上一次保存的坐标状态
    painter->restore();
}
// 绘制 裁剪框
void ImageCropperWidget::drawCropBox(QPainter *painter)
{
    // 保存 画家类对象 当前坐标状态
    painter->save();

    // 准备画笔
    QPen pen;
    pen.setWidth(m_borderPenWidth); // 设置画笔宽度
    pen.setColor(m_borderPenColor); // 设置画笔颜色
    pen.setDashPattern(QVector<qreal>() << 5 << 5 << 5 << 5); // 设置虚线模式（实线3，间隔3，实线3，间隔3）

    // 绘制 矩形裁剪边框
    painter->setPen(pen);
    painter->drawRect(m_recCropBox);

    // 绘制内部结构线
    pen.setDashPattern(QVector<qreal>() << 1 << 3 << 1 << 3); // 设置虚线模式（实线3，间隔3，实线3，间隔3）
    painter->setPen(pen);

    // 获取 裁剪框的尺寸
    int w = m_recCropBox.width();
    int h = m_recCropBox.height();

    // 绘制 2条横线、2条竖线
    painter->drawLine(QPoint(m_recCropBox.left() + int(w/3), m_recCropBox.top()),
                      QPoint(m_recCropBox.left() + int(w/3), m_recCropBox.bottom()));
    painter->drawLine(QPoint(m_recCropBox.left() + int(w*2/3), m_recCropBox.top()),
                      QPoint(m_recCropBox.left() + int(w*2/3), m_recCropBox.bottom()));

    painter->drawLine(QPoint(m_recCropBox.left(), m_recCropBox.top() + int(h/3)),
                      QPoint(m_recCropBox.right(), m_recCropBox.top() + int(h/3)));
    painter->drawLine(QPoint(m_recCropBox.left(), m_recCropBox.top() + int(h*2/3)),
                      QPoint(m_recCropBox.right(), m_recCropBox.top() + int(h*2/3)));


    // 恢复 画家类对象 上一次保存的坐标状态
    painter->restore();
}
// 绘制 遮罩
void ImageCropperWidget::drawMaskArea(QPainter *painter)
{
    // 保存 画家类对象 当前坐标状态
    painter->save();
    painter->setPen(Qt::NoPen); // 边框 不设置 画笔

    QPainterPath maskAreaPath;
    QPainterPath outRectPath;
    QPainterPath inRectPath;

    outRectPath.addRect(m_recDisplayedPixmap);
    inRectPath.addRect(m_recCropBox);

    // 将 外圈、内圈添加到路径
    maskAreaPath.addPath(outRectPath);
    maskAreaPath.addPath(inRectPath);

    // 绘制 边框
    painter->fillPath(maskAreaPath,QBrush(QColor(0, 0, 0, 255 * m_opacity)));

    // 恢复 画家类对象 上一次保存的坐标状态
    painter->restore();
}
// 绘制填充 正方形
void ImageCropperWidget::drawFillSquare(QPainter *painter, QPoint centerPos, int controlPointSize)
{
    // 保存 画家类对象 当前坐标状态
    painter->save();
    painter->setPen(Qt::NoPen); // 边框 不设置 画笔

    // 控制点（实际是一个小正方形）的几何尺长
    QRect squareRect(centerPos.x() - controlPointSize / 2, centerPos.y() - controlPointSize / 2, controlPointSize, controlPointSize);

    // 绘制控制点，并设置填充颜色
    painter->fillRect(squareRect, m_controlPointColor);

    // 恢复 画家类对象 上一次保存的坐标状态
    painter->restore();
}
// 绘制 全部控制点
void ImageCropperWidget::drawAllControlPoints(QPainter *painter)
{
    // 保存 画家类对象 当前坐标状态
    painter->save();
    painter->setPen(Qt::NoPen); // 边框 不设置 画笔

    // 绘制4个对角上的控制点
    drawFillSquare(painter, m_recCropBox.topLeft(), m_controlPointSize);
    drawFillSquare(painter, m_recCropBox.topRight(), m_controlPointSize);
    drawFillSquare(painter, m_recCropBox.bottomLeft(), m_controlPointSize);
    drawFillSquare(painter, m_recCropBox.bottomRight(), m_controlPointSize);

    // 绘制4个边框上的控制点
    // 获取截取框的中心点的坐标
    int central_X = m_recCropBox.left() + m_recCropBox.width() / 2;
    int central_Y = m_recCropBox.top() + m_recCropBox.height() / 2;

    // 在4条边上再绘制4个控制点，左上右下
    drawFillSquare(painter, QPoint(m_recCropBox.left(), central_Y), m_controlPointSize);
    drawFillSquare(painter, QPoint(central_X, m_recCropBox.top()), m_controlPointSize);
    drawFillSquare(painter, QPoint(m_recCropBox.right(), central_Y), m_controlPointSize);
    drawFillSquare(painter, QPoint(central_X, m_recCropBox.bottom()), m_controlPointSize);

    // 恢复 画家类对象 上一次保存的坐标状态
    painter->restore();
}

// 鼠标 按下事件
void ImageCropperWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton) // 按下 左键
    {
        // 记录 按下时 光标位置
        m_currPos = event->pos();
        m_lastPos = m_currPos;

        m_isLeftBtnPressed = true;
    }

    QWidget::mousePressEvent(event);
}
// 鼠标 释放事件
void ImageCropperWidget::mouseReleaseEvent(QMouseEvent *event)
{
    m_isLeftBtnPressed = false; // 鼠标未按下
    m_isCursorPosCalculated = false; // 未计算光标位置
    this->setCursor(Qt::ArrowCursor); // 设置光标未普通箭头
}
// 鼠标 移动事件
void ImageCropperWidget::mouseMoveEvent(QMouseEvent *event)
{
    m_currPos=event->pos(); // 记录当前光标位置

    // 只移动鼠标
    if (!m_isCursorPosCalculated) // 松开鼠标左键后，会置为未计算
    {
        // 光标位置未计算
        m_cursorPosInCropBox = getOrientationInCropBoxRect(m_currPos); // 判断光标在截取框哪一个位置
        changeCursor(); // 改变光标形状
    }

    // 按下左键，移动、拉伸裁剪框
    if (!m_isLeftBtnPressed) // 鼠标左键未按下
        return;
    if (!m_recDisplayedPixmap.contains(m_currPos)) // 光标不在图片矩形之内
        return;

    // 获取当前光标位置与上一次光标位置的 差，即 移动量
    int xOffset = m_currPos.x() - m_lastPos.x();
    int yOffset = m_currPos.y() - m_lastPos.y();

    // 更新上一次光标位置
    m_lastPos = m_currPos;


    // 拉伸后 裁剪框的尺寸
    int scraledCropBoxWidth = 0;
    int scraledCropBoxHeight = 0;

    // 根据 光标所在位置进行处理
    switch (m_cursorPosInCropBox)
    {
        // 光标在截取框外部
    case ImageCropBoxPosition::CROP_BOX_OUTSIZD:
        break; // 不做处理

        // 光标在截取框内部，进行移动
    case ImageCropBoxPosition::CROP_BOX_INSIDE:
    {
        // 不能进行缩放，只能进行移动
        // 确保截取框在 图片矩形之内 移动
        if(xOffset > 0) // 向右移动
        {
            if (m_recCropBox.right() + xOffset > m_recDisplayedPixmap.right())
                // 当前截取框右侧 加上 横移动量 大于 图片矩形右侧，即已到达右侧界限
                xOffset = 0; // 横向移动量置0
        }
        else if(xOffset < 0) // 向左移动
        {
            if (m_recCropBox.left() + xOffset < m_recDisplayedPixmap.left())
                // 当前截取框左侧 加上 横向移动量 小于 图片矩形左侧，即已到达左侧界限
                xOffset = 0; // 横向移动量置0
        }

        if (yOffset > 0) // 向下移动
        {
            if (m_recCropBox.bottom() + yOffset > m_recDisplayedPixmap.bottom())
                // 当前截取框底端 加上 垂直移动量 大于 图片矩形底端，即已到达底端界限
                yOffset = 0; // 垂直移动量置0
        }
        else if (yOffset < 0) // 向上移动
        {
            if (m_recCropBox.top() + yOffset < m_recDisplayedPixmap.top())
                // 当前截取框顶端 加上 垂直移动量 小于 图片矩形顶端，即已到达顶端界限
                yOffset = 0; // 垂直移动量置0
        }

        // 进行移动操作
        m_recCropBox.moveTo(m_recCropBox.left() + xOffset, m_recCropBox.top() + yOffset);
    }
        break;

        // 光标在截取框顶部，进行向左上角拉伸
    case ImageCropBoxPosition::CROP_BOX_TOP:
    {
        // 裁剪框底部 到 光标的垂直距离，（即拉伸后裁剪框的高度），以及拉伸后裁剪框的宽度
        scraledCropBoxHeight = int(m_recCropBox.bottom() - m_currPos.y());
        scraledCropBoxWidth = int(scraledCropBoxHeight * 3.0 / 4.0);

        // 设置 裁剪框 位置
        if ((scraledCropBoxHeight >= m_cropBoxMiHeight) && // 裁剪框的高度 要大于 最小高度
            (scraledCropBoxWidth >= m_cropBoxMinWidth) && // 裁剪框跟随变化的宽度 也要大于 最小宽度
            (m_recCropBox.bottom() - scraledCropBoxHeight >= m_recDisplayedPixmap.top()) && // 顶部不超过图片的顶部
            (m_recCropBox.right() - scraledCropBoxWidth >= m_recDisplayedPixmap.left())) // 左侧不超过图片的左侧
        {
            // 设置裁剪框的顶部为当前光标的位置，设置裁剪框的左侧，右侧和底部不动
            m_recCropBox.setTop(m_currPos.y());
            m_recCropBox.setLeft(m_recCropBox.right() - scraledCropBoxWidth);
        }
    }
        break;
        // 光标在截取框底部，进行向右下角拉伸
    case ImageCropBoxPosition::CROP_BOX_BOTTOM:
    {
        // 裁剪框顶部 到 光标的垂直距离，（即拉伸后裁剪框的高度），以及拉伸后裁剪框的宽度
        scraledCropBoxHeight = int(m_currPos.y() - m_recCropBox.top());
        scraledCropBoxWidth = int(scraledCropBoxHeight * 3.0 / 4.0);

        // 设置裁剪框位置
        if ((scraledCropBoxHeight >= m_cropBoxMiHeight) && // 裁剪框的高度要大于最小高度
            (scraledCropBoxWidth >= m_cropBoxMinWidth) && // 裁剪框的宽度 要大于 最小宽度
            (m_recCropBox.top() + scraledCropBoxHeight <= m_recDisplayedPixmap.bottom()) && // 裁剪框的底部不超过显示图片的底部
            (m_recCropBox.left() + scraledCropBoxWidth <= m_recDisplayedPixmap.right())) // 裁剪框的右侧不超过显示图片的右侧
        {
            // 设置裁剪框的底部为当前光标的位置，设置裁剪框的右侧，左侧和顶部不动
            m_recCropBox.setBottom(m_currPos.y());
            m_recCropBox.setRight(m_recCropBox.left() + scraledCropBoxWidth);
        }
    }
        break;

        // 光标在截取框左侧，进行向左上角拉伸
    case ImageCropBoxPosition::CROP_BOX_LEFT:
    {
        // 裁剪框右侧 到 光标的水平距离（即拉伸后裁剪框的宽度），以及拉伸后裁剪框的高度
        scraledCropBoxWidth = int(m_recCropBox.right() - m_currPos.x());
        scraledCropBoxHeight = int(scraledCropBoxWidth * 4.0 / 3.0);

        // 设置裁剪框位置
        if ((scraledCropBoxWidth >= m_cropBoxMinWidth) && // 裁剪框的高度 要大于 最小高度
            (scraledCropBoxHeight >= m_cropBoxMiHeight) && // 裁剪框的宽度 要大于 最小宽度
            (m_recCropBox.right() - scraledCropBoxWidth >= m_recDisplayedPixmap.left()) && // 裁剪框的左侧不超过显示图片的左侧
            (m_recCropBox.bottom() - scraledCropBoxHeight >= m_recDisplayedPixmap.top())) // 裁剪框的顶部不超过显示图片的顶部
        {
            // 设置裁剪框的左部为当前光标的位置，设置裁剪框的顶部，右侧和底部不动
            m_recCropBox.setLeft(m_currPos.x()); // 设置截取框的左侧值
            m_recCropBox.setTop(m_recCropBox.bottom() - scraledCropBoxHeight);
        }
    }
        break;
        // 光标在截取框右侧，进行向右下角拉伸
    case ImageCropBoxPosition::CROP_BOX_RIGHT:
    {
        // 裁剪框左侧 到 光标的水平距离（即拉伸后裁剪框的宽度），以及拉伸后裁剪框的高度
        scraledCropBoxWidth = int(m_currPos.x() - m_recCropBox.left());
        scraledCropBoxHeight = int(scraledCropBoxWidth * 4.0 / 3.0);

        // 设置裁剪框位置
        if ((scraledCropBoxWidth >= m_cropBoxMinWidth) && // 裁剪框的高度 要大于 最小高度
            (scraledCropBoxHeight >= m_cropBoxMiHeight) && // 裁剪框的宽度 要大于 最小宽度
            (m_recCropBox.left() + scraledCropBoxWidth <= m_recDisplayedPixmap.right()) && // 裁剪框的右侧不超过显示图片的右侧
            (m_recCropBox.top() + scraledCropBoxHeight <= m_recDisplayedPixmap.bottom())) // 裁剪框的底部不超过显示图片的底部
        {
            // 设置裁剪框的左部为当前光标的位置，设置裁剪框的顶部，右侧和底部不动
            m_recCropBox.setRight(m_currPos.x()); // 设置截取框的左侧值
            m_recCropBox.setBottom(m_recCropBox.top() + scraledCropBoxHeight);
        }
    }
        break;

        // 光标在截取框左上角，进行向左上角拉伸
    case ImageCropBoxPosition::CROP_BOX_TOP_LEFT:
    {
        // 光标到原裁剪框的左上角的距离（绝对值）
        int distanceCursorToBoxTopLeft_X = abs(m_recCropBox.left() - m_currPos.x());
        int distanceCursorToBoxTopLeft_Y = abs(m_recCropBox.top() - m_currPos.y());

        if(distanceCursorToBoxTopLeft_X > distanceCursorToBoxTopLeft_Y) // 主要在水平移动
        {
            // 此时，左侧与光标位置相同，高度自变化
            scraledCropBoxWidth = int(m_recCropBox.right() - m_currPos.x());
            scraledCropBoxHeight = int(scraledCropBoxWidth * 4.0 / 3.0);
        }
        else // 主要在垂直方向移动
        {
            // 此时，顶部与光标位置相同，宽度自变化
            scraledCropBoxHeight = int(m_recCropBox.bottom() - m_currPos.y());
            scraledCropBoxWidth = int(scraledCropBoxHeight * 3.0 / 4.0);
        }

        // 设置 裁剪框 位置
        if((scraledCropBoxWidth >= m_cropBoxMinWidth) && // 裁剪框的高度 要大于 最小高度
            (scraledCropBoxHeight >= m_cropBoxMiHeight) && // 裁剪框跟随变化的宽度 也要大于 最小宽度
            (m_recCropBox.bottom() - scraledCropBoxHeight >= m_recDisplayedPixmap.top()) && // 顶部不超过图片的顶部
            (m_recCropBox.right() - scraledCropBoxWidth >= m_recDisplayedPixmap.left())) // 左侧不超过图片的左侧
        {
            // 设置裁剪框的顶部、左侧，右侧和底部不动
            m_recCropBox.setTop(m_recCropBox.bottom() - scraledCropBoxHeight);
            m_recCropBox.setLeft(m_recCropBox.right() - scraledCropBoxWidth);
        }
    }
        break;
        // 光标在截取框右下角，进行向右下角拉伸
    case ImageCropBoxPosition::CROP_BOX_BOTTOM_RIGHT:
    {
        // 光标到原裁剪框的左上角的距离（绝对值）
        int distanceCursorToBoxTopLeft_X = abs(m_currPos.x() - m_recCropBox.left());
        int distanceCursorToBoxTopLeft_Y = abs(m_currPos.y() - m_recCropBox.top());

        if(distanceCursorToBoxTopLeft_X > distanceCursorToBoxTopLeft_Y) // 主要在水平方向移动
        {
            // 此时，右侧与光标位置相同，高度自变化
            scraledCropBoxWidth = int(m_currPos.x() - m_recCropBox.left());
            scraledCropBoxHeight = int(scraledCropBoxWidth * 4.0 / 3.0);
        }
        else // 主要垂直方向移动
        {
            // 此时，底部与光标位置相同，宽度自变化
            scraledCropBoxHeight = int(m_currPos.y() - m_recCropBox.top());
            scraledCropBoxWidth = int(scraledCropBoxHeight * 3.0 / 4.0);
        }

        // 设置裁剪框位置
        if ((scraledCropBoxHeight >= m_cropBoxMiHeight) && // 裁剪框的高度要大于最小高度
            (scraledCropBoxWidth >= m_cropBoxMinWidth) && // 裁剪框的宽度 要大于 最小宽度
            (m_recCropBox.top() + scraledCropBoxHeight <= m_recDisplayedPixmap.bottom()) && // 裁剪框的底部不超过显示图片的底部
            (m_recCropBox.left() + scraledCropBoxWidth <= m_recDisplayedPixmap.right())) // 裁剪框的右侧不超过显示图片的右侧
        {
            // 设置裁剪框的顶部为当前光标的位置，设置裁剪框的左侧，右侧和底部不动
            m_recCropBox.setBottom(m_recCropBox.top() + scraledCropBoxHeight); // 设置截取框的底部，原来的底部加上移动量
            m_recCropBox.setRight(m_recCropBox.left() + scraledCropBoxWidth);
        }
    }
        break;

        // 光标在截取框右上角，进行向右上角拉伸
    case ImageCropBoxPosition::CROP_BOX_TOP_RIGHT: // 光标在截取框右上角
    {
        // 光标到原裁剪框的左下角的距离（绝对值）
        int distanceCursorToBoxBottomLeft_X = abs(m_currPos.x() - m_recCropBox.left());
        int distanceCursorToBoxBottomLeft_Y = abs(m_recCropBox.bottom() - m_currPos.y());

        if(distanceCursorToBoxBottomLeft_X > distanceCursorToBoxBottomLeft_Y) // 主要在水平移动
        {
            // 此时，右侧与光标位置相同，高度自变化
            scraledCropBoxWidth = int(m_currPos.x() - m_recCropBox.left());
            scraledCropBoxHeight = int(scraledCropBoxWidth * 4.0 / 3.0);
        }
        else // 主要在垂直方向移动
        {
            // 此时，顶部与光标位置相同，宽度自变化
            scraledCropBoxHeight = int(m_recCropBox.bottom() - m_currPos.y());
            scraledCropBoxWidth = int(scraledCropBoxHeight * 3.0 / 4.0);
        }

        // 设置 裁剪框 位置
        if((scraledCropBoxWidth >= m_cropBoxMinWidth) && // 裁剪框的高度 要大于 最小高度
            (scraledCropBoxHeight >= m_cropBoxMiHeight) && // 裁剪框跟随变化的宽度 也要大于 最小宽度
            (m_recCropBox.bottom() - scraledCropBoxHeight >= m_recDisplayedPixmap.top()) && // 顶部不超过图片的顶部
            (m_recCropBox.left() + scraledCropBoxWidth <= m_recDisplayedPixmap.right())) // 左侧不超过图片的左侧
        {
            // 设置裁剪框的顶部、右侧，左侧和底部不动
            m_recCropBox.setTop(m_recCropBox.bottom() - scraledCropBoxHeight);
            m_recCropBox.setRight(m_recCropBox.left() + scraledCropBoxWidth);
        }
    }
        break;
        // 光标在截取框左下角，进行向左下角拉伸
    case ImageCropBoxPosition::CROP_BOX_BOTTOM_LEFT: // 光标在截取框左下角
    {
        // 光标到原裁剪框的右上角的距离（绝对值）
        int distanceCursorToBoxTopRight_X = abs(m_recCropBox.right() - m_currPos.x());
        int distanceCursorToBoxTopRight_Y = abs(m_currPos.y() - m_recCropBox.top());

        if(distanceCursorToBoxTopRight_X > distanceCursorToBoxTopRight_Y) // 主要在水平移动
        {
            // 此时，左侧与光标位置相同，高度自变化
            scraledCropBoxWidth = int(m_recCropBox.right() - m_currPos.x());
            scraledCropBoxHeight = int(scraledCropBoxWidth * 4.0 / 3.0);
        }
        else // 主要在垂直方向移动
        {
            // 此时，底部与光标位置相同，宽度自变化
            scraledCropBoxHeight = int(m_currPos.y() - m_recCropBox.top());
            scraledCropBoxWidth = int(scraledCropBoxHeight * 3.0 / 4.0);
        }

        // 设置 裁剪框 位置
        if((scraledCropBoxWidth >= m_cropBoxMinWidth) && // 裁剪框的高度 要大于 最小高度
            (scraledCropBoxHeight >= m_cropBoxMiHeight) && // 裁剪框跟随变化的宽度 也要大于 最小宽度
            (m_recCropBox.top() + scraledCropBoxHeight <= m_recDisplayedPixmap.bottom()) && // 底部不超过图片的底部
            (m_recCropBox.right() - scraledCropBoxWidth >= m_recDisplayedPixmap.left())) // 左侧不超过图片的左侧
        {
            // 设置裁剪框的底部、左侧，右侧和顶部不动
            m_recCropBox.setBottom(m_recCropBox.top() + scraledCropBoxHeight);
            m_recCropBox.setLeft(m_recCropBox.right() - scraledCropBoxWidth);
        }
    }
        break;
    default:
        break;
    }

    update();

}
// 绘画 事件
void ImageCropperWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event); // 不使用 事件
    //绘制准备工作
    QPainter painter(this); // 画家类 对象
    painter.setRenderHint(QPainter::Antialiasing); // 设置 抗锯齿

    drawBlankBackground(&painter);
    drawPixmapBackground(&painter);
    drawCropBox(&painter);
    drawMaskArea(&painter);
    drawAllControlPoints(&painter);
}
