#include "widget.h"
#include "serialportreader.h"

#include <QApplication>

#include <QSerialPort>
#include <QTextStream>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


//    // prima la porta seriale...
//    QSerialPort serialPort;
//    QString serialPortName = "COM3";
//    serialPort.setPortName(serialPortName);

//    int serialPortBaudRate = QSerialPort::Baud38400;
//    serialPort.setBaudRate(serialPortBaudRate);

//    if (!serialPort.open(QIODevice::ReadOnly)) {
//        QTextStream standardOutput(stdout);
//        standardOutput << "Azz..." << endl;
//        return 1;
//    }

//    SerialPortReader serialPortReader(&serialPort);


    // ... poi la finestrella :)
    Widget w;
    w.show();

    // Alla fine, parte la banda!!!
    return a.exec();
}
