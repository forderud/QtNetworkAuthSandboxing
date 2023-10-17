Reproducer for Qt NetworkAuth [low integrity level (low IL)](https://learn.microsoft.com/en-us/previous-versions/dotnet/articles/bb625960(v=msdn.10)) sandboxing issue on Windows.


# Steps to reproduce
Working baseline:
* Build project
* From a _regular_ command prompt: Run `QtNetworkAuthSandboxing.exe`
* Observe that `www.google.com` HTML sources are printed to console.

Failing case under low IL:
* Build project
* From an _administrator_ command prompt: Run `icacls QtNetworkAuthSandboxing.exe /setintegritylevel Low` to make the program run in low integrity level (low IL).
* From a _regular_ command prompt: Run `QtNetworkAuthSandboxing.exe`
* Observe that the program freezes without any console output.


## Root cause
The root cause appear to be the `CertOpenSystemStore(0, L"ROOT")` calls in [qtls_schannel.cpp](https://github.com/qt/qtbase/blob/6.6/src/plugins/tls/schannel/qtls_schannel.cpp). These call will fail when running under low IL due to lack of write access to the `HKCU\SOFTWARE\Microsoft\SystemCertificates\ROOT` registry key.


## Proposed fix
Replace the [`CertOpenSystemStore(0, L"ROOT")`](https://learn.microsoft.com/en-us/windows/win32/api/wincrypt/nf-wincrypt-certopensystemstorew) calls with [`CertOpenStore(CERT_STORE_PROV_SYSTEM, 0, NULL, CERT_STORE_READONLY_FLAG | CERT_SYSTEM_STORE_CURRENT_USER, L"ROOT")`](https://learn.microsoft.com/en-us/windows/win32/api/wincrypt/nf-wincrypt-certopenstore) so that the certificate store is opened in read-only mode.


# Similar issues
[enum_certificates() fails with ERROR_ACCESS_DENIED if python.exe run with low integrity level](https://github.com/python/cpython/issues/70127) that was fixed by [open the cert store readonly](https://hg.python.org/cpython/rev/3cddcf471c70).
