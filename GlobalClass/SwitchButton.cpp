#include "SwitchButton.h"

// 构造函数
SwitchButton::SwitchButton(QWidget *parent)
    : QWidget{parent}
{
    // 初始化
    initMemberVariable();
    initLayout();
    initConnect();
}

// 初始化 成员函数
void SwitchButton::initMemberVariable()
{
    // 初始化 值
    m_checked = false; // 开关按钮状态 默认为 关闭状态
    m_buttonStyle = SwitchButtonStyle::rectButton; // 开关按钮风格 默认为 矩形按钮

    m_grooveColorOff = QColor(225, 225, 225); // 关闭时，滑槽背景颜色 默认为
    m_grooveColorOn = QColor(250, 250, 250); // 打开时，滑槽背景颜色 默认为

    m_sliderColorOff = QColor(100, 100, 100); // 关闭时，滑块颜色 默认为
    m_sliderColorOn = QColor(100, 184, 255); // 打开时，滑块颜色 默认为

    m_textColorOff = QColor(255, 255, 255); // 关闭时，文字颜色 默认为
    m_textColorOn = QColor(10, 10, 10); // 打开时，文字颜色 默认为

    m_textOff = ""; // 关闭时，按钮的文字 默认为
    m_textOn = ""; // 打开时，按钮的文字 默认为

    m_imageOffPath = ""; // 关闭时，按钮的背景图片 默认为空
    m_imageOnPath = ""; // 打开时，按钮的背景图片 默认为空

    m_space = 2; // 滑块离槽的间隔 默认为 2
    m_rectRadius = 5; // 圆角角度

    m_step = (this->width() / 10) > 1? (this->width() / 10):1; // 每次移动的步长 默认为 按钮宽度的1/10
    m_startX = 0; // 滑块左侧 开始X轴坐标 默认为 0
    m_endX = 0; //滑块左侧 结束X轴坐标 默认为 0

    m_timer = new QTimer(this); // 新建 定时器
    m_timer->setInterval(5); //设置定时器时间间隔 为 5ms

    this->setFont(QFont("Microsoft Yahei", 10));
}
// 初始化 布局
void SwitchButton::initLayout()
{

}
// 初始化 绑定事件
void SwitchButton::initConnect()
{
    // 绑定 定时器 到时信号
    connect(m_timer, &QTimer::timeout, this, &SwitchButton::do_slider_update);
}

//设置 和 获取 滑槽颜色
void SwitchButton::setGrooveColorOff(QColor grooveColorOff)
{
    //设置 关闭状态的 滑槽颜色
    m_grooveColorOff = grooveColorOff;
    // 更新
    update();
}
void SwitchButton::setGrooveColorOn(QColor grooveColorOn)
{
    //设置 打开状态的 滑槽颜色
    m_grooveColorOn = grooveColorOn;
    // 更新
    update();
}
QColor SwitchButton::getGrooveColorOff() const
{
    return m_grooveColorOff;
}
QColor SwitchButton::getGrooveColorOn() const
{
    return m_grooveColorOn;
}

//设置 和 获取 滑块颜色
void SwitchButton::setSliderColorOff(QColor sliderColorOff)
{
    //设置 关闭状态的 滑块颜色
    m_sliderColorOff = sliderColorOff;
    // 更新
    update();
}
void SwitchButton::setSliderColorOn(QColor sliderColorOn)
{
    //设置 打开状态的 滑块颜色
    m_sliderColorOn = sliderColorOn;
    // 更新
    update();
}
QColor SwitchButton::getSliderColorOff()const
{
    return m_sliderColorOff;
}
QColor SwitchButton::getSliderColorOn()const
{
    return m_sliderColorOn;
}

//设置 和 获取 文本颜色
void SwitchButton::setTextColorOff(QColor textColorOff)
{
    //设置 关闭状态的 文字颜色
    m_textColorOff = textColorOff;
    // 更新
    update();
}
void SwitchButton::setTextColorOn(QColor textColorOn)
{
    //设置 打开状态的 文字颜色
    m_textColorOn = textColorOn;
    // 更新
    update();
}
QColor SwitchButton::getTextColorOff() const
{
    return m_textColorOff;
}
QColor SwitchButton::getTextColorOn() const
{
    return m_textColorOn;
}

//设置 和 获取 是否选中
void SwitchButton::setChecked(bool checked)
{
    // 当前 开关按钮状态 不等于 要设置的 状态
    if (this->m_checked != checked)
    {
        this->m_checked = checked; // 设置新状态
        emit checkedChanged(checked); // 发送 状态 改变信号
        // 更新
        update();
    }
}
bool SwitchButton::getChecked() const
{
    return m_checked;
}

//设置 和 获取 风格样式
void SwitchButton::setButtonStyle(SwitchButtonStyle buttonStyle)
{
    // 设置 开关按钮 风格
    m_buttonStyle = buttonStyle;
    // 更新
    update();
}
SwitchButtonStyle SwitchButton::getButtonStyle() const
{
    return m_buttonStyle;
}

//设置 和 获取 槽和滑块的间隔
void SwitchButton::setSpace(int space)
{
    //设置 间隔
    m_space = space;
    // 更新
    update();
}
int SwitchButton::getSpace() const
{
    return m_space;
}

//设置 和 获取 圆角半径
void SwitchButton::setRectRadius(int rectRadius)
{
    //设置 圆角半径
    m_rectRadius = rectRadius;
    // 更新
    update();
}
int SwitchButton::getRectRadius() const
{
    return m_rectRadius;
}

//设置 和 获取 文本
void SwitchButton::setText(QString textOff, QString textOn)
{
    //设置 关闭和打开状态的 文字
    m_textOff = textOff;
    m_textOn = textOn;
    // 更新
    update();
}
void SwitchButton::setTextOff(QString textOff)
{
    //设置 关闭和打开状态的 文字
    m_textOff = textOff;
    // 更新
    update();
}
void SwitchButton::setTextOn(QString textOn)
{
    //设置 关闭和打开状态的 文字
    m_textOn = textOn;
    // 更新
    update();
}
QString SwitchButton::getTextOff() const
{
    return m_textOff;
}
QString SwitchButton::getTextOn() const
{
    return m_textOn;
}

//设置 和 获取 背景图片
void SwitchButton::setImagePath(QString imageOffPath, QString imageOnPath)
{
    //设置 关闭和打开状态的 背景图片
    m_imageOffPath = imageOffPath;
    m_imageOnPath = imageOnPath;
    // 更新
    update();
}
void SwitchButton::setImageOffPath(QString imageOffPath)
{
    //设置 关闭和打开状态的 背景图片
    m_imageOffPath = imageOffPath;
    // 更新
    update();
}
void SwitchButton::setImageOnPath(QString imageOnPath)
{
    //设置 关闭和打开状态的 背景图片
    m_imageOnPath = imageOnPath;
    // 更新
    update();
}
QString SwitchButton::getImageOffPath() const
{
    return m_imageOffPath;
}
QString SwitchButton::getImageOnPath() const
{
    return m_imageOnPath;
}

// 绘制 滑槽
void SwitchButton::drawGroove(QPainter *painter)
{
    // 保存 画家类对象 当前坐标状态
    painter->save();
    painter->setPen(Qt::NoPen); // 边框 不设置 画笔

    if (!m_checked) // 当前未 按下，即关闭状态
    {
        // 设置 画刷颜色（填充颜色）为 关闭时滑槽的颜色
        painter->setBrush(m_grooveColorOff);
    }
    else // 当前已按下，即打开状态
    {
        // 设置 画刷颜色（填充颜色）为 打开时滑槽的颜色
        painter->setBrush(m_grooveColorOn);
    }

    // 判断 按钮 风格
    if (m_buttonStyle == SwitchButtonStyle::rectButton) // 矩形按钮
    {
        // 获取 控件尺寸，绘制圆角矩形的 滑槽
        painter->drawRoundedRect(rect(), m_rectRadius, m_rectRadius);
    }
    else if (m_buttonStyle == SwitchButtonStyle::circleInButton) // 内圆形按钮
    {
        // 滑槽的尺寸
        QRect rect(0, 0, width(), height());
        // 圆角半径为 高度的一半
        int radius = rect.height() / 2;
        // 圆的宽度为 高度
        int circleWidth = rect.height();

        // 绘画路径
        QPainterPath path;
        // 移动到 左侧的圆弧 与 上侧直线相切的点
        path.moveTo(radius, rect.top());
        // 在正方形内 绘制左侧圆弧，起始角度为90度，逆时针绘制180度（从圆心上方开始，经过圆心左侧，到圆心下方结束）
        path.arcTo(QRectF(rect.left(), rect.top(), circleWidth, circleWidth), 90, 180);
        // 绘制底部的直线，到右侧与圆弧相交的点
        path.lineTo(rect.width() - radius, rect.height());
        // 绘制 右侧圆弧
        path.arcTo(QRectF(rect.width() - rect.height(), rect.top(), circleWidth, circleWidth), 270, 180);
        // 绘制 顶部的直线
        path.lineTo(radius, rect.top());

        // 绘制 滑槽
        painter->drawPath(path);
    }
    else if(m_buttonStyle == SwitchButtonStyle::circleOutButton) // 外圆形按钮
    {
        // 滑槽的尺寸 为 控件尺寸 向内 偏移 m_space
        QRect rect(m_space, m_space, width() - m_space * 2, height() - m_space * 2);
        // 绘制 圆角矩形
        painter->drawRoundedRect(rect, m_rectRadius, m_rectRadius);
    }

    // 恢复 画家类对象 上一次保存的坐标状态
    painter->restore();
}
// 绘制 滑块
void SwitchButton::drawSlider(QPainter *painter)
{
    // 保存 画家类对象 当前坐标状态
    painter->save();
    painter->setPen(Qt::NoPen); // 边框 不设置 画笔

    if (!m_checked) // 当前未 按下，即关闭状态
    {
        // 设置 画刷颜色（填充颜色）为 关闭时滑块的颜色
        painter->setBrush(m_sliderColorOff);
    }
    else // 当前已按下，即打开状态
    {
        // 设置 画刷颜色（填充颜色）为 打开时滑块的颜色
        painter->setBrush(m_sliderColorOn);
    }

    // 判断 按钮 风格
    if (m_buttonStyle == SwitchButtonStyle::rectButton) // 矩形按钮
    {
        // 矩形滑块 的 宽高
        int sliderWidth = width() / 2 - m_space * 2; // 滑块的宽度为 控件的宽度一半，并减去左右间隔
        int sliderHeight = height() - m_space * 2; // 滑块的宽度为 控件的高度，并减去上下间隔
        // 滑块左侧为 滑块开始坐标向左偏移一个间距
        QRect sliderRect(m_startX + m_space, m_space, sliderWidth , sliderHeight);
        // 绘制 圆角矩形
        painter->drawRoundedRect(sliderRect, m_rectRadius, m_rectRadius);
    }
    else if (m_buttonStyle == SwitchButtonStyle::circleInButton) // 内圆形按钮
    {
        // 圆形滑块
        QRect rect(0, 0, width(), height()); // 控件的尺寸
        // 圆形滑块的直径 为 控件的高度减去上下间隔
        int sliderWidth = rect.height() - m_space * 2;
        // 圆形滑块 所在 的正方形
        QRect sliderRect(m_startX + m_space, m_space, sliderWidth, sliderWidth);
        // 绘制圆形
        painter->drawEllipse(sliderRect);
    }
    else if (m_buttonStyle == SwitchButtonStyle::circleOutButton) // 外圆形按钮
    {
        // 圆形滑块，但超出滑槽
        // 控件尺寸 向内 偏移m_space
        QRect rect(0, 0, width() - m_space, height() - m_space);
        // 滑块直径 为 向外矩形的高度
        int sliderWidth = rect.height();
        // 圆形滑块 所在 正方形
        QRect sliderRect(m_startX, m_space / 2, sliderWidth, sliderWidth);
        // 绘制 圆形滑块
        painter->drawEllipse(sliderRect);
    }

    // 恢复 画家类对象 上一次保存的坐标状态
    painter->restore();
}
// 绘制 文字
void SwitchButton::drawText(QPainter *painter)
{
    // 有问题

    // 保存 画家类对象 当前坐标状态
    painter->save();

    if (!m_checked) // 当前未 按下，即关闭状态
    {
        // 设置 画笔颜色 为 关闭状态的文字颜色
        painter->setPen(m_textColorOff);
        // 绘制 文字
        painter->drawText(width() / 2, 0, width() / 2 - m_space, height(), Qt::AlignCenter, m_textOff);
    }
    else // 当前已按下，即打开状态
    {
        // 设置 画笔颜色 为 打开状态的文字颜色
        painter->setPen(m_textColorOn);
        // 绘制 文字
        painter->drawText(0, 0, width() / 2 + m_space * 2, height(), Qt::AlignCenter, m_textOn);
    }

    // 恢复 画家类对象 上一次保存的坐标状态
    painter->restore();
}
// 绘制 图片
void SwitchButton::drawImage(QPainter *painter)
{
    // 保存 画家类对象 当前坐标状态
    painter->save();

    // 图画对象
    QPixmap pix;

    if (!m_checked) // 当前未 按下，即关闭状态
    {
        // 设置 图画 为 关闭状态的图画
        pix = QPixmap(m_imageOffPath);
    }
    else // 当前已按下，即打开状态
    {
        // 设置 图画 为 打开状态的图画
        pix = QPixmap(m_imageOnPath);
    }

    pix = scalePixmapFillRect(pix, this->rect());

    // 绘制 图画
    painter->drawPixmap(this->rect(), pix);

    // 恢复 画家类对象 上一次保存的坐标状态
    painter->restore();
}
// 缩放图片填充矩形
QPixmap SwitchButton::scalePixmapFillRect(QPixmap pix, QRect rect)
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
void SwitchButton::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event); // 不使用 事件

    m_checked = !m_checked; // 按钮状态 取反
    emit checkedChanged(m_checked); // 发送 按钮状态 改变信号

    //每次移动的步长为宽度的 50分之一
    m_step = width() / 50;

    //状态切换改变后自动计算终点坐标
    if (m_checked) // 按钮 处于打开 状态
    {
        if (m_buttonStyle == SwitchButtonStyle::rectButton) // 矩形按钮
        {
            // 矩形按钮 滑块宽度为按钮长度的一半，结束时 滑块左侧 X坐标为按钮长度的一半
            m_endX = width() - width() / 2;
        }
        else if (m_buttonStyle == SwitchButtonStyle::circleInButton) // 内圆形按钮
        {
            // 内圆形按钮 按钮的圆角半径为高度的一半，滑块左侧 X坐标为按钮长度 减去 整个滑块直径
            m_endX = width() - height();
        }
        else if (m_buttonStyle == SwitchButtonStyle::circleOutButton) // 外圆形按钮
        {
            // 外圆形按钮 按钮的圆角半径为高度的一半，滑块左侧 X坐标为按钮长度 减去 整个滑块直径
            // 另外 滑块的超出槽一定长度（m_space），故还要再加上 m_space
            m_endX = width() - height() + m_space;
        }
    }
    else // 按钮 处于关闭 状态
    {
        // 结束X坐标 为 0（与开始坐标重合）
        m_endX = 0;
    }

    // 起点 定时器
    m_timer->start();
}
// 绘画事件
void SwitchButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event); // 不使用 事件
    //绘制准备工作,启用反锯齿
    QPainter painter(this); // 画家类 对象
    painter.setRenderHint(QPainter::Antialiasing); // 设置 抗锯齿

    m_pixelRatio = painter.device()->devicePixelRatioF(); // 获取不同显示器的分辨率比例，防止不同分辨率下图片失真

    // 判断 按钮 风格
    if (m_buttonStyle == SwitchButtonStyle::imageButton) // 图片 开关按钮
    {
        //绘制图片
        drawImage(&painter);
    }
    else
    {
        //绘制滑槽
        drawGroove(&painter);
        //绘制滑块
        drawSlider(&painter);
        //绘制文字
        drawText(&painter);
    }
}
// 尺寸改变事件
void SwitchButton::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event); // 不使用 事件
    //每次移动的步长为宽度的 50分之一
    m_step = width() / 50;

    //尺寸大小改变后 自动将 终点位置 设置 起点坐标
    if (m_checked) // 按钮 处于打开 状态
    {
        if (m_buttonStyle == SwitchButtonStyle::rectButton) // 矩形按钮
        {
            m_startX = width() - width() / 2;
        }
        else if (m_buttonStyle == SwitchButtonStyle::circleInButton) // 内圆形按钮
        {
            m_startX = width() - height();
        }
        else if (m_buttonStyle == SwitchButtonStyle::circleOutButton) // 外圆形按钮
        {
            m_startX = width() - height() + m_space;
        }
    }
    else // 按钮 处于关闭 状态
    {
        m_startX = 0;
    }

    update();
}

// 定时器到时 更新 滑块的起点和终点坐标值
void SwitchButton::do_slider_update()
{
    if (m_checked) // 当前 已按下，即打开状态
    {
        if (m_startX < m_endX) // 滑块 未向右移动到 终点
        {
            // 更新 起点，向右移动一步
            m_startX = m_startX + m_step;
        }
        else // 滑块 移动到 终点
        {
            // 设置 起点 等于 终点
            m_startX = m_endX;
            // 定时器 停止
            m_timer->stop();
        }
    }
    else // 当前 未按下，即关闭状态
    {
        if (m_startX > m_endX) // 滑块 未向左移动到 终点
        {
            // 更新 起点，向左移动一步
            m_startX = m_startX - m_step;
        }
        else // 滑块 移动到 终点
        {
            // 设置 起点 等于 终点
            m_startX = m_endX;
            // 定时器 停止
            m_timer->stop();
        }
    }

    // 更新
    update();
}


