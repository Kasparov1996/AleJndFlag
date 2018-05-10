#ifndef SIGHANDLER_H
#define SIGHANDLER_H

#include <execinfo.h> // for backtrace
#include <dlfcn.h>    // for dladdr
#include <cxxabi.h>   // for __cxa_demangle

#include <sstream>

// This function produces a stack backtrace with demangled function & method names.
static void segfault_handler(int sig)
{
    void *callstack[128];
    const int nMaxFrames = sizeof(callstack) / sizeof(callstack[0]);
    char buf[1024];
    int nFrames = backtrace(callstack, nMaxFrames);
    char **symbols = backtrace_symbols(callstack, nFrames);
    int skip = 1;

    std::ostringstream trace_buf;
    for (int i = skip; i < nFrames; i++) {
        //printf("%s\n", symbols[i]);

        Dl_info info;
        if (dladdr(callstack[i], &info) && info.dli_sname) {
            char *demangled = NULL;
            int status = -1;
            if (info.dli_sname[0] == '_')
                demangled = abi::__cxa_demangle(info.dli_sname, NULL, 0, &status);
            snprintf(buf, sizeof(buf), "[bt]: %*p %s + %zd\n",
                     int(2 + sizeof(void*) * 2), callstack[i],
                     status == 0 ? demangled :
                     info.dli_sname == 0 ? symbols[i] : info.dli_sname,
                     (char *)callstack[i] - (char *)info.dli_saddr);
            free(demangled);
        } else {
            snprintf(buf, sizeof(buf), "[bt]: %*p %s\n",
                     int(2 + sizeof(void*) * 2), callstack[i], symbols[i]);
        }
        trace_buf << buf;
    }
    free(symbols);
    if (nFrames == nMaxFrames)
        trace_buf << "[truncated]\n";
    std::cout << trace_buf.str();
    exit(sig);
}

static void sigint_handler(int sig)
{
    std::cerr << "Ctrl-c detected!" << std::endl;
    exit(sig);
}

#endif
