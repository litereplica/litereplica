
/*************************************************************************************/

#ifdef _WIN32

#include <windows.h>
typedef ULONGLONG (WINAPI *PtrGetTickCount64)(void);
static PtrGetTickCount64 ptrGetTickCount64 = 0;

static void resolveLibs() {
  static BOOL done = FALSE;
  HMODULE kernel32;

  if (done) return;

  // try to get GetTickCount64 from the system
  kernel32 = GetModuleHandleW(L"kernel32");
  if (!kernel32) return;

#if defined(_WINCE)  //! TODO: check if this macro (_WINCE) is written correctly
  // does this function exist on WinCE, or will ever exist?
  ptrGetTickCount64 = (PtrGetTickCount64)GetProcAddress(kernel32, L"GetTickCount64");
#else
  ptrGetTickCount64 = (PtrGetTickCount64)GetProcAddress(kernel32, "GetTickCount64");
#endif

  done = TRUE;
}

#elif defined(__MACH__) && defined(__APPLE__)

#include <sys/time.h>
#include <unistd.h>
#include <mach/mach.h>
#include <mach/mach_time.h>

static mach_timebase_info_data_t info = {0,0};

static int64 absoluteToNSecs(int64 cpuTime)
{
  if (info.denom == 0) mach_timebase_info(&info);
  int64 nsecs = cpuTime * info.numer / info.denom;
  return nsecs;
}

static int64 absoluteToMSecs(int64 cpuTime) {
  return absoluteToNSecs(cpuTime) / 1000000;
}

#elif defined(__unix__)

#include <unistd.h>    /* POSIX flags */
#include <time.h>      /* clock_gettime(), time() */
#include <sys/time.h>  /* gethrtime(), gettimeofday() */

/*
bool isMonotonic() {
#if (_POSIX_MONOTONIC_CLOCK-0 > 0)
  return true;
#else
  static int returnValue = 0;

  if (returnValue == 0) {
#  if (_POSIX_MONOTONIC_CLOCK-0 < 0) || !defined(_SC_MONOTONIC_CLOCK)
    returnValue = -1;
#  elif (_POSIX_MONOTONIC_CLOCK == 0)
    // detect if the system support monotonic timers
    long x = sysconf(_SC_MONOTONIC_CLOCK);
    returnValue = (x >= 200112L) ? 1 : -1;
#  endif
  }

  return returnValue != -1;
#endif
} */

#endif


uint64 now_fixed_time() {

#if defined(_WIN32)
  static unsigned int highdword = 0;
  static unsigned int lastval = 0;
  unsigned int val;

  resolveLibs();

  if (ptrGetTickCount64)
    return ptrGetTickCount64();

  val = GetTickCount();
  if (val < lastval) highdword++;
  lastval = val;
  return ((uint64)highdword << 32) | val;

#elif (defined(__hpux) || defined(hpux)) || ((defined(__sun__) || defined(__sun) || defined(sun)) && (defined(__SVR4) || defined(__svr4__)))
  /* HP-UX, Solaris. ------------------------------------------ */
  return gethrtime() / 1000000;

#elif defined(__MACH__) && defined(__APPLE__)
  /* OSX. ----------------------------------------------------- */
  uint64 cpu_time = mach_absolute_time();
  return absoluteToMSecs(cpu_time);

#elif defined(_POSIX_VERSION)
  /* POSIX. --------------------------------------------------- */
#if defined(_POSIX_TIMERS) && (_POSIX_TIMERS > 0)
  {
    struct timespec ts;
#if defined(CLOCK_MONOTONIC_RAW)
    /* Linux. ------------------------------------------- */
    const clockid_t id = CLOCK_MONOTONIC_RAW;
#elif defined(CLOCK_MONOTONIC)
    /* AIX, BSD, Linux, POSIX, Solaris. ----------------- */
    const clockid_t id = CLOCK_MONOTONIC;
#elif defined(CLOCK_MONOTONIC_PRECISE)
    /* BSD. --------------------------------------------- */
    const clockid_t id = CLOCK_MONOTONIC_PRECISE;
#elif defined(CLOCK_HIGHRES)
    /* Solaris. ----------------------------------------- */
    const clockid_t id = CLOCK_HIGHRES;
#elif defined(CLOCK_REALTIME)
    /* AIX, BSD, HP-UX, Linux, POSIX. ------------------- */
    const clockid_t id = CLOCK_REALTIME;
#else
    const clockid_t id = (clockid_t)-1;  /* Unknown. */
#endif /* CLOCK_* */
    if ( id != (clockid_t)-1 && clock_gettime( id, &ts ) != -1 )
      return (ts.tv_sec * 1000) + (ts.tv_nsec / 1000000);
    /* Fall thru. */
  }
#endif /* _POSIX_TIMERS */

  /* AIX, BSD, Cygwin, HP-UX, Linux, OSX, POSIX, Solaris. ----- */
  struct timeval tm;
  gettimeofday( &tm, NULL );
  return (tm.tv_sec * 1000) + (tm.tv_usec / 1000);

#else

  //return clock();

  struct rusage ru;
  getrusage(RUSAGE_SELF, &ru);
  return ((ru.ru_utime.tv_sec + ru.ru_stime.tv_sec) * 1000) + ((ru.ru_utime.tv_usec + ru.ru_stime.tv_usec) / 1000);

#endif

}

/*************************************************************************************/

uint64 calc_expiration_time(int timeout) {   // timeout in milliseconds
  uint64 expiration_time;

  expiration_time = now_fixed_time() + timeout;
  //! what about the overflow?

  return expiration_time;

}

/*************************************************************************************/

uint64 elapsed_time_from(uint64 from_time) {   // in milliseconds

  if (from_time == 0) return 0;

  return now_fixed_time() - from_time;

}

/*************************************************************************************/
