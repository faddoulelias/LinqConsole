PROTOCOLE :
    // Lorsque un client se connecte, il envoie au serveur:
        CONNECT <username>

    // Le serveur envoie a tout les clients:
    // ID est propre a chaque client
        CONNECTED <ID> <JOUEUR 1> <JOUEUR 2> <JOUEUR 3> <JOUEUR 4> <JOUEUR 5>
    // Si Joueurs manquants, on envoie "-" à la place du nom du joueur

    // Si le nom du jour est deja pris
        DENIED <username>

    // Lorseque tout les joueurs sont connectés, le serveur envoie:
    // ID est propre a chaque client
    // ROLE est le role du joueur (0 = contre-espion, 1 = espion)
    // MOT est le mot de l'espion, si le joueur est contre-espion, il recoit "-"
        START <ID> <ROLE> <MOT> <JOUEUR 1> <JOUEUR 5> <JOUEUR 4> <JOUEUR 3> <JOUEUR 5>
    // l'ordre des joueurs est aléatoire, il indique l'ordre de jeu

    // Deux fois pour chaque joueur :
        // Lorsque c'est au tour d'un joueur de jouer, le serveur envoie:
            PLAY <ID> <JOUEUR 1>
        
        // Le client repond avec le mot qu'il a choisi:
            WORD <ID> <MOT>

        // Le serveur envoie a tout les clients le mot choisi:
            SAID <JOUEUR 1> <MOT>

    // Lorseque tout les joueurs ont joué, le serveur envoie:
        // Le client vote pour les deux espions qu'il pense etre les espions:
            VOTE <ID> <JOUEUR 1> <JOUEUR 2>
        // Le serveur envoie a tout les clients les votes:
            VOTED <JOUEUR 1> <ESPION 1> <ESPION 2>

    // Lorseque tout les joueurs ont voté, le serveur envoie:
        // Si les deux espions sont trouvés:
            WIN 0 <JOUEUR 1> <JOUEUR 2> <JOUEUR 3>
        
        // Sinon :
            WIN 1 <JOUEUR 4> <JOUEUR 5>

    // SI LA REQUETE EST INVALIDE:
        INVALID <CODE>

Les requetes a implementer sont donc:

Cote client:
    CONNECT <username>
    WORD <ID> <MOT>
    CAST <ID> <JOUEUR 1> <JOUEUR 2>

Cote serveur:
    CONNECTED <ID> <JOUEUR 1> <JOUEUR 2> <JOUEUR 3> <JOUEUR 4> <JOUEUR 5>
    DENIED <username>
    INVALID <CODE>
    START <ID> <ROLE> <MOT> <JOUEUR 1> <JOUEUR 5> <JOUEUR 4> <JOUEUR 3> <JOUEUR 5>
    PLAY <ID> <JOUEUR 1>
    VOTE <ID> <JOUEUR 1>
    SAID <JOUEUR 1> <MOT>
    VOTED <JOUEUR 1> <ESPION 1> <ESPION 2>
    WIN 0 <JOUEUR 1> <JOUEUR 2> <JOUEUR 3>
    WIN 1 <JOUEUR 4> <JOUEUR 5>

