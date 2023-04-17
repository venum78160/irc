/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaouil <itaouil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 20:38:50 by vl-hotel          #+#    #+#             */
/*   Updated: 2023/04/15 15:39:32 by itaouil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"

/*
// Définir une fonction pour traiter les commandes IRC
void handle_command(std::string command, Client* sender, std::map<std::string, channel*>& channels, std::map<int, client*>& clients);

// Définir une fonction pour gérer les connexions de nouveaux clients
void handle_new_connection(int sock_fd, std::map<int, Client*>& clients);

// Définir une fonction pour gérer les messages envoyés par les clients
void handle_client_message(Client* sender, std::string message, std::map<std::string, channel*>& channels, std::map<int, client*>& clients);
void handle_new_connection(int sock_fd, std::map<int, Client*>& clients) {
    // Créer un nouveau client et l'ajouter à la map des clients
}

void handle_client_message(Client* sender, std::string message, std::map<std::string, channel*>& channels, std::map<int, client*>& clients) {
    // Analyser le message pour déterminer s'il s'agit d'une commande ou d'un message normal
    // Si c'est une commande, appeler handle_command
    // Sinon, acheminer le message vers tous les clients dans le même canal que l'expéditeur
}

void handle_command(std::string command, Client* sender, std::map<std::string, channel*>& channels, std::map<int, client*>& clients) {
    // Interpréter la commande et effectuer les actions nécessaires, telles que rejoindre un canal, quitter un canal, changer de pseudonyme, etc.
}
*/

int main(int ac, char **av) {

	if (ac != 3)
	{
		std::cerr << "Usage: " << av[0] << " <port> <password>" << std::endl;
		return 1;
	}
	int port;
	try // plutôt try et catch dans le main avant d'envoyer port au constructeur
	{
		port = std::atoi(av[1]);
	}
	catch (const std::invalid_argument& e)
	{
		std::cerr << "Erreur : la chaîne n'est pas un entier valide." << std::endl;
		exit(1);
	}
	catch (const std::out_of_range& e)
	{
		std::cerr << "Erreur : la chaîne représente un entier en dehors de la plage de valeurs valides." << std::endl;
		exit(1);
	}
	Server server(av[2], port);

	server.start();
	// Gérer les connexions en appelant handle_new_connection
	// Gérer les messages en appelant handle_client_message
	// Gérer les commandes en appelant handle_command
	return 0;
}

// Le client se connecte au serveur IRC en utilisant une adresse IP et un port spécifiques. Le serveur écoute sur ce port et attend les connexions entrantes.

// Le client envoie une commande "NICK" au serveur, suivi du surnom qu'il souhaite utiliser pour se connecter. Le serveur vérifie que ce surnom n'est pas déjà utilisé par un autre client et envoie une réponse appropriée.

// Le client envoie une commande "USER" au serveur, suivi de son nom d'utilisateur, son nom réel et son adresse IP. Le serveur crée alors une instance de la classe Client pour le nouveau client, en utilisant les informations fournies.

// Le serveur envoie une réponse "001" au client pour confirmer que la connexion a été établie avec succès.

// Le client peut ensuite rejoindre un ou plusieurs canaux en envoyant une commande "JOIN" au serveur, suivi du nom du canal. Le serveur vérifie que le canal existe et ajoute le client à la liste des utilisateurs de ce canal.

// Le client peut envoyer des messages aux autres utilisateurs du canal en utilisant une commande "PRIVMSG". Le serveur relaie alors le message aux autres clients connectés au canal.

// Si le client se déconnecte en envoyant une commande "QUIT", le serveur supprime l'instance de la classe Client correspondant à ce client et envoie un message "PART" aux autres clients connectés au(x) canal(aux) qu'il a quitté.

// C'est en gros le processus de connexion d'un client à un serveur IRC. Il y a bien sûr d'autres commandes et fonctionnalités disponibles, mais les étapes ci-dessus représentent les actions de base que le serveur effectue lorsqu'un client se connecte.