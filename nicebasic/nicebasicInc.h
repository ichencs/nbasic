#pragma once

#ifdef _DEBUG
#pragma comment(lib,"nicebasicd.lib") 
#pragma message("Automatically linking with nicebasicd debug lib")
#else
#pragma comment(lib,"nicebasic.lib") 
#pragma message("Automatically linking with nicebasic release lib")
#endif

#include "Pointer.h"
#include "interfaces.h"
#include "pair.h"
#include "List.h"
#include "Dictionary.h"