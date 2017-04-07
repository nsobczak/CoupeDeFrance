#1. Notes I2C

## Capteurs

I2C de la carte capteur => carte mère :
- envoi des valeurs des capteurs ir et us (5 + 4 = 9 fois (10 s'ils fonctionnent tous))

## Gyroscope

Gyro => carte mère:
- envoi angle pour le connaitre en temps réel (1 donnée: angle selon z)

## Carte mère

carte mère => carte asserv:
- envoi angle pour stopper rotation quand angle est bien
- envoi consigne de vitesse

carte mère => pince:
- envoi ordre à déclencher (y'en a 3)


# 2. Notes SPI

## Ecran

ecran => carte mère:
- envoi de la stratégie
- envoi de la couleur


# 3. Autre

## funny action

funny action :
- carte mère envoie un état haut à la nano qui commence à compter 90" et déclenche la funny action à la fin
