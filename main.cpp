// #include "basedframelessmainwindow.h"
#include "FramelessMainWindow/HomeMainWindow.h"
#include "FramelessMainWindow/EditMainWindow.h"

#include <QApplication>
#include <QFile>
#include "GlobalClass/AppEnumClass.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_UseSoftwareOpenGL);
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::Floor);
    /*
     * QApplication::setHighDpiScaleFactorRoundingPolicy是一个Qt框架中的函数，
     * 用于设置高DPI缩放因子的舍入策略。在支持高分辨率显示的环境中，应用程序可能需要适应不同的屏幕密度，
     * 并进行缩放以确保界面元素在各种设备上呈现正确。这个函数可以设置高DPI缩放因子的舍入策略，
     * 以确定如何进行精确计算。
     *
     * 可选的舍入策略包括：
     * Qt::HighDpiScaleFactorRoundingPolicy::PassThrough：将原始缩放因子直接应用于界面元素。
     *
     * Qt::HighDpiScaleFactorRoundingPolicy::RoundPreferFloor：将原始缩放因子四舍五入到最近的整数，
     * 并向下取整。
     *
     * Qt::HighDpiScaleFactorRoundingPolicy::RoundPreferCeil：将原始缩放因子四舍五入到最近的整数，
     * 并向上取整。
     *
     * 通过调用QApplication::setHighDpiScaleFactorRoundingPolicy函数并传递所需的舍入策略参数，
     * 可以指定应用程序在高DPI环境中如何处理缩放因子。
    */
    QApplication a(argc, argv);

    if(!QFile::exists(QDir::currentPath() + "/Config/Ssttings.ini")) // 配置文件不存在
    {
        // 新建 配置文件
        QFile settingsFile(QDir::currentPath() + "/Config/Ssttings.ini"); // 文件操作对象
        settingsFile.open(QIODevice::WriteOnly|QIODevice::Text); // 创建文件
        settingsFile.close(); // 关闭 文件

        // 将 默认qss主题 文件路径 写入配置文件
        saveCurrentTheme(":/theme/default.qss");
    }

    // 检查 settings 是否存在
    if(settings == nullptr)
        settings = new QSettings(QDir::currentPath() + "/Config/Ssttings.ini", QSettings::IniFormat);

    // 读取 配置文件中的 当前主题
    settings->beginGroup("APP_THEME");
    QString currentTheme = settings->value("CURRENT_THEME",":/theme/default.qss").toString();
    settings->endGroup();

    HomeMainWindow w;
    // EditMainWindow w;
    w.setMinimumSize(800,540);
    w.show();

    // QFile defaultQSSFile(":/theme/briar.qss");
    // QFile defaultQSSFile(":/theme/dark.qss");
    // QFile defaultQSSFile(":/theme/default.qss");
    // QFile defaultQSSFile(":/theme/sky blue.qss");

    QFile defaultQSSFile(currentTheme);
    if(defaultQSSFile.open(QFile::ReadOnly))
    {
        a.setStyleSheet(defaultQSSFile.readAll());
    }
    defaultQSSFile.close();

    return a.exec();
}
