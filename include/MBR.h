/* 
 * File:   MBR.h
 * Author: hsk81
 *
 * Created on February 2, 2014, 10:26 PM
 */

#ifndef MBR_H
#define	MBR_H

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#include "Message.h"

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

namespace MBR {
    bool read_message(int fd, struct p2p_message *message, bool *read_ok);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#endif	/* MBR_H */
