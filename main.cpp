

#include "application.h"


int main(int argc, char *argv[])
{
    // Just it :)
    Application a(argc, argv);

    // set application data
    QCoreApplication::setApplicationName("waspsender");
    QCoreApplication::setApplicationVersion("0.0.1");

    QCoreApplication::setOrganizationName("LiceoDaVinciJesi");
    QCoreApplication::setOrganizationDomain("liceodavincijesi.gov.it");

    return a.exec();
}
