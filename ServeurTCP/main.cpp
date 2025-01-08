#include <QTcpServer>
#include <QTcpSocket>
#include <QCoreApplication>
#include <QDebug>

// Classe TcpServer qui hérite de QTcpServer pour gérer un serveur TCP
class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    TcpServer() {
        // Le serveur écoute sur le port 1234, sur toutes les interfaces réseau
        if (this->listen(QHostAddress::Any, 1234)) {
            qDebug() << "Serveur en écoute sur le port 1234...";
        }
    }

protected:
    // Cette méthode est appelée lorsqu'une nouvelle connexion entrante est détectée
    void incomingConnection(qintptr socketDescriptor) override {
        // Crée un nouveau socket pour communiquer avec le client
        QTcpSocket *clientSocket = new QTcpSocket(this);

        // Si la connexion du client est réussie, on configure le socket
        if (clientSocket->setSocketDescriptor(socketDescriptor)) {
            qDebug() << "Client connecté: " << clientSocket->peerAddress().toString();

            // Connexion des signaux du socket au gestionnaire de slots
            connect(clientSocket, &QTcpSocket::readyRead, this, &TcpServer::readData);
            connect(clientSocket, &QTcpSocket::disconnected, this, &TcpServer::clientDisconnected);
        } else {
            // En cas d'échec, on supprime le socket
            delete clientSocket;
        }
    }

private slots:
    // Slot appelé lorsque des données sont disponibles pour la lecture
    void readData() {
        // Récupère le socket client qui a envoyé les données
        QTcpSocket *clientSocket = qobject_cast<QTcpSocket *>(sender());
        if (clientSocket) {
            // Lit toutes les données reçues
            QByteArray data = clientSocket->readAll();
            qDebug() << "Données reçues: " << data;

            // Envoie une réponse au client
            clientSocket->write("Réponse du serveur: " + data);
        }
    }

    // Slot appelé lorsque le client se déconnecte
    void clientDisconnected() {
        // Récupère le socket client déconnecté
        QTcpSocket *clientSocket = qobject_cast<QTcpSocket *>(sender());
        if (clientSocket) {
            qDebug() << "Client déconnecté";
            clientSocket->deleteLater();  // Nettoyage du socket
        }
    }
};

int main(int argc, char *argv[])
{
    // Initialisation de l'application Qt
    QCoreApplication a(argc, argv);

    // Création de l'objet serveur qui gère les connexions entrantes
    TcpServer server;

    // Exécution de la boucle d'événements de l'application Qt
    return a.exec();
}

#include "main.moc"
