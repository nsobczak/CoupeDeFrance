# Bot AI Story

## TODO

### Généralités

- aller ramasser et placer les cylindres
- éventuellement s'occuper du ramassage de balles
- funny action
- utiliser un timer pour vérifier si l'épreuve est finie ou pas

### Notes techniques

- faire une classe Bot qui aura toutes les fonctions du robot
- penser à prévoir des manoeuvres/actions spéciales en cas de bug (robot coincé dans un coin par exemple)
- prévoir la liste des positions théoriques des cylindres
- prévoir la liste des positions théoriques des emplacements où placer les cylindres

## 1\. Départ

On sélectionne le programme avec l'écran puis on tire la tirette.

- si on détecte que la tirette est tirée => on lance le programme
- le programme écrit un état haut dans la nano qui déclenche le timer de la funny action (à la fin des 90", on lance la funny action)
- le programme lance un timer + déplacement vers un cylindre

## 2\. Déplacement (cylindres + boules)

On se déplace vers une position théorique du cylindre.

- si c'est la fin des 90" - le temps de lâcher un cylindre,

  - si on a un cylindre, on le lâche
  - dans tous les cas, on s'arrête

- sinon

  - on corrige avec l'asservissement
  - si on détecte l'adversaire => manoeuvre d'évitement
  - si on détecte un cylindre => on le ramasse

### 2.1 Eviter l'adversaire

**TODO:** Il faut décider de quelle manière on va réagir :

- on l'attaque en lui tirant dessus ou en faisant la toupies à fond par exemple
- on s'arrête en espérant qu'il finisse par bouger
- on s'arrête pendant un certain temps, puis on repars s'il n'est plus là (sinon on recommence l'action de s'arrêter)
- on l'évite en reculant ou partant n'importe où
- on regarde ce qu'il fait, où il va et on part de l'autre côté (ça peut être difficile, faut réussir à analyser où il va)

### 2.2 Détection cylindre

**NOTE:** On ne détecte que les cylindres qui sont debouts.

**TODO:** faire une liste des positions théoriques des cylindre que l'on cherche à ramasser.

- si on est proche de la position théorique d'un cylindre, on cherche un cylindre avec les capteurs pendant une certaine durée (**TODO: à déterminer**) et si capteur renvoie l'info de présence d'un cylindre => on a trouvé un cylindre \o/

  - on se déplace pour l'attraper
  - on lance une fonction du module pince : attraper + monter + tourner
  - on se déplace pour aller le placer à une position théorique
  - une fois qu'on est arrivé, on lance une fonction du module pince : (tourner) + descendre + lâcher
  - on va à une autre position théorique de cylindre

- sinon (si on dépasse la période et qu'on a pas détecté de cylindre), on va ramasser le cylindre suivant de la liste.

### 2.3 Détection minerai

**TODO:** Cherche-t-on à ramasser les minerais ?

## 3\. Funny action

- à la fin des 90", l'arduino nano lance la funny action avec une fonction
