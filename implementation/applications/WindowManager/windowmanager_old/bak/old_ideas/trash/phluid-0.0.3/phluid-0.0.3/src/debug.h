#ifdef DEBUG
#define DEBUG_PRINT(c) fprintf(stdout, c)
#define DEBUG_VAR(a, b) fprintf(stdout, a, b)
#define DEBUG_CLIENT_LIST() debug_client_list()
#else
#define DEBUG_PRINT(c)
#define DEBUG_VAR(a, b)
#define DEBUG_CLIENT_LIST()
#endif

extern void debug_client_list(void);
