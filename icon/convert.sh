#!/bin/bash

# see https://learn.microsoft.com/en-us/windows/win32/uxguide/vis-icons?redirectedfrom=MSDN#size-requirements

inkscape -e s16.png -h 16  ic_settings_48px.svg
inkscape -e s32.png -h 32  ic_settings_48px.svg
inkscape -e s48.png -h 48  ic_settings_48px.svg
inkscape -e s256.png -h 256  ic_settings_48px.svg

convert s16.png s32.png s48.png s256.png icon.ico
