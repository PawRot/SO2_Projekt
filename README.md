# Projekt na przedmiot Systemy Operacyjne 2

### Cel projektu:
Celem projektu jest stworzenie wielowątkowego programu z interfejsem graficznym.
Projekt składa się z trzech etapów:
1. Etap 1 - Program ma wykorzystywać dynamiczne tworzenie i kończenie wątków.
2. Etap 2 - Program ma wykorzystywać prostą synchronizację opartą na mutex'ach.
3. Etap 3 - Program ma unikać pętli gorącego czekania w swoim działaniu.

### Opis zadania:
- Kolejne kulki pojawiają się na ekranie, po czym poruszają się po prostych ze stałą, losową prędkością, odbijając się od krawędzi.
- Po pięciu odbiciach kulki znikają
- Prostokąt porusza się między poziomymi krawędziami, z prędkością zmienianą losowo po każdym odbiciu.
- Program kończy się po naciśnięciu spacji.
- Do wizualizacji użyć biblioteki ncurses.

### Zadanie na etap 2.
- Kulki odbijają się od prostokąta.

Opis zasad odbić:
- Kulki po odbiciu od prostokąta lecą w kierunku przeciwnym do kierunku, w którym leciały przed odbiciem.
- Jeśli kulka lecąca poziomo zostanie uderzona przez prostokąt, to zaczyna poruszać się po skosie.
    - W tej sytuacji, jeśli jej prędkość jest mniejsza od prędkości prostokąta, to zostaje przyśpieszona.
- Kulka "zgnieciona" między prostokątem a ścianą znika.

### Zadanie na etap 3.
- Piłka odbita od prostokąta do czasu uderzenia w krawędź zmienia swój typ (oznaczone przez zmiane wizualizacji).
- Tylko jedna piłka tego typu może się poruszać
- Kolejne tego typu czekają aż poprzedzająca je zniknie lub zmieni się z powrotem w zwykłą piłkę
  - (piłki zatrzymują się dopiero kiedy wyjdą z obszaru możliwego zderzenia z prostokątem)
