#ifndef INFOANIME_H
#define INFOANIME_H

#include <QFile>
#include <QString>
#include <iostream>
#include <QDebug>
#include<fstream>
#include<cstring>
#include<string>
#include <algorithm>
#include "configpc.h"

class leitorarquivos;

class infoanime
{
public:
    void parseArquivo();
    infoanime();

private:
    configPC *pastas;
//    leitorarquivos *leitor;
};

#endif // INFOANIME_H
