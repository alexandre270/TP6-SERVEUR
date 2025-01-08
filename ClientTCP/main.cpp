#include <QTcpSocket>
#include <QCoreApplication>
#include <QDebug>

// Classe TcpClient qui hérite de QObject pour gérer les connexions TCP
class TcpClient : public QObject
{
    Q_OBJECT
public:
    TcpClient() {
        // Création d'un objet QTcpSocket pour la communication réseau
        socket = new QTcpSocket(this);

        // Connexion des signaux à leurs slots respectifs
        connect(socket, &QTcpSocket::connected, this, &TcpClient::onConnected);
        connect(socket, &QTcpSocket::readyRead, this, &TcpClient::onReadyRead);
        connect(socket, &QTcpSocket::disconnected, this, &TcpClient::onDisconnected);

        // Connexion au serveur à l'adresse 127.0.0.1 et au port 1234
        socket->connectToHost("127.0.0.1", 1234);
    }

private slots:
    // Slot appelé lorsque la connexion au serveur est établie
    void onConnected() {
        qDebug() << "Connecté au serveur.";

        // Envoi de données au serveur après la connexion
        socket->write("Bonjour, serveur!");
    }

    // Slot appelé lorsqu'il y a des données à lire depuis le serveur
    void onReadyRead() {
        QByteArray data = socket->readAll();  // Lire toutes les données reçues
        qDebug() << "Données reçues du serveur: " << data;
    }

    // Slot appelé lorsque la connexion est fermée
    void onDisconnected() {
        qDebug() << "Déconnecté du serveur.";
        socket->deleteLater();  // Nettoyage de l'objet socket
    }

private:
    QTcpSocket *socket;  // Pointeur vers l'objet QTcpSocket pour la communication
};

int main(int argc, char *argv[])
{
    // Initialisation de l'application Qt
    QCoreApplication a(argc, argv);

    // Création de l'objet TcpClient qui gère la communication avec le serveur
    TcpClient client;

    // Exécution de la boucle d'événements de l'application Qt
    return a.exec();
}

#include "main.moc"
