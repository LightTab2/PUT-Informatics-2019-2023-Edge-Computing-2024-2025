next(swinoujscie,kolobrzeg).
next(swinoujscie, szczecin).
next(kolobrzeg,ustka).
next(ustka, gdansk).
next(gdansk,torun).
next(gdansk,olsztyn).
next(olsztyn,torun).
next(olsztyn,bialystok).
next(szczecin,bydgoszcz).
next(szczecin, gorzow-wlkp).
next(bydgoszcz,torun).
next(bydgoszcz,poznan).
next(poznan,torun).
next(gorzow-wlkp,poznan).
next(gorzow-wlkp,zielona-gora).
next(zielona-gora,poznan).
next(zielona-gora,wroclaw).
next(torun,lodz).
next(torun,warszawa).
next(bialystok, warszawa).
next(bialystok,lublin).
next(poznan,lodz).
next(warszawa,radom).
next(warszawa,lodz).
next(radom,lublin).
next(radom,kielce).
next(wroclaw,lodz).
next(lodz,radom).
next(lodz,czestochowa).
next(czestochowa,kielce).
next(czestochowa,katowice).
next(katowice,krakow).
next(kielce,krakow).
next(kielce,rzeszow).
next(lublin,kielce).
next(lublin,rzeszow).
next(krakow,rzeszow).

polaczenie(X, Y) :- next(X, Y).
polaczenie(X, Y) :- next(X, Z), polaczenie(Z, Y), Z \= Y.
zjazd(X,Y,Z) :- next(X, Z), next(Z, Y).
zjazd(X,Y,Z) :- next(X, P), zjazd(P, Y, Z).
