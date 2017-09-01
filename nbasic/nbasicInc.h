#pragma once

#ifdef _DEBUG
#pragma comment(lib,"nbasicd.lib") 
#pragma message("Automatically linking with nbasicd debug lib")
#else
#pragma comment(lib,"nbasic.lib") 
#pragma message("Automatically linking with nbasic release lib")
#endif

#include "Pointer.h"
#include "interfaces.h"
#include "pair.h"
#include "List.h"
#include "Dictionary.h"
#include "OperationCopyFrom.h"