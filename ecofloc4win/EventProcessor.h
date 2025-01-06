#pragma once

#include <thread>
#include <windows.h>
#include <iostream>
#include <evntrace.h>
#include <tdh.h>
#include <conio.h>
#include <atomic>
#include <csignal>
#include <tlhelp32.h>
#include <psapi.h>
#include <string>
#include <map>
//#include "Timer.h"
#include <future>
#include <evntcons.h>


class EventProcessor {
    struct IoEventInfo {
        DWORD pid;
        std::wstring processName;
        USHORT operationType;
        ULONG bytesTransferred = 0;  // Track total bytes transferred for each IRP
    };
public:
    EventProcessor();
    ~EventProcessor();

    void EventRecordCallback(EVENT_RECORD* pEvent);
    void processTrace();
    void signalHandler(int signal);
    void stopExistingKernelLogger();
    bool stopKernelLogger(EVENT_TRACE_PROPERTIES* sessionProperties);
    bool startTracing(EVENT_TRACE_PROPERTIES* sessionProperties);
    void initializeTraceProperties(EVENT_TRACE_PROPERTIES* sessionProperties, ULONG bufferSize, ULONG flags);
    void initializeLogFile(EVENT_TRACE_LOGFILE& logFile);

    void my_callback();
    TRACEHANDLE hTrace;
private:
    std::atomic<bool> stopProcessing;
    double total_read_bytes;
    double total_write_bytes;
    double energy;
    std::map<ULONGLONG, IoEventInfo> ioEvents;

    std::wstring GetProcessNameByPid(DWORD processID);
    ULONG GetDataField(EVENT_RECORD* pEvent, LPCWSTR propertyName, PBYTE buffer, ULONG bufferSize);
    double energy_calculation(double total_read, double total_write, double interval_ms);
};

