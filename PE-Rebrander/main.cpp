/*
 * Copyright (c) 2022 Mobanisto UG (haftungsbeschraenkt). All rights reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.  Mobanisto designates this
 * particular file as subject to the "Classpath" exception as provided
 * by Oracle in the LICENSE file that accompanied this code.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version
 * 2 along with this work; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <iostream>
#include "jpackage.h"
#include "tstrings.h"

struct OptionProperty {
    std::wstring option;
    std::wstring property1;
    std::wstring property2;
};

BOOL FileExists(LPCTSTR szPath)
{
    DWORD dwAttrib = GetFileAttributes(szPath);

    return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
        !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

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

void printUsage()
{
    std::cout << "Usage: PE-Rebrander <exe> [options]" << std::endl;
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
}

int main(int argc, char* argv[])
{

    if (argc < 2) {
        printUsage();
        return 1;
    }

    // The executable we're going to work on
    std::wstring executable = tstrings::winStringToUtf16(argv[1]);

    // The icon we're going to set
    std::wstring icon;
    bool changeIcon = false;

    // The properties we're going to update
    tstring_array props;

    for (int c = 1; c < argc; c++) {
        std::wstring option = tstrings::winStringToUtf16(argv[c]);
        if (option.compare(L"--version") == 0) {
            std::cout << "PE-Rebrander version 1.0.0" << std::endl;
            return 0;
        }
        else if (option.compare(L"--help") == 0) {
            printUsage();
            return 0;
        }
    }

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

    if (!FileExists(executable.c_str())) {
        std::cout << "executable file not found" << std::endl;
        return 1;
    }

    if (changeIcon) {
        std::cout << "Changing executable icon: " << icon << std::endl;
        if (!FileExists(icon.c_str())) {
            std::cout << "icon file not found" << std::endl;
            return 1;
        }
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