#ifndef _E_PID_H_

#define _E_PID_H_

#include "../Defines/CommonDefines.hpp"

typedef enum _EPid : u8
{
    EPid_ProcessController  = 0,
    EPid_ModelController    = 1
} EPid;

#endif
