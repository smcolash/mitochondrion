#ifndef __MITO_EXPORT_H__
#define __MITO_EXPORT_H__

#ifdef WIN32
#pragma warning (disable:4251)
#pragma warning (disable:4996)
#pragma warning (disable:4835)
#ifdef __DLL_EXPORT__
#define __DLL__ __declspec (dllexport)
#else
#define __DLL__ __declspec (dllimport)
#endif
#else
#define __DLL__
#endif

#endif
