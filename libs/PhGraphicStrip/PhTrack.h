#ifndef PHTRACK_H
#define PHTRACK_H

#include "PhGraphicStrip/PhNextPeopleModel.h"

#include "PhStrip/PhStripTextModel.h"

class PhTrack
{
public:
	PhTrack();

	PhStripTextModel * stripTextModel() const;
	PhStripTextModel * stripPeopleModel() const;
	PhNextPeopleModel * offDetectModel() const;
	PhNextPeopleModel * semiOffDetectModel() const;
	PhNextPeopleModel * arrowUpDetectModel() const;
	PhNextPeopleModel * arrowDownDetectModel() const;

private:
	PhStripTextModel *_stripTextModel;
	PhStripTextModel *_stripPeopleModel;
	PhNextPeopleModel *_offDetectModel;
	PhNextPeopleModel *_semiOffDetectModel;
	PhNextPeopleModel *_arrowUpDetectModel;
	PhNextPeopleModel *_arrowDownDetectModel;
};

#endif // PHTRACK_H
