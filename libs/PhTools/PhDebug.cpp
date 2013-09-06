/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#include "PhDebug.h"

// Global static pointer used to ensure a single instance of the class.
PhDebug* PhDebug::d = NULL;
// This function is called to create an instance of the class.
// Calling the constructor publicly is not allowed. The constructor
// is private and is only called by this Instance function.


PhDebug PhDebug::instance()
{
   if (!d)   // Only allow one instance of class to be generated.
      d = new PhDebug;
   return * d;
}
