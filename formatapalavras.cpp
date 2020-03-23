#include "formatapalavras.h"

FormataPalavras::FormataPalavras(QObject *parent) : QObject(parent)
{

}

//Vamos testar todas as combinações possíveis. Se for igual em alguma, retornamos true.
//se não for igual em nenhuma, é false.
bool FormataPalavras::fcomparaNomes(QString rnome1, QString rnome2)
{
    if(rnome1.isEmpty() || rnome2.isEmpty())
        return false;

    if(rnome1.compare(rnome2, Qt::CaseInsensitive) == 0)
        return true;

    if(fremovePontuacao(rnome1).compare(fremovePontuacao(rnome2)) == 0)
        return true;

    if(fremoveSeason(rnome1).compare(fremoveSeason(rnome2)) == 0)
        return true;

    if(fremoveCaracteresEspeciais(rnome1).compare(fremoveCaracteresEspeciais(rnome2)) == 0)
        return true;

    if(fmudaCaracteresEspeciais(rnome1).compare(fmudaCaracteresEspeciais(rnome2)) == 0)
        return true;

    if(fmudaNumeracaoArabePraRomana(rnome1).compare(fmudaNumeracaoArabePraRomana(rnome2)) == 0)
        return true;

    if(fremoveNumeracao(rnome1).compare(fremoveNumeracao(rnome2)) == 0)
        return true;

    if(fmudaNumeracaoRomanaPraArabe(rnome1).compare(fmudaNumeracaoRomanaPraArabe(rnome2)) == 0)
        return true;

    if(fremoveNumeracaoRomana(rnome1).compare(fremoveNumeracaoRomana(rnome2)) == 0)
        return true;

    if(fremoveCaracteresExtras(rnome1).compare(fremoveCaracteresExtras(rnome2)) == 0)
        return true;

    if(rnome1.contains("OVA") || rnome1.contains("Special") || rnome2.contains("OVA") || rnome2.contains("Special")){
        if(fmudaOVAPraSpecials(rnome1).compare(fmudaOVAPraSpecials(rnome2)) == 0)
            return true;
        if(fmudaSpecialsPraOVA(rnome1).compare(fmudaSpecialsPraOVA(rnome2)) == 0)
            return true;
        if(fmudaOVAPraSpecial(rnome1).compare(fmudaOVAPraSpecial(rnome2)) == 0)
            return true;
        if(fmudaSpecialPraOVA(rnome1).compare(fmudaSpecialPraOVA(rnome2)) == 0)
            return true;
    }

    if(fremoveTudo(rnome1).compare(fremoveTudo(rnome2)) == 0)
        return true;

    return false;
}

QString FormataPalavras::fremoveTudo(QString rpalavra)
{
    rpalavra = fremovePontuacao(rpalavra);
    rpalavra = fremoveCaracteresEspeciais(rpalavra);
    rpalavra = fremoveSeason(rpalavra);
    rpalavra = fmudaNumeracaoArabePraRomana(rpalavra);
    rpalavra = fremoveNumeracao(rpalavra);
    rpalavra = fmudaNumeracaoRomanaPraArabe(rpalavra);
    rpalavra = fremoveNumeracaoRomana(rpalavra);
    rpalavra = fremoveCaracteresExtras(rpalavra);
    rpalavra = fmudaOVAPraSpecials(rpalavra);
    rpalavra = fmudaSpecialsPraOVA(rpalavra);
    rpalavra = fmudaOVAPraSpecial(rpalavra);
    rpalavra = fmudaSpecialPraOVA(rpalavra);
    return rpalavra;
}

QString FormataPalavras::fremovePontuacao(QString rpalavra)
{
    rpalavra = rpalavra.toLower();
    rpalavra.remove(".");
    rpalavra.remove(",");
    rpalavra.remove("?");
    rpalavra.remove("!");
    rpalavra.remove(";");
    rpalavra.remove("¿");
    rpalavra.remove("¡");
    rpalavra = rpalavra.simplified();
    return rpalavra;
}

QString FormataPalavras::fremoveCaracteresEspeciais(QString rpalavra)
{
    rpalavra = rpalavra.toLower();
    rpalavra.replace("☆", " ");
    rpalavra.replace("△", " ");
    rpalavra.replace("♥", " ");
    rpalavra = rpalavra.simplified();
    return rpalavra;
}

QString FormataPalavras::fmudaCaracteresEspeciais(QString rpalavra)
{
    rpalavra = rpalavra.toLower();
    rpalavra.replace("☆", "-");
    rpalavra.replace("△", "-");
    rpalavra.replace("♥", "-");
    rpalavra = rpalavra.simplified();
    return rpalavra;
}

QString FormataPalavras::fremoveSeason(QString rpalavra)
{
    rpalavra = rpalavra.toLower();
    rpalavra.replace("season 1", "1");
    rpalavra.replace("season 2", "2");
    rpalavra.replace("season 3", "3");
    rpalavra.replace("season 4", "4");
    rpalavra.replace("season 5", "5");
    rpalavra.replace("season 6", "6");
    rpalavra.replace("season 7", "7");
    rpalavra.replace("season 8", "8");
    rpalavra.replace("season 9", "9");
    rpalavra.replace("season 10", "10");
    rpalavra.replace("season1", "1");
    rpalavra.replace("season2", "2");
    rpalavra.replace("season3", "3");
    rpalavra.replace("season4", "4");
    rpalavra.replace("season5", "5");
    rpalavra.replace("season6", "6");
    rpalavra.replace("season7", "7");
    rpalavra.replace("season8", "8");
    rpalavra.replace("season9", "9");
    rpalavra.replace("season10", "10");
    rpalavra.replace("s1", "1");
    rpalavra.replace("s2", "2");
    rpalavra.replace("s3", "3");
    rpalavra.replace("s4", "4");
    rpalavra.replace("s5", "5");
    rpalavra.replace("s6", "6");
    rpalavra.replace("s7", "7");
    rpalavra.replace("s8", "8");
    rpalavra.replace("s9", "9");
    rpalavra.replace("s10", "10");
    rpalavra = rpalavra.simplified();
    return rpalavra;
}

QString FormataPalavras::fmudaNumeracaoArabePraRomana(QString rpalavra)
{
    rpalavra = rpalavra.toLower();
    rpalavra.replace(" 1", " I");
    rpalavra.replace(" 2", " II");
    rpalavra.replace(" 3", " III");
    rpalavra.replace(" 4", " IV");
    rpalavra.replace(" 5", " V");
    rpalavra.replace(" 6", " VI");
    rpalavra.replace(" 7", " VII");
    rpalavra.replace(" 8", " VIII");
    rpalavra.replace(" 9", " IX");
    rpalavra.replace(" 10", " X");
    rpalavra = rpalavra.simplified();
    return rpalavra;
}

QString FormataPalavras::fremoveNumeracao(QString rpalavra)
{
    rpalavra = rpalavra.toLower();
    rpalavra.remove("1");
    rpalavra.remove("2");
    rpalavra.remove("3");
    rpalavra.remove("4");
    rpalavra.remove("5");
    rpalavra.remove("6");
    rpalavra.remove("7");
    rpalavra.remove("8");
    rpalavra.remove("9");
    rpalavra.remove("10");
    rpalavra = rpalavra.simplified();
    return rpalavra;

}

QString FormataPalavras::fmudaNumeracaoRomanaPraArabe(QString rpalavra)
{
    rpalavra = rpalavra.toLower();
    rpalavra.replace(" I", " 1");
    rpalavra.replace(" II", " 2");
    rpalavra.replace(" III", " 3");
    rpalavra.replace(" IV", " 4");
    rpalavra.replace(" V", " 5");
    rpalavra.replace(" VI", " 6");
    rpalavra.replace(" VII", " 7");
    rpalavra.replace(" VIII", " 8");
    rpalavra.replace(" IX", " 9");
    rpalavra.replace(" X", " 10");
    rpalavra = rpalavra.simplified();
    return rpalavra;

}

QString FormataPalavras::fremoveNumeracaoRomana(QString rpalavra)
{
    rpalavra = rpalavra.toLower();
    rpalavra.remove(" I");
    rpalavra.remove(" II");
    rpalavra.remove(" III");
    rpalavra.remove(" IV");
    rpalavra.remove(" V");
    rpalavra.remove(" VI");
    rpalavra.remove(" VII");
    rpalavra.remove(" VIII");
    rpalavra.remove(" IX");
    rpalavra.remove(" X");
    rpalavra = rpalavra.simplified();
    return rpalavra;
}

QString FormataPalavras::fremoveCaracteresExtras(QString rpalavra)
{
    rpalavra = rpalavra.toLower();
    rpalavra.remove("@");
    rpalavra.remove("#");
    rpalavra.remove("$");
    rpalavra.remove("%");
    rpalavra.remove("&");
    rpalavra.remove("*");
    rpalavra.remove("(");
    rpalavra.remove(")");
    rpalavra.remove(">");
    rpalavra.remove("<");
    rpalavra.remove("/");
    rpalavra.remove("\\");
    rpalavra.remove("+");
    rpalavra.remove("-");
    rpalavra.remove("=");
    rpalavra.remove(":");
    rpalavra = rpalavra.simplified();
    return rpalavra;

}

QString FormataPalavras::fmudaOVAPraSpecials(QString rpalavra)
{
    rpalavra = rpalavra.toLower();
    rpalavra.replace("OVA", "Specials");
    rpalavra = rpalavra.simplified();
    return rpalavra;
}

QString FormataPalavras::fmudaSpecialsPraOVA(QString rpalavra)
{
    rpalavra = rpalavra.toLower();
    rpalavra.replace("Specials", "OVA");
    rpalavra = rpalavra.simplified();
    return rpalavra;
}

QString FormataPalavras::fmudaOVAPraSpecial(QString rpalavra)
{
    rpalavra = rpalavra.toLower();
    rpalavra.replace("OVA", "Special");
    rpalavra = rpalavra.simplified();
    return rpalavra;
}

QString FormataPalavras::fmudaSpecialPraOVA(QString rpalavra)
{
    rpalavra = rpalavra.toLower();
    rpalavra.replace("Special", "OVA");
    rpalavra = rpalavra.simplified();
    return rpalavra;
}
