#ifndef _TTAPI_H_INCLUDED_
#define _TTAPI_H_INCLUDED_

#include <windows.h>

/*
	Trivial (and dumb) Threads API
*/

#ifdef _GPA_ENABLED
	#include <tal.h>
#endif // _GPA_ENABLED


typedef VOID (*PTTAPI_WORKER_FUNC)( LPVOID lpWorkerParameters );
typedef PTTAPI_WORKER_FUNC LPPTTAPI_WORKER_FUNC;


extern "C"  {

	// Initializes subsystem
	// Returns zero for error, and number of workers on success
	PARTICLES_API DWORD  ttapi_Init( /*_processor_info* ID */);

	// Destroys subsystem
	PARTICLES_API VOID  ttapi_Done();

	// Return number of workers
	PARTICLES_API DWORD  ttapi_GetWorkersCount();

	// Adds new task
	// No more than TTAPI_HARDCODED_THREADS should be added
	PARTICLES_API VOID  ttapi_AddWorker( LPPTTAPI_WORKER_FUNC lpWorkerFunc , LPVOID lpvWorkerFuncParams );

	// Runs and wait for all workers to complete job
	PARTICLES_API VOID  ttapi_RunAllWorkers();

}

#endif // _TTAPI_H_INCLUDED_