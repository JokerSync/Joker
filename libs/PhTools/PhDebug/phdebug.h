#ifndef PHDEBUG_H
#define PHDEBUG_H
#include <QString>
#include <QObject>

namespace test
{
enum MethodLogLevel
{
    None = 0,	// 0
    Basic = 1,	// 1
    Action = 1 << 1,	// 2
    Extra = 1 << 2,	// 4
    Process = 1 << 3,	// 8
    Render = 1 << 4,	// 16
    Clock = 1 << 5,	// 32
    LockPainter = 1 << 6,	// 64
    PresentVideo = 1 << 7,	// 128
    Audio = 1 << 8,	// 256
    LockAudio = 1 << 9,	// 512
    OnChange = 1 << 10,	// 1024
    LTC = 1 << 11,	// 2048
    LockDoc = 1 << 12,	// 4096
    Time = 1 << 13,	// 8192
    DoCommand = 1 << 14,	// 16384
    Shape = 1 << 15,	// 32768
    WinForm = 1 << 16,	// 65536
    LockPlayer = 1 << 17,
    RunEdit = 1 << 18,
    Sony = 1 << 19
};
enum MethodLogType
{
    Empty = 0,
    Debug = 1,
    Console = 2,
    File = 4,
    Trace = 8
};

}

void PhDebug(QString msg);


#endif // PHDEBUG_H
