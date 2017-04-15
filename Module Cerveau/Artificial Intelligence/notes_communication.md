# Notes de communication

## 1. Notes I2C

- envois sur adresse paire
- receptions sur adresse impaire

### Capteurs

I2C de la carte capteur => carte mère  (adresse 11):
- envoi des valeurs des capteurs ir et us (5 + 4 = 9 fois (10 s'ils fonctionnent tous))

### Gyroscope

Gyro => carte mère (adresse 68/69):
- envoi angle pour le connaitre en temps réel (1 donnée: angle selon z)

### Carte mère

carte mère => carte asservissement (adresse 6):
- envoi angle pour stopper rotation quand angle est bien
- envoi consigne de vitesse: 1er octet correspond à la partie entière de la consigne de vitesse, le 2ème octet correspond à la partie décimale

carte mère => pince (adresse 8):
- envoi ordre à déclencher (y'en a 3)


## 2. Notes SPI

### Ecran

ecran => carte mère:
- envoi de la stratégie
- envoi de la couleur


## 3. Autre

### funny action

funny action :
- carte mère envoie un état haut à la nano qui commence à compter 90" et déclenche la funny action à la fin
