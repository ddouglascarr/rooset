
/*** Version ***/
#define MOONBR_VERSION_STRING "1.0.1"


/*** Compile-time configuration ***/

#define MOONBR_LUA_PANIC_BUG_WORKAROUND 1


/*** C preprocessor macros for portability support ***/

#ifndef __has_include
#define __has_include(x) 0
#endif


/*** Include directives for used system libraries ***/

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <poll.h>
#include <time.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <getopt.h>
#include <sys/file.h>
#include <syslog.h>
#if defined(__FreeBSD__) || __has_include(<libutil.h>)
#include <libutil.h>
#endif
#if defined(__linux__) || __has_include(<bsd/libutil.h>)
#include <bsd/libutil.h>
#endif
#if defined(__linux__) || __has_include(<bsd/unistd.h>)
#include <bsd/unistd.h>
#endif


/*** Fallback definitions for missing constants on some platforms ***/

/* INFTIM is used as timeout parameter for poll() */
#ifndef INFTIM
#define INFTIM -1
#endif


/*** Include directives for Lua ***/

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>


/*** Include directive for moonbridge_io library ***/

#include "moonbridge_io.h"


/*** Constants ***/

/* Backlog option for listen() call */
#define MOONBR_LISTEN_BACKLOG 1024

/* Maximum length of a timestamp used for strftime() */
#define MOONBR_LOG_MAXTIMELEN 40

/* Maximum length of a log message */
#define MOONBR_LOG_MAXMSGLEN 4095

/* Exitcodes passed to exit() call */
#define MOONBR_EXITCODE_GRACEFUL 0
#define MOONBR_EXITCODE_CMDLINEERROR 1
#define MOONBR_EXITCODE_ALREADYRUNNING 2
#define MOONBR_EXITCODE_STARTUPERROR 3
#define MOONBR_EXITCODE_RUNTIMEERROR 4

/* Maximum length of a line sent to stderr by child processes */
#define MOONBR_MAXERRORLINELEN 1024

/* Maximum length of an error string returned by strerror() */
#define MOONBR_MAXSTRERRORLEN 80

/* Error message for noncompliant strerror_r() implementation on GNU systems */
#define MOONBR_STRERROR_R_MSG "Error detail unavailable due to noncompliant strerror_r() implementation"

/* Status bytes exchanged between master and child processes */
#define MOONBR_STATUS_IDLE 'I'
#define MOONBR_COMMAND_CONNECT 'C'
#define MOONBR_COMMAND_TERMINATE 'T'
#define MOONBR_STATUS_GOODBYE 'B'

/* Constant file descriptors */
#define MOONBR_FD_STDERR 2
#define MOONBR_FD_CONTROL 3
#define MOONBR_FD_END 4

/* Return values of moonbr_try_destroy_worker() */
#define MOONBR_DESTROY_NONE 0
#define MOONBR_DESTROY_PREPARE 1
#define MOONBR_DESTROY_IDLE_OR_ASSIGNED 2


/*** Types ***/

/* Enum for 'moonbr_pstate' */
#define MOONBR_PSTATE_STARTUP 0
#define MOONBR_PSTATE_RUNNING 1
#define MOONBR_PSTATE_FORKED  2

/* Enum for 'proto' field of struct moonbr_listener */
#define MOONBR_PROTO_INTERVAL 1
#define MOONBR_PROTO_LOCAL 2
#define MOONBR_PROTO_TCP 3

/* Data structure for a pool's listener that can accept incoming connections */
struct moonbr_listener {
  struct moonbr_pool *pool;
  struct moonbr_listener *prev_listener;  /* previous idle or(!) connected listener */
  struct moonbr_listener *next_listener;  /* next idle or(!) connected listener */
  int proto;
  union {
    struct {
      char *name;  /* name of interval passed to 'connect' function as 'interval' field in table */
      int main;    /* nonzero = termination of 'connect' function causes shutdown */
      int strict;  /* nonzero = runtime of 'connect' function does not delay interval */
      struct timeval delay;   /* interval between invocations of 'connect' function */
      struct timeval wakeup;  /* point in time of next invocation */
    } interval;
    struct {
      union {
        struct sockaddr addr_abstract;
        struct sockaddr_un addr_un;
        struct sockaddr_in addr_in;
        struct sockaddr_in6 addr_in6;
      } addr;
      socklen_t addrlen;
    } socket;
  } type_specific;
  union {
    struct {
      char ip[INET6_ADDRSTRLEN];  /* IP to listen on */
      int port;  /* port number to listen on (in host endianess) */
    } tcp;
  } proto_specific;
  int listenfd;  /* -1 = none */
  int pollidx;   /* -1 = none */
};

/* Data structure for a child process that is handling incoming connections */
struct moonbr_worker {
  struct moonbr_pool *pool;
  struct moonbr_worker *prev_worker;
  struct moonbr_worker *next_worker;
  struct moonbr_worker *prev_idle_worker;
  struct moonbr_worker *next_idle_worker;
  int main;       /* nonzero = terminate Moonbridge when this worker dies */
  int idle;       /* nonzero = waiting for command from parent process */
  int assigned;   /* nonzero = currently handling a connection */
  pid_t pid;
  int controlfd;  /* socket to send/receive control message to/from child process */
  int errorfd;    /* socket to receive error output from child process' stderr */
  char *errorlinebuf;  /* optional buffer for collecting stderr data from child process */
  int errorlinelen;    /* number of bytes stored in 'errorlinebuf' */
  int errorlineovf;    /* nonzero = line length overflow */
  struct timeval idle_expiration;  /* point in time until child process may stay in idle state */
  struct moonbr_listener *restart_interval_listener;  /* set while interval listener is assigned */
};

/* Data structure for a pool of workers and listeners */
struct moonbr_pool {
  int poolnum;  /* number of pool for log output */
  struct moonbr_pool *next_pool;  /* next entry in linked list starting with 'moonbr_first_pool' */
  struct moonbr_worker *first_worker;       /* first worker of pool */
  struct moonbr_worker *last_worker;        /* last worker of pool */
  struct moonbr_worker *first_idle_worker;  /* first idle worker of pool */
  struct moonbr_worker *last_idle_worker;   /* last idle worker of pool */
  int idle_worker_count;
  int unassigned_worker_count;
  int total_worker_count;
  int worker_count_stat;  /* only needed for statistics */
  int pre_fork;  /* desired minimum number of unassigned workers */
  int min_fork;  /* desired minimum number of workers in total */
  int max_fork;  /* maximum number of workers */
  struct timeval fork_delay;   /* delay after each fork() until a fork may happen again */
  struct timeval fork_wakeup;  /* point in time when a fork may happen again (unless a worker terminates before) */
  struct timeval fork_error_delay;   /* delay between fork()s when an error during fork or preparation occurred */
  struct timeval fork_error_wakeup;  /* point in time when fork may happen again if an error in preparation occurred */
  int use_fork_error_wakeup;         /* nonzero = error in preparation occured; gets reset on next fork */
  struct timeval exit_delay;    /* delay for terminating excessive workers (unassigned_worker_count > pre_fork) */
  struct timeval exit_wakeup;   /* point in time when terminating an excessive worker */
  struct timeval idle_timeout;  /* delay before an idle worker is terminated */
  size_t memory_limit;  /* maximum bytes of memory that the Lua machine may allocate */
  int listener_count;  /* total number of listeners of pool (and size of 'listener' array at end of this struct) */
  struct moonbr_listener *first_idle_listener;       /* first listener that is idle (i.e. has no waiting connection) */
  struct moonbr_listener *last_idle_listener;        /* last  listener that is idle (i.e. has no waiting connection) */
  struct moonbr_listener *first_connected_listener;  /* first listener that has a pending connection */
  struct moonbr_listener *last_connected_listener;   /* last  listener that has a pending connection */
  struct moonbr_listener listener[1];  /* static array of variable(!) size to contain 'listener' structures */
};

/* Enum for 'channel' field of struct moonbr_poll_worker */
#define MOONBR_POLL_WORKER_CONTROLCHANNEL 1
#define MOONBR_POLL_WORKER_ERRORCHANNEL 2

/* Structure to refer from 'moonbr_poll_worker_fds' entry to worker structure */
struct moonbr_poll_worker {
  struct moonbr_worker *worker;
  int channel;  /* field indicating whether file descriptor is 'controlfd' or 'errorfd' */
};

/* Variable indicating that clean shutdown was requested */
static int moonbr_shutdown_in_progress = 0;


/*** Macros for Lua registry ***/

/* Lightuserdata keys for Lua registry to store 'prepare', 'connect', and 'finish' functions */
#define moonbr_luakey_prepare_func(pool) ((void *)(intptr_t)(pool) + 0)
#define moonbr_luakey_connect_func(pool) ((void *)(intptr_t)(pool) + 1)
#define moonbr_luakey_finish_func(pool)  ((void *)(intptr_t)(pool) + 2)


/*** Global variables ***/

/* State of process execution */
static int moonbr_pstate = MOONBR_PSTATE_STARTUP;

/* Process ID of the main process */
static pid_t moonbr_masterpid;

/* Condition variables set by the signal handler */
static volatile sig_atomic_t moonbr_cond_poll = 0;
static volatile sig_atomic_t moonbr_cond_terminate = 0;
static volatile sig_atomic_t moonbr_cond_interrupt = 0;
static volatile sig_atomic_t moonbr_cond_child = 0;

/* Socket pair to denote signal delivery when signal handler was called just before poll() */
static int moonbr_poll_signalfds[2];
#define moonbr_poll_signalfd_read moonbr_poll_signalfds[0]
#define moonbr_poll_signalfd_write moonbr_poll_signalfds[1]

/* Global variables for pidfile and logging */
static struct pidfh *moonbr_pidfh = NULL;
static FILE *moonbr_logfile = NULL;
static int moonbr_use_syslog = 0;

/* First and last entry of linked list of all created pools during initialization */
static struct moonbr_pool *moonbr_first_pool = NULL;
static struct moonbr_pool *moonbr_last_pool = NULL;

/* Total count of pools */
static int moonbr_pool_count = 0;

/* Set to a nonzero value if dynamic part of 'moonbr_poll_fds' ('moonbr_poll_worker_fds') needs an update */
static int moonbr_poll_refresh_needed = 0;

/* Array passed to poll(), consisting of static part and dynamic part ('moonbr_poll_worker_fds') */
static struct pollfd *moonbr_poll_fds = NULL;  /* the array */
static int moonbr_poll_fds_bufsize = 0;        /* memory allocated for this number of elements */
static int moonbr_poll_fds_count = 0;          /* total number of elements */
static int moonbr_poll_fds_static_count;       /* number of elements in static part */

/* Dynamic part of 'moonbr_poll_fds' array */
#define moonbr_poll_worker_fds (moonbr_poll_fds+moonbr_poll_fds_static_count)

/* Additional information for dynamic part of 'moonbr_poll_fds' array */
struct moonbr_poll_worker *moonbr_poll_workers;  /* the array */
static int moonbr_poll_workers_bufsize = 0;     /* memory allocated for this number of elements */
static int moonbr_poll_worker_count = 0;        /* number of elements in array */

/* Variable set to nonzero value to disallow further calls of 'listen' function */
static int moonbr_booted = 0;

/* Verbosity settings */
static int moonbr_debug = 0;
static int moonbr_stat = 0;

/* Memory consumption by Lua machine */
static size_t moonbr_memory_usage = 0;
static size_t moonbr_memory_limit = 0;


/*** Functions for signal handling ***/

/* Signal handler for master and child processes */
static void moonbr_signal(int sig) {
  int errno2 = errno;  /* backup errno variable */
  if (getpid() == moonbr_masterpid) {
    /* master process */
    switch (sig) {
    case SIGHUP:
    case SIGINT:
      /* fast shutdown requested */
      moonbr_cond_interrupt = 1;
      break;
    case SIGTERM:
      /* clean shutdown requested */
      moonbr_cond_terminate = 1;
      break;
    case SIGCHLD:
      /* child process terminated */
      moonbr_cond_child = 1;
      break;
    }
    if (moonbr_cond_poll) {
      /* avoid race condition if signal handler is invoked right before poll() */
      char buf[1] = {0};
      write(moonbr_poll_signalfd_write, buf, 1);
    }
  } else {
    /* child process forwards certain signals to parent process */
    switch (sig) {
    case SIGHUP:
    case SIGINT:
      kill(moonbr_masterpid, sig);
    }
  }
  errno = errno2;  /* restore errno from backup */
}

/* Initialize signal handling */
static void moonbr_signal_init(){
  moonbr_masterpid = getpid();
  signal(SIGHUP, moonbr_signal);
  signal(SIGINT, moonbr_signal);
  signal(SIGTERM, moonbr_signal);
  signal(SIGCHLD, moonbr_signal);
}


/*** Functions for logging in master process ***/

/* Logs a pre-formatted message with given syslog() priority */
static void moonbr_log_msg(int priority, const char *msg) {
  if (moonbr_logfile) {
    /* logging to logfile desired (timestamp is prepended in that case) */
    time_t now_time = 0;
    struct tm now_tmstruct;
    char timestr[MOONBR_LOG_MAXTIMELEN+1];
    time(&now_time);
    localtime_r(&now_time, &now_tmstruct);
    if (!strftime(
      timestr, MOONBR_LOG_MAXTIMELEN+1, "%Y-%m-%d %H:%M:%S %Z: ", &now_tmstruct
    )) timestr[0] = 0;
    fprintf(moonbr_logfile, "%s%s\n", timestr, msg);
  }
  if (moonbr_use_syslog) {
    /* logging through syslog desired */
    syslog(priority, "%s", msg);
  }
}

/* Formats a message via vsnprintf() and logs it with given syslog() priority */
static void moonbr_log(int priority, const char *message, ...) {
  char msgbuf[MOONBR_LOG_MAXMSGLEN+1];  /* buffer of static size to store formatted message */
  int msglen;  /* length of full message (may exceed MOONBR_LOG_MAXMSGLEN) */
  {
    /* pass variable arguments to vsnprintf() to format message */
    va_list ap;
    va_start(ap, message);
    msglen = vsnprintf(msgbuf, MOONBR_LOG_MAXMSGLEN+1, message, ap);
    va_end(ap);
  }
  {
    /* split and log message line by line */
    char *line = msgbuf;
    while (1) {
      char *endptr = strchr(line, '\n');
      if (endptr) {
        /* terminate string where newline character is found */
        *endptr = 0;
      } else if (line != msgbuf && msglen > MOONBR_LOG_MAXMSGLEN) {
        /* break if line is incomplete and not the first line */
        break;
      }
      moonbr_log_msg(priority, line);
      if (!endptr) break;  /* break if end of formatted message is reached */
      line = endptr+1;     /* otherwise continue with remaining message */
    }
  }
  if (msglen > MOONBR_LOG_MAXMSGLEN) {
    /* print warning if message was truncated */
    moonbr_log_msg(priority, "Previous log message has been truncated due to excessive length");
  }
}


/*** Termination function ***/

/* Kill all child processes, remove PID file (if existent), and exit master process with given exitcode */
static void moonbr_terminate(int exitcode) {
  {
    struct moonbr_pool *pool;
    for (pool=moonbr_first_pool; pool; pool=pool->next_pool) {
      {
        struct moonbr_worker *worker;
        for (worker=pool->first_worker; worker; worker=worker->next_worker) {
          moonbr_log(LOG_INFO, "Sending SIGKILL to child with PID %i", (int)worker->pid);
          if (kill(worker->pid, SIGKILL)) {
            moonbr_log(LOG_ERR, "Error while killing child process: %s", strerror(errno));
          }
        }
      }
      {
        int i;
        for (i=0; i<pool->listener_count; i++) {
          struct moonbr_listener *listener = &pool->listener[i];
          if (listener->proto == MOONBR_PROTO_LOCAL) {
            moonbr_log(LOG_INFO, "Unlinking local socket \"%s\"", listener->type_specific.socket.addr.addr_un.sun_path);
            if (unlink(listener->type_specific.socket.addr.addr_un.sun_path)) {
              moonbr_log(LOG_ERR, "Error while unlinking local socket: %s", strerror(errno));
            }
          }
        }
      }
    }
  }
  moonbr_log(exitcode ? LOG_ERR : LOG_NOTICE, "Terminating with exit code %i", exitcode);
  if (moonbr_pidfh && pidfile_remove(moonbr_pidfh)) {
    moonbr_log(LOG_ERR, "Error while removing PID file: %s", strerror(errno));
  }
  exit(exitcode);
}

/* Terminate with either MOONBR_EXITCODE_STARTUPERROR or MOONBR_EXITCODE_RUNTIMEERROR */
#define moonbr_terminate_error() \
  moonbr_terminate( \
    moonbr_pstate == MOONBR_PSTATE_STARTUP ? \
    MOONBR_EXITCODE_STARTUPERROR : \
    MOONBR_EXITCODE_RUNTIMEERROR \
  )


/*** Helper functions ***/

/* Fills a 'struct timeval' structure with the current time (using CLOCK_MONOTONIC) */
static void moonbr_now(struct timeval *now) {
  struct timespec ts = {0, };
  if (clock_gettime(CLOCK_MONOTONIC, &ts)) {
    moonbr_log(LOG_CRIT, "Error in clock_gettime() call: %s", strerror(errno));
    moonbr_terminate_error();
  }
  *now = (struct timeval){ .tv_sec = ts.tv_sec, .tv_usec = ts.tv_nsec / 1000 };
}

/* Formats a 'struct timeval' value (not thread-safe) */
static char *moonbr_format_timeval(struct timeval *t) {
  static char buf[32];
  snprintf(buf, 32, "%ji.%06ji seconds", (intmax_t)t->tv_sec, (intmax_t)t->tv_usec);
  return buf;
}


/*** Functions for pool creation and startup ***/

/* Creates a 'struct moonbr_pool' structure with a given number of listeners */
static struct moonbr_pool *moonbr_create_pool(int listener_count) {
  struct moonbr_pool *pool;
  pool = calloc(1,
    sizeof(struct moonbr_pool) +  /* size of 'struct moonbr_pool' with one listener */
    (listener_count-1) * sizeof(struct moonbr_listener)  /* size of extra listeners */
  );
  if (!pool) {
    moonbr_log(LOG_CRIT, "Memory allocation error");
    moonbr_terminate_error();
  }
  pool->listener_count = listener_count;
  {
    /* initialization of listeners */
    int i;
    for (i=0; i<listener_count; i++) {
      struct moonbr_listener *listener = &pool->listener[i];
      listener->pool = pool;
      listener->listenfd = -1;
      listener->pollidx = -1;
    }
  }
  return pool;
}

/* Destroys a 'struct moonbr_pool' structure before it has been started */
static void moonbr_destroy_pool(struct moonbr_pool *pool) {
  int i;
  for (i=0; i<pool->listener_count; i++) {
    struct moonbr_listener *listener = &pool->listener[i];
    if (
      listener->proto == MOONBR_PROTO_INTERVAL &&
      listener->type_specific.interval.name
    ) {
      free(listener->type_specific.interval.name);
    }
  }
  free(pool);
}

/* Starts a all listeners in a pool */
static int moonbr_start_pool(struct moonbr_pool *pool) {
  moonbr_log(LOG_INFO, "Creating pool", pool->poolnum);
  {
    int i;
    for (i=0; i<pool->listener_count; i++) {
      struct moonbr_listener *listener = &pool->listener[i];
      switch (listener->proto) {
      case MOONBR_PROTO_INTERVAL:
        if (listener->type_specific.interval.main) {
          /* nothing to do here: starting main thread is performed in moonbr_run() function */
          if (!listener->type_specific.interval.name) {
            moonbr_log(LOG_INFO, "Adding unnamed main thread");
          } else {
            moonbr_log(LOG_INFO, "Adding main thread \"%s\"", listener->type_specific.interval.name);
          }
        } else {
          /* nothing to do here: starting intervals is performed in moonbr_run() function */
          if (!listener->type_specific.interval.name) {
            moonbr_log(LOG_INFO, "Adding unnamed interval listener");
          } else {
            moonbr_log(LOG_INFO, "Adding interval listener \"%s\"", listener->type_specific.interval.name);
          }
        }
        break;
      case MOONBR_PROTO_LOCAL:
        moonbr_log(LOG_INFO, "Adding local socket listener for path \"%s\"", listener->type_specific.socket.addr.addr_un.sun_path);
        listener->listenfd = socket(PF_LOCAL, SOCK_STREAM|SOCK_CLOEXEC|SOCK_NONBLOCK, 0);
        if (listener->listenfd == -1) goto moonbr_start_pool_error;
        if (!unlink(listener->type_specific.socket.addr.addr_un.sun_path)) {
          moonbr_log(LOG_WARNING, "Unlinked named socket \"%s\" prior to listening", listener->type_specific.socket.addr.addr_un.sun_path);
        } else {
          if (errno != ENOENT) {
            moonbr_log(LOG_ERR, "Could not unlink named socket \"%s\" prior to listening: %s", listener->type_specific.socket.addr.addr_un.sun_path, strerror(errno));
          }
        }
        if (
          bind(listener->listenfd, &listener->type_specific.socket.addr.addr_abstract, listener->type_specific.socket.addrlen)
        ) goto moonbr_start_pool_error;
        if (listen(listener->listenfd, MOONBR_LISTEN_BACKLOG)) goto moonbr_start_pool_error;
        break;
      case MOONBR_PROTO_TCP:
        moonbr_log(LOG_INFO, "Adding TCP listener on interface \"%s\", port %i", listener->proto_specific.tcp.ip, listener->proto_specific.tcp.port);
        listener->listenfd = socket(listener->type_specific.socket.addr.addr_abstract.sa_family, SOCK_STREAM|SOCK_CLOEXEC|SOCK_NONBLOCK, 0);  /* NOTE: not correctly using PF_* but AF_* constants here */
        if (listener->listenfd == -1) goto moonbr_start_pool_error;
        {
          /* avoid "Address already in use" error when restarting service */
          static const int reuseval = 1;
          if (setsockopt(
            listener->listenfd, SOL_SOCKET, SO_REUSEADDR, &reuseval, sizeof(reuseval)
          )) goto moonbr_start_pool_error;
        }
        {
          /* default to send TCP RST when process terminates unexpectedly */
          static const struct linger lingerval = {
            .l_onoff = 1,
            .l_linger = 0
          };
          if (setsockopt(
            listener->listenfd, SOL_SOCKET, SO_LINGER, &lingerval, sizeof(lingerval)
          )) goto moonbr_start_pool_error;
        }
        if (
          bind(listener->listenfd, &listener->type_specific.socket.addr.addr_abstract, listener->type_specific.socket.addrlen)
        ) goto moonbr_start_pool_error;
        if (listen(listener->listenfd, MOONBR_LISTEN_BACKLOG)) goto moonbr_start_pool_error;
        break;
      default:
        moonbr_log(LOG_CRIT, "Internal error (should not happen): Unexpected value in listener.proto field");
        moonbr_terminate_error();
      }
    }
    goto moonbr_start_pool_ok;
    moonbr_start_pool_error:
    {
      int j = i;
      int errno2 = errno;
      for (; i>=0; i--) {
        struct moonbr_listener *listener = &pool->listener[i];
        if (listener->listenfd != -1) close(listener->listenfd);
      }
      errno = errno2;
      return j;
    }
  }
  moonbr_start_pool_ok:
  pool->poolnum = ++moonbr_pool_count;
  moonbr_log(LOG_INFO, "Pool #%i created", pool->poolnum);
  if (moonbr_last_pool) moonbr_last_pool->next_pool = pool;
  else moonbr_first_pool = pool;
  moonbr_last_pool = pool;
  return -1;
}


/*** Function to send data and a file descriptor to child process */

/* Sends control message of one bye plus optional file descriptor plus optional pointer to child process */
static void moonbr_send_control_message(struct moonbr_worker *worker, char status, int fd, void *ptr) {
  {
    struct iovec iovector = { .iov_base = &status, .iov_len = 1 };      /* carrying status byte */
    char control_message_buffer[CMSG_SPACE(sizeof(int))] = {0, };       /* used to transfer file descriptor */
    struct msghdr message = { .msg_iov = &iovector, .msg_iovlen = 1 };  /* data structure passed to sendmsg() call */
    if (moonbr_debug) {
      if (fd == -1) {
        moonbr_log(LOG_DEBUG, "Sending control message \"%c\" to child process in pool #%i (PID %i)", (int)status, worker->pool->poolnum, (int)worker->pid);
      } else {
        moonbr_log(LOG_DEBUG, "Sending control message \"%c\" with file descriptor #%i to child process in pool #%i (PID %i)", (int)status, fd, worker->pool->poolnum, (int)worker->pid);
      }
    }
    if (fd != -1) {
      /* attach control message with file descriptor */
      message.msg_control = control_message_buffer;
      message.msg_controllen = CMSG_SPACE(sizeof(int));
      {
        struct cmsghdr *control_message = CMSG_FIRSTHDR(&message);
        control_message->cmsg_level = SOL_SOCKET;
        control_message->cmsg_type = SCM_RIGHTS;
        control_message->cmsg_len = CMSG_LEN(sizeof(int));
        memcpy(CMSG_DATA(control_message), &fd, sizeof(int));
      }
    }
    while (sendmsg(worker->controlfd, &message, MSG_NOSIGNAL) < 0) {
      if (errno == EPIPE) {
        moonbr_log(LOG_ERR, "Error while communicating with idle child process in pool #%i (PID %i): %s", worker->pool->poolnum, (int)worker->pid, strerror(errno));
        return;  /* do not close socket; socket is closed when reading from it */
      }
      if (errno != EINTR) {
        moonbr_log(LOG_CRIT, "Unexpected error while communicating with idle child process in pool #%i (PID %i): %s", worker->pool->poolnum, (int)worker->pid, strerror(errno));
        moonbr_terminate_error();
      }
    }
  }
  if (ptr) {
    char buf[sizeof(void *)];
    char *pos = buf;
    int len = sizeof(void *);
    ssize_t written;
    if (moonbr_debug) {
      moonbr_log(LOG_DEBUG, "Sending memory pointer to child process in pool #%i (PID %i)", (int)status, worker->pool->poolnum, (int)worker->pid);
    }
    memcpy(buf, &ptr, sizeof(void *));
    while (len) {
      written = send(worker->controlfd, pos, len, MSG_NOSIGNAL);
      if (written > 0) {
        pos += written;
        len -= written;
      } else if (errno == EPIPE) {
        moonbr_log(LOG_ERR, "Error while communicating with idle child process in pool #%i (PID %i): %s", worker->pool->poolnum, (int)worker->pid, strerror(errno));
        return;  /* do not close socket; socket is closed when reading from it */
      } else if (errno != EINTR) {
        moonbr_log(LOG_CRIT, "Unexpected error while communicating with idle child process in pool #%i (PID %i): %s", worker->pool->poolnum, (int)worker->pid, strerror(errno));
        moonbr_terminate_error();
      }
    }
  }
}


/*** Functions running in child process ***/

/* Logs an error in child process */
static void moonbr_child_log(const char *message) {
  fprintf(stderr, "%s\n", message);
}

/* Logs a fatal error in child process and terminates process with error status */
static void moonbr_child_log_fatal(const char *message) {
  moonbr_child_log(message);
  exit(1);
}

/* Logs an error in child process while appending error string for global errno variable */
static void moonbr_child_log_errno(const char *message) {
  char errmsg[MOONBR_MAXSTRERRORLEN] = MOONBR_STRERROR_R_MSG;
  strerror_r(errno, errmsg, MOONBR_MAXSTRERRORLEN);  /* use thread-safe call in case child created threads */
  fprintf(stderr, "%s: %s\n", message, errmsg);
}

/* Logs a fatal error in child process while appending error string for errno and terminating process */
static void moonbr_child_log_errno_fatal(const char *message) {
  moonbr_child_log_errno(message);
  exit(1);
}

/* Receives a control message consisting of one character plus an optional file descriptor from parent process */
static void moonbr_child_receive_control_message(int socketfd, char *status, int *fd) {
  struct iovec iovector = { .iov_base = status, .iov_len = 1 };  /* reference to status byte variable */
  char control_message_buffer[CMSG_SPACE(sizeof(int))] = {0, };  /* used to receive file descriptor */
  struct msghdr message = {  /* data structure passed to recvmsg() call */
    .msg_iov = &iovector,
    .msg_iovlen = 1,
    .msg_control = control_message_buffer,
    .msg_controllen = CMSG_SPACE(sizeof(int))
  };
  {
    int received;
    while ((received = recvmsg(socketfd, &message, MSG_CMSG_CLOEXEC)) < 0) {
      if (errno != EINTR) {
        moonbr_child_log_errno_fatal("Error while trying to receive connection socket from parent process");
      }
    }
    if (!received) {
      moonbr_child_log_fatal("Unexpected EOF while trying to receive connection socket from parent process");
    }
  }
  {
    struct cmsghdr *control_message = CMSG_FIRSTHDR(&message);
    if (control_message) {
      if (control_message->cmsg_level != SOL_SOCKET) {
        moonbr_child_log_fatal("Received control message with cmsg_level not equal to SOL_SOCKET");
      }
      if (control_message->cmsg_type != SCM_RIGHTS) {
        moonbr_child_log_fatal("Received control message with cmsg_type not equal to SCM_RIGHTS");
      }
      memcpy(fd, CMSG_DATA(control_message), sizeof(int));
    } else {
      *fd = -1;
    }
  }
}

/* Receives a pointer from parent process */
static void *moonbr_child_receive_pointer(int socketfd) {
  char buf[sizeof(void *)];
  char *pos = buf;
  int len = sizeof(void *);
  ssize_t bytes_read;
  while (len) {
    bytes_read = recv(socketfd, pos, len, 0);
    if (bytes_read > 0) {
      pos += bytes_read;
      len -= bytes_read;
    } else if (!bytes_read) {
      moonbr_child_log_fatal("Unexpected EOF while trying to receive memory pointer from parent process");
    } else if (errno != EINTR) {
      moonbr_child_log_errno_fatal("Error while trying to receive memory pointer from parent process");
    }
  }
  {
    void *ptr;  /* avoid breaking strict-aliasing rules */
    memcpy(&ptr, buf, sizeof(void *));
    return ptr;
  }
}

/* Main function of child process to be called after fork() and file descriptor rearrangement */
void moonbr_child_run(struct moonbr_pool *pool, lua_State *L) {
  char controlmsg;
  int fd;
  struct itimerval notimer = { { 0, }, { 0, } };
  lua_rawgetp(L, LUA_REGISTRYINDEX, moonbr_luakey_prepare_func(pool));
  if (lua_isnil(L, -1)) lua_pop(L, 1);
  else if (lua_pcall(L, 0, 0, 1)) {
    fprintf(stderr, "Error in \"prepare\" function: %s\n", lua_tostring(L, -1));
    exit(1);
  }
  moonbr_io_catch_sigterm(L);  // NOTE: should not fail
  while (1) {
    struct moonbr_listener *listener;
    if (setitimer(ITIMER_REAL, &notimer, NULL)) {
      moonbr_child_log_errno_fatal("Could not reset ITIMER_REAL via setitimer()");
    }
    controlmsg = MOONBR_STATUS_IDLE;
    if (write(MOONBR_FD_CONTROL, &controlmsg, 1) <= 0) {
      moonbr_child_log_errno_fatal("Error while sending ready message to parent process");
    }
    moonbr_child_receive_control_message(MOONBR_FD_CONTROL, &controlmsg, &fd);
    if (!(
      (controlmsg == MOONBR_COMMAND_TERMINATE && fd == -1) ||
      (controlmsg == MOONBR_COMMAND_CONNECT)
    )) {
      moonbr_child_log_fatal("Received illegal control message from parent process");
    }
    if (controlmsg == MOONBR_COMMAND_TERMINATE) break;
    moonbr_io_sigterm_flag = 0;  /* ignore any prior SIGTERM (can't be handled in blocking recv anyway) */
    listener = moonbr_child_receive_pointer(MOONBR_FD_CONTROL);
    if (
      listener->proto != MOONBR_PROTO_LOCAL &&
      listener->proto != MOONBR_PROTO_TCP &&
      fd >= 0
    ) {
      moonbr_child_log_fatal("Received unexpected file descriptor from parent process");
    } else if (
      listener->proto != MOONBR_PROTO_INTERVAL && fd < 0
    ) {
      moonbr_child_log_fatal("Missing file descriptor from parent process");
    }
    if (fd >= 0) moonbr_io_pushhandle(L, fd);
    lua_rawgetp(L, LUA_REGISTRYINDEX, moonbr_luakey_connect_func(pool));
    if (fd < 0) {
      lua_newtable(L);
      if (listener->proto == MOONBR_PROTO_INTERVAL) {
        lua_pushstring(L,
          listener->type_specific.interval.name ?
          listener->type_specific.interval.name : ""
        );
        if (listener->type_specific.interval.main) {
          lua_setfield(L, -2, "main");
        } else {
          lua_setfield(L, -2, "interval");
        }
      }
    } else {
      lua_pushvalue(L, -2);
    }
    if (lua_pcall(L, 1, 1, 1)) {
      fprintf(stderr, "Error in \"connect\" function: %s\n", lua_tostring(L, -1));
      exit(1);
    }
    if (fd >= 0) moonbr_io_closehandle(L, -2, 0);  /* attemt clean close */
    if (
      moonbr_io_sigterm_flag ||
      lua_type(L, -1) != LUA_TBOOLEAN || !lua_toboolean(L, -1)
    ) break;
#ifdef MOONBR_LUA_PANIC_BUG_WORKAROUND
    lua_settop(L, 2);
#else
    lua_settop(L, 1);
#endif
  }
  controlmsg = MOONBR_STATUS_GOODBYE;
  if (write(MOONBR_FD_CONTROL, &controlmsg, 1) <= 0) {
    moonbr_child_log_errno_fatal("Error while sending goodbye message to parent process");
  }
  if (close(MOONBR_FD_CONTROL) && errno != EINTR) {
    moonbr_child_log_errno("Error while closing control socket");
  }
  lua_rawgetp(L, LUA_REGISTRYINDEX, moonbr_luakey_finish_func(pool));
  if (lua_isnil(L, -1)) lua_pop(L, 1);
  else if (lua_pcall(L, 0, 0, 1)) {
    fprintf(stderr, "Error in \"finish\" function: %s\n", lua_tostring(L, -1));
    exit(1);
  }
  lua_close(L);
  exit(0);
}


/*** Functions to spawn child process ***/

/* Helper function to send an error message to a file descriptor (not needing a file stream) */
static void moonbr_child_emergency_print(int fd, char *message) {
  size_t len = strlen(message);
  ssize_t written;
  while (len) {
    written = write(fd, message, len);
    if (written > 0) {
      message += written;
      len -= written;
    } else {
      if (written != -1 || errno != EINTR) break;
    }
  }
}

/* Helper function to send an error message plus a text for errno to a file descriptor and terminate the process */
static void moonbr_child_emergency_error(int fd, char *message) {
  int errno2 = errno;
  moonbr_child_emergency_print(fd, message);
  moonbr_child_emergency_print(fd, ": ");
  moonbr_child_emergency_print(fd, strerror(errno2));
  moonbr_child_emergency_print(fd, "\n");
  exit(1);
}

/* Creates a child process and (in case of success) registers it in the 'struct moonbr_pool' structure */
static int moonbr_create_worker(struct moonbr_pool *pool, lua_State *L) {
  struct moonbr_worker *worker;
  worker = calloc(1, sizeof(struct moonbr_worker));
  if (!worker) {
    moonbr_log(LOG_CRIT, "Memory allocation error");
    return -1;
  }
  worker->pool = pool;
  {
    int controlfds[2];
    int errorfds[2];
    if (socketpair(PF_LOCAL, SOCK_STREAM | SOCK_CLOEXEC, 0, controlfds)) {
      moonbr_log(LOG_ERR, "Could not create control socket pair for communcation with child process: %s", strerror(errno));
      free(worker);
      return -1;
    }
    if (socketpair(PF_LOCAL, SOCK_STREAM | SOCK_CLOEXEC, 0, errorfds)) {
      moonbr_log(LOG_ERR, "Could not create socket pair to redirect stderr of child process: %s", strerror(errno));
      close(controlfds[0]);
      close(controlfds[1]);
      free(worker);
      return -1;
    }
    if (moonbr_logfile && fflush(moonbr_logfile)) {
      moonbr_log(LOG_CRIT, "Could not flush log file prior to forking: %s", strerror(errno));
      moonbr_terminate_error();
    }
    worker->pid = fork();
    if (worker->pid == -1) {
      moonbr_log(LOG_ERR, "Could not fork: %s", strerror(errno));
      close(controlfds[0]);
      close(controlfds[1]);
      close(errorfds[0]);
      close(errorfds[1]);
      free(worker);
      return -1;
    } else if (!worker->pid) {
      moonbr_pstate = MOONBR_PSTATE_FORKED;
#ifdef MOONBR_LUA_PANIC_BUG_WORKAROUND
      lua_pushliteral(L, "Failed to pass error message due to bug in Lua panic handler (hint: not enough memory?)");
#endif
      moonbr_memory_limit = pool->memory_limit;
      if (moonbr_pidfh && pidfile_close(moonbr_pidfh)) {
        moonbr_child_emergency_error(errorfds[1], "Could not close PID file in forked child process");
      }
      if (moonbr_logfile && moonbr_logfile != stderr && fclose(moonbr_logfile)) {
        moonbr_child_emergency_error(errorfds[1], "Could not close log file in forked child process");
      }
      if (dup2(errorfds[1], MOONBR_FD_STDERR) == -1) {
        moonbr_child_emergency_error(errorfds[1], "Could not duplicate socket to stderr file descriptor");
      }
      if (dup2(controlfds[1], MOONBR_FD_CONTROL) == -1) {
        moonbr_child_emergency_error(errorfds[1], "Could not duplicate control socket");
      }
      closefrom(MOONBR_FD_END);
      moonbr_child_run(pool, L);
    }
    if (moonbr_stat) {
      moonbr_log(LOG_INFO, "Created new worker in pool #%i with PID %i", worker->pool->poolnum, (int)worker->pid);
    }
    worker->controlfd = controlfds[0];
    worker->errorfd = errorfds[0];
    if (close(controlfds[1]) && errno != EINTR) {
      moonbr_log(LOG_CRIT, "Could not close opposite end of control file descriptor after forking");
      moonbr_terminate_error();
    }
    if (close(errorfds[1]) && errno != EINTR) {
      moonbr_log(LOG_CRIT, "Could not close opposite end of control file descriptor after forking");
      moonbr_terminate_error();
    }
  }
  worker->prev_worker = pool->last_worker;
  if (worker->prev_worker) worker->prev_worker->next_worker = worker;
  else pool->first_worker = worker;
  pool->last_worker = worker;
  pool->unassigned_worker_count++;
  pool->total_worker_count++;
  pool->worker_count_stat = 1;
  moonbr_poll_refresh_needed = 1;
  return 0;  /* return zero only in case of success */
}


/*** Functions for queues of 'struct moonbr_listener' ***/

/* Appends a 'struct moonbr_listener' to the queue of idle listeners and registers it for poll() */
static void moonbr_add_idle_listener(struct moonbr_listener *listener) {
  listener->prev_listener = listener->pool->last_idle_listener;
  if (listener->prev_listener) listener->prev_listener->next_listener = listener;
  else listener->pool->first_idle_listener = listener;
  listener->pool->last_idle_listener = listener;
  if (listener->pollidx != -1) moonbr_poll_fds[listener->pollidx].events |= POLLIN;
}

/* Removes a 'struct moonbr_listener' from the queue of idle listeners and unregisters it from poll() */
static void moonbr_remove_idle_listener(struct moonbr_listener *listener) {
  if (listener->prev_listener) listener->prev_listener->next_listener = listener->next_listener;
  else listener->pool->first_idle_listener = listener->next_listener;
  if (listener->next_listener) listener->next_listener->prev_listener = listener->prev_listener;
  else listener->pool->last_idle_listener = listener->prev_listener;
  listener->prev_listener = NULL;
  listener->next_listener = NULL;
  if (listener->pollidx != -1) moonbr_poll_fds[listener->pollidx].events &= ~POLLIN;
}

/* Adds a listener to the queue of connected listeners (i.e. waiting to have their incoming connection accepted) */
static void moonbr_add_connected_listener(struct moonbr_listener *listener) {
  listener->prev_listener = listener->pool->last_connected_listener;
  if (listener->prev_listener) listener->prev_listener->next_listener = listener;
  else listener->pool->first_connected_listener = listener;
  listener->pool->last_connected_listener = listener;
}

/* Removes and returns the first connected listener in the queue */
static struct moonbr_listener *moonbr_pop_connected_listener(struct moonbr_pool *pool) {
  struct moonbr_listener *listener = pool->first_connected_listener;
  listener->pool->first_connected_listener = listener->next_listener;
  if (listener->pool->first_connected_listener) listener->pool->first_connected_listener->prev_listener = NULL;
  else listener->pool->last_connected_listener = NULL;
  listener->next_listener = NULL;
  return listener;
}


/*** Functions to handle polling ***/

/* Returns an index to a new initialized entry in moonbr_poll_fds[] */
int moonbr_poll_fds_nextindex() {
  if (moonbr_poll_fds_count >= moonbr_poll_fds_bufsize) {
    if (moonbr_poll_fds_bufsize) moonbr_poll_fds_bufsize *= 2;
    else moonbr_poll_fds_bufsize = 1;
    moonbr_poll_fds = realloc(
      moonbr_poll_fds, moonbr_poll_fds_bufsize * sizeof(struct pollfd)
    );
    if (!moonbr_poll_fds) {
      moonbr_log(LOG_CRIT, "Memory allocation error");
      moonbr_terminate_error();
    }
  }
  moonbr_poll_fds[moonbr_poll_fds_count] = (struct pollfd){0, };
  return moonbr_poll_fds_count++;
}

/* Returns an index to a new initialized entry in moonbr_poll_workers[] */
int moonbr_poll_workers_nextindex() {
  if (moonbr_poll_worker_count >= moonbr_poll_workers_bufsize) {
    if (moonbr_poll_workers_bufsize) moonbr_poll_workers_bufsize *= 2;
    else moonbr_poll_workers_bufsize = 1;
    moonbr_poll_workers = realloc(
      moonbr_poll_workers, moonbr_poll_workers_bufsize * sizeof(struct moonbr_poll_worker)
    );
    if (!moonbr_poll_workers) {
      moonbr_log(LOG_CRIT, "Memory allocation error");
      moonbr_terminate_error();
    }
  }
  moonbr_poll_workers[moonbr_poll_worker_count] = (struct moonbr_poll_worker){0, };
  return moonbr_poll_worker_count++;
}

/* Queues all listeners as idle, and initializes static part of moonbr_poll_fds[], which is passed to poll() */
static void moonbr_poll_init() {
  if (socketpair(
    PF_LOCAL,
    SOCK_STREAM | SOCK_CLOEXEC | SOCK_NONBLOCK,
    0,
    moonbr_poll_signalfds
  )) {
    moonbr_log(LOG_CRIT, "Could not create socket pair for signal delivery during polling: %s", strerror(errno));
    moonbr_terminate_error();
  }
  {
    int j = moonbr_poll_fds_nextindex();
    struct pollfd *pollfd = &moonbr_poll_fds[j];
    pollfd->fd = moonbr_poll_signalfd_read;
    pollfd->events = POLLIN;
  }
  {
    struct moonbr_pool *pool;
    for (pool=moonbr_first_pool; pool; pool=pool->next_pool) {
      int i;
      for (i=0; i<pool->listener_count; i++) {
        struct moonbr_listener *listener = &pool->listener[i];
        if (listener->listenfd != -1) {
          int j = moonbr_poll_fds_nextindex();
          listener->pollidx = j;
          moonbr_poll_fds[j].fd = listener->listenfd;
        }
        moonbr_add_idle_listener(listener);
      }
    }
  }
  moonbr_poll_fds_static_count = moonbr_poll_fds_count;  /* remember size of static part of array */
}

/* Disables polling of all listeners (required for clean shutdown) */
static void moonbr_poll_shutdown() {
  int i;
  for (i=1; i<moonbr_poll_fds_static_count; i++) {
    moonbr_poll_fds[i].fd = -1;
  }
}

/* (Re)builds dynamic part of moonbr_poll_fds[] array, and (re)builds moonbr_poll_workers[] array */
static void moonbr_poll_refresh() {
  moonbr_poll_refresh_needed = 0;
  moonbr_poll_fds_count = moonbr_poll_fds_static_count;
  moonbr_poll_worker_count = 0;
  {
    struct moonbr_pool *pool;
    for (pool=moonbr_first_pool; pool; pool=pool->next_pool) {
      struct moonbr_worker *worker;
      for (worker=pool->first_worker; worker; worker=worker->next_worker) {
        if (worker->controlfd != -1) {
          int j = moonbr_poll_fds_nextindex();
          int k = moonbr_poll_workers_nextindex();
          struct pollfd *pollfd = &moonbr_poll_fds[j];
          struct moonbr_poll_worker *poll_worker = &moonbr_poll_workers[k];
          pollfd->fd = worker->controlfd;
          pollfd->events = POLLIN;
          poll_worker->channel = MOONBR_POLL_WORKER_CONTROLCHANNEL;
          poll_worker->worker = worker;
        }
        if (worker->errorfd != -1) {
          int j = moonbr_poll_fds_nextindex();
          int k = moonbr_poll_workers_nextindex();
          struct pollfd *pollfd = &moonbr_poll_fds[j];
          struct moonbr_poll_worker *poll_worker = &moonbr_poll_workers[k];
          pollfd->fd = worker->errorfd;
          pollfd->events = POLLIN;
          poll_worker->channel = MOONBR_POLL_WORKER_ERRORCHANNEL;
          poll_worker->worker = worker;
        }
      }
    }
  }
}

/* resets socket and 'revents' field of moonbr_poll_fds[] for signal delivery just before poll() is called */
static void moonbr_poll_reset_signal() {
  ssize_t readcount;
  char buf[1];
  moonbr_poll_fds[0].revents = 0;
  while ((readcount = read(moonbr_poll_signalfd_read, buf, 1)) < 0) {
    if (errno == EAGAIN) break;
    if (errno != EINTR) {
      moonbr_log(LOG_CRIT, "Error while reading from signal delivery socket: %s", strerror(errno));
      moonbr_terminate_error();
    }
  }
  if (!readcount) {
    moonbr_log(LOG_CRIT, "Unexpected EOF when reading from signal delivery socket: %s", strerror(errno));
    moonbr_terminate_error();
  }
}


/*** Shutdown initiation ***/

/* Sets global variable 'moonbr_shutdown_in_progress', closes listeners, and demands worker termination */
static void moonbr_initiate_shutdown() {
  struct moonbr_pool *pool;
  int i;
  struct moonbr_worker *worker;
  if (moonbr_shutdown_in_progress) {
    moonbr_log(LOG_NOTICE, "Shutdown already in progress");
    return;
  }
  moonbr_shutdown_in_progress = 1;
  moonbr_log(LOG_NOTICE, "Initiate shutdown");
  for (pool = moonbr_first_pool; pool; pool = pool->next_pool) {
    for (i=0; i<pool->listener_count; i++) {
      struct moonbr_listener *listener = &pool->listener[i];
      if (listener->listenfd != -1) {
        if (close(listener->listenfd) && errno != EINTR) {
          moonbr_log(LOG_CRIT, "Could not close listening socket: %s", strerror(errno));
          moonbr_terminate_error();
        }
      }
    }
    for (worker=pool->first_worker; worker; worker=worker->next_worker) {
      if (moonbr_debug) {
        moonbr_log(LOG_DEBUG, "Sending SIGTERM to child with PID %i", (int)worker->pid);
      }
      if (kill(worker->pid, SIGTERM)) {
        moonbr_log(LOG_ERR, "Error while terminating child process: %s", strerror(errno));
      }
    }
  }
  moonbr_poll_shutdown();  /* avoids loops due to error condition when polling closed listeners */
}


/*** Functions to handle previously created 'struct moonbr_worker' structures ***/

#define moonbr_try_destroy_worker_stat(str, field) \
  moonbr_log(LOG_INFO, "Resource usage in pool #%i for PID %i: " str " %li", worker->pool->poolnum, (int)worker->pid, (long)childusage.field);

/* Destroys a worker structure if socket connections have been closed and child process has terminated */
static int moonbr_try_destroy_worker(struct moonbr_worker *worker) {
  if (worker->controlfd != -1 || worker->errorfd != -1) return MOONBR_DESTROY_NONE;
  {
    int childstatus;
    struct rusage childusage;
    {
      pid_t waitedpid;
      while (
        (waitedpid = wait4(worker->pid, &childstatus, WNOHANG, &childusage)) == -1
      ) {
        if (errno != EINTR) {
          moonbr_log(LOG_CRIT, "Error in wait4() call: %s", strerror(errno));
          moonbr_terminate_error();
        }
      }
      if (!waitedpid) return 0;  /* return 0 if worker couldn't be destroyed */
      if (waitedpid != worker->pid) {
        moonbr_log(LOG_CRIT, "Wrong PID returned by wait4() call");
        moonbr_terminate_error();
      }
    }
    if (WIFEXITED(childstatus)) {
      if (WEXITSTATUS(childstatus) || moonbr_stat) {
        moonbr_log(
          WEXITSTATUS(childstatus) ? LOG_WARNING : LOG_INFO,
          "Child process in pool #%i with PID %i returned with exit code %i", worker->pool->poolnum, (int)worker->pid, WEXITSTATUS(childstatus)
        );
      }
    } else if (WIFSIGNALED(childstatus)) {
      if (WCOREDUMP(childstatus)) {
        moonbr_log(LOG_ERR, "Child process in pool #%i with PID %i died from signal %i (core dump was created)", worker->pool->poolnum, (int)worker->pid, WTERMSIG(childstatus));
      } else if (WTERMSIG(childstatus) == SIGALRM) {
        moonbr_log(LOG_WARNING, "Child process in pool #%i with PID %i exited prematurely due to timeout", worker->pool->poolnum, (int)worker->pid);
      } else {
        moonbr_log(LOG_ERR, "Child process in pool #%i with PID %i died from signal %i", worker->pool->poolnum, (int)worker->pid, WTERMSIG(childstatus));
      }
    } else {
      moonbr_log(LOG_CRIT, "Illegal exit status from child process in pool #%i with PID %i", worker->pool->poolnum, (int)worker->pid);
      moonbr_terminate_error();
    }
    if (moonbr_stat) {
      moonbr_log(LOG_INFO, "Resource usage in pool #%i for PID %i: user time %s", worker->pool->poolnum, (int)worker->pid, moonbr_format_timeval(&childusage.ru_utime));
      moonbr_log(LOG_INFO, "Resource usage in pool #%i for PID %i: system time %s", worker->pool->poolnum, (int)worker->pid, moonbr_format_timeval(&childusage.ru_stime));
      moonbr_try_destroy_worker_stat("max resident set size", ru_maxrss);
      moonbr_try_destroy_worker_stat("integral shared memory size", ru_ixrss);
      moonbr_try_destroy_worker_stat("integral unshared data", ru_idrss);
      moonbr_try_destroy_worker_stat("integral unshared stack", ru_isrss);
      moonbr_try_destroy_worker_stat("page replaims", ru_minflt);
      moonbr_try_destroy_worker_stat("page faults", ru_majflt);
      moonbr_try_destroy_worker_stat("swaps", ru_nswap);
      moonbr_try_destroy_worker_stat("block input operations", ru_inblock);
      moonbr_try_destroy_worker_stat("block output operations", ru_oublock);
      moonbr_try_destroy_worker_stat("messages sent", ru_msgsnd);
      moonbr_try_destroy_worker_stat("messages received", ru_msgrcv);
      moonbr_try_destroy_worker_stat("signals received", ru_nsignals);
      moonbr_try_destroy_worker_stat("voluntary context switches", ru_nvcsw);
      moonbr_try_destroy_worker_stat("involuntary context switches", ru_nivcsw);
    }
  }
  {
    int retval = (
      (worker->idle || worker->assigned) ?
      MOONBR_DESTROY_IDLE_OR_ASSIGNED :
      MOONBR_DESTROY_PREPARE
    );
    if (worker->main && !moonbr_shutdown_in_progress) moonbr_initiate_shutdown();
    if (worker->prev_worker) worker->prev_worker->next_worker = worker->next_worker;
    else worker->pool->first_worker = worker->next_worker;
    if (worker->next_worker) worker->next_worker->prev_worker = worker->prev_worker;
    else worker->pool->last_worker = worker->prev_worker;
    if (worker->idle) {
      if (worker->prev_idle_worker) worker->prev_idle_worker->next_idle_worker = worker->next_idle_worker;
      else worker->pool->first_idle_worker = worker->next_idle_worker;
      if (worker->next_idle_worker) worker->next_idle_worker->prev_idle_worker = worker->prev_idle_worker;
      else worker->pool->last_idle_worker  = worker->prev_idle_worker;
      worker->pool->idle_worker_count--;
    }
    if (!worker->assigned) worker->pool->unassigned_worker_count--;
    worker->pool->total_worker_count--;
    worker->pool->worker_count_stat = 1;
    if (worker->errorlinebuf) free(worker->errorlinebuf);
    free(worker);
    return retval;
  }
}

/* Marks a worker as idle and stores it in a queue, optionally setting 'idle_expiration' value */
static void moonbr_add_idle_worker(struct moonbr_worker *worker) {
  worker->prev_idle_worker = worker->pool->last_idle_worker;
  if (worker->prev_idle_worker) worker->prev_idle_worker->next_idle_worker = worker;
  else worker->pool->first_idle_worker = worker;
  worker->pool->last_idle_worker = worker;
  worker->idle = 1;
  worker->pool->idle_worker_count++;
  if (worker->assigned) {
    worker->assigned = 0;
    worker->pool->unassigned_worker_count++;
  }
  worker->pool->worker_count_stat = 1;
  if (timerisset(&worker->pool->idle_timeout)) {
    struct timeval now;
    moonbr_now(&now);
    timeradd(&now, &worker->pool->idle_timeout, &worker->idle_expiration);
  }
}

/* Pops a worker from the queue of idle workers (idle queue must not be empty) */
static struct moonbr_worker *moonbr_pop_idle_worker(struct moonbr_pool *pool) {
  struct moonbr_worker *worker;
  worker = pool->first_idle_worker;
  pool->first_idle_worker = worker->next_idle_worker;
  if (pool->first_idle_worker) pool->first_idle_worker->prev_idle_worker = NULL;
  else pool->last_idle_worker = NULL;
  worker->next_idle_worker = NULL;
  worker->idle = 0;
  worker->pool->idle_worker_count--;
  worker->assigned = 1;
  worker->pool->unassigned_worker_count--;
  worker->pool->worker_count_stat = 1;
  return worker;
}


/*** Functions to communicate with child processes ***/

/* Tells child process to terminate */
static void moonbr_terminate_idle_worker(struct moonbr_worker *worker) {
  moonbr_send_control_message(worker, MOONBR_COMMAND_TERMINATE, -1, NULL);
}

/* Handles status messages from child process */
static void moonbr_read_controlchannel(struct moonbr_worker *worker) {
  char controlmsg;
  {
    ssize_t bytes_read;
    while ((bytes_read = read(worker->controlfd, &controlmsg, 1)) <= 0) {
      if (bytes_read == 0 || errno == ECONNRESET) {
        moonbr_log(LOG_WARNING, "Child process in pool #%i with PID %i unexpectedly closed control socket", worker->pool->poolnum, (int)worker->pid);
        if (close(worker->controlfd) && errno != EINTR) {
          moonbr_log(LOG_CRIT, "Error while closing control socket to child process in pool #%i with PID %i: %s", worker->pool->poolnum, (int)worker->pid, strerror(errno));
          moonbr_terminate_error();
        }
        worker->controlfd = -1;
        moonbr_poll_refresh_needed = 1;
        return;
      }
      if (errno != EINTR) {
        moonbr_log(LOG_CRIT, "Unexpected error while reading control socket from child process in pool #%i with PID %i: %s", worker->pool->poolnum, (int)worker->pid, strerror(errno));
        moonbr_terminate_error();
      }
    }
  }
  if (worker->idle) {
    moonbr_log(LOG_CRIT, "Unexpected data from supposedly idle child process in pool #%i with PID %i", worker->pool->poolnum, (int)worker->pid);
    moonbr_terminate_error();
  }
  if (moonbr_debug) {
    moonbr_log(LOG_DEBUG, "Received control message from child in pool #%i with PID %i: \"%c\"", worker->pool->poolnum, (int)worker->pid, (int)controlmsg);
  }
  switch (controlmsg) {
  case MOONBR_STATUS_IDLE:
    if (moonbr_stat) {
      moonbr_log(LOG_INFO, "Child process in pool #%i with PID %i reports as idle", worker->pool->poolnum, (int)worker->pid);
    }
    moonbr_add_idle_worker(worker);
    break;
  case MOONBR_STATUS_GOODBYE:
    if (moonbr_stat) {
      moonbr_log(LOG_INFO, "Child process in pool #%i with PID %i announced termination", worker->pool->poolnum, (int)worker->pid);
    }
    if (close(worker->controlfd) && errno != EINTR) {
      moonbr_log(LOG_CRIT, "Error while closing control socket to child process in pool #%i with PID %i: %s", worker->pool->poolnum, (int)worker->pid, strerror(errno));
      moonbr_terminate_error();
    }
    worker->controlfd = -1;
    moonbr_poll_refresh_needed = 1;
    break;
  default:
    moonbr_log(LOG_CRIT, "Received illegal data (\"%c\") while reading control socket from child process in pool #%i with PID %i", (int)controlmsg, worker->pool->poolnum, (int)worker->pid);
    moonbr_terminate_error();
  }
}

/* Handles stderr stream from child process */
static void moonbr_read_errorchannel(struct moonbr_worker *worker) {
  char staticbuf[MOONBR_MAXERRORLINELEN+1];
  char *buf = worker->errorlinebuf;
  if (!buf) buf = staticbuf;
  {
    ssize_t bytes_read;
    while (
      (bytes_read = read(
        worker->errorfd,
        buf + worker->errorlinelen,
        MOONBR_MAXERRORLINELEN+1 - worker->errorlinelen
      )) <= 0
    ) {
      if (bytes_read == 0 || errno == ECONNRESET) {
        if (moonbr_debug) {
          moonbr_log(LOG_DEBUG, "Child process in pool #%i with PID %i closed stderr socket", worker->pool->poolnum, (int)worker->pid);
        }
        if (close(worker->errorfd) && errno != EINTR) {
          moonbr_log(LOG_CRIT, "Error while closing stderr socket to child process in pool #%i with PID %i: %s", worker->pool->poolnum, (int)worker->pid, strerror(errno));
          moonbr_terminate_error();
        }
        worker->errorfd = -1;
        moonbr_poll_refresh_needed = 1;
        break;
      }
      if (errno != EINTR) {
        moonbr_log(LOG_CRIT, "Unexpected error while reading stderr from child process in pool #%i with PID %i: %s", worker->pool->poolnum, (int)worker->pid, strerror(errno));
        moonbr_terminate_error();
      }
    }
    worker->errorlinelen += bytes_read;
  }
  {
    int i;
    for (i=0; i<worker->errorlinelen; i++) {
      if (buf[i] == '\n') buf[i] = 0;
      if (!buf[i]) {
        if (worker->errorlineovf) {
          worker->errorlineovf = 0;
        } else {
          moonbr_log(LOG_WARNING, "[Pool #%i, PID %i] %s", worker->pool->poolnum, (int)worker->pid, buf);
        }
        worker->errorlinelen -= i+1;
        memmove(buf, buf+i+1, worker->errorlinelen);
        i = -1;
      }
    }
    if (i > MOONBR_MAXERRORLINELEN) {
      buf[MOONBR_MAXERRORLINELEN] = 0;
      if (!worker->errorlineovf) {
        moonbr_log(LOG_WARNING, "[Pool #%i, PID %i](line has been truncated) %s", worker->pool->poolnum, (int)worker->pid, buf);
      }
      worker->errorlinelen = 0;
      worker->errorlineovf = 1;
    }
  }
  if (!worker->errorlinebuf && worker->errorlinelen) {  /* allocate buffer on heap only if necessary */
    worker->errorlinebuf = malloc((MOONBR_MAXERRORLINELEN+1) * sizeof(char));
    if (!worker->errorlinebuf) {
      moonbr_log(LOG_CRIT, "Memory allocation error");
      moonbr_terminate_error();
    }
    memcpy(worker->errorlinebuf, staticbuf, worker->errorlinelen);
  }
}


/*** Handler for incoming connections ***/

/* Accepts one or more incoming connections on listener socket and passes it to worker(s) popped from idle queue */
static void moonbr_connect(struct moonbr_pool *pool) {
  struct moonbr_listener *listener = moonbr_pop_connected_listener(pool);
  struct moonbr_worker *worker;
  if (listener->proto == MOONBR_PROTO_INTERVAL) {
    worker = moonbr_pop_idle_worker(pool);
    if (listener->type_specific.interval.main) {
      if (moonbr_stat) {
        moonbr_log(LOG_INFO, "Dispatching main thread \"%s\" of pool #%i to PID %i", listener->type_specific.interval.name, listener->pool->poolnum, (int)worker->pid);
      }
      worker->main = 1;
    } else {
      if (moonbr_stat) {
        moonbr_log(LOG_INFO, "Dispatching interval timer \"%s\" of pool #%i to PID %i", listener->type_specific.interval.name, listener->pool->poolnum, (int)worker->pid);
      }
      worker->restart_interval_listener = listener;
    }
    moonbr_send_control_message(worker, MOONBR_COMMAND_CONNECT, -1, listener);
    /* do not push listener to queue of idle listeners (yet) */
  } else {
    int peerfd;
    do {
#if defined(__linux__) && !defined(_GNU_SOURCE)
      peerfd = accept(listener->listenfd, NULL, NULL);
      if (peerfd != -1) {
        if (fcntl(peerfd, F_SETFD, FD_CLOEXEC) == -1) {
          moonbr_log(LOG_ERR, "Error in fcntl() call: %s", strerror(errno));
          moonbr_terminate_error();
        }
      }
#else
      peerfd = accept4(listener->listenfd, NULL, NULL, SOCK_CLOEXEC);
#endif
      if (peerfd == -1) {
        if (errno == EWOULDBLOCK) {
          break;
        } else if (errno == ECONNABORTED) {
          moonbr_log(LOG_WARNING, "Connection aborted before accepting it");
          break;
        } else if (errno != EINTR) {
          moonbr_log(LOG_ERR, "Could not accept socket connection: %s", strerror(errno));
          moonbr_terminate_error();
        }
      } else {
        worker = moonbr_pop_idle_worker(pool);
        if (moonbr_stat) {
          moonbr_log(LOG_INFO, "Dispatching connection for pool #%i to PID %i", listener->pool->poolnum, (int)worker->pid);
        }
        moonbr_send_control_message(worker, MOONBR_COMMAND_CONNECT, peerfd, listener);
        if (close(peerfd) && errno != EINTR) {
          moonbr_log(LOG_ERR, "Could not close incoming socket connection in parent process: %s", strerror(errno));
          moonbr_terminate_error();
        }
      }
    } while (pool->first_idle_worker);
    moonbr_add_idle_listener(listener);
  }
}


/*** Functions to initialize and restart interval timers ***/

/* Initializes all interval timers */
static void moonbr_interval_initialize() {
  struct timeval now;
  struct moonbr_pool *pool;
  moonbr_now(&now);
  for (pool=moonbr_first_pool; pool; pool=pool->next_pool) {
    int i;
    for (i=0; i<pool->listener_count; i++) {
      struct moonbr_listener *listener = &pool->listener[i];
      if (listener->proto == MOONBR_PROTO_INTERVAL) {
        timeradd(
          &now,
          &listener->type_specific.interval.delay,
          &listener->type_specific.interval.wakeup
        );
      }
    }
  }
}

/* If necessary, restarts interval timers and queues interval listener as idle after a worker changed status */
static void moonbr_interval_restart(
  struct moonbr_worker *worker,
  struct timeval *now           /* passed to synchronize with moonbr_run() function */
) {
  struct moonbr_listener *listener = worker->restart_interval_listener;
  if (listener) {
    moonbr_add_idle_listener(listener);
    worker->restart_interval_listener = NULL;
    if (listener->type_specific.interval.strict) {
      timeradd(
        &listener->type_specific.interval.wakeup,
        &listener->type_specific.interval.delay,
        &listener->type_specific.interval.wakeup
      );
      if (timercmp(&listener->type_specific.interval.wakeup, now, <)) {
        listener->type_specific.interval.wakeup = *now;
      }
    } else {
      timeradd(
        now,
        &listener->type_specific.interval.delay,
        &listener->type_specific.interval.wakeup
      );
    }
  }
}


/*** Main loop and helper functions ***/

/* Stores the earliest required wakeup time in 'wait' variable */
static void moonbr_calc_wait(struct timeval *wait, struct timeval *wakeup) {
  if (!timerisset(wait) || timercmp(wakeup, wait, <)) *wait = *wakeup;
}

/* Main loop of Moonbridge system (including initialization of signal handlers and polling structures) */
static void moonbr_run(lua_State *L) {
  struct timeval now;
  struct moonbr_pool *pool;
  struct moonbr_worker *worker;
  struct moonbr_worker *next_worker;  /* needed when worker is removed during iteration of workers */
  struct moonbr_listener *listener;
  struct moonbr_listener *next_listener;  /* needed when listener is removed during iteration of listeners */
  int i;
  moonbr_poll_init();    /* must be executed before moonbr_signal_init() */
  moonbr_signal_init();
  moonbr_interval_initialize();
  moonbr_pstate = MOONBR_PSTATE_RUNNING;
  while (1) {
    struct timeval wait = {0, };  /* point in time when premature wakeup of poll() is required */
    if (moonbr_cond_interrupt) {
      moonbr_log(LOG_WARNING, "Fast shutdown requested");
      moonbr_terminate(MOONBR_EXITCODE_GRACEFUL);
    }
    if (moonbr_cond_terminate) {
      moonbr_initiate_shutdown();
      moonbr_cond_terminate = 0;
    }
    moonbr_cond_child = 0;  /* must not be reset between moonbr_try_destroy_worker() and poll() */
    moonbr_now(&now);
    for (pool=moonbr_first_pool; pool; pool=pool->next_pool) {
      int terminated_worker_count = 0;  /* allows shortcut for new worker creation */
      /* terminate idle workers when expired */
      if (timerisset(&pool->idle_timeout)) {
        while ((worker = pool->first_idle_worker) != NULL) {
          if (timercmp(&worker->idle_expiration, &now, >)) break;
          moonbr_pop_idle_worker(pool);
          moonbr_terminate_idle_worker(worker);
        }
      }
      /* mark listeners as connected when incoming connection is pending */
      for (listener=pool->first_idle_listener; listener; listener=next_listener) {
        next_listener = listener->next_listener;  /* extra variable necessary due to changing list */
        if (listener->pollidx != -1) {
          if (moonbr_poll_fds[listener->pollidx].revents) {
            moonbr_poll_fds[listener->pollidx].revents = 0;
            moonbr_remove_idle_listener(listener);
            moonbr_add_connected_listener(listener);
          }
        } else if (
          listener->proto != MOONBR_PROTO_INTERVAL ||
          !timercmp(&listener->type_specific.interval.wakeup, &now, >)
        ) {
          moonbr_remove_idle_listener(listener);
          moonbr_add_connected_listener(listener);
        }
      }
      /* process input from child processes */
      for (i=0; i<moonbr_poll_worker_count; i++) {
        if (moonbr_poll_worker_fds[i].revents) {
          moonbr_poll_worker_fds[i].revents = 0;
          struct moonbr_poll_worker *poll_worker = &moonbr_poll_workers[i];
          switch (poll_worker->channel) {
          case MOONBR_POLL_WORKER_CONTROLCHANNEL:
            moonbr_read_controlchannel(poll_worker->worker);
            moonbr_interval_restart(poll_worker->worker, &now);
            break;
          case MOONBR_POLL_WORKER_ERRORCHANNEL:
            moonbr_read_errorchannel(poll_worker->worker);
            break;
          }
        }
      }
      /* collect dead child processes */
      for (worker=pool->first_worker; worker; worker=next_worker) {
        next_worker = worker->next_worker;  /* extra variable necessary due to changing list */
        switch (moonbr_try_destroy_worker(worker)) {
        case MOONBR_DESTROY_PREPARE:
          pool->use_fork_error_wakeup = 1;
          break;
        case MOONBR_DESTROY_IDLE_OR_ASSIGNED:
          terminated_worker_count++;
          break;
        }
      }
      if (!moonbr_shutdown_in_progress) {
        /* connect listeners with idle workers */
        while (pool->first_connected_listener && pool->first_idle_worker) {
          moonbr_connect(pool);
        }
        /* create new worker processes */
        while (
          pool->total_worker_count < pool->max_fork && (
            pool->unassigned_worker_count < pool->pre_fork ||
            pool->total_worker_count < pool->min_fork
          )
        ) {
          if (pool->use_fork_error_wakeup) {
            if (timercmp(&pool->fork_error_wakeup, &now, >)) {
            moonbr_calc_wait(&wait, &pool->fork_error_wakeup);
            break;
            }
          } else {
            if (terminated_worker_count) {
              terminated_worker_count--;
            } else if (timercmp(&pool->fork_wakeup, &now, >)) {
              moonbr_calc_wait(&wait, &pool->fork_wakeup);
              break;
            }
          }
          if (moonbr_create_worker(pool, L)) {
            /* on error, enforce error delay */
            timeradd(&now, &pool->fork_error_delay, &pool->fork_error_wakeup);
            pool->use_fork_error_wakeup = 1;
            moonbr_calc_wait(&wait, &pool->fork_error_wakeup);
            break;
          } else {
            /* normal fork delay on success */
            timeradd(&now, &pool->fork_delay, &pool->fork_wakeup);
            timeradd(&now, &pool->fork_error_delay, &pool->fork_error_wakeup);
            pool->use_fork_error_wakeup = 0;  /* gets set later if error occures during preparation */
          }
        }
        /* terminate excessive worker processes */
        while (
          pool->total_worker_count > pool->min_fork &&
          pool->idle_worker_count > pool->pre_fork
        ) {
          if (timerisset(&pool->exit_wakeup)) {
            if (timercmp(&pool->exit_wakeup, &now, >)) {
              moonbr_calc_wait(&wait, &pool->exit_wakeup);
              break;
            }
            moonbr_terminate_idle_worker(moonbr_pop_idle_worker(pool));
            timeradd(&now, &pool->exit_delay, &pool->exit_wakeup);
          } else {
            timeradd(&now, &pool->exit_delay, &pool->exit_wakeup);
            break;
          }
        }
        if (!(
          pool->total_worker_count > pool->min_fork &&
          pool->idle_worker_count > pool->pre_fork
        )) {
          timerclear(&pool->exit_wakeup);  /* timer gets restarted later when there are excessive workers */
        }
      }
      /* optionally output worker count stats */
      if (moonbr_stat && pool->worker_count_stat) {
        pool->worker_count_stat = 0;
        moonbr_log(
          LOG_INFO,
          "Worker count for pool #%i: %i idle, %i assigned, %i total",
          pool->poolnum, pool->idle_worker_count,
          pool->total_worker_count - pool->unassigned_worker_count,
          pool->total_worker_count);
      }
      /* calculate wakeup time for interval listeners */
      for (listener=pool->first_idle_listener; listener; listener=listener->next_listener) {
        if (listener->proto == MOONBR_PROTO_INTERVAL) {
          moonbr_calc_wait(&wait, &listener->type_specific.interval.wakeup);
        }
      }
      /* calculate wakeup time for idle workers (only first idle worker is significant) */
      if (timerisset(&pool->idle_timeout) && pool->first_idle_worker) {
        moonbr_calc_wait(&wait, &pool->first_idle_worker->idle_expiration);
      }
    }
    /* terminate idle workers in case of shutdown and check if shutdown is complete */
    if (moonbr_shutdown_in_progress) {
      int remaining = 0;
      for (pool=moonbr_first_pool; pool; pool=pool->next_pool) {
        while (pool->idle_worker_count) {
          moonbr_terminate_idle_worker(moonbr_pop_idle_worker(pool));
        }
        if (pool->first_worker) remaining = 1;
      }
      if (!remaining) {
        moonbr_log(LOG_INFO, "All worker threads have terminated");
        moonbr_terminate(MOONBR_EXITCODE_GRACEFUL);
      }
    }
    if (moonbr_poll_refresh_needed) moonbr_poll_refresh();
    moonbr_cond_poll = 1;
    if (!moonbr_cond_child && !moonbr_cond_terminate && !moonbr_cond_interrupt) {
      int timeout;
      if (timerisset(&wait)) {
        if (timercmp(&wait, &now, <)) {
          moonbr_log(LOG_CRIT, "Internal error (should not happen): Future is in the past");
          moonbr_terminate_error();
        }
        timersub(&wait, &now, &wait);
        timeout = wait.tv_sec * 1000 + wait.tv_usec / 1000;
      } else {
        timeout = INFTIM;
      }
      if (moonbr_debug) {
        moonbr_log(LOG_DEBUG, "Waiting for I/O");
      }
      poll(moonbr_poll_fds, moonbr_poll_fds_count, timeout);
    } else {
      if (moonbr_debug) {
        moonbr_log(LOG_DEBUG, "Do not wait for I/O");
      }
    }
    moonbr_cond_poll = 0;
    moonbr_poll_reset_signal();
  }
}


/*** Lua interface ***/

static int moonbr_lua_panic(lua_State *L) {
  const char *errmsg;
  errmsg = lua_tostring(L, -1);
  if (!errmsg) {
    if (lua_isnoneornil(L, -1)) errmsg = "(error message is nil)";
    else errmsg = "(error message is not a string)";
  }
  if (moonbr_pstate == MOONBR_PSTATE_FORKED) {
    fprintf(stderr, "Uncaught Lua error: %s\n", errmsg);
    exit(1);
  } else {
    moonbr_log(LOG_CRIT, "Uncaught Lua error: %s", errmsg);
    moonbr_terminate_error();
  }
  return 0;
}

static int moonbr_addtraceback(lua_State *L) {
  luaL_traceback(L, L, luaL_tolstring(L, 1, NULL), 1);
  return 1;
}

/* Memory allocator that allows limiting memory consumption */
static void *moonbr_alloc (void *ud, void *ptr, size_t osize, size_t nsize) {
  (void)ud;  /* not used */
  if (nsize == 0) {
    if (ptr) {
      moonbr_memory_usage -= osize;
      free(ptr);
    }
    return NULL;
  } else if (ptr) {
    if (
      moonbr_memory_limit &&
      nsize > osize &&
      moonbr_memory_usage + (nsize - osize) > moonbr_memory_limit
    ) {
      return NULL;
    } else {
      ptr = realloc(ptr, nsize);
      if (ptr) moonbr_memory_usage += nsize - osize;
    }
  } else {
    if (
      moonbr_memory_limit &&
      moonbr_memory_usage + nsize > moonbr_memory_limit
    ) {
      return NULL;
    } else {
      ptr = realloc(ptr, nsize);
      if (ptr) moonbr_memory_usage += nsize;
    }
  }
  return ptr;
}

static int moonbr_lua_tonatural(lua_State *L, int idx) {
  int isnum;
  lua_Number n;
  n = lua_tonumberx(L, idx, &isnum);
  if (isnum && n>=0 && n<INT_MAX && (lua_Number)(int)n == n) return n;
  else return -1;
}

static int moonbr_lua_totimeval(lua_State *L, int idx, struct timeval *value) {
  int isnum;
  lua_Number n;
  n = lua_tonumberx(L, idx, &isnum);
  if (isnum && n>=0 && n<=100000000) {
    value->tv_sec = n;
    value->tv_usec = 1e6 * (n - value->tv_sec);
    return 1;
  } else {
    return 0;
  }
}

static int moonbr_timeout(lua_State *L) {
  struct itimerval oldval;
  if (lua_isnoneornil(L, 1) && lua_isnoneornil(L, 2)) {
    getitimer(ITIMER_REAL, &oldval);
  } else {
    struct itimerval newval = {};
    timerclear(&newval.it_interval);
    timerclear(&newval.it_value);
    if (lua_toboolean(L, 1)) {
      luaL_argcheck(
        L, moonbr_lua_totimeval(L, 1, &newval.it_value), 1,
        "interval in seconds expected"
      );
    }
    if (lua_isnoneornil(L, 2)) {
      if (setitimer(ITIMER_REAL, &newval, &oldval)) {
        moonbr_log(LOG_CRIT, "Could not set ITIMER_REAL via setitimer()");
        moonbr_terminate_error();
      }
    } else {
      getitimer(ITIMER_REAL, &oldval);
      if (!timerisset(&oldval.it_value)) {
        if (setitimer(ITIMER_REAL, &newval, NULL)) {
          moonbr_log(LOG_CRIT, "Could not set ITIMER_REAL via setitimer()");
          moonbr_terminate_error();
        }
        lua_call(L, lua_gettop(L) - 2, LUA_MULTRET);
        timerclear(&newval.it_value);
        if (setitimer(ITIMER_REAL, &newval, NULL)) {
          moonbr_log(LOG_CRIT, "Could not set ITIMER_REAL via setitimer()");
          moonbr_terminate_error();
        }
      } else if (timercmp(&newval.it_value, &oldval.it_value, <)) {
        struct itimerval remval;
        if (setitimer(ITIMER_REAL, &newval, NULL)) {
          moonbr_log(LOG_CRIT, "Could not set ITIMER_REAL via setitimer()");
          moonbr_terminate_error();
        }
        lua_call(L, lua_gettop(L) - 2, LUA_MULTRET);
        getitimer(ITIMER_REAL, &remval);
        timersub(&oldval.it_value, &newval.it_value, &newval.it_value);
        timeradd(&newval.it_value, &remval.it_value, &newval.it_value);
        if (setitimer(ITIMER_REAL, &newval, NULL)) {
          moonbr_log(LOG_CRIT, "Could not set ITIMER_REAL via setitimer()");
          moonbr_terminate_error();
        }
      } else {
        lua_call(L, lua_gettop(L) - 2, LUA_MULTRET);
      }
      return lua_gettop(L) - 1;
    }
  }
  lua_pushnumber(L, oldval.it_value.tv_sec + 1e-6 * oldval.it_value.tv_usec);
  return 1;
}

#define moonbr_listen_init_pool_forkoption(luaname, cname, defval) do { \
  lua_getfield(L, 2, luaname); \
  pool->cname = lua_isnil(L, -1) ? (defval) : moonbr_lua_tonatural(L, -1); \
} while(0)

#define moonbr_listen_init_pool_timeoption(luaname, cname, defval, defvalu) ( \
  lua_getfield(L, 2, luaname), \
  lua_isnil(L, -1) ? ( \
    pool->cname.tv_sec = (defval), pool->cname.tv_usec = (defvalu), \
    1 \
  ) : ( \
    (lua_isboolean(L, -1) && !lua_toboolean(L, -1)) ? ( \
      pool->cname.tv_sec = 0, pool->cname.tv_usec = 0, \
      1 \
    ) : ( \
      moonbr_lua_totimeval(L, -1, &pool->cname) \
    ) \
  ) \
)

static int moonbr_listen_init_pool(lua_State *L) {
  struct moonbr_pool *pool;
  const char *proto;
  int is_main;
  int i;
  int dynamic = 0;  /* nonzero = listeners exist which require dynamic worker creation */
  pool = lua_touserdata(L, 1);
  for (i=0; i<pool->listener_count; i++) {
    struct moonbr_listener *listener = &pool->listener[i];
    lua_settop(L, 2);
#if LUA_VERSION_NUM >= 503
    lua_geti(L, 2, i+1);
#else
    lua_pushinteger(L, i+1);
    lua_gettable(L, 2);
#endif
    lua_getfield(L, 3, "proto");
    proto = lua_tostring(L, -1);
    is_main = !strcmp(proto, "main");
    if (proto && (is_main || !strcmp(proto, "interval"))) {
      listener->proto = MOONBR_PROTO_INTERVAL;
      lua_getfield(L, 3, "name");
      {
        const char *name = lua_tostring(L, -1);
        if (name) {
          char *name_dup = strdup(name);
          if (!name_dup) {
            moonbr_log(LOG_CRIT, "Memory allocation_error");
            moonbr_terminate_error();
          }
          listener->type_specific.interval.name = name_dup;
        }
      }
      if (is_main) {
        listener->type_specific.interval.main = 1;
      } else {
        listener->type_specific.interval.main = 0;
        dynamic = 1;
        lua_getfield(L, 3, "delay");
        if (
          !moonbr_lua_totimeval(L, -1, &listener->type_specific.interval.delay) ||
          !timerisset(&listener->type_specific.interval.delay)
        ) {
          luaL_error(L, "No valid interval delay specified; use listen{{proto=\"interval\", delay=...}, ...}");
        }
        lua_getfield(L, 3, "strict");
        if (!lua_isnil(L, -1)) {
          if (lua_isboolean(L, -1)) {
            if (lua_toboolean(L, -1)) listener->type_specific.interval.strict = 1;
          } else {
            luaL_error(L, "Option \"strict\" must be a boolean if set; use listen{{proto=\"interval\", strict=true, ...}, ...}");
          }
        }
      }
    } else if (proto && !strcmp(proto, "local")) {
      const char *path;
      const int path_maxlen = (
        sizeof(listener->type_specific.socket.addr.addr_un) -
        ((void *)listener->type_specific.socket.addr.addr_un.sun_path - (void *)&listener->type_specific.socket.addr.addr_un)
      ) - 1;  /* one byte for termination */
      dynamic = 1;
      listener->proto = MOONBR_PROTO_LOCAL;
      lua_getfield(L, 3, "path");
      path = lua_tostring(L, -1);
      if (!path) {
        luaL_error(L, "No valid path specified for local socket; use listen{{proto=\"local\", path=...}, ...}");
      }
      if (strlen(path) > path_maxlen) {
        luaL_error(L, "Path name for local socket exceeded maximum length of %i characters", path_maxlen);
      }
      strcpy(listener->type_specific.socket.addr.addr_un.sun_path, path);
    } else if (proto && !strcmp(proto, "tcp")) {
      const char *host, *port;
      struct addrinfo hints = { 0, };
      struct addrinfo *res, *addrinfo;
      int errcode;
      const char *ip;
      dynamic = 1;
      lua_getfield(L, 3, "host");
      host = lua_isnil(L, -1) ? "::" : lua_tostring(L, -1);
      if (!host) {
        luaL_error(L, "No host specified; use listen{{proto=\"tcp\", host=...}, ...}");
      }
      lua_getfield(L, 3, "port");
      port = lua_tostring(L, -1);
      if (!port) {
        luaL_error(L, "No port specified; use listen{{proto=\"tcp\", host=...}, ...}");
      }
      hints.ai_family = AF_UNSPEC;
      hints.ai_socktype = SOCK_STREAM;
      hints.ai_protocol = IPPROTO_TCP;
      hints.ai_flags = AI_ADDRCONFIG | AI_PASSIVE;
      errcode = getaddrinfo(host, port, &hints, &res);
      if (errcode) {
        freeaddrinfo(res);
        if (errcode == EAI_SYSTEM) {
          char errmsg[MOONBR_MAXSTRERRORLEN] = MOONBR_STRERROR_R_MSG;
          strerror_r(errno, errmsg, MOONBR_MAXSTRERRORLEN);  /* use thread-safe call in case child created threads */
          luaL_error(L, "Could not resolve host: %s: %s", gai_strerror(errcode), errmsg);
        } else {
          luaL_error(L, "Could not resolve host: %s", gai_strerror(errcode));
        }
      }
      for (addrinfo=res; addrinfo; addrinfo=addrinfo->ai_next) {
        if (addrinfo->ai_family == AF_INET6) goto moonbr_listen_init_pool_found;
      }
      for (addrinfo=res; addrinfo; addrinfo=addrinfo->ai_next) {
        if (addrinfo->ai_family == AF_INET) goto moonbr_listen_init_pool_found;
      }
      addrinfo = res;
      moonbr_listen_init_pool_found:
      if (addrinfo->ai_addrlen > sizeof(listener->type_specific.socket.addr)) {
        moonbr_log(LOG_CRIT, "Size of ai_addrlen is unexpectedly big (should not happen)");
        moonbr_terminate_error();
      }
      memcpy(&listener->type_specific.socket.addr, addrinfo->ai_addr, addrinfo->ai_addrlen);
      listener->type_specific.socket.addrlen = addrinfo->ai_addrlen;
      switch (addrinfo->ai_family) {
      case AF_INET6:
        ip = inet_ntop(
          addrinfo->ai_family,
          &((struct sockaddr_in6 *)addrinfo->ai_addr)->sin6_addr,
          listener->proto_specific.tcp.ip,
          INET6_ADDRSTRLEN
        );
        if (!ip) {
          moonbr_log(LOG_CRIT, "System error in inet_ntop call: %s", strerror(errno));
          moonbr_terminate_error();
        }
        listener->proto_specific.tcp.port = ntohs(((struct sockaddr_in6 *)addrinfo->ai_addr)->sin6_port);
        break;
      case AF_INET:
        ip = inet_ntop(
          addrinfo->ai_family,
          &((struct sockaddr_in *)addrinfo->ai_addr)->sin_addr,
          listener->proto_specific.tcp.ip,
          INET6_ADDRSTRLEN
        );
        if (!ip) {
          moonbr_log(LOG_CRIT, "System error in inet_ntop call: %s", strerror(errno));
          moonbr_terminate_error();
        }
        listener->proto_specific.tcp.port = ntohs(((struct sockaddr_in *)addrinfo->ai_addr)->sin_port);
        break;
      default:
        strcpy(listener->proto_specific.tcp.ip, "unknown");
        listener->proto_specific.tcp.port = 0;
      }
      listener->proto = MOONBR_PROTO_TCP;
    } else if (proto) {
      luaL_error(L, "Unknown protocol \"%s\"", proto);
    } else {
      luaL_error(L, "No valid protocol specified; use listen{{proto=..., ...}, ...}");
    }
  }
  lua_settop(L, 2);
  if (dynamic) {
    moonbr_listen_init_pool_forkoption("pre_fork", pre_fork, 1);
    moonbr_listen_init_pool_forkoption("min_fork", min_fork, pool->pre_fork > 2 ? pool->pre_fork : 2);
    moonbr_listen_init_pool_forkoption("max_fork", max_fork, pool->min_fork > 16 ? pool->min_fork : 16);
    if (!moonbr_listen_init_pool_timeoption("fork_delay", fork_delay, 0, 250000)) {
      luaL_error(L, "Option \"fork_delay\" is expected to be a non-negative number");
    }
    if (!moonbr_listen_init_pool_timeoption("fork_error_delay", fork_error_delay, 2, 0)) {
      luaL_error(L, "Option \"fork_error_delay\" is expected to be a non-negative number");
    }
    if (!moonbr_listen_init_pool_timeoption("exit_delay", exit_delay, 60, 0)) {
      luaL_error(L, "Option \"exit_delay\" is expected to be a non-negative number");
    }
    if (timercmp(&pool->fork_error_delay, &pool->fork_delay, <)) {
      pool->fork_error_delay = pool->fork_delay;
    }
    if (!moonbr_listen_init_pool_timeoption("idle_timeout", idle_timeout, 0, 0)) {
      luaL_error(L, "Option \"idle_timeout\" is expected to be a non-negative number");
    }
  } else {
    pool->pre_fork = 0;
    pool->min_fork = pool->listener_count;
    pool->max_fork = pool->listener_count;
  }
  lua_getfield(L, 2, "memory_limit");
  if (!lua_isnil(L, -1)) {
    int isnum;
    lua_Number n;
    n = lua_tonumberx(L, -1, &isnum);
    if (n < 0 || !isnum) {
      luaL_error(L, "Option \"memory_limit\" is expected to be a non-negative number");
    }
    pool->memory_limit = n;
  }
  lua_settop(L, 2);
  lua_getfield(L, 2, "prepare");
  if (!lua_isnil(L, -1) && !lua_isfunction(L, -1)) {
    luaL_error(L, "Option \"prepare\" must be nil or a function");
  }
  lua_rawsetp(L, LUA_REGISTRYINDEX, moonbr_luakey_prepare_func(pool));
  lua_getfield(L, 2, "connect");
  if (!lua_isfunction(L, -1)) {
    luaL_error(L, "Option \"connect\" must be a function; use listen{{...}, {...}, connect=function(socket) ... end, ...}");
  }
  lua_rawsetp(L, LUA_REGISTRYINDEX, moonbr_luakey_connect_func(pool));
  lua_getfield(L, 2, "finish");
  if (!lua_isnil(L, -1) && !lua_isfunction(L, -1)) {
    luaL_error(L, "Option \"finish\" must be nil or a function");
  }
  lua_rawsetp(L, LUA_REGISTRYINDEX, moonbr_luakey_finish_func(pool));
  return 0;
}

static int moonbr_listen(lua_State *L) {
  struct moonbr_pool *pool;
  lua_Integer listener_count;
  if (moonbr_booted) luaL_error(L, "Moonbridge bootup is already complete");
  luaL_checktype(L, 1, LUA_TTABLE);
  listener_count = luaL_len(L, 1);
  if (!listener_count) luaL_error(L, "No listen ports specified; use listen{{proto=..., port=...},...}");
  if (listener_count > 100) luaL_error(L, "Too many listeners");
  pool = moonbr_create_pool(listener_count);
  lua_pushcfunction(L, moonbr_listen_init_pool);
  lua_pushlightuserdata(L, pool);
  lua_pushvalue(L, 1);
  if (lua_pcall(L, 2, 0, 0)) goto moonbr_listen_error;
  {
    int i;
    i = moonbr_start_pool(pool);
    if (i >= 0) {
      lua_pushfstring(L, "Could not initialize listener #%d: %s", i+1, strerror(errno));
      moonbr_listen_error:
      moonbr_destroy_pool(pool);
      lua_pushnil(L);
      lua_rawsetp(L, LUA_REGISTRYINDEX, moonbr_luakey_prepare_func(pool));
      lua_pushnil(L);
      lua_rawsetp(L, LUA_REGISTRYINDEX, moonbr_luakey_connect_func(pool));
      lua_pushnil(L);
      lua_rawsetp(L, LUA_REGISTRYINDEX, moonbr_luakey_finish_func(pool));
      lua_error(L);
    }
  }
  return 0;
}


/*** Function to modify Lua's library path and/or cpath ***/

#if defined(MOONBR_LUA_PATH) || defined(MOONBR_LUA_CPATH)
static void moonbr_modify_path(lua_State *L, char *key, char *value) {
  int stackbase;
  stackbase = lua_gettop(L);
  lua_getglobal(L, "package");
  lua_getfield(L, stackbase+1, key);
  {
    const char *current_str;
    size_t current_strlen;
    luaL_Buffer buf;
    current_str = lua_tolstring(L, stackbase+2, &current_strlen);
    luaL_buffinit(L, &buf);
    if (current_str) {
      lua_pushvalue(L, stackbase+2);
      luaL_addvalue(&buf);
      if (current_strlen && current_str[current_strlen-1] != ';') {
        luaL_addchar(&buf, ';');
      }
    }
    luaL_addstring(&buf, value);
    luaL_pushresult(&buf);
  }
  lua_setfield(L, stackbase+1, key);
  lua_settop(L, stackbase);
}
#endif


/*** Main function and command line invokation ***/

static void moonbr_usage(int err, const char *cmd) {
  FILE *out;
  out = err ? stderr : stdout;
  if (!cmd) cmd = "moonbridge";
  fprintf(out, "Get this help message: %s {-h|--help}\n", cmd);
  fprintf(out, "Usage: %s \\\n", cmd);
  fprintf(out, "           [-b|--background] \\\n");
  fprintf(out, "           [-d|--debug] \\\n");
  fprintf(out, "           [-f|--logfacility {DAEMON|USER|0|1|...|7}] \\\n");
  fprintf(out, "           [-i|--logident <syslog ident> \\\n");
  fprintf(out, "           [-l|--logfile <logfile>] \\\n");
  fprintf(out, "           [-p|--pidfile <pidfile>] \\\n");
  fprintf(out, "           [-s|--stats] \\\n");
  fprintf(out, "           -- <Lua script> [<cmdline options for Lua script>]\n");
  exit(err);
}

#define moonbr_usage_error() moonbr_usage(MOONBR_EXITCODE_CMDLINEERROR, argc ? argv[0] : NULL)

int main(int argc, char **argv) {
  {
    int daemonize = 0;
    int log_facility = LOG_USER;
    const char *log_ident = "moonbridge";
    const char *log_filename = NULL;
    const char *pid_filename = NULL;
    int option;
    struct option longopts[] = {
      { "background",  no_argument,       NULL, 'b' },
      { "debug",       no_argument,       NULL, 'd' },
      { "logfacility", required_argument, NULL, 'f' },
      { "help",        no_argument,       NULL, 'h' },
      { "logident",    required_argument, NULL, 'i' },
      { "logfile",     required_argument, NULL, 'l' },
      { "pidfile",     required_argument, NULL, 'p' },
      { "stats",       no_argument,       NULL, 's' },
      { NULL,          0,                 NULL, 0   }
    };
    while ((option = getopt_long(argc, argv, "bdf:hi:l:p:s", longopts, NULL)) != -1) {
      switch (option) {
      case 'b':
        daemonize = 1;
        break;
      case 'd':
        moonbr_debug = 1;
        moonbr_stat = 1;
        break;
      case 'f':
        if (!strcmp(optarg, "DAEMON")) {
          log_facility = LOG_DAEMON;
        } else if (!strcmp(optarg, "USER")) {
          log_facility = LOG_USER;
        } else if (!strcmp(optarg, "0")) {
          log_facility = LOG_LOCAL0;
        } else if (!strcmp(optarg, "1")) {
          log_facility = LOG_LOCAL1;
        } else if (!strcmp(optarg, "2")) {
          log_facility = LOG_LOCAL2;
        } else if (!strcmp(optarg, "3")) {
          log_facility = LOG_LOCAL3;
        } else if (!strcmp(optarg, "4")) {
          log_facility = LOG_LOCAL4;
        } else if (!strcmp(optarg, "5")) {
          log_facility = LOG_LOCAL5;
        } else if (!strcmp(optarg, "6")) {
          log_facility = LOG_LOCAL6;
        } else if (!strcmp(optarg, "7")) {
          log_facility = LOG_LOCAL7;
        } else {
          moonbr_usage_error();
        }
        moonbr_use_syslog = 1;
        break;
      case 'h':
        moonbr_usage(MOONBR_EXITCODE_GRACEFUL, argv[0]);
        break;
      case 'i':
        log_ident = optarg;
        moonbr_use_syslog = 1;
        break;
      case 'l':
        log_filename = optarg;
        break;
      case 'p':
        pid_filename = optarg;
        break;
      case 's':
        moonbr_stat = 1;
        break;
      default:
        moonbr_usage_error();
      }
    }
    if (argc - optind < 1) moonbr_usage_error();
    if (pid_filename) {
      pid_t otherpid;
      while ((moonbr_pidfh = pidfile_open(pid_filename, 0644, &otherpid)) == NULL) {
        if (errno == EEXIST) {
          if (otherpid == -1) {
            fprintf(stderr, "PID file \"%s\" is already locked\n", pid_filename);
          } else {
            fprintf(stderr, "PID file \"%s\" is already locked by process with PID: %i\n", pid_filename, (int)otherpid);
          }
          exit(MOONBR_EXITCODE_ALREADYRUNNING);
        } else if (errno != EINTR) {
          fprintf(stderr, "Could not write PID file \"%s\": %s\n", pid_filename, strerror(errno));
          exit(MOONBR_EXITCODE_STARTUPERROR);
        }
      }
    }
    if (log_filename) {
      int logfd;
      while (
        ( logfd = flopen(
            log_filename,
            O_WRONLY|O_NONBLOCK|O_CREAT|O_APPEND|O_CLOEXEC,
            0640
          )
        ) < 0
      ) {
        if (errno == EWOULDBLOCK) {
          fprintf(stderr, "Logfile \"%s\" is locked\n", log_filename);
          exit(MOONBR_EXITCODE_ALREADYRUNNING);
        } else if (errno != EINTR) {
          fprintf(stderr, "Could not open logfile \"%s\": %s\n", log_filename, strerror(errno));
          exit(MOONBR_EXITCODE_STARTUPERROR);
        }
      }
      moonbr_logfile = fdopen(logfd, "a");
      if (!moonbr_logfile) {
        fprintf(stderr, "Could not open write stream to logfile \"%s\": %s\n", log_filename, strerror(errno));
        exit(MOONBR_EXITCODE_STARTUPERROR);
      }
    }
    if (daemonize == 0 && !moonbr_logfile) moonbr_logfile = stderr;
    if (moonbr_logfile) setlinebuf(moonbr_logfile);
    else moonbr_use_syslog = 1;
    if (moonbr_use_syslog) openlog(log_ident, LOG_NDELAY | LOG_PID, log_facility);
    if (daemonize) {
      if (daemon(1, 0)) {
        moonbr_log(LOG_ERR, "Could not daemonize moonbridge process");
        moonbr_terminate_error();
      }
    }
  }
  moonbr_log(LOG_NOTICE, "Starting moonbridge server");
  if (moonbr_pidfh && pidfile_write(moonbr_pidfh)) {
    moonbr_log(LOG_ERR, "Could not write pidfile (after locking)");
  }
  {
    lua_State *L;
    L = lua_newstate(moonbr_alloc, NULL);
    if (!L) {
      moonbr_log(LOG_CRIT, "Could not initialize Lua state");
      moonbr_terminate_error();
    }
    lua_atpanic(L, moonbr_lua_panic);
    lua_pushliteral(L, MOONBR_VERSION_STRING);
    lua_setglobal(L, "_MOONBRIDGE_VERSION");
    luaL_openlibs(L);
    luaL_requiref(L, "moonbridge_io", luaopen_moonbridge_io, 1);
    lua_pop(L, 1);
#ifdef MOONBR_LUA_PATH
    moonbr_modify_path(L, "path", MOONBR_LUA_PATH);
#endif
#ifdef MOONBR_LUA_CPATH
    moonbr_modify_path(L, "cpath", MOONBR_LUA_CPATH);
#endif
    lua_pushcfunction(L, moonbr_timeout);
    lua_setglobal(L, "timeout");
    lua_pushcfunction(L, moonbr_listen);
    lua_setglobal(L, "listen");
    lua_pushcfunction(L, moonbr_addtraceback);  /* on stack position 1 */
    moonbr_log(LOG_INFO, "Loading \"%s\"", argv[optind]);
    if (luaL_loadfile(L, argv[optind])) {
      moonbr_log(LOG_ERR, "Error while loading \"%s\": %s", argv[optind], lua_tostring(L, -1));
      moonbr_terminate_error();
    }
    { int i; for (i=optind+1; i<argc; i++) lua_pushstring(L, argv[i]); }
    if (lua_pcall(L, argc-(optind+1), 0, 1)) {
      moonbr_log(LOG_ERR, "Error while executing \"%s\": %s", argv[optind], lua_tostring(L, -1));
      moonbr_terminate_error();
    }
    if (!moonbr_first_pool) {
      moonbr_log(LOG_WARNING, "No listener initialized.");
      moonbr_terminate_error();
    }
    lua_getglobal(L, "listen");
    lua_pushcfunction(L, moonbr_listen);
    if (lua_compare(L, -2, -1, LUA_OPEQ)) {
      lua_pushnil(L);
      lua_setglobal(L, "listen");
    }
    lua_settop(L, 1);
    lua_gc(L, LUA_GCCOLLECT, 0);  /* collect garbage before forking later */
    moonbr_run(L);
  }
  return 0;
}

