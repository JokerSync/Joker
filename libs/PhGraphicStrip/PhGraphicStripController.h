#ifndef PHGRAPHICSTRIPCONTROLLER_H
#define PHGRAPHICSTRIPCONTROLLER_H


#include "PhStrip/PhStripDoc.h"
#include "PhGraphic/PhGraphicController.h"


class PhGraphicStripController : public PhGraphicController
{
public:
    PhGraphicStripController();
    PhStripDoc getDoc();
    void loadSettings();
    PhString getLastFile();
    void setLastFile(PhString fileName);
    bool openDoc(PhString fileName);
    bool getNaturalScrollPref();

signals:
    void docChanged();
    void onRateChanged(float rate);
    void onPositionChanged(int move);

public slots:
    //Testing
    void onRateChanged2(float rate);
    void onPositionChanged2(int move);
    //End-Testing

private:
    PhString _settingsPath;
    QSettings::Format _settingsFormat;
    QSettings * _settings;
    PhStripDoc * _doc;
};

#endif // PHGRAPHICSTRIPCONTROLLER_H
