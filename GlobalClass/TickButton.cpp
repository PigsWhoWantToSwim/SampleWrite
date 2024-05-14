#include "TickButton.h"

// 构造函数
TickButton::TickButton(QWidget *parent)
    : QWidget{parent}
{
    // 初始化
    // 默认未勾选
    m_checked = false;
    // 默认 按钮样式
    m_buttonStyle = TickButtonStyle::rectButton;

    // 默认颜色
    m_aboveColor = QColor(255,  255,  255);
    m_belowColor = QColor(0, 0, 0);
    m_borderColor = QColor(255, 255, 255);

    // 默认图片
    m_imagePath = ":/image/tick button default image.png";

    // 默认边框宽度、圆角半径
    m_borderWidth = 2;
    m_rectRadius = 15;

    // 阴影效果
    QGraphicsDropShadowEffect *coverShadowEffect = new QGraphicsDropShadowEffect(this); // 创建 阴影效果
    coverShadowEffect->setOffset(0, 0); // 阴影效果 不偏移
    coverShadowEffect->setColor(Qt::black); // 设置阴影效果 的颜色QColor(QStringLiteral("black"))
    coverShadowEffect->setBlurRadius(15); // 设置 模糊半径
    this->setGraphicsEffect(coverShadowEffect); // 为 容器控件设置阴影效果
}

// 析构函数
TickButton::~TickButton()
{

}

// 设置 是否选中
void TickButton::setChecked(bool checked)
{
    // 当前 开关按钮状态 不等于 要设置的 状态
    if (this->m_checked != checked)
        this->m_checked = checked; // 设置新状态
}
// 获取 是否选中
bool TickButton::getChecked() const
{
    return m_checked;
}

// 设置 按钮风格
void TickButton::setButtonStyle(TickButtonStyle buttonStyle)
{
    m_buttonStyle = buttonStyle;
    if(buttonStyle == TickButtonStyle::circleButton) // 如果是 圆形 按钮
    {
        // 获取 较小的边
        int minEdge = this->width() > this->height() ? this->height() : this->width();
        // 设置按钮为 正方形
        this->resize(minEdge , minEdge);
    }
    // 更新
    update();
}
// 获取 按钮风格
TickButtonStyle TickButton::getButtonStyle() const
{
    return m_buttonStyle;
}
// 设置 边框宽度
void TickButton::setBorderWidth(int borderWidth)
{
    m_borderWidth = borderWidth;
    update();
}
// 获取 边框宽度
int TickButton::getBorderWidth() const
{
    return m_borderWidth;
}
// 设置 矩形按钮 圆角半径
void TickButton::setRectRadius(int rectRadius)
{
    m_rectRadius = rectRadius;
    update();
}
// 获取 矩形按钮 圆角半径
int TickButton::getRectRadius() const
{
    return m_rectRadius;
}
// 设置 背景图片路径
void TickButton::setImage(QString imagePath)
{
    m_imagePath = imagePath;
    update();
}
// 获取 背景图片路径
QString TickButton::getImagePath() const
{
    return m_imagePath;
}

// 设置 按钮上颜色
void TickButton::setAboveColor(const QColor &aboveColor)
{
    m_aboveColor = aboveColor;

    // 设置 下颜色、边框颜色
    // setBelowColor(QColor(aboveColor.red()*0.75,aboveColor.green()*0.75,aboveColor.blue()*0.75));
    // setBorderColor(QColor(aboveColor.red()*5<255?aboveColor.red()*5:255,
    //                       aboveColor.green()*1.3<255?aboveColor.green()*1.3:255,
    //                       aboveColor.green()*1.3<255?aboveColor.green()*1.3:255));

    update();
}
// 获取 按钮上颜色
QColor TickButton::getAboveColor() const
{
    return m_aboveColor;
}
// 设置 按钮下颜色
void TickButton::setBelowColor(const QColor &belowColor)
{
    m_belowColor = belowColor;
    update();
}
// 获取 按钮下颜色
QColor TickButton::getBelowColor() const
{
    return m_belowColor;
}
// 设置 边框颜色
void TickButton::setBorderColor(const QColor &borderColor)
{
    m_borderColor = borderColor;
    update();
}
// 获取 边框颜色
QColor TickButton::getBorderColor() const
{
    return m_borderColor;
}

// 绘制 背景
void TickButton::drawBackground(QPainter *painter)
{
    // 保存 画家类对象 当前坐标状态
    painter->save();
    painter->setPen(Qt::NoPen); // 边框 不设置 画笔

    // 绘画路径
    QPainterPath abovePath;
    QPainterPath belowPath;

    // 判断 按钮风格
    if(m_buttonStyle == TickButtonStyle::rectButton) // 矩形按钮
    {
        // 获取矩形按钮 尺寸
        QRect rectButtonRect(0, 0, width(), height());

        // 绘制 上部分背景
        // 移动到 左上角圆弧  与 上方直线 相切的点
        abovePath.moveTo(m_rectRadius, rectButtonRect.top());
        // 绘制 左上角圆弧，在一个正方形内，从90°开始，逆时针绘制90°的圆弧
        abovePath.arcTo(QRect(rectButtonRect.left(), rectButtonRect.top(), m_rectRadius, m_rectRadius), 90, 90);
        // 绘制 左侧直线
        abovePath.lineTo(rectButtonRect.left(),rectButtonRect.top()+rectButtonRect.height()/2);
        // 绘制 下方直线
        abovePath.lineTo(rectButtonRect.right(),rectButtonRect.top()+rectButtonRect.height()/2);
        // 绘制 右侧直线
        abovePath.lineTo(rectButtonRect.right(),rectButtonRect.top()-m_rectRadius);
        // 绘制 右上角圆弧，在一个正方形内，从0°开始，逆时针绘制90°的圆弧
        abovePath.arcTo(QRectF(rectButtonRect.right()-m_rectRadius, rectButtonRect.top(), m_rectRadius, m_rectRadius), 0, 90);
        // 绘制 上方直线
        abovePath.lineTo(rectButtonRect.left()+m_rectRadius,rectButtonRect.top());

        // 绘制 下部分背景
        // 移动到 左上角
        belowPath.moveTo(rectButtonRect.left(), rectButtonRect.top()+rectButtonRect.height()/2);
        // 绘制 左侧直线
        belowPath.lineTo(rectButtonRect.left(),rectButtonRect.bottom()-m_rectRadius);
        // 绘制 左下角圆弧，在一个正方形内，从180°开始，逆时针绘制90°的圆弧
        belowPath.arcTo(QRect(rectButtonRect.left(), rectButtonRect.bottom()-m_rectRadius, m_rectRadius, m_rectRadius), 180, 90);
        // 绘制 下方直线
        belowPath.lineTo(rectButtonRect.right()-m_rectRadius,rectButtonRect.bottom());
        // 绘制 右下角圆弧，在一个正方形内，从270°开始，逆时针绘制90°的圆弧
        belowPath.arcTo(QRectF(rectButtonRect.right()-m_rectRadius, rectButtonRect.bottom()-m_rectRadius, m_rectRadius, m_rectRadius), 270, 90);
        // 绘制 右侧直线
        belowPath.lineTo(rectButtonRect.right(),rectButtonRect.top()+rectButtonRect.height()/2);
        // 绘制 上方直线
        belowPath.lineTo(rectButtonRect.left(),rectButtonRect.top()+rectButtonRect.height()/2);

    }
    else if(m_buttonStyle == TickButtonStyle::circleButton) // 圆形 按钮
    {
        // 获取 较小的边
        int minEdge = this->width() > this->height() ? this->height() : this->width();

        int x = this->rect().left() + (this->width() - minEdge)/2;
        // 获取按钮 尺寸，形状为 正方形
        QRect circleButtonRect(x, 0, minEdge, minEdge);

        // 绘制 上部分背景
        // 移动到 最右侧的点
        abovePath.moveTo(circleButtonRect.right(), circleButtonRect.top()+circleButtonRect.height()/2);
        // 绘制 上方圆弧，在一个正方形内，从0°开始，逆时针绘制180°的圆弧
        abovePath.arcTo(circleButtonRect, 0, 180);
        // 绘制 下方直线
        abovePath.lineTo(circleButtonRect.right(), circleButtonRect.top()+circleButtonRect.height()/2);

        // 绘制 下部分背景
        // 移动到 最左侧的点
        belowPath.moveTo(circleButtonRect.left(), circleButtonRect.top()+circleButtonRect.height()/2);
        // 绘制 下方圆弧，在一个正方形内，从180°开始，逆时针绘制180°的圆弧
        belowPath.arcTo(circleButtonRect, 180, 180);
        // 绘制 上方直线
        belowPath.lineTo(circleButtonRect.left(), circleButtonRect.top()+circleButtonRect.height()/2);
    }

    // 绘制 上部分背景
    painter->fillPath(abovePath,QBrush(m_aboveColor));
    // 绘制 下部分背景
    painter->fillPath(belowPath,QBrush(m_belowColor));

    // 恢复 画家类对象 上一次保存的坐标状态
    painter->restore();
}
// 绘制 图片
void TickButton::drawImage(QPainter *painter)
{
    // 保存 画家类对象 当前坐标状态
    painter->save();

    // 原始图片
    QPixmap pixImage = QPixmap(m_imagePath);

    // 获取按钮 尺寸
    QRect outRect;
    QPainterPath clipPath; // 裁剪路径，用于制作圆角图片

    // 判断 按钮风格
    if(m_buttonStyle == TickButtonStyle::rectImageButton)
    {
        outRect = QRect(this->rect().left(), this->rect().top(), width(), height());

        // 绘制 裁剪区路径
        // 移动到 左上角圆弧  与 上方直线 相切的点
        clipPath.moveTo(outRect.left()+m_rectRadius, outRect.top());
        // 绘制 左上角圆弧，在一个正方形内，从90°开始，逆时针绘制90°的圆弧
        clipPath.arcTo(QRect(outRect.left(), outRect.top(), m_rectRadius, m_rectRadius), 90, 90);
        // 绘制 左侧直线
        clipPath.lineTo(outRect.left(),outRect.bottom()-m_rectRadius);
        // 绘制 左下角圆弧
        clipPath.arcTo(QRect(outRect.left(), outRect.bottom()-m_rectRadius, m_rectRadius, m_rectRadius), 180, 90);
        // 绘制 下方直线
        clipPath.lineTo(outRect.right()-m_rectRadius,outRect.bottom());
        // 绘制 右下角圆弧
        clipPath.arcTo(QRect(outRect.right()-m_rectRadius, outRect.bottom()-m_rectRadius, m_rectRadius, m_rectRadius), 270, 90);
        // 绘制 右侧直线
        clipPath.lineTo(outRect.right(),outRect.top()+m_rectRadius);
        // 绘制 右上角圆弧，在一个正方形内，从0°开始，逆时针绘制90°的圆弧
        clipPath.arcTo(QRectF(outRect.right()-m_rectRadius, outRect.top(), m_rectRadius, m_rectRadius), 0, 90);
        // 绘制 上方直线
        clipPath.lineTo(outRect.left()+m_rectRadius,outRect.top());
    }
    else if(m_buttonStyle == TickButtonStyle::circleImageButton)
    {
        // 获取 较小的边
        int minEdge = this->width() > this->height() ? this->height() : this->width();
        int offset_X = (this->width() - minEdge)/2;
        // 获取按钮 尺寸，形状为 正方形
        outRect = QRect(this->rect().left()+offset_X, this->rect().top(), minEdge, minEdge);

        // 移到 外圈最左侧的点
        clipPath.moveTo(outRect.left(),outRect.top()+outRect.height()/2);
        // 绘制 外圈圆弧，在一个正方形内，从180°开始，逆时针绘制360°的圆弧
        clipPath.arcTo(outRect,180,360);
    }
    // 裁剪并缩放图片
    pixImage = scalePixmapFillRect(pixImage,outRect);
    // 设置 裁剪路径（裁剪掉外围）
    painter->setClipPath(clipPath);
    // 绘制 图片
    painter->drawPixmap(outRect, pixImage);

    // 恢复 画家类对象 上一次保存的坐标状态
    painter->restore();
}
// 绘制 勾号
void TickButton::drawTick(QPainter *painter)
{
    // 保存 画家类对象 当前坐标状态
    painter->save();
    painter->setPen(Qt::NoPen); // 边框 不设置 画笔

    // 获取 较小的边
    int minWidgetEdge = this->width() > this->height() ? this->height() : this->width();
    int tickEdge = minWidgetEdge / 3;

    QRect tickSquareRect(this->rect().center().x()-tickEdge/2,this->rect().center().y()-tickEdge/2,tickEdge,tickEdge);

    // 勾号 6点
    QPoint p0(tickSquareRect.left()+ tickEdge*0.146, tickSquareRect.top()+tickEdge*0.488);
    QPoint p1(tickSquareRect.left()+ tickEdge*0.401, tickSquareRect.top()+tickEdge*0.743);
    QPoint p2(tickSquareRect.left()+ tickEdge*0.854, tickSquareRect.top()+tickEdge*0.290);
    QPoint p3(tickSquareRect.left()+ tickEdge*0.783, tickSquareRect.top()+tickEdge*0.220);
    QPoint p4(tickSquareRect.left()+ tickEdge*0.401, tickSquareRect.top()+tickEdge*0.602);
    QPoint p5(tickSquareRect.left()+ tickEdge*0.217, tickSquareRect.top()+tickEdge*0.418);

    // 绘画路径
    QPainterPath tickPath; // 用于绘制  勾号
    QPainterPath path; // 用于绘制圆圈 与 勾号之间的区域
    // 绘制 勾号
    tickPath.moveTo(p0);
    tickPath.lineTo(p1);
    tickPath.lineTo(p2);
    tickPath.lineTo(p3);
    tickPath.lineTo(p4);
    tickPath.lineTo(p5);
    tickPath.closeSubpath(); // 闭合路径，第一个点和最后一个点 相连

    path.addEllipse(tickSquareRect); // 添加 外圆
    path.addPath(tickPath); // 添加 勾号

    painter->fillPath(path,QBrush(m_borderColor)); // 绘制圆圈 与 勾号之间的区域

    // 恢复 画家类对象 上一次保存的坐标状态
    painter->restore();
}
// 绘制 边框
void TickButton::drawBorder(QPainter *painter)
{
    // 保存 画家类对象 当前坐标状态
    painter->save();
    painter->setPen(Qt::NoPen); // 边框 不设置 画笔

    // 绘画路径
    QPainterPath borderPath; // 用于绘制 边框
    QPainterPath outBorderPath;
    QPainterPath inBorderPath;

    // 判断 按钮风格
    if(m_buttonStyle == TickButtonStyle::rectButton || m_buttonStyle == TickButtonStyle::rectImageButton) // 矩形按钮
    {
        // 获取按钮 尺寸
        QRect outRect(0, 0, width(), height());
        QRect inRect(m_borderWidth, m_borderWidth, (width()-m_borderWidth*2), (height()-m_borderWidth*2));
        int inRectRadius = m_rectRadius-m_borderWidth; // 内圈圆角半径
        inRectRadius = inRectRadius > 0? inRectRadius:0; // 确保 内圈圆角半径 大于0

        // 1. 直接 画 圆角矩形 有bug，不会完全贴合背景
        // borderPath.addRoundedRect(outRectWidget,m_rectRadius,m_rectRadius);
        // borderPath.addRoundedRect(QRect(outRectWidget.left()+m_borderWidth, outRectWidget.top()+m_borderWidth,
        //                               outRectWidget.width()-m_borderWidth*2,outRectWidget.height()-m_borderWidth*2),
        //                           m_rectRadius,m_rectRadius);

        // 2. 绘制 外圈
        // 移动到 左上角圆弧  与 上方直线 相切的点
        outBorderPath.moveTo(outRect.left()+m_rectRadius, outRect.top());
        // 绘制 左上角圆弧，在一个正方形内，从90°开始，逆时针绘制90°的圆弧
        outBorderPath.arcTo(QRect(outRect.left(), outRect.top(), m_rectRadius, m_rectRadius), 90, 90);
        // 绘制 左侧直线
        outBorderPath.lineTo(outRect.left(),outRect.bottom()-m_rectRadius);
        // 绘制 左下角圆弧
        outBorderPath.arcTo(QRect(outRect.left(), outRect.bottom()-m_rectRadius, m_rectRadius, m_rectRadius), 180, 90);
        // 绘制 下方直线
        outBorderPath.lineTo(outRect.right()-m_rectRadius,outRect.bottom());
        // 绘制 右下角圆弧
        outBorderPath.arcTo(QRect(outRect.right()-m_rectRadius, outRect.bottom()-m_rectRadius, m_rectRadius, m_rectRadius), 270, 90);
        // 绘制 右侧直线
        outBorderPath.lineTo(outRect.right(),outRect.top()+m_rectRadius);
        // 绘制 右上角圆弧，在一个正方形内，从0°开始，逆时针绘制90°的圆弧
        outBorderPath.arcTo(QRectF(outRect.right()-m_rectRadius, outRect.top(), m_rectRadius, m_rectRadius), 0, 90);
        // 绘制 上方直线
        outBorderPath.lineTo(outRect.left()+m_rectRadius,outRect.top());

        // 绘制 内圈
        // 移动到 左上角圆弧  与 上方直线 相切的点
        inBorderPath.moveTo(inRect.left()+inRectRadius, inRect.top());
        // 绘制 左上角圆弧，在一个正方形内，从90°开始，逆时针绘制90°的圆弧
        inBorderPath.arcTo(QRect(inRect.left(), inRect.top(), inRectRadius, inRectRadius), 90, 90);
        // 绘制 左侧直线
        inBorderPath.lineTo(inRect.left(),inRect.bottom()-inRectRadius);
        // 绘制 左下角圆弧
        inBorderPath.arcTo(QRect(inRect.left(), inRect.bottom()-inRectRadius, inRectRadius, inRectRadius), 180, 90);
        // 绘制 下方直线
        inBorderPath.lineTo(inRect.right()-inRectRadius,inRect.bottom());
        // 绘制 右下角圆弧
        inBorderPath.arcTo(QRect(inRect.right()-inRectRadius, inRect.bottom()-inRectRadius, inRectRadius, inRectRadius), 270, 90);
        // 绘制 右侧直线
        inBorderPath.lineTo(inRect.right(),inRect.top()+inRectRadius);
        // 绘制 右上角圆弧，在一个正方形内，从0°开始，逆时针绘制90°的圆弧
        inBorderPath.arcTo(QRectF(inRect.right()-inRectRadius, inRect.top(), inRectRadius, inRectRadius), 0, 90);
        // 绘制 上方直线
        inBorderPath.lineTo(inRect.left()+inRectRadius,inRect.top());
    }
    else if(m_buttonStyle == TickButtonStyle::circleButton || m_buttonStyle == TickButtonStyle::circleImageButton) // 圆形 按钮
    {
        // 获取 较小的边
        int minEdge = this->width() > this->height() ? this->height() : this->width();

        int x = this->rect().left() + (this->width() - minEdge)/2;
        // 获取按钮 尺寸，形状为 正方形
        QRect outRect(x, 0, minEdge, minEdge);
        QRect inRect(x+m_borderWidth, m_borderWidth, minEdge-m_borderWidth*2, minEdge-m_borderWidth*2);

        // 移到 外圈最左侧的点
        outBorderPath.moveTo(outRect.left(),outRect.top()+outRect.height()/2);
        // 绘制 外圈圆弧，在一个正方形内，从180°开始，逆时针绘制360°的圆弧
        outBorderPath.arcTo(outRect,180,360);

        // 移到 内圈最左侧的点
        outBorderPath.moveTo(inRect.left(),inRect.top()+inRect.height()/2);
        // 绘制 内圈圆弧，在一个正方形内，从180°开始，逆时针绘制360°的圆弧
        outBorderPath.arcTo(inRect,180,360);
    }

    // 将 外圈、内圈添加到路径
    borderPath.addPath(outBorderPath);
    borderPath.addPath(inBorderPath);

    // 绘制 边框
    painter->fillPath(borderPath,QBrush(m_borderColor));

    // 恢复 画家类对象 上一次保存的坐标状态
    painter->restore();
}
// 缩放图片填充矩形
QPixmap TickButton::scalePixmapFillRect(QPixmap pix, QRect rect)
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

    // 缩放后的图片
    QPixmap scaledPix;

    // 图片原始尺寸
    int pixWidth = pix.width();
    int pixHeight = pix.height();

    // 矩形缩放后尺寸
    int rectScaledWidth = pixWidth;
    int rectScaledHeight = pixHeight;

    // 缩放比
    double scaledRate;

    // 在原始图片上进行裁剪的起始坐标
    int start_X = 0;
    int start_Y = 0;

    // 面积法
    if (pixWidth * rect.height() > pixHeight * rect.width())
    {
        // 此时，按宽度对齐缩放能全部显示图片，但不能完全填充矩形
        // 故应该按高度对齐，此时缩放后图片宽度大于矩形，需进行裁剪
        scaledRate = double(rect.height()) / double(pixHeight); // 缩放比

        rectScaledWidth = int(rect.width() / scaledRate);

        start_X = (pixWidth - rectScaledWidth)/2;
    }
    else
    {
        // 此时，按高度对齐缩放能全部显示图片，但不能完全填充矩形
        // 故应该按宽度对齐，此时缩放后图片高度大于矩形，需进行裁剪
        scaledRate = double(rect.width()) / double(pixWidth); // 缩放比

        rectScaledHeight = int(rect.height() / scaledRate);

        start_Y = (pixHeight - rectScaledHeight)/2;
    }

    // 进行 裁剪，截取中间
    scaledPix = pix.copy(start_X, start_Y, rectScaledWidth, rectScaledHeight);

    // 进行 缩放，缩放到 填充矩形
    scaledPix = scaledPix.scaled(QSize(rect.width()*m_pixelRatio, rect.height()*m_pixelRatio),
                                 Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    // 返回 裁剪后的图片
    return scaledPix;
}

// 鼠标按下事件
void TickButton::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event); // 不使用 事件

    m_checked = !m_checked;

    emit themeButton_clicked(); // 发送 按钮状态 改变信号
    emit themeButton_clicked(m_checked);

    update();

    QWidget::mousePressEvent(event);
}
// 绘画事件
void TickButton::paintEvent(QPaintEvent *event)
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

    if(m_checked) // 被 勾选
    {
        drawBorder(&painter); // 绘制 边框
        drawTick(&painter); // 绘制 勾号
    }

}
