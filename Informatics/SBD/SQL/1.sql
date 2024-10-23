--zad 1
DECLARE
    vTekst VARCHAR2(255) := 'Witaj, świecie!';
    vLiczba NUMBER := 1000.456;
BEGIN
    DBMS_OUTPUT.PUT_LINE('Zmienna vTekst: ' || vTekst);
    DBMS_OUTPUT.PUT_LINE('Zmienna vLiczba: ' || vLiczba);
END;

--zad 2
DECLARE
    vTekst VARCHAR2(255) := 'Witaj, świecie!';
    vLiczba NUMBER := 1000.456;
BEGIN
    vTekst := vTekst || ' Witaj, nowy dniu!';
    vLiczba := vLiczba + 10**15;
    DBMS_OUTPUT.PUT_LINE('Zmienna vTekst: ' || vTekst);
    DBMS_OUTPUT.PUT_LINE('Zmienna vLiczba: ' || vLiczba);
END;

--zad 3
DECLARE
    vLiczba1 NUMBER := 10.2356000;
    vLiczba2 NUMBER := 0.0000001;
BEGIN
    DBMS_OUTPUT.PUT_LINE('Wynik dodawania ' || vLiczba1 ||
    ' i ' || vLiczba2 || ' : ' || (vLiczba1 + vLiczba2));
END;

--zad 4
DECLARE
    r NUMBER := 5;
    cPI CONSTANT NUMBER := 3.14;
BEGIN
    DBMS_OUTPUT.PUT_LINE('Obwód koła o promieniu równym ' || r || ': ' 
    || 2*cPI*r);
    DBMS_OUTPUT.PUT_LINE('Pole koła o promieniu równym ' || r || ': ' 
    || cPI*r**2);
END;

--zad 5
DECLARE
    vNazwisko pracownicy.nazwisko%type;
    vEtat pracownicy.etat%type;
BEGIN
    SELECT nazwisko, etat 
    INTO vNazwisko, vEtat
    FROM (SELECT nazwisko, etat, placa_pod+NVL(placa_dod, 0) placa 
    FROM pracownicy ORDER BY placa DESC FETCH FIRST 1 ROW ONLY);

    DBMS_OUTPUT.PUT_LINE('Najlepiej zarabia pracownik ' || vNazwisko || '.');
    DBMS_OUTPUT.PUT_LINE('Pracuje on jako ' || vEtat || '.');
END;

--zad 6
DECLARE
    TYPE tPracownik IS RECORD (
        nazwisko pracownicy.nazwisko%type,
        etat pracownicy.etat%type
    );
    vPracownik tPracownik;
BEGIN
    SELECT nazwisko, etat 
    INTO vPracownik
    FROM (SELECT nazwisko, etat, placa_pod+NVL(placa_dod, 0) placa 
    FROM pracownicy ORDER BY placa DESC FETCH FIRST 1 ROW ONLY);

    DBMS_OUTPUT.PUT_LINE('Najlepiej zarabia pracownik ' || vPracownik.nazwisko || '.');
    DBMS_OUTPUT.PUT_LINE('Pracuje on jako ' || vPracownik.etat || '.');
END;

--zad 7
DECLARE
    SUBTYPE tPieniadze IS NUMBER;

    vNazwisko pracownicy.nazwisko%type := 'SLOWINSKI';
    zarobki tPieniadze;
BEGIN
    SELECT 12*placa_pod+NVL(placa_dod, 0)
    INTO zarobki
    FROM pracownicy
    WHERE nazwisko = vNazwisko;

    DBMS_OUTPUT.PUT_LINE('Pracownik ' || vNazwisko
    || ' zarabia rocznie ' || zarobki);
END;

--zad 8
DECLARE
vTemp NUMBER := 0;
BEGIN
    LOOP
        SELECT EXTRACT (SECOND FROM (CURRENT_TIMESTAMP)) INTO vTemp FROM dual;
        IF FLOOR(vTemp) = 25 THEN
            EXIT;
        END IF;
    END LOOP;
    DBMS_OUTPUT.PUT_LINE('Nadeszła 25 sekunda!');
END;

--zad 9
DECLARE
vN NUMBER := 10;
vFactorial NUMBER := 1;
vTemp NUMBER := 1;
BEGIN
    LOOP
        vFactorial := vFactorial * vTemp;
        IF vTemp = vN THEN
            EXIT;
        END IF;
        vTemp := vTemp + 1;
    END LOOP;
    DBMS_OUTPUT.PUT_LINE('Silnia dla n=' || vN || ': ' || vFactorial);
END;

--zad 10
DECLARE
vEND NUMBER := 1200;
vTemp NUMBER := 0;
vDay NUMBER;
BEGIN
    LOOP
        IF vTemp = vEND THEN
            EXIT;
        END IF;
        SELECT TO_CHAR(ADD_MONTHS(DATE '2001-01-13', vTemp), 'D') INTO vDay FROM DUAL;
        IF vDay = 6
        THEN
            DBMS_OUTPUT.PUT_LINE(ADD_MONTHS(DATE '2001-01-13', vTemp));
        END IF;
        vTemp := vTemp + 1;
    END LOOP;
END;

