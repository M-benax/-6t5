#include "../minishell.h" // أو المسار الصحيح

// This global variable is for signals as per subject.
// It should be declared in one .c file and can be extern'd in .h if needed by other modules,
// but subject implies only handler uses it.
// volatile sig_atomic_t g_received_signal = 0; // Example

// _sigint_handler is likely static if only used by signal_init in this file.
// If it needs to be accessed from outside, it needs a prototype in .h
static void _sigint_handler(int sig_num)
{
    (void)sig_num; // To satisfy -Wextra if sig_num is not used
    // g_received_signal = SIGINT; // Set the global signal flag

    // Behavior for interactive mode (Ctrl-C displays a new prompt on a new line)
    write(STDOUT_FILENO, "\n", 1); // New line
    rl_on_new_line(); // Notify readline that we've moved to a new line
    rl_replace_line("", 0); // Clear the current input buffer
    rl_redisplay(); // Display the prompt and cleared line
}

// SIGQUIT (Ctrl-\) should do nothing in interactive mode as per subject.
// SIG_IGN handles this.

void signal_init(void)
{
    struct sigaction sa_int;
    struct sigaction sa_quit;

    // Setup for SIGINT (Ctrl+C)
    sa_int.sa_handler = _sigint_handler;
    sigemptyset(&sa_int.sa_mask); // Block no other signals during handler
    sa_int.sa_flags = SA_RESTART; // Restart syscalls if interrupted by this signal
    if (sigaction(SIGINT, &sa_int, NULL) == -1)
        perror("minishell: sigaction for SIGINT failed");

    // Setup for SIGQUIT (Ctrl+\) - ignore it
    sa_quit.sa_handler = SIG_IGN; // Ignore the signal
    sigemptyset(&sa_quit.sa_mask);
    sa_quit.sa_flags = 0;
    if (sigaction(SIGQUIT, &sa_quit, NULL) == -1)
        perror("minishell: sigaction for SIGQUIT failed");
}