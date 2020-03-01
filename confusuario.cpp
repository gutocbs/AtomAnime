#include "confusuario.h"

confUsuario::confUsuario(QObject *parent) : QObject(parent) {
  vterminouChecagem = false;
  vlista = 0;
  //    vdiretorioAnimes.append("E:/Animes/");
  //    vdiretorioAnimes.append("E:/Downloads/");
}

QString confUsuario::fretornaDiretorioEspecifico(int id) {
  if (vterminouChecagem == true)
    return vdiretorioEspecificoAnime[QString::number(id)];
  else
    return "";
}

QVector<QString> confUsuario::fretornaDiretoriosAnimes() {
  return vdiretorioAnimes;
}

QString confUsuario::fsimplificaNomeArquivo(QString rnomeArquivo) {
  anitomy::Anitomy lanitomy;
  lanitomy.Parse(rnomeArquivo.toStdWString());
  const auto &lelements = lanitomy.elements();
  rnomeArquivo =
      QString::fromStdWString(lelements.get(anitomy::kElementAnimeTitle));
  rnomeArquivo = rnomeArquivo.toLower();
  rnomeArquivo.remove(".");
  rnomeArquivo.remove("?");
  rnomeArquivo.remove(":");
  rnomeArquivo.remove("-");

  rnomeArquivo.remove("s1");
  rnomeArquivo.remove("s2");
  rnomeArquivo.remove("s3");
  rnomeArquivo.remove("s4");
  rnomeArquivo.remove("s01");
  rnomeArquivo.remove("s02");
  rnomeArquivo.remove("s03");
  rnomeArquivo.remove("s04");
  rnomeArquivo.remove("season 1");
  rnomeArquivo.remove("season 2");
  rnomeArquivo.remove("season 3");
  rnomeArquivo.remove("season 4");
  rnomeArquivo.remove("/");
  rnomeArquivo.remove("♥");
  rnomeArquivo = rnomeArquivo.simplified();
  return rnomeArquivo;
}

void confUsuario::fbuscaDiretoriosAnimes() {
  qDebug() << "Searching for anime folders";
  // Busca cada diretorio existente nas configurações
  for (int i = 0; i < vdiretorioAnimes.size(); i++) {
    // Procura apenas por diretorios e subdiretorios
    QDirIterator lit(vdiretorioAnimes[i], QDir::Dirs | QDir::NoDotAndDotDot,
                     QDirIterator::Subdirectories);
    while (lit.hasNext()) {
      QFile lfile(lit.next());
      // Pega o nome de cada diretorio e divide, pegando apenas o nome do anime
      QString lnomeAnime = fsimplificaNomeArquivo(lit.fileName());
      // Compara o nome do anime com os animes da lista
      for (int w = 0; w < vlistaAnimes.size(); w++) {
        // Anitomy, aqui, simplifica os nomes dos anime para garantir que a
        // função vai comparar os nomes certos
        QString lnomeAnimeLista =
            fsimplificaNomeArquivo(vlistaAnimes[w]->vnome);
        QString lnomeInglesAnimeLista =
            fsimplificaNomeArquivo(vlistaAnimes[w]->vnomeIngles);
        // Compara os nomes, sempre ignorando letras maiusculas e minusculas
        // para garantir que o anime será encontrado.
        if (vdiretorioEspecificoAnime.contains(vlistaAnimes[w]->vid)) {
          if (!vdiretorioEspecificoAnime[vlistaAnimes[w]->vid].isEmpty()) {
            if (QFile::exists(
                    vdiretorioEspecificoAnime[vlistaAnimes[w]->vid])) {
              vlistaAnimes.remove(w);
              break;
            } else
              vdiretorioEspecificoAnime.remove(vlistaAnimes[w]->vid);
          }
        }
        if (lnomeAnime.compare(lnomeAnimeLista, Qt::CaseInsensitive) == 0 &&
            lnomeAnime.isEmpty() == false) {
          vdiretorioEspecificoAnime.insert(vlistaAnimes[w]->vid,
                                           lfile.fileName());
          vlistaAnimes.remove(w);
          break;
        } else if (lnomeAnime.compare(lnomeInglesAnimeLista,
                                      Qt::CaseInsensitive) == 0 &&
                   lnomeInglesAnimeLista.isEmpty() == false) {
          vdiretorioEspecificoAnime.insert(vlistaAnimes[w]->vid,
                                           lfile.fileName());
          vlistaAnimes.remove(w);
          break;
        } else {
          // Compara os nomes alternativos dos animes, pro caso de serem usados
          // nos arquivos Ex: Okaa-san Online em vez de Tsuujou Kougeki ga Zentai
          // Kougeki de Ni-kai Kougeki no Okaasan wa Suki Desu ka?
          for (int z = 0; z < vlistaAnimes[w]->vnomeAlternativo.size(); z++) {
            QString lnomeAlternativoAnimeLista =
                fsimplificaNomeArquivo(vlistaAnimes[w]->vnomeAlternativo.at(z));
            if (lnomeAnime.compare(lnomeAlternativoAnimeLista,
                                   Qt::CaseInsensitive) == 0) {
              vdiretorioEspecificoAnime.insert(vlistaAnimes[w]->vid,
                                               lfile.fileName());
              vlistaAnimes.remove(w);
              break;
            }
          }
        }
      }
    }
  }
  fsetupListasPraBusca();
}

/// POSSO REMOVER ESSA FUNÇÃO CASO NECESSÁRIO, SERVE APENAS PARA DEBUG
void confUsuario::fmostraPastas() {
  foreach (QString key, vdiretorioEspecificoAnime.keys()) {
    qDebug() << key << " - " << vdiretorioEspecificoAnime.value(key);
  }
}

void confUsuario::fbuscaPastasThread(QThread &dThread) {
  vlista = 0;
  connect(&dThread, &QThread::started, this,
          &confUsuario::fsetupListasPraBusca);
}

void confUsuario::fsalvaPastasArquivos() {
  QFile t("Configurações/Temp/animeFoldersTemp.txt");
  if (t.open(QIODevice::WriteOnly)) {
    QTextStream lstreamTexto(&t);
    foreach (QString key, vdiretorioEspecificoAnime.keys()) {
      //            qDebug() << vdiretorioEspecificoAnime[key];
      lstreamTexto << key << ";" << vdiretorioEspecificoAnime[key] << endl;
    }
    t.close();
  }
  if (QFile::exists("Configurações/Temp/animeFolders.txt")) {
    if (QFile::remove("Configurações/Temp/animeFolders.txt"))
      t.rename("Configurações/Temp/animeFolders.txt");
  } else
    t.rename("Configurações/Temp/animeFolders.txt");
}

void confUsuario::flePastasArquivos() {
  QFile t("Configurações/Temp/animeFolders.txt");
  QTextStream lstreamTexto(&t);
  if (t.open(QIODevice::ReadOnly)) {
    while (!lstreamTexto.atEnd()) {
      QStringList llinha = lstreamTexto.readLine().split(";");
      vdiretorioEspecificoAnime.insert(llinha.at(0), llinha.at(1));
    }
    vterminouChecagem = true;
    t.close();
  }
}

// Aparentemente nunca entra aqui
void confUsuario::fsetupListasPraBusca() {
  qDebug() << "Setup Pastas";
  if (vlista == 0) {
    if (this->thread()->isInterruptionRequested()) {
      this->thread()->exit(0);
      return;
    }
    vlistaAnimes = cleitorlistaanimes->retornaListaWatching();
    vlista++;
    fbuscaDiretoriosAnimes();
  } else if (vlista == 1) {
    if (this->thread()->isInterruptionRequested()) {
      this->thread()->exit(0);
      return;
    }
    vlistaAnimes = cleitorlistaanimes->retornaListaCompleted();
    vlista++;
    fbuscaDiretoriosAnimes();
  } else if (vlista == 2) {
    if (this->thread()->isInterruptionRequested()) {
      this->thread()->exit(0);
      return;
    }
    vlistaAnimes = cleitorlistaanimes->retornaListaDropped();
    vlista++;
    fbuscaDiretoriosAnimes();
  } else if (vlista == 3) {
    if (this->thread()->isInterruptionRequested()) {
      this->thread()->exit(0);
      return;
    }
    vlistaAnimes = cleitorlistaanimes->retornaListaOnHold();
    vlista++;
    fbuscaDiretoriosAnimes();
  } else if (vlista == 4) {
    if (this->thread()->isInterruptionRequested()) {
      this->thread()->exit(0);
      return;
    }
    vlistaAnimes = cleitorlistaanimes->retornaListaPlanToWatch();
    vlista++;
    fbuscaDiretoriosAnimes();
  } else if (vlista == 5) {
    emit schecouPastas();
    vterminouChecagem = true;
    qDebug() << "All animes in the computer were found";
    this->thread()->exit(0);
    fsalvaPastasArquivos();
  }
}

void confUsuario::frecebeListaAnime(leitorlistaanimes *rlistaAnime) {
  cleitorlistaanimes = rlistaAnime;
}

void confUsuario::frecebeConfigs(QStringList ldiretorios) {
  vdiretorioAnimes = ldiretorios.toVector();
}
