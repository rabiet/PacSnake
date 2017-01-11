# Planung

## Spezifikation
* Snake im PacMan-Feld
  * Schlange bewegt sich durch ein vordefiniertes Feld
    * Aufgabe ist Punkte zu fressen
  * Dadurch wird die Schlange länger
  * Schlange hält nicht an und kann nicht durch Wände gehen
  * Spieler verliert wenn Schlange sich selber berührt
  * (PacMan-)Geister können Teile der Schlange abbeißen
  * Steuerung
    * WASD und Pfeiltasten bestimmen Richtungswechsel
  * Ziel ist möglichst hohe Punktzahl (lange Schlange), es kann nicht gewonnen werden
  * Powerups (haben eine Chance zu spawnen wenn Punkt gefressen wird)
    * Geister fressen
    * Langsamer werden
    * Schneller werden
    * Alle Geister in die Mitte setzen

## Design
### (i) Programmstruktur
Das Programm startet in der `main()` Funktion. Zuerst stellt diese das Spielfeld dar. Sobald der Spieler auf Enter startet sich eine Schleife welche solange läuft wie der Spieler am Leben ist. In dieser wird die `gameTick()` aufgerufen in welcher unter anderem die Funktionen `movePlayer()` und `moveGhost()` aufgerufen werden.
Stirbt der Spieler wird der Schriftzug “Game Over” angezeigt und nach drücken der Enter-Taste wird das Programm beendet.

### (ii) wichtige Funktionen
#### Funktion die das Spielfeld aktualisiert und Routinefunktionen aufruft. 
``` c
int gameTick();
``` 

#### Bewegt die Schlange um ein Feld nach vorne.
``` c
struct Player movePlayer(struct Player p);
```

#### Bewegt den übergebenen Geist.
``` c
struct Ghost moveGhost(struct Ghost g);
``` 

#### Wendet ein Powerup an. Zu fressende Punkte werden ebenfalls als Powerup angesehen.
``` c
void takePowerup(struct PowerUp p);
``` 

### (iii) zentrale Datentypen
#### Struct für Powerups und Punkte
``` c
struct PowerUp {
  int id;
  int time;
  Position pos;
};
```

#### Struct für Positionen
``` c
struct Position {
  int x;
  int y;
};
```

#### Schlange
``` c
struct Player {
  int length;
  Position head;
  tail t;
};
```

#### Alle Elemente der Schlange (außer dem Kopf)
``` c
struct Tail {
  Position pos;
  Tail t;
};
``` 

#### Geister/Gegner
``` c
struct Ghost {
  Position pos;
  int direction;
};
``` 
