#include "confbase.h"

confBase::confBase(QObject *parent) : QObject(parent)
{
    fcriaDiretoriosBase();
}

void confBase::fcriaDiretoriosBase()
{
    QString lconfPath = "Configurações";
    QString limagePath = lconfPath + QDir::separator() + "Imagens";
    QString ltempPath = lconfPath + QDir::separator() + "Temp";
    qDebug() << "Checking if folder exists" << lconfPath;
    if(!QDir("Configurações").exists()){
        if(QDir().mkdir(lconfPath))
            qDebug() << "Folder created";
        else
            qDebug() << "Could not create the folder";
    }
    else
        qDebug() << "The folder already exists";
    qDebug() << "Checking if folder exists" << limagePath;
    if(!QDir(limagePath).exists()){
        if(QDir().mkdir(limagePath))
            qDebug() << "Folder created";
        else
            qDebug() << "Could not create the folder";
    }
    else
        qDebug() << "The folder already exists";
    qDebug() << "Checking if folder exists" << limagePath + QDir::separator() + "Pequeno";
    if(!QDir(limagePath + QDir::separator() + "Pequeno").exists()){
        if(QDir().mkdir(limagePath + QDir::separator() + "Pequeno"))
            qDebug() << "Folder created";
        else
            qDebug() << "Could not create the folder";
    }
    else
        qDebug() << "The folder already exists";
    qDebug() << "Checking if folder exists" << limagePath + QDir::separator() + "Medio";
    if(!QDir(limagePath + QDir::separator() + "Medio").exists()){
        if(QDir().mkdir(limagePath + QDir::separator() + "Medio"))
            qDebug() << "Folder created";
        else
            qDebug() << "Could not create the folder";
    }
    else
        qDebug() << "The folder already exists";
    qDebug() << "Checking if folder exists" << limagePath + QDir::separator() + "Grande";
    if(!QDir(limagePath + QDir::separator() + "Grande").exists()){
        if(QDir().mkdir(limagePath + QDir::separator() + "Grande"))
            qDebug() << "Folder created";
        else
            qDebug() << "Could not create the folder";
    }
    else
        qDebug() << "The folder already exists";
    qDebug() << "Checking if folder exists" << ltempPath;
    if(!QDir(ltempPath).exists()){
        if(QDir().mkdir(ltempPath))
            qDebug() << "Folder created";
        else
            qDebug() << "Could not create the folder";
    }
    else
        qDebug() << "The folder already exists";
    qDebug() << "Checking if folder exists" << ltempPath + QDir::separator() + "Torrents";
    if(!QDir("Configurações/Temp/Torrents").exists()){
        if(QDir().mkdir(ltempPath + QDir::separator() + "Torrents"))
            qDebug() << "Folder created";
        else
            qDebug() << "Could not create the folder";
    }
    else
        qDebug() << "The folder already exists";
    vdiretorioImagensPequenas = limagePath + QDir::separator() + "Pequeno" + QDir::separator();
    vdiretorioImagensMedio = limagePath + QDir::separator() + "Medio" + QDir::separator();
    vdiretorioImagensGrandes = limagePath + QDir::separator() + "Grande" + QDir::separator();
}

