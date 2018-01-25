/* -*- Mode: C++; indent-tabs-mode: nil; tab-width: 4 -*-
 * -*- coding: utf-8 -*-
 *
 * Copyright (C) 2011 ~ 2017 Deepin, Inc.
 *               2011 ~ 2017 Wang Yong
 *
 * Author:     Wang Yong <wangyong@deepin.com>
 * Maintainer: Wang Yong <wangyong@deepin.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */ 

#include "settings.h"
#include <QProcess>
#include <QThread>
#include <proc/readproc.h>
#include <proc/sysinfo.h>

class RecordProcess : public QThread
{
    Q_OBJECT
    
    typedef std::map<int, proc_t> StoredProcType;
    
public:
    static const int RECORD_TYPE_VIDEO;
    static const int RECORD_TYPE_GIF;
    static const int RECORD_GIF_SLEEP_TIME;
    
    RecordProcess(QObject *parent = 0);
    
    void setRecordInfo(int recordX, int recordY, int record_width, int recordHeight, QString areaName, int screenWidth, int screenHeight);
    void setRecordType(int recordType);
    void startRecord();
    void stopRecord();
    void recordGIF();
    void recordVideo();
    void initProcess();
    int readSleepProcessPid();
    
protected:
    void run();

private:
    QProcess* process;

    int recordX;
    int recordY;
    int recordWidth;
    int recordHeight;
    int recordType;
    
    QString savePath;
    QString saveBaseName;
    QString saveTempDir;
    QString saveDir;
    QString defaultSaveDir;
    QString saveAreaName;
    
    Settings* settings;
    
    int byzanzProcessId;
    int sleepProcessId;
};
