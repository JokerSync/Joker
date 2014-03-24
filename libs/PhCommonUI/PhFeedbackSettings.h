/**
* Copyright (C) 2012-2014 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#ifndef PHFEEDBACKSETTINGS_H
#define PHFEEDBACKSETTINGS_H

#include "PhTools/PhGenericSettings.h"

class PhFeedbackSettings
{
public:
	/**
	 * @brief The setting file path
	 * @return The path
	 */
	virtual QString settingsFileName() = 0;

	virtual QStringList emailList() = 0;
	virtual void setEmailList(QStringList emails) = 0;

};
#endif // PHFEEDBACKSETTINGS_H
