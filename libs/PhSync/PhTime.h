/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHTIME_H
#define PHTIME_H

#include <QtGlobal>

/** @brief Time unit in ticks : 1 second = 24000 ticks */
typedef qint64 PhTime;

/** @brief PhTimeScale is a type used for time scale value */
typedef int PhTimeScale;

/** @brief PhRate is a type used for rate value */
typedef double PhRate;

/** @brief Type for the timecode frame */
typedef long PhFrame;

/** @brief The number of time unit per seconds */
#define PHTIMEBASE 24000

/** @brief The maximum PhTime value */
#define PHTIMEMAX std::numeric_limits<PhTime>::max()

/** @brief The minimum PhTime value */
#define PHTIMEMIN std::numeric_limits<PhTime>::min()

/** @brief The maximum PhFrame value */
#define PHFRAMEMAX std::numeric_limits<PhFrame>::max()

/** @brief The minimum PhFrame value */
#define PHFRAMEMIN std::numeric_limits<PhFrame>::min()

#endif // PHTIME_H
