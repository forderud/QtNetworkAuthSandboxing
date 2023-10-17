
:: TODO: Update path to match installed Qt version
set PATH=%PATH%;C:\Qt\6.5.1\msvc2019_64\bin

:: Create Visual Studio project
cmake.exe  .

:: Open in Visual subst
start QtNetworkAuthSandboxing.sln
