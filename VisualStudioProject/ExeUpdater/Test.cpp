#include <iostream>
#include "jpackage.h"
#include "tstrings.h"

int main(int argc, char* argv[])
{
    if (argc < 3) {
        std::cout << "Usage: ExeUpdater <exe> <icon>" << std::endl;
        return 1;
    }

    std::cout << "Changing executable icon" << std::endl;

    std::wstring executable = tstrings::winStringToUtf16(argv[1]);
    std::wstring icon = tstrings::winStringToUtf16(argv[2]);

    tstring_array props;
    props.push_back(L"FIXEDFILEINFO_FileVersion");
    props.push_back(L"1.1.0.0");
    props.push_back(L"CompanyName");
    props.push_back(L"Mobanisto");
    props.push_back(L"ProductName");
    props.push_back(L"The Product");
    props.push_back(L"ProductVersion");
    props.push_back(L"1.1.0");

    std::cout << "Executable: " << executable << std::endl;
    std::cout << "Icon: " << icon << std::endl;
    HANDLE handle = ExecutableRebrander_lockResource(executable);
    ExecutableRebrander_iconSwap(handle, icon);
    ExecutableRebrander_versionSwap(handle, props);
    ExecutableRebrander_unlockResource(handle);
    return 0;
}