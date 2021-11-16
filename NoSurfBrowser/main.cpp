/****************************************************************************
NoSurf Web Browser - based on QTs web browser QtWebEngine
source code based of minibrowser example

Websites are hard coded in.
Be sure not to link to any search engine as that defeats the purpose of no surf.

------------------------------------------------------------------
How it works.
hard code in what websites you plan on visiting.
Compile and build. The build folder is now your new web browser.
Delete your old webbrowser. ;-)

When NoSurf loads you will see 2 programs
one is the web selector and other is the browser.
select what website you want to visit via the selector then
hit the GO button on the browser.
------------------------------------------------------------------

User can hold ctrl to zoom in and out.
No ad blocking is build in unfortunately but there there is alternative
solutions for that. pi-hole etc.

The QtWebEngine works quite well but has glitches that you may not see
with with a regular browsers with better support. But overall it works with
about 98 percent with websites I tried. Hitting the reload button sometimes
fixes odd rendering glitches.

https://github.com/oligprojects/

More info about the NoSurf movement at
https://www.reddit.com/r/nosurf/wiki/index
****************************************************************************/

#include <QtCore/QUrl>
#include <QtCore/QCommandLineOption>
#include <QtCore/QCommandLineParser>
#include <QGuiApplication>
#include <QStyleHints>
#include <QScreen>
#include <QQmlApplicationEngine>
#include <QtQml/QQmlContext>
#include <QtWebView/QtWebView>
#include <QComboBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QApplication>



// Workaround: As of Qt 5.4 QtQuick does not expose QUrl::fromUserInput.
class Utils : public QObject {
    Q_OBJECT
public:
    Utils(QObject *parent = nullptr) : QObject(parent) { }
    Q_INVOKABLE static QUrl fromUserInput(const QString& userInput);
};






QUrl Utils::fromUserInput(const QString& userInput)
{
    if (userInput.isEmpty())
        return QUrl::fromUserInput("about:blank");
    const QUrl result = QUrl::fromUserInput(userInput);
    return result.isValid() ? result : QUrl::fromUserInput("about:blank");
}

#include "main.moc"

int main(int argc, char *argv[])
{
//! [0]
    QtWebView::initialize();
    QGuiApplication app(argc, argv);


    QApplication app2(argc, argv);
    QWidget w;
    w.setWindowTitle("NoSurf");

    QVBoxLayout *layout = new QVBoxLayout(&w);
    QLabel *label = new QLabel("about:blank", &w);
    QComboBox *combo = new QComboBox(&w);
    combo->addItem("about:blank");
    layout->addWidget(label);
    layout->addWidget(combo);
    label->setStyleSheet("color: gray");







    //when adding a website make sure to include https:// before www
    // you can add as many combo->addItem as you want

    combo->addItem("https://www.reddit.com/r/nosurf/");
    combo->addItem("https://www.whatsmybrowser.org/");
    combo->addItem("https://github.com/oligprojects");
    combo->addItem("https://www.amazon.de");   
    combo->addItem("https://www.discord.com");
    combo->addItem("https://twhl.info/wiki");
    combo->addItem("https://www.xbox.com/en-GB/");
    combo->addItem("https://linuxgizmos.com/");
    combo->addItem("https://github.com/");
    combo->addItem("https://hackaday.com/");  
    combo->addItem("https://status.othernet.is/"); 
    combo->addItem("http://192.168.0.1");
    combo->addItem("http://192.168.0.100/");
    combo->addItem("https://192.168.1.1");
    combo->addItem("http://192.168.1.254");










    //This is for QT6
    QObject::connect(combo, SIGNAL(currentTextChanged(QString)), label, SLOT(setText(QString)));

    // If you decide to use QT5 you need to use the below line, and comment out the above line
    //QObject::connect(combo, SIGNAL(currentIndexChanged(QString)), label, SLOT(setText(QString)));


    w.show();


    //! [0]
    QGuiApplication::setApplicationDisplayName(QCoreApplication::translate("main","NoSurf Web Browser"));
    QCommandLineParser parser;
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);
    parser.setApplicationDescription(QGuiApplication::applicationDisplayName());
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("url", "The initial URL to open.");
    QStringList arguments = app.arguments();
    parser.process(arguments);
    const QString initialUrl = parser.positionalArguments().isEmpty() ?
        QStringLiteral("https://www.qt.io") : parser.positionalArguments().first();

    QQmlApplicationEngine engine;
    QQmlContext *context = engine.rootContext();
    context->setContextProperty(QStringLiteral("utils"), new Utils(&engine));
    context->setContextProperty(QStringLiteral("initialUrl"),
                                Utils::fromUserInput(label->text()));

    context->setContextProperty("thelable", label);

    QRect geometry = QGuiApplication::primaryScreen()->availableGeometry();
    if (!QGuiApplication::styleHints()->showIsFullScreen()) {
        const QSize size = geometry.size() * 4 / 5;
        const QSize offset = (geometry.size() - size) / 2;
        const QPoint pos = geometry.topLeft() + QPoint(offset.width(), offset.height());
        geometry = QRect(pos, size);
    }
    context->setContextProperty(QStringLiteral("initialX"), geometry.x());
    context->setContextProperty(QStringLiteral("initialY"), geometry.y());
    context->setContextProperty(QStringLiteral("initialWidth"), geometry.width());
    context->setContextProperty(QStringLiteral("initialHeight"), geometry.height());

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}

