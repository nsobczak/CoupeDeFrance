# Bot AI Story

## TODO

### Généralités
- aller ramasser et placer les cylindres
- éventuellement s'occuper du rammassage de balles
- funny action

### Technique
- faire une classe Bot qui aura toutes les fonctions du robots
- penser à prévoir des manoeuvres/actions spéciales en cas de bug (robot coincé dans un coin par exemple)

## 1. Départ

On sélectionne le programme avec l'écran puis on tire la tirette.

- si on détecte que la tirette est tirée => on lance le programme
- le programme lance un timer + déplacement vers un cylindre
- à la fin des 90", on lance la funny action


## 2. Déplacement (cylindres + boules)

On se déplace vers une position théorique du cylindre tout

- si c'est la fin des 90" - le temps de lâcher un cylindre,
	* si on a un cylindre, on le lache
	* dans tous les cas, on s'arrête et on attends de dépasser les 90" pour lancer la funny action avec une fonction
- sinon
	* on corrige avec l'asservissement
	* si on détecte l'adversaire => manoeuvre dévitement
	* si on détecte un cylindre => on le ramasse


### 2.1 Eviter l'adversaire

**TODO:** Il faut décider de quelle manière on va réagir :
- on l'attaque en lui tirant dessus ou en faisant la toupies à fond par exemple
- on s'arrête en espérant qu'il finisse par bouger
- on s'arrête pendant un certain temps, puis on repars s'il n'est plus là (sinon on recommence l'action de s'arrêter)
- on l'évite en reculant ou partant n'importe où
- on regarde ce qu'il fait, où il va et on part de l'autre côté (ça peut être difficile, faut réussir à analyser où il va)


### 2.2 Détection cylindre

**TODO:** Ne détecte-t-on que les cylindres qui sont debouts ? Cherche-t-on également à ramasser les cylindre qui sont par couchés ?

**TODO:** faire une liste des positions théoriques des cylindre que l'on cherche à ramasser.

- si on est proche de la position théorique d'un cylindre, on cherche un cylindre avec les capteurs pendant une certaine durée (**TODO: à déterminer**) et si capteur renvoie l'info de présence d'un cylindre => on a trouvé un cylindre \o/
	* on se déplace pour l'attraper
	* on lance une fonction du module pince : attrapper + monter + tourner
	* on se déplace pour aller le placer où il faut
	* une fois qu'on est arrivé, on lance une fonction du module pince : (tourner) + descendre + lacher
	* on va à une autre position théorique de cylindre
- sinon (si on dépasse la période et qu'on a pas détecter de cylindre), on va ramasser le cylindre suivant de la liste.

### 2.3 Détection minerai

**TODO:** Cherche-t-on à rammasser les minerais ?

## 3. Funny action

- à la fin des 90", on lance la funny action avec une fonction
