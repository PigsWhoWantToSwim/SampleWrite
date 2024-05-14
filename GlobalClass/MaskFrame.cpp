#include "maskframe.h"

MaskFrame::MaskFrame(QWidget *parent): QWidget{parent},m_maskedWidget(nullptr),m_upperWidget(nullptr)
{
    initWidgets();

}

void MaskFrame::initWidgets()
{
    /*
    Qt::FramelessWindowHint
    1.产生一个无边框的窗口，无法移动和改变大小。
    Qt::Tool
    1.工具窗口，如果有父窗口，则工具窗口将始终保留在它的顶部。
    2.默认情况下，当应用程序处于非活动状态时，工具窗口则消失。
    */
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    setMaskColor(QColor(0,0,0),0.6f);

    //1.返回具有键盘输入焦点的顶级窗口,如果没有应用程序窗口具有焦点，则返回0.
    //2.但是即使没有焦点，也可能有一个活动窗口。例如一个窗口没有小部件接收关键事件。
    m_maskedWidget = QApplication::activeWindow();
}

void MaskFrame::setUpperWidget(QWidget *upperWidget)
{
    if(upperWidget == nullptr)
        return;

    // 监视窗口显示事件
    upperWidget->installEventFilter(this); // 安装事件过滤器
    m_upperWidget = upperWidget;

    connect(m_upperWidget,&QWidget::destroyed,this,[=](){m_upperWidget = nullptr;});
}

void MaskFrame::setMaskedWidget(QWidget *maskedWidget)
{
    if(maskedWidget == nullptr)
        return;

    m_maskedWidget = maskedWidget;
}

void MaskFrame::setMaskColor(const QColor &color, float opacity)
{
    if(!color.isValid())
    {
        return;
    }

    QPalette palette = this->palette();
    //通常指窗口部件的背景色
    palette.setColor(QPalette::Window,color);
    this->setPalette(palette);

    setWindowOpacity(opacity);
}

bool MaskFrame::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == m_upperWidget)
    {
        if(event->type() == QEvent::Show)
        {
            //显示窗口显示，则遮罩显示，this就是遮罩本身
            this->show();
        }

        if(event->type() == QEvent::Hide)
        {
            this->hide();
        }
    }

    return QObject::eventFilter(watched, event);
}

void MaskFrame::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);

    // 获取控件 绝对坐标
    // 将m_maskedWidget (0, 0)处的坐标（即控件左上角）映射为全局坐标
    QPoint maskedWidgetGlobalPos = m_maskedWidget->mapToGlobal(QPoint(0,0));

    //确定显示的位置
    this->setGeometry(maskedWidgetGlobalPos.x(), maskedWidgetGlobalPos.y(), m_maskedWidget->width(), m_maskedWidget->height());
}
