/*
 * Copyright (c) 2011, 2020, Oracle and/or its affiliates. All rights reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.  Oracle designates this
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
 *
 * Please contact Oracle, 500 Oracle Parkway, Redwood Shores, CA 94065 USA
 * or visit www.oracle.com if you need additional information or have any
 * questions.
 */

#include "ResourceEditor.h"
#include "IconSwap.h"
#include "VersionInfo.h"
#include "tstrings.h"

HANDLE ExecutableRebrander_lockResource(std::wstring executable) {
    return ResourceEditor::FileLock(executable).ownHandle(false).get();
}

void ExecutableRebrander_unlockResource(HANDLE resourceLock) {
    ResourceEditor::FileLock(resourceLock).ownHandle(true);
}

int ExecutableRebrander_iconSwap(HANDLE resourceLock, std::wstring iconTarget) {
    const ResourceEditor::FileLock lock(resourceLock);

    if (ChangeIcon(lock.get(), iconTarget)) {
        return 0;
    }

    return 1;
}

int ExecutableRebrander_versionSwap(HANDLE resourceLock, tstring_array props) {
    VersionInfo vi;

    tstring_array::const_iterator it = props.begin();
    tstring_array::const_iterator end = props.end();
    for (; it != end; ++it) {
        const tstring name = *it;
        const tstring value = *++it;
        vi.setProperty(name, value);
    }

    const ResourceEditor::FileLock lock(resourceLock);
    vi.apply(lock);

    return 0;
}