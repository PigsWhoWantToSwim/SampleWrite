#include "AppEnumClass.h"

QSettings *settings = nullptr;



/* 将RGB颜色 转变为 带#号 16进制字符串 */
QString converRGB16HexStr(QColor color)
{
    //r的rgb转化为16进制，2位数字，16进制，填充0
    QString redStr = QString("%1").arg(color.red(),2,16,QChar('0'));
    //g的rgb转化为16进制
    QString greenStr = QString("%1").arg(color.green(),2,16,QChar('0'));
    //b的rgb转化为16进制
    QString blueStr = QString("%1").arg(color.blue(),2,16,QChar('0'));
    //将各rgb的拼接在一起形成#000000
    QString hexStr = "#"+redStr+greenStr+blueStr;
    //返回hexStr
    return hexStr;
}






/* 生成 指定长度 的大小写字母字符串 */
QString getRandomLetterString(int length)
{
    QString originalString = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int strLength = originalString.length();


    QString resultString = "";
    int randomNum = 0;

    for(int i = 0; i < length; i++)
    {
        // 随机生成 一个[0, strLength) 之间的 数字
        randomNum = QRandomGenerator::global()->bounded(strLength);
        resultString += originalString.at(randomNum);
    }

    return resultString;
}

/* 设置保存主题 */
void saveCurrentTheme(QString qssFilePath)
{
    if(settings == nullptr)
    {
        settings = new QSettings(QDir::currentPath() + "/Config/Ssttings.ini", QSettings::IniFormat);
    }

    settings->beginGroup("APP_THEME");
    settings->setValue("CURRENT_THEME", qssFilePath);
    settings->endGroup();
}
