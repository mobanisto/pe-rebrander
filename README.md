# PE-Rebrander

PE-Rebrander is a tool for rebranding Portable Executable (PE) files such
as `.exe` or `.dll` files.
This software is based on the native code from OpenJDK's JPackage.
It has been copied from [openjdk/jdk](https://github.com/openjdk/jdk) at
[5795c760db5](https://github.com/openjdk/jdk/commit/5795c760db5bbfd7ff3b56a0c6236827526df70a)
and has been adapted for this single purpose.

## Build instructions

Currently builing is only partly automated. First build the executable from Visual Studio.
Make sure to build for x86 if you are on a 64 bit system.

Afterwards, rebrand and package the executable and accompanying files using Gradle:

    ./gradlew distZip

## Release checklist

When releasing a new version, make sure to increment the version code in both locations:
* `main.cpp` - at the location where the version is printed to stdout
* `build.gradle` - the `version` variable
