# QtNetworkAuthSandboxing

Reproducer for Qt NetworkAuth sandboxing issue on Windows.


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

# Similar issues
[enum_certificates() fails with ERROR_ACCESS_DENIED if python.exe run with low integrity level](https://github.com/python/cpython/issues/70127) that was fixed by [open the cert store readonly](https://hg.python.org/cpython/rev/3cddcf471c70).
