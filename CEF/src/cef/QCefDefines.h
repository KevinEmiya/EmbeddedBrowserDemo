#ifndef QCEFDEFINES_H
#define QCEFDEFINES_H

enum CEF_KEY_CODE
{
#ifdef CEF_LINUX
    CEF_F5 = 71,
    CEF_F9 = 75,
    CEF_F11 = 95,
    CEF_F12 = 96
#else
    CEF_F5 = 116,
    CEF_F9 = 120,
    CEF_F11 = 122,
    CEF_F12 = 123
#endif
};

#endif // QCEFDEFINES_H
