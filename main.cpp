#include <QtCore>
#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <stdexcept>

#include <Windows.h>
#pragma comment(lib, "Crypt32.lib")

//#define REPRODUCE_ROOT_CAUSE
//#define FIX_ROOT_CAUSE

/** Reproducer for low integrity level (low IL) problem. */
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

        // this line will deadlock
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

/** Steps to reproduce:
* From an _administrator_ command prompt: Run `icacls QtNetworkAuthSandboxing.exe /setintegritylevel Low`.
* From a _regular_ command prompt: Run `QtNetworkAuthSandboxing.exe`
* Observe that the program freezes without any console output.
*/
int main(int argc, char *argv[]) {
#ifdef REPRODUCE_ROOT_CAUSE
    // failing call from https://github.com/qt/qtbase/blob/6.5/src/plugins/tls/schannel/qtls_schannel.cpp#L301
    HCERTSTORE store = CertOpenSystemStoreW(0, L"ROOT");
    DWORD err = GetLastError(); // 5 = Access is denied.
    assert(store);
#elif defined FIX_ROOT_CAUSE
    // possible fix
    HCERTSTORE store = CertOpenStore(CERT_STORE_PROV_SYSTEM, 0, NULL, CERT_STORE_READONLY_FLAG | CERT_SYSTEM_STORE_CURRENT_USER, L"ROOT");
    assert(store);
#else
    // Full QNetworkAccessManager reproducer
    QCoreApplication a(argc, argv);

    HttpClient httpClient(nullptr);
    QByteArray result = httpClient.Get(QUrl("https://www.google.com/"));
    qInfo() << result;
#endif
    return 0;
}
