#include <iostream>
#include "jpackage.h"
#include "tstrings.h"

struct OptionProperty {
    std::wstring option;
    std::wstring property1;
    std::wstring property2;
};

int main(int argc, char* argv[])
{
    OptionProperty optionProperties[] = {
        {L"--file-version", L"FIXEDFILEINFO_FileVersion", L"FIXEDFILEINFO_ProductVersion"},
        {L"--company-name", L"CompanyName", L""},
        {L"--file-description", L"FileDescription", L""},
        {L"--internal-name", L"InternalName", L""},
        {L"--legal-copyright", L"LegalCopyright", L""},
        {L"--original-filename", L"OriginalFilename", L""},
        {L"--product-name", L"ProductName", L""},
        {L"--product-version", L"ProductVersion", L"FileVersion"},
    };


    if (argc < 2) {
        std::cout << "Usage: ExeUpdater <exe> [options]" << std::endl;
        std::cout << "  --icon <ico file>" << std::endl;
        for (int i = 0; i < sizeof(optionProperties) / sizeof(OptionProperty); i++) {
            OptionProperty op = optionProperties[i];
            if (op.property2.empty()) {
                std::cout << "  " << op.option << " <arg> (sets " << op.property1
                    << " property)" << std::endl;
            }
            else {
                std::cout << "  " << op.option << " <arg> (sets " << op.property1
                    << " and " << op.property2 << " properties)" << std::endl;
            }
        }
        return 1;
    }

    // The executable we're going to work on
    std::wstring executable = tstrings::winStringToUtf16(argv[1]);

    // The icon we're going to set
    std::wstring icon;
    bool changeIcon = false;

    // The properties we're going to update
    tstring_array props;

    for (int c = 2; c + 1 < argc; c += 2) {
        std::wstring option = tstrings::winStringToUtf16(argv[c]);
        std::wstring arg = tstrings::winStringToUtf16(argv[c + 1]);
        bool optionKnown = false;
        if (option.compare(L"--icon") == 0) {
            optionKnown = true;
            changeIcon = true;
            icon = arg;
        }
        for (int i = 0; i < sizeof(optionProperties) / sizeof(OptionProperty); i++) {
            OptionProperty op = optionProperties[i];
            if (option.compare(op.option) == 0) {
                optionKnown = true;
                props.push_back(op.property1);
                props.push_back(arg);
                if (!op.property2.empty()) {
                    props.push_back(op.property2);
                    props.push_back(arg);
                }
            }
        }
        if (!optionKnown) {
            std::cout << "Unknown option: '" << option << "'" << std::endl;
            return 1;
        }
    }

    std::cout << "Executable: " << executable << std::endl;

    if (changeIcon) {
        std::cout << "Changing executable icon: " << icon << std::endl;
    }
    else {
        std::cout << "Not changing executable icon" << std::endl;
    }

    if (!props.empty()) {
        std::cout << "Updating " << props.size() / 2 << " properties" << std::endl;
    }
    else {
        std::cout << "Not updating any properties" << std::endl;
    }

    HANDLE handle = ExecutableRebrander_lockResource(executable);
    if (changeIcon) {
        ExecutableRebrander_iconSwap(handle, icon);
    }
    if (!props.empty()) {
        ExecutableRebrander_versionSwap(handle, props);
    }
    ExecutableRebrander_unlockResource(handle);
    return 0;
}