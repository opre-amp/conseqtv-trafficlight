#include "settings.h"
#include <iostream>
#include <QDebug>
#include <QFile>


#include <QMessageBox>
#include <QNetworkReply>

delaySettings::delaySettings()
{ }

void delaySettings::upload(){
     qDebug() << "TODO :Delay settings go through TCP";
}

int delaySettings::getTime(int t){
    try{

         return timeset[t-1];

    }catch(const std::exception& e){
        qDebug() << "Error while indexing getter";
    }

    return 0;
}

void delaySettings::setTime(QString timer, int value)
{
   QChar end = timer.back();
   int idx = 0;

   try{
        int idx = end.digitValue();
        timeset[idx-1] = value;

   }catch(const std::exception& e){
       qDebug() << "Error while indexing setter";
   }

   switch (idx) {
        case 1: {qDebug() << "a_time set"; break;}
        case 2: {qDebug() << "b_time set"; break;}
        case 3: {qDebug() << "c_time set"; break;}
        case 4: {qDebug() << "cd_time set"; break;}
        case 5: {qDebug() << "d_time set"; break;}
        case 6: {qDebug() << "e_time set"; break;}
        case 7: {qDebug() << "f_time set"; break;}
        case 8: {qDebug() << "g_time set"; break;}
        case 9: {qDebug() << "h_time set"; break;}
   }
}
