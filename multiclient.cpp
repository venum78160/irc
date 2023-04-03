#include "Serveur.hpp"

int main(int argc, char *argv[]) {

    // Création du socket d'écoute

    Serveur serv(argv[1], argv[2]);
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Erreur lors de la création du socket d'écoute" << std::endl;
        return 1;
    }

    // Définition des paramètres du socket d'écoute
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8080);

    // Association du socket d'écoute à l'adresse et au port spécifiés
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Erreur lors de l'association du socket d'écoute à l'adresse et au port spécifiés" << std::endl;
        return 1;
    }

    // Mise en attente de connexions entrantes
    if (listen(serverSocket, MAX_CLIENTS) < 0) {
        std::cerr << "Erreur lors de la mise en attente de connexions entrantes" << std::endl;
        return 1;
    }

    // Création de la structure pollfd pour surveiller les descripteurs de fichiers
    std::vector<pollfd> pollFds;
    pollfd serverPollFd = {serverSocket, POLLIN, 0};
    pollFds.push_back(serverPollFd);

    // Boucle principale
    while (true) {

        // Attente d'un événement sur l'un des descripteurs de fichiers surveillés
        if (poll(&pollFds[0], pollFds.size(), -1) < 0) {
            std::cerr << "Erreur lors de l'appel à la fonction poll()" << std::endl;
            break;
        }

        // Parcours de la structure pollfd pour traiter les événements
        for (int i = 0; i < pollFds.size(); i++)
        {
            // Vérification si un événement s'est produit sur le socket d'écoute
            if (pollFds[i].fd == serverSocket && pollFds[i].revents & POLLIN)
            {

                // Acceptation de la connexion entrante
                struct sockaddr_in clientAddr;
                socklen_t clientAddrLen = sizeof(clientAddr);
                int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLen);
                if (clientSocket < 0) {
                    std::cerr << "Erreur lors de l'acceptation de la connexion entrante" << std::endl;
                    break;
                }

                // Ajout du descripteur de fichier du client à la structure pollfd
                pollfd clientPollFd = {clientSocket, POLLIN, 0};
                pollFds.push_back(clientPollFd);

                std::cout << "Nouvelle connexion entrante depuis " << inet_ntoa(clientAddr.sin_addr) << std::endl;
            }

            // Vérification si un événement s'est produit sur l'un des sockets des clients
            else if (pollFds[i].fd != serverSocket && pollFds[i].revents & POLLIN)
            {
              eventClient(pollFds[i]);
            }
        }

struct pollfd fds[10];
int nfds = 1;

// Ajout du descripteur de fichier du socket d'écoute
fds[0].fd = listen_sock;
fds[0].events = POLLIN;

while (1) {
  // Attendre un événement avec poll()
  int poll_result = poll(fds, nfds, -1);

  if (poll_result == -1) {
    perror("poll() a échoué");
    exit(1);
  }

  // Vérifier si un événement s'est produit sur le socket d'écoute
  if (fds[0].revents & POLLIN) {
    // Accepter la connexion entrante
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int client_sock = accept(listen_sock, (struct sockaddr*)&client_addr, &client_addr_len);

    // Ajouter le descripteur de fichier du client à la structure pollfd
    fds[nfds].fd = client_sock;
    fds[nfds].events = POLLIN;
    nfds++;

    printf("Nouvelle connexion entrante depuis %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
  }

  // Parcourir les descripteurs de fichiers surveillés pour vérifier s'il y a des données à lire
  for (int i = 1; i < nfds; i++) {
    if (fds[i].revents & POLLIN) {
      char buffer[1024];
      int bytes_read = recv(fds[i].fd, buffer, sizeof(buffer), 0);

      if (bytes_read <= 0) {
        // Le client s'est déconnecté, supprimer son descripteur de fichier
        close(fds[i].fd);
        fds[i] = fds[nfds-1];
        nfds--;
        i--;
      } else {
        // Afficher les données reçues et les envoyer à tous les autres clients connectés
        buffer[bytes_read] = '\0';
        printf("Données reçues de %d : %s\n", fds[i].fd, buffer);

        for (int j = 1; j < nfds; j++) {
          if (j != i) {
            send(fds[j].fd, buffer, bytes_read, 0);
          }
        }
      }
    }
  }
}

// Fermer le socket d'écoute
close(listen_sock);

return 0;
}

