#include "AppDatabase.h"

AppDatabase *AppDatabase::appDatabase = nullptr;

QSqlDatabase AppDatabase::database;
QString AppDatabase::dbPath = QDir::currentPath() + "/Config";
QString AppDatabase::dbFile = "AppDateBase.db3";


AppDatabase::AppDatabase(QObject *parent)
    : QObject{parent}
{
    dbPath = QDir::currentPath() + "/Config";
    dbFile = "AppDateBase.db3";

    //数据库文件目录，不存在则创建
    QDir dir;
    if(!dir.exists(dbPath))//判断路径是否存在
        dir.mkpath(dbPath); //创建路径

    // 创建 数据库文件
    if(openDatabase())
        initDatabase();

    // 创建本地章节文件夹、小说封面文件夹
    if(!dir.exists(dbPath + "/LocalChapters"))
        dir.mkpath(dbPath + "/LocalChapters"); //创建 章节文件夹
    if(!dir.exists(dbPath + "/NovelCover"))
    {
        dir.mkpath(dbPath + "/NovelCover"); //创建 小说封面文件夹
        // 直接复制一份小说默认封面
        QFile::copy(":/image/default book cover.png", dbPath + "/NovelCover/default book cover.png");
    }


}

AppDatabase *AppDatabase::getInstance()
{

    if(appDatabase == nullptr)
    {
        appDatabase = new AppDatabase();
    }

    return appDatabase;

}

void AppDatabase::quit()
{
    if(database.isOpen())
        database.close();
}

bool AppDatabase::openDatabase()
{
    //建立并打开数据库
    if(QSqlDatabase::contains("qt_sql_default_connection"))
        database = QSqlDatabase::database("qt_sql_default_connection");
    else
        database = QSqlDatabase::addDatabase("QSQLITE");

    database.setDatabaseName(dbPath + "/" + dbFile);

    if (!database.open())
        return false;
    else
        return true;
}

bool AppDatabase::initDatabase()
{
    if (database.open())
    {
        QSqlQuery sql_query;

        //创建 novels表
        QString novelsTable_sql = "CREATE TABLE novels ("
                             "novel_id VARCHAR(15) PRIMARY KEY NOT NULL UNIQUE, "
                             "novel_title VARCHAR (128) NOT NULL, "
                             "novel_author VARCHAR (64), "
                             "novel_coverURL VARCHAR (2083), "
                             "novel_type VARCHAE(5), "
                             "volumes_count INT, "
                             "chapters_count INT, "
                             "words_count INT, "
                             "novel_index INT, "
                             "is_deleted INT(1), "
                             "deleted_time VARCHAR(19));";
        sql_query.prepare(novelsTable_sql);
        if(!sql_query.exec())
            return false;

        //创建 volumes表
        QString volumesTable_sql = "CREATE TABLE volumes ("
                                   "volume_id VARCHAR(15) PRIMARY KEY UNIQUE NOT NULL, "
                                   "volume_title VARCHAR(128), "
                                   "novel_id VARCHAR(15) NOT NULL, "
                                   "chapters_count INT, "
                                   "volume_index INT, "
                                   "is_deleted INT(1), "
                                   "deleted_time VARCHAR(19));";
        sql_query.prepare(volumesTable_sql);
        if(!sql_query.exec())
            return false;

        //创建 chapters表
        QString chaptersTable_sql = "CREATE TABLE chapters ("
                                    "chapter_id VARCHAR (15) PRIMARY KEY UNIQUE NOT NULL, "
                                    "chapter_title VARCHAR (128), "
                                    "novel_id VARCHAR(15) NOT NULL, "
                                    "volume_id VARCHAR(15), "
                                    "file_pathURL VARCHAR(2083), "
                                    "words_count INT, "
                                    "chapter_index INT, "
                                    "is_deleted INT(1), "
                                    "deleted_time VARCHAR(19));";
        sql_query.prepare(chaptersTable_sql);
        if(!sql_query.exec())
            return false;

        //创建 backup_chapters表
        QString backupChaptersTable_sql = "CREATE TABLE backup_chapters ("
                                          "version_id VARCHAR(15) PRIMARY KEY UNIQUE NOT NULL, "
                                          "chapter_id VARCHAR(15) NOT NULL, "
                                          "birth_time VARCHAR(19), "
                                          "words_count INT, "
                                          "file_pathURL VARCHAR(2083));";
        sql_query.prepare(backupChaptersTable_sql);
        if(!sql_query.exec())
            return false;

        return true;
    }
    else
        return false;
}
