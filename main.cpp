#include <QtCore>
#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <stdexcept>


class HttpClient {
public:
    HttpClient(QObject * parent) : m_mgr(parent) {
    }

    QByteArray Get(QUrl url) {
        QEventLoop loop;
        QObject::connect(&m_mgr, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);

        // initiate HTTP GET request
        QNetworkRequest request(url);
        request.setMaximumRedirectsAllowed(0); // trigger failure on SAML login screens
        QNetworkReply* reply = m_mgr.get(request);

        // block until the download have completed
        loop.exec();

        auto err = reply->error();
        if (err == QNetworkReply::TooManyRedirectsError)
            throw std::runtime_error("TooManyRedirectsError");
        else if (err != QNetworkReply::NoError)
            throw std::runtime_error("HTTP GET failed");

        QByteArray data = reply->readAll();
        return data;
    }

private:
    QNetworkAccessManager m_mgr;
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    HttpClient httpClient(nullptr);
    QByteArray result = httpClient.Get(QUrl("https://www.google.com/"));
    qInfo() << result;

    return 0;
}
