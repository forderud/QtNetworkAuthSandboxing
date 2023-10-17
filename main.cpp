#include <QtCore>
#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <stdexcept>

#include <Windows.h>
#pragma comment(lib, "Crypt32.lib")

#define REPRODUCE_ROOT_CAUSE


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
#ifdef REPRODUCE_ROOT_CAUSE
    // call from https://github.com/qt/qtbase/blob/6.5/src/plugins/tls/schannel/qtls_schannel.cpp#L301
    HCERTSTORE store = CertOpenSystemStoreW(0, L"ROOT");
    assert(store);
#else
    QCoreApplication a(argc, argv);

    HttpClient httpClient(nullptr);
    QByteArray result = httpClient.Get(QUrl("https://www.google.com/"));
    qInfo() << result;
#endif
    return 0;
}
