#include "confbase.h"

confBase::confBase(QObject *parent) : QObject(parent)
{
}

void confBase::fcriaDiretoriosBase()
{
    QString lconfPath = "Configurações";
    QString limagePath = lconfPath + QDir::separator() + "Imagens";
    QString ltempPath = lconfPath + QDir::separator() + "Temp";
    qDebug() << "Trying to create folder" << lconfPath;
    if(!QDir("Configurações").exists()){
        if(QDir().mkdir(lconfPath))
            qDebug() << "Done successfully";
        else
            qDebug() << "Could not create the folder";
    }
    else
        qDebug() << "The folder already exists";
    qDebug() << "Trying to create folder" << limagePath;
    if(!QDir(limagePath).exists()){
        if(QDir().mkdir(limagePath))
            qDebug() << "Done successfully";
        else
            qDebug() << "Could not create the folder";
    }
    else
        qDebug() << "The folder already exists";
    qDebug() << "Trying to create folder" << limagePath + QDir::separator() + "Pequeno";
    if(!QDir(limagePath + QDir::separator() + "Pequeno").exists()){
        if(QDir().mkdir(limagePath + QDir::separator() + "Pequeno"))
            qDebug() << "Done successfully";
        else
            qDebug() << "Could not create the folder";
    }
    else
        qDebug() << "The folder already exists";
    qDebug() << "Trying to create folder" << limagePath + QDir::separator() + "Medio";
    if(!QDir(limagePath + QDir::separator() + "Medio").exists()){
        if(QDir().mkdir(limagePath + QDir::separator() + "Medio"))
            qDebug() << "Done successfully";
        else
            qDebug() << "Could not create the folder";
    }
    else
        qDebug() << "The folder already exists";
    qDebug() << "Trying to create folder" << limagePath + QDir::separator() + "Grande";
    if(!QDir(limagePath + QDir::separator() + "Grande").exists()){
        if(QDir().mkdir(limagePath + QDir::separator() + "Grande"))
            qDebug() << "Done successfully";
        else
            qDebug() << "Could not create the folder";
    }
    else
        qDebug() << "The folder already exists";
    qDebug() << "Trying to create folder" << ltempPath;
    if(!QDir(ltempPath).exists()){
        if(QDir().mkdir(ltempPath))
            qDebug() << "Done successfully";
        else
            qDebug() << "Could not create the folder";
    }
    else
        qDebug() << "The folder already exists";
    qDebug() << "Trying to create folder" << ltempPath + QDir::separator() + "Torrents";
    if(!QDir("Configurações/Temp/Torrents").exists()){
        if(QDir().mkdir(ltempPath + QDir::separator() + "Torrents"))
            qDebug() << "Done successfully";
        else
            qDebug() << "Could not create the folder";
    }
    else
        qDebug() << "The folder already exists";
    vdiretorioImagensPequenas = limagePath + QDir::separator() + "Pequeno" + QDir::separator();
    vdiretorioImagensMedio = limagePath + QDir::separator() + "Medio" + QDir::separator();
    vdiretorioImagensGrandes = limagePath + QDir::separator() + "Grande" + QDir::separator();
}

void confBase::fcriaArquivoLog()
{
    qDebug() << "Trying to create log file";
    //Apesar da função chamar cria arquivo log, ela apaga o arquivo antigo para que um novo possa ser criado no lugar
    QFile llog("Configurações/Temp/log.txt");
    if(llog.exists())
        llog.remove();
}

