/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHFEEDBACKSETTINGS_H
#define PHFEEDBACKSETTINGS_H

#include "PhTools/PhGenericSettings.h"

/**
 * @brief The PhFeedbackSettings class
 *
 */
class PhFeedbackSettings
{
public:
	/**
	 * @brief The setting file path
	 * @return The path
	 */
	virtual QString settingsFileName() = 0;

	/**
	 * @brief emailList
	 * @return The users list.
	 */
	virtual QStringList emailList() = 0;
	/**
	 * @brief setEmailList
	 *
	 * Replace the current list with the provided one.
	 *
	 * @param emails
	 */
	virtual void setEmailList(QStringList emails) = 0;

};
#endif // PHFEEDBACKSETTINGS_H
