#include "mainwindow.h"

#include <QApplication>
#include <QTextStream>
#include <QScopedPointer>

QScopedPointer<QFile> m_logFile;

void messageHanlder(QtMsgType type, const QMessageLogContext &context, const QString &msg);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    m_logFile.reset(new QFile("logFile.txt"));
    m_logFile.data()->open(QFile::Append | QFile::Text);
    qInstallMessageHandler(messageHanlder);

    MainWindow w;
    w.show();
    return a.exec();
}

QTextStream& qStdOut()
{
    static QTextStream ts( stdout );
    return ts;
}

QTextStream& logFileOut()
{
    static QTextStream ts( m_logFile.data() );
    return ts;
}

void messageHanlder(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    Q_UNUSED(context);
    if (type == QtMsgType::QtInfoMsg) logFileOut() << msg << endl;
    logFileOut().flush();
    qStdOut() << msg << endl;
}
