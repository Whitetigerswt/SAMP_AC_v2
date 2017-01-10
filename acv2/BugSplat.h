
///////////////////////////////////////////////////////////////////////////////
//
// BugSplat.h
//
// This header file contains the code used to add BugSplat crash reporting to
// native Windows applications.
//
// For the simplest implementation read the documentaion at 
// https://www.bugsplat.com
//
//
// Copyright 2003-2016, BugSplat 
// All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

//! \mainpage
//! Integrating BugSplat crash reporting into your native Windows application is easy!
//! The following example code is from the myConsoleCrasher application:
//! \include  myConsoleCrasher.cpp


#ifndef BUGSPLAT_H
#define BUGSPLAT_H

#ifdef BUGSPLAT_EXPORTS
    #define MDS_EXPORT __declspec(dllexport)
#else
    #define MDS_EXPORT __declspec(dllimport)
#endif

#include <Windows.h>

#ifdef __cplusplus

//! If a callback function is registered with MiniDmpSender
//! it is notified when data or information is required from the main app
//! A list of callback codes appears at the end of this file...
typedef bool (__cdecl *MiniDmpSenderCallback)(UINT nCode, LPVOID lVal1, LPVOID lVal2);

// The following definitions are valid only for C++ clients.
// C clients need to use the methods listed after the MiniDmpSender class.

// forward declare internal class
class MiniDumper;

//! The MiniDmpSender class is used to add BugSplat crash reporting to your app.
class MDS_EXPORT MiniDmpSender 
{
public:
    //! @param szDatabase database name on bugsplat.com
    //! @param szApp application name as supplied with PDB files on bugsplat.com
    //! @param szVersion version identifier as supplied with PDB files on bugsplat.com
    //! @param szAppIdentifier generic field to contain app details (locale, build #, etc...)
    //! @param dwFlags Option flags.  see MDSF_FLAG... defines
    MiniDmpSender(unsigned short const *szDatabase,
                  unsigned short const *szApp,
                  unsigned short const *szVersion,
                  unsigned short const *szAppIdentifier = NULL,
                  DWORD dwFlags = 0x04 /* MDSF_PREVENTHIJACKING */
                  );

    MiniDmpSender(LPCSTR szDatabase,
                  LPCSTR szApp,
                  LPCSTR szVersion,
                  LPCSTR szAppIdentifier = NULL,
                  DWORD dwFlags = 0x04 /* MDSF_PREVENTHIJACKING */
                  );

    MiniDmpSender(__wchar_t const *szDatabase,
                  __wchar_t const *szApp,
                  __wchar_t const *szVersion,
                  __wchar_t const *szAppIdentifier = NULL,
                  DWORD dwFlags = 0x04 /* MDSF_PREVENTHIJACKING */
                  );
    virtual ~MiniDmpSender();
public:
    //! To disable / enable the unhandled exception filter call this function with the desired mode
    //! This function is not valid when MDSF_PREVENTHIJACKING is used.
    bool enableExceptionFilter(bool enable = true);

    //! Returns the current state of the unhandled exception filter, ie the last value sent to enableExceptionFilter.
    bool isExceptionFilterEnabled() const;

    //! Limited support for full memory minidumps: dump will be created and a message box displayed indicating path to dump file; app will then exit w/o reporting crash.
    bool enableFullMemoryDumpAndExit(bool enable = true);
    //! Returns the current full minidump state 
    bool isFullMemoryDumpAndExitEnabled() const;

    //! Set the option flags; see MDSF_FLAG... defines below.
    bool setFlags( DWORD dwFlags );
    //! Get the option flags.
    DWORD getFlags() const;

    //! Use to change the version string at runtime.
    void resetVersionString(LPCSTR szVersion);
    void resetVersionString(const unsigned short * wszVersion);
    void resetVersionString(const __wchar_t * wszVersion);

    //! Use to change the generic app identifier field at runtime.
    void resetAppIdentifier(LPCSTR szDescription);
    void resetAppIdentifier(const unsigned short * wszDescription);
    void resetAppIdentifier(const __wchar_t * wszDescription);

    //! Send additional file(s) along with the crash report; may be called repeatedly to send multiple files.
    void sendAdditionalFile(LPCSTR szPath);
    void sendAdditionalFile(const unsigned short * wszPath);
    void sendAdditionalFile(const __wchar_t * wszPath);

    //! Use to set full path for crash report zip file; default is a path in the %TEMP% folder.
    void setUserZipPath(LPCSTR szPath);
    void setUserZipPath(const unsigned short * wszPath);
    void setUserZipPath(const __wchar_t * wszPath);

    //! Use to set full path for BsSndRpt's resource DLL (allows dialog customizations, e.g. language); default is ./BugSplatRc.dll (or ./BugSplatRc64.dll).
	void setResourceDllPath(LPCSTR szPath);
	void setResourceDllPath(const unsigned short * wszPath);
	void setResourceDllPath(const __wchar_t * wszPath);

    //! Use to set the default user name.  Useful for quiet-mode applications that don't prompt for user/email/description at crash time.
    void setDefaultUserName(LPCSTR szName);
    void setDefaultUserName(const unsigned short * wszName);
    void setDefaultUserName(const __wchar_t * wszName);

    //! Use to set the default user email.  Useful for quiet-mode applications that don't prompt for user/email/description at crash time.
    void setDefaultUserEmail(LPCSTR szEmail);
    void setDefaultUserEmail(const unsigned short * wszEmail);
    void setDefaultUserEmail(const __wchar_t * wszEmail);

    //! Use to set the default user description.  Useful for quiet-mode applications that don't prompt for user/email/description at crash time.
    void setDefaultUserDescription(LPCSTR szDescription);
    void setDefaultUserDescription(const unsigned short * wszDescription);
    void setDefaultUserDescription(const __wchar_t * wszDescription);

    //! Use to send a BugSplat crash report outside of the unhandled exception filters.
    //! For example, you could send a report directly from your own try/catch clause.
    void createReport();
    void createReport(EXCEPTION_POINTERS * pExcepInfo);
    //! Use to send an XML stack trace to BugSplat, bypassing minidump creation.
    void createReport(LPCSTR szStackTracePath);
    void createReport(const unsigned short * wszStackTracePath);
    void createReport(const __wchar_t * wszStackTracePath);
    //! Use to send a report and exit
    inline void createReportAndExit() { setFlags(getFlags() | 0x02/*MDSF_FORCEEXIT*/); throw NULL; }

    //! Use with MDSF_CUSTOMEXCEPTIONFILTER flag.
    LONG unhandledExceptionHandler( PEXCEPTION_POINTERS pExceptionInfo );

    //! Use to assign a callback function for sharing additional information with the app after a crash occurs.
    void setCallback(MiniDmpSenderCallback fnCallback);

	//! Get path to minidump.  
	void getMinidumpPath(LPSTR buffer, size_t len);
	void getMinidumpPath(unsigned short * buffer, size_t len);
	void getMinidumpPath(__wchar_t * buffer, size_t len);

	//! Internal method.
    LPVOID imp();

private:
    MiniDumper * m_pMiniDumper;
};

#endif //__cplusplus

// ***************************
// The following methods are intended for use by C-only clients. Non-UNICODE only is supported
// for C clients. note functionality in this instance is a little more limited than using the C++ class directly
#ifdef __cplusplus
extern "C"
{
#endif
    int             CreateMiniDmpSender(const char* szDatabase, const char* szApp, const char* szVersion, const char* szAppId /*recommend:NULL*/, unsigned int dwFlags /*recommend:MDSF_PREVENTHIJACKING*/);
    void            DestroyMiniDmpSender(int handle);
    unsigned int    MDSGetFlags(int handle);
    void            MDSSetFlags(int handle, unsigned int dwFlags);
    void            MDSResetVersionString(int handle, const char* szVersion);
    void            MDSResetAppIdentifier(int handle, const char* szDescription);
    void            MDSStoreUserLog(int handle, const char* szPath);
#ifdef __cplusplus
}
#endif


// ***************************
// Definitions for the dwFlags parameter of MiniDmpSender constructor and the getFlags/setFlags methods.
// The non-interactive flag is for applications that run unattended or without a user interface.  

//! This flag automatically submits a crash report, without prompting the user with a dialog box.
#define MDSF_NONINTERACTIVE     0x0001

//! This flag will force the application to exit after posting a BugSplat report.  The default behavior
//! is to continue calling the other unhandled exception filters in the unhandled exception chain.
#define MDSF_FORCEEXIT          0x0002

//! This flag prevents other threads in the process from hijacking the Exception filter.
//! It is useful when you want to prevent 3rd party libraries included with your application
//! or the Operating System from disabling BugSplat integration.
//! It is *recommended* that you use this setting
#define MDSF_PREVENTHIJACKING   0x0004

//! This flag launches the BugSplatHD.exe process with the current process id.
//! The process runs in low priority, and periodically performs tests on the main process id 
//! to determine if it has hung.
//! The burden on the OS is extremely low, and will only consume a few CPU cycles 
//! each minute.
#define MDSF_DETECTHANGS        0x0008

//! The guard memory flag causes the BugSplat library to pre-allocate a 4MB chunk of data
//! on the heap. When a crash is encountered, this heap data is deleted and might be useful
//! in low memory scenarios
#define MDSF_USEGUARDMEMORY     0x0010

//! BugSplat operates using the SetUnhandledExceptionFilter method. Use this flag if you want to
//! use your own exception handling mechanism instead.
//! To initiate a BugSplat crash report, you need to call unhandledExceptionHandler within your own
//! exception filter method
#define MDSF_CUSTOMEXCEPTIONFILTER 0x0020

//! EXPERIMENTAL
//! Use this flag to suspend all threads in the process prior to capturing the
//! crash report data. This might be used to prevent security libraries from detecting
//! the debugging commands used by BugSplat and terminating the app before the 
//! crash has a chance to be reported.
#define MDSF_SUSPENDALLTHREADS  0x0800

//! These flags control output of the BugSplat log output, typically used for development/debugging.
//! If MDSF_LOGCONSOLE is set, logging information is sent to the debugger console.
//! If MDSF_LOGFILE is set, logging information is added to the file 'bugsplat.log' in the system temp directory and included in the crash report zip file.
#define MDSF_LOGCONSOLE         0x1000
#define MDSF_LOGFILE            0x2000
#define MDSF_LOG_VERBOSE        0x4000

//! Definitions for the BugSplat exception call back codes.

//! nCode: MDSCB_EXCEPTIONCODE
//! lVal1: EXCEPTION_RECORD*, pointer to the exception record responsible for the crash
//!           Note: this might be NULL if report created using 'void createReport()' method
//! lVal2: Reserved
#define MDSCB_EXCEPTIONCODE            0x0050

//! nCode: MDSCB_GETADDITIONALFILECOUNT
//! lVal1: long*, pointer to long variable obtaining the additional file count
//! lVal2: Reserved
//#define MDSCB_GETADDITIONALFILECOUNT   0x0100  // obsolete:  please use sendAdditionalFile() instead
#endif //~BUGSPLAT_H
