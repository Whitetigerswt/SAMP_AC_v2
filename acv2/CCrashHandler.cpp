#include "CCrashHandler.h"
#include "CClientUpdater.h"
#include <string>
#include "CLog.h"

int CALLBACK CrashCallback(CR_CRASH_CALLBACK_INFO* pInfo)
{
	// The application has crashed!

	// Add additional log file.
	CLog log = CLog("exception_info.txt");
	log.Write("Exception Type: %d", pInfo->pExceptionInfo->exctype);

	if (pInfo->pExceptionInfo->exctype == CR_CPP_INVALID_PARAMETER)
	{
		log.Write("Function: %s", pInfo->pExceptionInfo->function);
		log.Write("Line: %d", pInfo->pExceptionInfo->line);
		log.Write("Expression: %s", pInfo->pExceptionInfo->expression);
		log.Write("File: %s", pInfo->pExceptionInfo->file);
	}

	if (pInfo->pExceptionInfo->exctype == CR_SEH_EXCEPTION)
	{
		log.Write("Code: %d", pInfo->pExceptionInfo->code);
	}

	if (pInfo->pExceptionInfo->exctype == CR_CPP_SIGFPE)
	{
		log.Write("fpe_subcode: %d", pInfo->pExceptionInfo->fpe_subcode);
	}

	log.Write("EAX: 0x%x || EBX: 0x%x", pInfo->pExceptionInfo->pexcptrs->ContextRecord->Eax, pInfo->pExceptionInfo->pexcptrs->ContextRecord->Ebx);
	log.Write("ECX: 0x%x || EDX: 0x%x", pInfo->pExceptionInfo->pexcptrs->ContextRecord->Ecx, pInfo->pExceptionInfo->pexcptrs->ContextRecord->Edx);
	log.Write("EDI: 0x%x || ESI: 0x%x", pInfo->pExceptionInfo->pexcptrs->ContextRecord->Edi, pInfo->pExceptionInfo->pexcptrs->ContextRecord->Esi);
	log.Write("EBP: 0x%x || ESP: 0x%x", pInfo->pExceptionInfo->pexcptrs->ContextRecord->Ebp, pInfo->pExceptionInfo->pexcptrs->ContextRecord->Esp);

	log.Write("EFlags: 0x%x", pInfo->pExceptionInfo->pexcptrs->ContextRecord->EFlags);

	// Return CR_CB_DODEFAULT to generate error report
	return CR_CB_DODEFAULT;
}

void SetCrashCallback()
{
	// Add crash callback
	crSetCrashCallback(CrashCallback, NULL);
}

CCrashHandler::CCrashHandler()
{
	// Create new crash info.
	CR_INSTALL_INFO info;

	// Reset it's memory.
	memset(&info, 0, sizeof(CR_INSTALL_INFO));
	info.cb = sizeof(CR_INSTALL_INFO);

	info.pszAppName = "ACv2_Client";

	// Format the versions
	char szVersion[90];
	sprintf_s(szVersion, sizeof(szVersion), "%f - %f", CURRENT_MAJOR_VERSION, CURRENT_VERSION);

	info.pszAppVersion = szVersion;
	info.pszEmailSubject = "SA:MP AC Version 2 crash";
	info.pszEmailTo = "whitetigerswt@gmail.com";
	info.pszUrl = "http://sixtytiger.com/tiger/acv2/crashrpt.php";

	info.uPriorities[CR_HTTP] = 3;  // First try send report over HTTP 
	info.uPriorities[CR_SMTP] = 2;  // Second try send report over SMTP  
	info.uPriorities[CR_SMAPI] = 1; // Third try send report over Simple MAPI 

	// Install all available exception handlers
	info.dwFlags |= CR_INST_ALL_POSSIBLE_HANDLERS;
	// Keep the reports in a queue incase the user doesn't send them immediately.
	info.dwFlags |= CR_INST_SEND_QUEUED_REPORTS;

	// Remove the "Close" button so the user has to send us the report (unless they terminate the process...)
	info.dwFlags |= CR_INST_SEND_MANDATORY;

	// Show the additional fields by default, this is the E-Mail and description of what they were doing before crash fields.
	info.dwFlags |= CR_INST_SHOW_ADDITIONAL_INFO_FIELDS;

	// Automatically attach the crash reporter to all newly created threads.
	info.dwFlags |= CR_INST_AUTO_THREAD_HANDLERS;

	// Define the Privacy Policy URL 
	info.pszPrivacyPolicyURL = "http://sixtytiger.com/tiger/acv2/privacypolicy.html";

	// Install crash reporting
	int nResult = crInstall(&info);
	if (nResult != 0)
	{
		// Crash reporter failed to initialize.
		return;
	}

	// Add the crash callback.
	SetCrashCallback();

	// Create a screenshot that we can investigate.
	crAddScreenshot2(CR_AS_MAIN_WINDOW | CR_AS_USE_JPEG_FORMAT, 90);
}


CCrashHandler::~CCrashHandler()
{
}