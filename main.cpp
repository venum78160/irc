#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>


#include "Serveur.hpp"
using namespace std;


// Définir une structure pour les canaux
struct channel {
    string name;
    string topic;
    vector<client*> members;
};

// Définir une fonction pour traiter les commandes IRC
void handle_command(string command, client* sender, map<string, channel*>& channels, map<int, client*>& clients);

// Définir une fonction pour gérer les connexions de nouveaux clients
void handle_new_connection(int sock_fd, map<int, client*>& clients);

// Définir une fonction pour gérer les messages envoyés par les clients
void handle_client_message(client* sender, string message, map<string, channel*>& channels, map<int, client*>& clients);

int main() {
    // Initialiser le serveur et attendre les connexions entrantes
    // Gérer les connexions en appelant handle_new_connection
    // Gérer les messages en appelant handle_client_message
    // Gérer les commandes en appelant handle_command
    return 0;
}

void handle_new_connection(int sock_fd, map<int, client*>& clients) {
    // Créer un nouveau client et l'ajouter à la map des clients
}

void handle_client_message(client* sender, string message, map<string, channel*>& channels, map<int, client*>& clients) {
    // Analyser le message pour déterminer s'il s'agit d'une commande ou d'un message normal
    // Si c'est une commande, appeler handle_command
    // Sinon, acheminer le message vers tous les clients dans le même canal que l'expéditeur
}

void handle_command(string command, client* sender, map<string, channel*>& channels, map<int, client*>& clients) {
    // Interpréter la commande et effectuer les actions nécessaires, telles que rejoindre un canal, quitter un canal, changer de pseudonyme, etc.
}


// /create nom_channel
// Créé un nouveau channel avec pour nom nom_channel,
// /join nom_channel
// Rejoint le channel nom_channel,
// /rename nom_channel nouveau_nom
// Renomme le channel nom_channel en nouveau_nom,
// /part nom_channel
// Quitte le channel nom_channel,
// /delete nom_channel
// Supprime le channel nom_channel,
// /nick nouveau_nom
// Renomme l'utilisateur en nouveau_nom,
// /users
// Affiche la liste des utilisateurs connectés au channel / serveur,
// /list [string]
// Affiche la liste des channels disponibles. Si string est donné en paramètre, affiche la liste des channels contenant string dans leur nom,
// /msg nom_utilisateur message
// Envoie message à nom_utilisateur,
// message
// Envoie un message dans le channel courant.
// kick un utilisateur
// BAn un utilisateur

// Le client se connecte au serveur IRC en utilisant une adresse IP et un port spécifiques. Le serveur écoute sur ce port et attend les connexions entrantes.

// Le client envoie une commande "NICK" au serveur, suivi du surnom qu'il souhaite utiliser pour se connecter. Le serveur vérifie que ce surnom n'est pas déjà utilisé par un autre client et envoie une réponse appropriée.

// Le client envoie une commande "USER" au serveur, suivi de son nom d'utilisateur, son nom réel et son adresse IP. Le serveur crée alors une instance de la classe Client pour le nouveau client, en utilisant les informations fournies.

// Le serveur envoie une réponse "001" au client pour confirmer que la connexion a été établie avec succès.

// Le client peut ensuite rejoindre un ou plusieurs canaux en envoyant une commande "JOIN" au serveur, suivi du nom du canal. Le serveur vérifie que le canal existe et ajoute le client à la liste des utilisateurs de ce canal.

// Le client peut envoyer des messages aux autres utilisateurs du canal en utilisant une commande "PRIVMSG". Le serveur relaie alors le message aux autres clients connectés au canal.

// Si le client se déconnecte en envoyant une commande "QUIT", le serveur supprime l'instance de la classe Client correspondant à ce client et envoie un message "PART" aux autres clients connectés au(x) canal(aux) qu'il a quitté.

// C'est en gros le processus de connexion d'un client à un serveur IRC. Il y a bien sûr d'autres commandes et fonctionnalités disponibles, mais les étapes ci-dessus représentent les actions de base que le serveur effectue lorsqu'un client se connecte.