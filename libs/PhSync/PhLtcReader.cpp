#include "PhLtcReader.h"

PhLtcReader::PhLtcReader(QObject *parent) :
    QObject(parent),
    _clock(PhTimeCodeType25)
{
    decoder = ltc_decoder_create(1920, 3840);
    init();
    PHDEBUG << "Reader created";
}

void PhLtcReader::init(QString input)
{
    QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();

    QList<QAudioDeviceInfo> list = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);

    foreach(QAudioDeviceInfo device, list)
    {
        if(device.deviceName() == input)
            info = device;
    }

    QAudioFormat format;
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleRate(48000);
    format.setChannelCount(1);
    format.setSampleSize(8);
    format.setSampleType(QAudioFormat::SignedInt);

    if(!info.isFormatSupported(format))
    {
        qDebug() << "unsupported";
        return;
    }

    _input = new QAudioInput(info, format);

    connect(_input, SIGNAL(notify()), this, SLOT(onNotify()));
    buffer2 = _input->start();
    _input->setNotifyInterval(10);
}

QList<QString> PhLtcReader::inputList()
{
    QList<QString> names;
    QList<QAudioDeviceInfo> list = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);

    foreach(QAudioDeviceInfo device, list)
        names.append(device.deviceName());

    return names;
}

PhClock *PhLtcReader::clock()
{
    return &_clock;
}

void PhLtcReader::onNotify()
{
    QByteArray array = buffer2->readAll();
    char max = 0;
    for(int i = 0; i < array.count(); i++)
    {
        if(array.at(i) > max)
            max = array.at(i);
    }

    ltc_decoder_write(decoder, (ltcsnd_sample_t*)array.data(), array.count(), position);

    LTCFrameExt frame;
    unsigned int *hhmmssff = new unsigned int[4];
    while(ltc_decoder_read(decoder, &frame))
    {
        PhFrame oldFrame = _clock.frame();
        hhmmssff[0] = frame.ltc.hours_tens * 10 + frame.ltc.hours_units;
        hhmmssff[1] = frame.ltc.mins_tens * 10 + frame.ltc.mins_units;
        hhmmssff[2] = frame.ltc.secs_tens * 10 + frame.ltc.secs_units;
        hhmmssff[3] = frame.ltc.frame_tens * 10 + frame.ltc.frame_units;
        _clock.setFrame(PhTimeCode::frameFromHhMmSsFf(hhmmssff, PhTimeCodeType25));
#warning TODO : fix me
        if(oldFrame < _clock.frame())
            _clock.setRate(1);
        else if(oldFrame == _clock.frame())
            _clock.setRate(0);
        else
            _clock.setRate(-1);

    }
    delete hhmmssff;

    position += array.count();
}
