/*
Copyright (c) 2006-2009, Tom Thielicke IT Solutions

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301, USA.
*/

/****************************************************************
**
** File name: connection.h
**
****************************************************************/

#ifndef CONNECTION_H
#define CONNECTION_H

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QList>
#include <QMessageBox>
#include <QSettings>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>
#include <QStringList>
#include <QTextStream>

#include "def/defines.h"
#include "def/errordefines.h"
#include "sql/startsql.h"
#include "widget/errormessage.h"

// Database connection to SQLite
static bool createConnection()
{
    // Database exist
    bool dbExist = false;
    // Path do the database
    QString dbPath;
    // Path do the home database
    QString dbHomeTemp;
    QString dbFolderTemp;
    // Filename of the template database
    QString dbNameTemplate = APP_DB;
    // Filename of the user database
    QString dbNameUser = APP_USER_DB;
    // Connection to SQLite
    QSqlDatabase db;
    QList<QString> lessonId;
    QList<QString> lessonLesson;
    QList<QString> lessonTime;
    QList<QString> lessonToken;
    QList<QString> lessonStrokes;
    QList<QString> lessonErrors;
    QList<QString> lessonStamp;
    QList<QString> charUnicode;
    QList<QString> charTarget;
    QList<QString> charMistake;
    QList<QString> charOccur;
    int lessonCounter = 0;
    int charCounter = 0;
    int i;

    // Read user db path from settings, if not exist as argument
#if APP_PORTABLE
    QSettings settings(
        QCoreApplication::applicationDirPath() + "/portable/settings.ini",
        QSettings::IniFormat);
#else
    QSettings settings;
#endif
    settings.beginGroup("database");
    dbPath = settings.value("pathpro", "").toString();
    settings.endGroup();

    // Portable version
    if (APP_PORTABLE && dbPath != "") {

        dbPath = QCoreApplication::applicationDirPath() + "/portable/"
            + dbNameUser;
    }

    // User path specified?
    if (dbPath != "") {
        // User path specified
        if (QFile::exists(dbPath)) {
            // User path and file exist
            dbExist = true;
        } else {
            // User file lost?
            // -> error message
            /*ErrorMessage *errorMessage = new ErrorMessage();
            errorMessage->showMessage(ERR_SQL_DB_USER_EXIST,
            ErrorMessage::Type::Info, ErrorMessage::Cancel::No, "Betroffener
            Pfad:\n" + dbPath);*/
            // Try to create new databae in user path
            // Exist a database in the program dir?
#if APP_PORTABLE
            QString dbTemplatePath = QCoreApplication::applicationDirPath()
                + dbNameTemplate;
#else
            QString dbTemplatePath =  INSTALLPREFIX "/share/tipp10/" + dbNameTemplate;
#endif

            if (QFile::exists(dbTemplatePath)) {
                // A database exist in the program dir
                // -> copy database to user home dir
                QFile file(dbTemplatePath);

                if (file.copy(dbPath)) {
                    QFile::setPermissions(
                        dbPath, QFile::permissions(dbPath) | QFile::WriteUser);
                    dbExist = true;
                } else {
                    ErrorMessage* errorMessage = new ErrorMessage();
                    errorMessage->showMessage(ERR_SQL_DB_USER_COPY,
                        ErrorMessage::Type::Warning, ErrorMessage::Cancel::No,
                        QObject::tr("Betroffener Kopierpfad:\n") + dbPath);
                }
            } else {
                // No database found in program dir
                ErrorMessage* errorMessage = new ErrorMessage();
                errorMessage->showMessage(ERR_SQL_DB_APP_EXIST,
                    ErrorMessage::Type::Critical, ErrorMessage::Cancel::Program,
                    QObject::tr("Betroffener Pfad:\n") + dbPath);
                return false;
            }
        }
    }
    // No user path specified or file lost
    // (first program start oder registry was cleaned)
    if (!dbExist) {
        if (!APP_PORTABLE) {
            dbHomeTemp = QDir::homePath();
            dbFolderTemp = "tipp10";
            if (!QFile::exists(
                    QDir::homePath() + "/" + dbFolderTemp + "/" + dbNameUser)) {
                dbFolderTemp = "TIPP10";
#ifdef APP_WIN
                QSettings homeAppPath("HKEY_CURRENT_"
                                      "USER\\Software\\Microsoft\\Windows\\Curr"
                                      "entVersion\\Explorer\\Shell Folders",
                    QSettings::NativeFormat);
                dbHomeTemp = homeAppPath.value("AppData").toString();
#endif
#ifdef APP_MAC
                dbHomeTemp.append("/Library/Application Support");
#endif
            }
            dbPath = QDir::homePath() + "/" + dbFolderTemp + "/" + dbNameUser;

        } else {
            // Portable version
            dbHomeTemp = QCoreApplication::applicationDirPath();
            dbFolderTemp = "portable";
            dbPath = QCoreApplication::applicationDirPath() + "/portable/"
                + dbNameUser;
        }
        // Exist a database in user's home dir?
        if (!QFile::exists(dbPath)) {
            // Exist a database template in the program dir?
            dbPath = INSTALLPREFIX "/share/tipp10/" + dbNameTemplate;
            // dbPath = ":/" + dbNameTemplate;
            if (QFile::exists(dbPath)) {
                // A database template exist in the program dir
                // -> copy database to user home dir
                QDir dir(dbHomeTemp);
                dir.mkdir(dbFolderTemp);
                dir.cd(dbFolderTemp);
                QFile file(dbPath);
                if (file.copy(dir.path() + "/" + dbNameUser)) {
                    QFile::setPermissions(dir.path() + "/" + dbNameUser,
                        QFile::permissions(dir.path() + "/" + dbNameUser)
                            | QFile::WriteUser);
                    dbPath = dir.path() + "/" + dbNameUser;
                } else {
                    ErrorMessage* errorMessage = new ErrorMessage();
                    errorMessage->showMessage(ERR_SQL_DB_APP_COPY,
                        ErrorMessage::Type::Critical, ErrorMessage::Cancel::No,
                        QObject::tr("Betroffener Kopierpfad:\n") + dbPath);
                }
            } else {
                // No database found in program dir
                ErrorMessage* errorMessage = new ErrorMessage();
                errorMessage->showMessage(ERR_SQL_DB_APP_EXIST,
                    ErrorMessage::Type::Critical, ErrorMessage::Cancel::Program,
                    QObject::tr("Betroffener Pfad:\n") + dbPath);
                return false;
            }
        }
    }

    // Check wether the database exists to avoid that it
    // will be created if it doesn't exist
    /*if (!QFile::exists(dbPath)) {
                // Error message
                ErrorMessage *errorMessage = new ErrorMessage();
                errorMessage->showMessage(ERR_SQL_DB,
       ErrorMessage::Type::Critical, ErrorMessage::Cancel::Program); return
       false;
        }*/
    if (QSqlDatabase::contains()) {
        db = QSqlDatabase::database();
        db.close();
    } else {
        db = QSqlDatabase::addDatabase("QSQLITE");
    }

    // Set database
    db.setDatabaseName(dbPath);
    // Open the database
    if (!db.open()) {
        // Error message
        ErrorMessage* errorMessage = new ErrorMessage();
        errorMessage->showMessage(ERR_SQL_CONNECTION,
            ErrorMessage::Type::Critical, ErrorMessage::Cancel::Program,
            QObject::tr("Betroffener Pfad:\n") + dbPath);
        return false;
    }

    settings.beginGroup("database");
    settings.setValue("pathpro", dbPath);
    settings.endGroup();

    QSqlQuery queryUpdate;

    if (!queryUpdate.exec("SELECT * FROM db_version ORDER BY version DESC;")) {
        // Error message
        ErrorMessage* errorMessage = new ErrorMessage();
        errorMessage->showMessage(ERR_DB_VERSION_READABLE,
            ErrorMessage::Type::Critical, ErrorMessage::Cancel::Update);
    } else {
        if (!queryUpdate.first()) {
            // Error message
            ErrorMessage* errorMessage = new ErrorMessage();
            errorMessage->showMessage(ERR_DB_VERSION_READABLE,
                ErrorMessage::Type::Critical, ErrorMessage::Cancel::Update);
        } else {
            // Server DB version is 0
            // -> software is too old to update
            if (queryUpdate.value(0).toInt() < (int)COMPILED_UPDATE_VERSION) {

                // Update the database
                QStringList updateFileName = QString(UPDATE_URL_SQL).split("/");

                QFile sqlFile(
                    ":/update/" + updateFileName[updateFileName.count() - 1]);
                if (!sqlFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
                    // Error message
                    ErrorMessage* errorMessage = new ErrorMessage();
                    errorMessage->showMessage(ERR_TEMP_FILE_CREATION,
                        ErrorMessage::Type::Critical,
                        ErrorMessage::Cancel::Update);
                }

                // Go to the beginning of the version file
                sqlFile.seek(0);

                QTextStream in(&sqlFile);

                QString line;

                // Execute all sql command of the downloaded file
                while (!in.atEnd()) {
                    line = in.readLine();
                    line = line.trimmed();
                    // Exclude comments and empty lines
                    if (line != ""
                        && !line.startsWith("//", Qt::CaseSensitive)) {
                        // Without error handling, because DROP-Statements are
                        // allowed to be invalid (there exist also a IF EXISTS
                        // statement in the SQLite library which suppresses an
                        // error, but it didn't work when I try it)
                        if (!queryUpdate.exec(line)
                            && !line.startsWith("drop", Qt::CaseInsensitive)) {
                            // Error message + failed sql string
                            ErrorMessage* errorMessage = new ErrorMessage();
                            errorMessage->showMessage(ERR_UPDATE_SQL_EXECUTION,
                                ErrorMessage::Type::Critical,
                                ErrorMessage::Cancel::Update, line);
                            break;
                        }
                    }
                }
                StartSql* lessonSql = new StartSql();
                if (!lessonSql->analyzeLessons("lesson")) {
                    ErrorMessage* errorMessage = new ErrorMessage();
                    errorMessage->showMessage(ERR_ANALYZE_TABLE_FILL,
                        ErrorMessage::Type::Critical,
                        ErrorMessage::Cancel::Update, line);
                }
                if (!lessonSql->analyzeLessons("open")) {
                    ErrorMessage* errorMessage = new ErrorMessage();
                    errorMessage->showMessage(ERR_ANALYZE_TABLE_FILL,
                        ErrorMessage::Type::Critical,
                        ErrorMessage::Cancel::Update, line);
                }
                if (!lessonSql->analyzeLessons("own")) {
                    ErrorMessage* errorMessage = new ErrorMessage();
                    errorMessage->showMessage(ERR_ANALYZE_TABLE_FILL,
                        ErrorMessage::Type::Critical,
                        ErrorMessage::Cancel::Update, line);
                }
            }
        }
    }

    return true;

    // If to connect directly to an online database
    // Example: database connection to MySql
    /*QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
db.setHostName("http://www.domain.com");
db.setDatabaseName("dbname");
db.setUserName("user");
db.setPassword("pass");*/
}

#endif // CONNECTION_H
