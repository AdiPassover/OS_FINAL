#include "graph/graph.hpp"
#include <iostream>

int main() {
    std::cout << "Hello world" << std::endl;
}

//#include "ConvexHull.hpp"
//#include <iostream>
//#include <string.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <unistd.h>
//#include <sys/types.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>
//#include <netdb.h>
//#include <poll.h>
//#include <signal.h>
//
//#define PORT "9034"
//#define BUFFER_SIZE 50
//
//using std::cout;
//using std::endl;
//using std::string;
//
//bool server_running = true;
//void handle_sigint(int sig) {
//    cout << "\nReceived SIGINT (" << sig << "), shutting down server..." << endl;
//    server_running = false;
//}
//
//string process_command(vector<Point>& points, char* line, size_t& points_to_receive, int& creator_fd, int sender_fd) {
//    if (points_to_receive > 0) {
//        if (creator_fd != sender_fd)
//            return "Another client is creating a graph";
//
//        float x, y;
//        if (sscanf(line, "%f,%f", &x, &y) != 2)
//            return "Invalid coordinates format while waiting for points";
//        points.emplace_back(x, y);
//        points_to_receive--;
//        if (points_to_receive == 0) {
//            creator_fd = -1;
//            return "Graph creation complete";
//        }
//        return "Point added";
//    }
//
//    if (strncmp(line, "Newgraph", 8) == 0) {
//        size_t n;
//        if (sscanf(line, "Newgraph %zu", &n) != 1)
//            return "Invalid Newgraph command format";
//        if (n == 0)
//            return "Graph must have at least one point";
//
//        points.clear();
//        points.reserve(n);
//        creator_fd = sender_fd;
//        points_to_receive = n;
//        return "Expecting points for new graph";
//    }
//    else if (strcmp(line,"CH") == 0) {
//        float area = ConvexHull::convexHullArea(points);
//        return "Convex hull area: " + std::to_string(area);
//    }
//    else if (strncmp(line,"Newpoint", 8) == 0) {
//        float x, y;
//        if (sscanf(line, "Newpoint %f,%f", &x, &y) != 2)
//            return "Invalid coordinates format";
//        points.emplace_back(x, y);
//        return "Point added";
//    }
//    else if (strncmp(line,"Removepoint", 11) == 0) {
//        float x, y;
//        if (sscanf(line, "Removepoint %f,%f", &x, &y) != 2)
//            return "Invalid coordinates format";
//
//        for (unsigned int i = 0; i < points.size(); i++) {
//            if (points[i].getX() == x && points[i].getY() == y) {
//                points[i] = points.back();
//                points.pop_back();
//                break;
//            }
//        }
//        return "Point removed";
//    }
//    return "Unknown command";
//}
//
//// Get sockaddr, IPv4 or IPv6:
//void *get_in_addr(struct sockaddr *sa)
//{
//    if (sa->sa_family == AF_INET) {
//        return &(((struct sockaddr_in*)sa)->sin_addr);
//    }
//
//    return &(((struct sockaddr_in6*)sa)->sin6_addr);
//}
//
//// Return a listening socket
//int get_listener_socket(void)
//{
//    int listener;     // Listening socket descriptor
//    int yes=1;        // For setsockopt() SO_REUSEADDR, below
//    int rv;
//
//    struct addrinfo hints, *ai, *p;
//
//    // Get us a socket and bind it
//    memset(&hints, 0, sizeof hints);
//    hints.ai_family = AF_UNSPEC;
//    hints.ai_socktype = SOCK_STREAM;
//    hints.ai_flags = AI_PASSIVE;
//    if ((rv = getaddrinfo(NULL, PORT, &hints, &ai)) != 0) {
//        fprintf(stderr, "pollserver: %s\n", gai_strerror(rv));
//        exit(1);
//    }
//
//    for(p = ai; p != NULL; p = p->ai_next) {
//        listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
//        if (listener < 0) {
//            continue;
//        }
//
//        // Lose the pesky "address already in use" error message
//        setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
//
//        if (bind(listener, p->ai_addr, p->ai_addrlen) < 0) {
//            close(listener);
//            continue;
//        }
//
//        break;
//    }
//
//    freeaddrinfo(ai); // All done with this
//
//    // If we got here, it means we didn't get bound
//    if (p == NULL) {
//        return -1;
//    }
//
//    // Listen
//    if (listen(listener, 10) == -1) {
//        return -1;
//    }
//
//    return listener;
//}
//
//// Add a new file descriptor to the set
//void add_to_pfds(struct pollfd *pfds[], int newfd, int *fd_count, int *fd_size)
//{
//    // If we don't have room, add more space in the pfds array
//    if (*fd_count == *fd_size) {
//        *fd_size *= 2; // Double it
//
//        *pfds = (struct pollfd *)realloc(*pfds, sizeof(**pfds) * (*fd_size));
//    }
//
//    (*pfds)[*fd_count].fd = newfd;
//    (*pfds)[*fd_count].events = POLLIN; // Check ready-to-read
//
//    (*fd_count)++;
//}
//
//// Remove an index from the set
//void del_from_pfds(struct pollfd pfds[], int i, int *fd_count)
//{
//    // Copy the one from the end over this one
//    pfds[i] = pfds[*fd_count-1];
//
//    (*fd_count)--;
//}
//
//int main()
//{
//    signal(SIGINT, handle_sigint);
//
//    vector<Point> points;
//    size_t points_to_receive = 0;
//    int creator_fd = -1;
//
//    int listener;     // Listening socket descriptor
//
//    int newfd;        // Newly accept()ed socket descriptor
//    struct sockaddr_storage remoteaddr; // Client address
//    socklen_t addrlen;
//
//    char buf[256];    // Buffer for client data
//
//    char remoteIP[INET6_ADDRSTRLEN];
//
//    // Start off with room for 5 connections
//    // (We'll realloc as necessary)
//    int fd_count = 0;
//    int fd_size = 5;
//    struct pollfd *pfds = (struct pollfd *) malloc(sizeof *pfds * fd_size);
//
//    // Set up and get a listening socket
//    listener = get_listener_socket();
//
//    if (listener == -1) {
//        fprintf(stderr, "error getting listening socket\n");
//        exit(1);
//    }
//
//    // Add the listener to set
//    pfds[0].fd = listener;
//    pfds[0].events = POLLIN; // Report ready to read on incoming connection
//
//    fd_count = 1; // For the listener
//
//    // Main loop
//    while (server_running) {
//        int poll_count = poll(pfds, fd_count, -1);
//
//        if (poll_count == -1) {
//            if (errno == EINTR) continue;
//            perror("poll");
//            exit(1);
//        }
//
//        // Run through the existing connections looking for data to read
//        for(int i = 0; i < fd_count; i++) {
//
//            // Check if someone's ready to read
//            if (pfds[i].revents & POLLIN) { // We got one!!
//
//                if (pfds[i].fd == listener) {
//                    // If listener is ready to read, handle new connection
//
//                    addrlen = sizeof remoteaddr;
//                    newfd = accept(listener,
//                                   (struct sockaddr *)&remoteaddr,
//                                   &addrlen);
//
//                    if (newfd == -1) {
//                        perror("accept");
//                    } else {
//                        add_to_pfds(&pfds, newfd, &fd_count, &fd_size);
//
//                        printf("pollserver: new connection from %s on "
//                               "socket %d\n",
//                               inet_ntop(remoteaddr.ss_family,
//                                         get_in_addr((struct sockaddr*)&remoteaddr),
//                                         remoteIP, INET6_ADDRSTRLEN),
//                               newfd);
//                        send(newfd, ">> ", 3, 0);
//                    }
//                } else {
//                    // If not the listener, we're just a regular client
//                    int nbytes = recv(pfds[i].fd, buf, sizeof buf, 0);
//
//                    int sender_fd = pfds[i].fd;
//
//                    if (nbytes <= 0) {
//                        // Got error or connection closed by client
//                        if (nbytes == 0) {
//                            // Connection closed
//                            printf("pollserver: socket %d hung up\n", sender_fd);
//                        } else {
//                            perror("recv");
//                        }
//
//                        if (sender_fd == creator_fd) {
//                            points.clear();
//                            points_to_receive = 0;
//                            creator_fd = -1;
//                            cout << "Graph creation cancelled, graph reset." << endl;
//                        }
//
//                        close(pfds[i].fd); // Bye!
//
//                        del_from_pfds(pfds, i, &fd_count);
//
//                    } else {
//                        // We got some good data from a client
//                        buf[nbytes-1] = '\0';
//                        printf("pollserver: socket %d sent '%s'\n", sender_fd, buf);
//                        string response =
//                                process_command(points, buf, points_to_receive, creator_fd, sender_fd) + "\n>> ";
//                        send(sender_fd, response.c_str(), response.size(), 0);
//                    }
//                } // END handle data from client
//            } // END got ready-to-read from poll()
//        } // END looping through file descriptors
//    } // END for(;;)--and you thought it would never end!
//
//    for (int i = 0; i < fd_count; i++)
//        close(pfds[i].fd);
//    free(pfds);
//
//    return 0;
//}