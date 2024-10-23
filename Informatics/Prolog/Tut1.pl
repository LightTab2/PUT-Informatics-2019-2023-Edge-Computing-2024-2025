mezczyzna(piotr).
mezczyzna(adam).
mezczyzna(robert).
mezczyzna(marek).
mezczyzna(jan).
mezczyzna(krzystof).
mezczyzna(radek).
mezczyzna(darek).
mezczyzna(tomek).
mezczyzna(jacek).

kobieta(maria).
kobieta(ewa).
kobieta(agata).
kobieta(anna).
kobieta(joanna).
kobieta(agnieszka).
kobieta(beata).
kobieta(iwona).

rodzic(maria, agata).
rodzic(maria, marek).
rodzic(maria, jan).
rodzic(piotr, agata).
rodzic(piotr, marek).
rodzic(piotr, jan).

rodzic(adam, anna).
rodzic(adam, joanna).
rodzic(adam, krzysztof).
rodzic(ewa, anna).
rodzic(ewa, joanna).
rodzic(ewa, krzysztof).

rodzic(robert, radek).
rodzic(robert, beata).
rodzic(agata, radek).
rodzic(agata, beata).

rodzic(jan, darek).
rodzic(jan, tomek).
rodzic(anna, darek).
rodzic(anna, tomek).

rodzic(krzysztof, jacek).
rodzic(krzysztof, iwona).
rodzic(agnieszka, jacek).
rodzic(agnieszka, iwona).

ojciec(X, Y) :- mezczyzna(X), rodzic(X, Y).
matka(X, Y) :- kobieta(X), rodzic(X, Y).
brat(X, Y) :- mezczyzna(X), rodzic(Z, X), rodzic(Z, Y), X\=Y.
siostra(X, Y) :- kobieta(X), rodzic(Z, X), rodzic(Z, Y), X\=Y.
babcia(X, Y) :- kobieta(X), rodzic(X, Z), rodzic(Z, Y).
dziadek(X, Y) :- mezczyzna(X), rodzic(X, Z), rodzic(Z, Y).
stryj(X, Y) :- ojciec(Z, Y), brat(X, Z).
wuj(X, Y) :- matka(Z, Y), brat(X, Z).
kuzyn(X, Y) :- mezczyzna(X), dziadek(Z, X), dziadek(Z, Y), X\=Y.
kuzynka(X, Y) :- kobieta(X), dziadek(Z, X), dziadek(Z, Y), X\=Y.

malzenstwo(maria, piotr).
malzenstwo(adam, ewa).
malzenstwo(robert, agata).
malzenstwo(jan, anna).
malzenstwo(krzystof, agnieszka).

tesciowa(X, Y) :- kobieta(Z), malzenstwo(Z, Y), matka(X, Z).
tesciowa(X, Y) :- kobieta(Z), malzenstwo(Y, Z), matka(X, Z).

szwagier(X, Y) :- kobieta(Z), malzenstwo(Y, Z), brat(X, Z).
szwagier(X, Y) :- kobieta(Z), malzenstwo(Z, Y), brat(X, Z).
szwagier(X, Y) :- mezczyzna(X), siostra(Z, Y), malzenstwo(Z, X).
szwagier(X, Y) :- mezczyzna(X), siostra(Z, Y), malzenstwo(X, Z).
