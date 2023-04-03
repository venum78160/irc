    // Add server socket to pollfd array
    fds[0].fd = server_socket;
    fds[0].events = POLLIN;

    while (1) {
        // Wait for events on any of the sockets
        int ret = poll(fds, num_clients+1, -1);
        if (ret == -1) {
            perror("poll");
            exit(1);
        }

        // Check if there are new client connections
        if (fds[0].revents & POLLIN) {
            // Accept new client connection
            client_socket[num_clients] = accept(server_socket, (struct sockaddr *)&client_address, &client_address_len);
            if (client_socket[num_clients] == -1) {
                perror("accept");
                exit(1);
            }

            // Add new client socket to pollfd array
            fds[num_clients+1].fd = client_socket[num_clients];
            fds[num_clients+1].events = POLLIN;
            num_clients++;

            printf("New client connected: %s:%d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
        }

        // Check for input from existing clients
        for (int i = 0; i < num_clients; i++) {
            if (fds[i+1].revents & POLLIN) {
                // Read data from client
                char buffer[1024];
                int bytes_read = recv(client_socket[i], buffer, sizeof(buffer), 0);
                if (bytes_read == -1) {
                    perror("recv");
                    exit(1);
                }

                // Handle received data
                if (bytes_read == 0) {
                    // Client disconnected
                    printf("Client disconnected: %s:%d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
                    close(client_socket[i]);

                    // Remove client socket from pollfd array
                    for (int j = i+1; j < num_clients+1; j++) {
                        fds[j-1] = fds[j];
                    }
                    num_clients--;
                    i--;
                } else {
                    // Print received data
                    printf("Received %d bytes from client %s:%d: %.*s\n", bytes_read, inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port), bytes_read, buffer);
                }
            }
        }
    }

    return 0;
}
