#ifndef APPDATABASE_H
#define APPDATABASE_H

#include <QObject>
#include <QDir>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QMutex>


class AppDatabase : public QObject
{
    Q_OBJECT
public:
    static AppDatabase* getInstance();

    static void quit();

    bool openDatabase();
    bool initDatabase();


private:

private:
    explicit AppDatabase(QObject *parent = nullptr);
    // AppDatabase();//禁止构造函数
    AppDatabase(const AppDatabase&){}//禁止拷贝构造函数
    AppDatabase & operator=(const AppDatabase &){}//禁止赋值拷贝构造函数

signals:

private:
    static AppDatabase *appDatabase;

    static QSqlDatabase database;
    static QString dbPath;
    static QString dbFile;
    // static QString dbName;
    // static QString dbUser;
    // static QString dbPasswd;

};

#endif // APPDATABASE_H
