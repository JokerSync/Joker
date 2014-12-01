#include "PhGraphicStrip/PhTrack.h"

PhTrack::PhTrack()
{
	_stripTextModel = new PhStripTextModel();
	_stripPeopleModel = new PhStripTextModel();
	_offDetectModel = new PhNextPeopleModel();
	_semiOffDetectModel = new PhNextPeopleModel();
	_arrowUpDetectModel = new PhNextPeopleModel();
	_arrowDownDetectModel = new PhNextPeopleModel();
}

PhStripTextModel * PhTrack::stripTextModel() const {
	return _stripTextModel;
}

PhStripTextModel * PhTrack::stripPeopleModel() const {
	return _stripPeopleModel;
}

PhNextPeopleModel * PhTrack::offDetectModel() const {
	return _offDetectModel;
}

PhNextPeopleModel * PhTrack::semiOffDetectModel() const {
	return _semiOffDetectModel;
}

PhNextPeopleModel * PhTrack::arrowUpDetectModel() const {
	return _arrowUpDetectModel;
}

PhNextPeopleModel * PhTrack::arrowDownDetectModel() const {
	return _arrowDownDetectModel;
}
