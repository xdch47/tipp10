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
** Definition of the LessonResult class
** File name: lessonresult.h
**
****************************************************************/

#include <QChar>
#include <QList>
#include <QPushButton>
#include <QSqlQuery>
#include <QWidget>

#ifndef LESSONRESULT_H
#define LESSONRESULT_H

//! The LessonTableSql class provides a table widget with lessons.
/*!
        @author Tom Thielicke, s712715
        @version 0.0.2
        @date 16.06.2006
*/
class LessonResult : public QWidget {
    Q_OBJECT

public:
    LessonResult(int row, QList<QChar> charlist, QList<int> mistakelist,
        QWidget* parent = nullptr);

private slots:
    void createPrintOutput();

private:
    void readData();
    void createOutput();
    QPushButton* buttonPrintLesson;
    QList<QChar> charList;
    QList<int> mistakeList;
    int lessonRow;

    QString settingsDuration;
    QString settingsError;
    QString settingsHelp;

    QString lessonName;
    QString lessonTimestamp;
    QString lessonTimeLen;
    QString lessonTokenLen;
    QString lessonErrorNum;
    QString lessonCpm;
    QString lessonGrade;
    QString lessonRate;
};

#endif // LESSONRESULT_H
