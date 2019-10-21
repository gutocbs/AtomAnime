#include "logger.h"

QString logger::varquivo = QDir::currentPath() + QDir::separator() + "Configurações/Temp/" + QDir::separator() + "log.txt";
bool logger::vlogging = false;
static const QtMessageHandler QT_DEFAULT_MESSAGE_HANDLER = qInstallMessageHandler(nullptr);

logger::logger(QObject *parent) : QObject(parent)
{

}

void logger::fattachLogger(){
    vlogging = true;
    qInstallMessageHandler(fhandlerMensagem);
}

void logger::fhandlerMensagem(QtMsgType rtipo, const QMessageLogContext &rcontexto, const QString &rmensagem){
    if(vlogging){
        QString lmensagemLog;
        switch (rtipo) {
            case QtInfoMsg:
                lmensagemLog = QString("Info: %1").arg(rmensagem);
            break;
            case QtDebugMsg:
                    lmensagemLog = QString("Debug: %1").arg(rmensagem);
            break;
            case QtCriticalMsg:
                lmensagemLog = QString("Critical Error: %1").arg(rmensagem);
            break;
            case QtWarningMsg:
                lmensagemLog = QString("Error: %1").arg(rmensagem);
            break;
            case QtFatalMsg:
                lmensagemLog = QString("FATAL ERROR: %1").arg(rmensagem);
            break;
        }
        QFile lfile(logger::varquivo);
        if(lfile.open(QIODevice::WriteOnly | QIODevice::Append)) {
            QTextStream lstreamTexto(&lfile);
            QString larquivo = rcontexto.file;
            larquivo.remove(larquivo.lastIndexOf('.'), 4);
            lstreamTexto << lmensagemLog << " - " << larquivo.mid(10) << " - " << rcontexto.line << endl;
            lstreamTexto.flush();
            lfile.close();
        }
    }
    (*QT_DEFAULT_MESSAGE_HANDLER)(rtipo, rcontexto, rmensagem);
}