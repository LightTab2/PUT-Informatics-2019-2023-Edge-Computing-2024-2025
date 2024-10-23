zadanieH(L, P, Wynik) :- rozbite(L, W1), unikalne(W1,U1), bubblesort(U1, B1), liczbe(L1, B1), rozbite(P, W2), unikalne(W2, U2), bubblesort(U2, B2), liczbe(L2, B2), odleglosc(L,[L1,L2], O1), odleglosc(P, [L1,L2], O2), Wynik = [O1, O2].

odleglosc([H1,T1], [H2,T2], W) :- W is sqrt((H2 - H1) * (H2 - H1) + (T2 - T1) * (T2 - T1)), !.

unikalne([],[]).
unikalne([H|T1],[H|T2]):-not(member(H,T1)),unikalne(T1,T2), !.
unikalne([_|T1],T2):-unikalne(T1,T2), !.

rozbite([], []) :- !.
rozbite([H|T], T2) :- rozbij(H, R), append(R, Z, T2), rozbite(T, Z), !.
rozbij(0,[]) :- !.
rozbij(1,[1]) :- !.
rozbij(X,T) :- M is mod(X,10), X2 is div(X,10), append(Y,[M],T), rozbij(X2,Y), !.

bubblesort(L,S):- z(L,L1),!,bubblesort(L1,S).
bubblesort(S,S).
z([X,Y|R],[Y,X|R]):- X<Y.
z([Z|R],[Z|R1]):- z(R,R1).

liczbe(0,[]):-!.
liczbe(X,L):-append(L1,[B],L), liczbe(X2,L1), X is B + X2*10, !.
