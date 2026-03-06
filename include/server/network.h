/**
 * network.h - Network communication module for Mahjong Server
 */

#ifndef NETWORK_H
#define NETWORK_H

#include <stdbool.h>
#include <pthread.h>

// Server configuration
#define DEFAULT_PORT 8888
#define MAX_CLIENTS 100
#define BUFFER_SIZE 1024
#define BACKLOG 10

// Server state
typedef struct {
    int server_fd;
    int port;
    int max_clients;
    bool running;
    pthread_t accept_thread;
    pthread_mutex_t lock;
} ServerState;

// Client connection
typedef struct {
    int fd;
    struct sockaddr_in addr;
    pthread_t thread;
    bool connected;
    char username[32];
} ClientConnection;

// Function prototypes

/**
 * Initialize server with given configuration
 * @param port Server port (0 for default)
 * @param max_clients Maximum concurrent clients
 * @return Initialized server state or NULL on error
 */
ServerState* server_init(int port, int max_clients);

/**
 * Start the server
 * @param state Server state
 * @return 0 on success, -1 on error
 */
int server_start(ServerState* state);

/**
 * Stop the server gracefully
 * @param state Server state
 */
void server_stop(ServerState* state);

/**
 * Cleanup server resources
 * @param state Server state
 */
void server_cleanup(ServerState* state);

/**
 * Handle client connection in separate thread
 * @param arg Client connection pointer
 * @return NULL
 */
void* handle_client(void* arg);

/**
 * Send message to client
 * @param client_fd Client file descriptor
 * @param message Message to send
 * @return Number of bytes sent, -1 on error
 */
int send_to_client(int client_fd, const char* message);

/**
 * Broadcast message to all connected clients
 * @param state Server state
 * @param message Message to broadcast
 * @param exclude_fd Client fd to exclude (or -1 for all)
 */
void broadcast_message(ServerState* state, const char* message, int exclude_fd);

/**
 * Get current client count
 * @param state Server state
 * @return Number of connected clients
 */
int get_client_count(ServerState* state);

/**
 * Load server configuration from file
 * @param filename Configuration file
 * @param port Output port
 * @param max_clients Output max clients
 * @return 0 on success, -1 on error
 */
int load_config(const char* filename, int* port, int* max_clients);

/**
 * Save server configuration to file
 * @param filename Configuration file
 * @param port Server port
 * @param max_clients Max clients
 * @return 0 on success, -1 on error
 */
int save_config(const char* filename, int port, int max_clients);

#endif // NETWORK_H