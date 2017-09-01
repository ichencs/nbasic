#pragma once

#ifdef _DEBUG
#pragma comment(lib,"nbasicd.lib") 
#pragma message("Automatically linking with nbasicd debug lib")
#else
#pragma comment(lib,"nbasic.lib") 
#pragma message("Automatically linking with nbasic release lib")
#endif

#include "Basic.h"
#include "Pointer.h"
#include ".\Collections\interfaces.h"
#include ".\Collections\pair.h"
#include ".\Collections\List.h"
#include ".\Collections\Dictionary.h"
#include ".\Collections\OperationCopyFrom.h"