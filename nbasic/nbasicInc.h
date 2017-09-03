#pragma once

#ifdef _DEBUG
#pragma comment(lib,"nbasicd.lib") 
#pragma message("Automatically linking with nbasicd debug lib")
#else
#pragma comment(lib,"nbasic.lib") 
#pragma message("Automatically linking with nbasic release lib")
#endif

#include "Basic.h"
#include "nchar.h"

#include "Pointer.h"
#include "String.h"
#include "Console.h"

#include ".\Stream\Accessor.h"
#include ".\Stream\BroadcastStream.h"
#include ".\Stream\CacheStream.h"
#include ".\Stream\CharFormat.h"
#include ".\Stream\FileStream.h"
#include ".\Stream\Interfaces.h"
#include ".\Stream\memorystream.h"
#include ".\Stream\recorderstream.h"

#include ".\Collections\interfaces.h"
#include ".\Collections\pair.h"
#include ".\Collections\List.h"
#include ".\Collections\Dictionary.h"
#include ".\Collections\OperationCopyFrom.h"