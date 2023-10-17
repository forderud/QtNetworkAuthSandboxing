# QtNetworkAuthSandboxing

Reproducer for Qt NetworkAuth sandboxing issue on Windows.


# Steps to reproduce
* Build project
* From a _regular_ command prompt: Run `QtNetworkAuthSandboxing.exe`
* Observe that `www.google.com` HTML sources are printed to console.
* From an _administrator_ command prompt: Run `[icacls](https://learn.microsoft.com/en-us/windows-server/administration/windows-commands/icacls) QtNetworkAuthSandboxing.exe /setintegritylevel Low` to make the program run in low integrity level (low IL).
* From a _regular_ command prompt: Run `QtNetworkAuthSandboxing.exe`
* Observe that the program freezes without any console output.
