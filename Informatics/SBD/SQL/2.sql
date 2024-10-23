--zad 1
CREATE OR REPLACE PROCEDURE NowyPracownik(pNazwisko VARCHAR, pZespol VARCHAR, pSzef VARCHAR,
    pPlaca NUMBER, pEtat VARCHAR := 'STAZYSTA', pZatrudniony DATE := CURRENT_DATE) IS
    vIdPrac Pracownicy.id_prac%TYPE;
    vLastIdPrac Pracownicy.id_prac%TYPE;
BEGIN
    SELECT id_prac INTO vLastIdPrac FROM pracownicy 
    ORDER BY id_prac DESC FETCH FIRST 1 ROWS ONLY;
    
    INSERT INTO pracownicy(id_prac, nazwisko, etat, placa_pod, zatrudniony)
    VALUES(vLastIdPrac+10, pNazwisko, pEtat, pPlaca, pZatrudniony)
    RETURNING id_prac INTO vIdPrac;
    
    UPDATE pracownicy
    SET id_zesp = (SELECT id_zesp FROM zespoly WHERE nazwa = pZespol)
    WHERE id_prac = vIdPrac;
    
    UPDATE pracownicy
    SET id_szefa = (SELECT id_prac FROM pracownicy WHERE nazwisko = pSzef)
    WHERE id_prac = vIdPrac;
    
 END NowyPracownik;
 
 --zad 2
CREATE OR REPLACE FUNCTION PlacaNetto(pWartosc NUMBER, pPodatek NUMBER)
RETURN NUMBER
IS
BEGIN
    RETURN pWartosc * (100-pPodatek)/100;
END PlacaNetto;

--zad 3
CREATE OR REPLACE FUNCTION Silnia(pLiczba NUMBER)
RETURN NUMBER
IS
vCounter NUMBER := 1;
vSilnia NUMBER := 1;
BEGIN
    IF pLiczba < 2 THEN
        RETURN 1;
    END IF;
    LOOP
        vCounter := vCounter + 1;
        vSilnia := vSilnia * vCounter;
        IF vCounter = pLiczba THEN
            EXIT;
        END IF;
    END LOOP;
    RETURN vSilnia;
END Silnia;

--zad 4
CREATE OR REPLACE FUNCTION SilniaRek(pLiczba NUMBER)
RETURN NUMBER
IS
BEGIN
    IF pLiczba < 2 THEN
        RETURN 1;
    END IF;
    RETURN pLiczba * SilniaRek(pLiczba-1);
END SilniaRek;

--zad 5
CREATE OR REPLACE FUNCTION IleLat(dat DATE)
RETURN NUMBER
IS
BEGIN
    RETURN FLOOR(MONTHS_BETWEEN(CURRENT_DATE, dat)/12);
END IleLat;

--zad 6
CREATE OR REPLACE PACKAGE Konwersja IS
    FUNCTION Fahr_To_Cels(pTemperature NUMBER)
    RETURN NUMBER;
    
    FUNCTION Cels_To_Fahr(pTemperature NUMBER)
    RETURN NUMBER;
END Konwersja;

CREATE OR REPLACE PACKAGE BODY Konwersja IS
    FUNCTION Fahr_To_Cels(pTemperature NUMBER)
        RETURN NUMBER
    IS
    BEGIN
        RETURN 5*(pTemperature - 32)/9;
    END;
        
    FUNCTION Cels_To_Fahr(pTemperature NUMBER)
        RETURN NUMBER
    IS
    BEGIN
        RETURN 9*pTemperature/5 + 32;
    END;
END Konwersja;

--zad 8
CREATE OR REPLACE PACKAGE IntZespoly IS
    PROCEDURE dodajZespol(pNazwa VARCHAR, pAdres VARCHAR);
    PROCEDURE usunZespol(pNazwa VARCHAR);
    PROCEDURE usunZespol(pId NUMBER);
    PROCEDURE zmienNazwe(pId NUMBER, pNazwa VARCHAR);
    PROCEDURE zmienAdres(pId NUMBER, pAdres VARCHAR);
    FUNCTION wezIdentyfikator(pNazwa VARCHAR)
        RETURN NUMBER;
    FUNCTION wezNazwe(pID NUMBER)
        RETURN VARCHAR;
    FUNCTION wezAdres(pID NUMBER)
        RETURN VARCHAR;
END IntZespoly;

CREATE OR REPLACE PACKAGE BODY IntZespoly IS
    PROCEDURE dodajZespol(pNazwa VARCHAR, pAdres VARCHAR) IS
        vLastId NUMBER;
    BEGIN
        SELECT id_zesp INTO vLastId FROM zespoly 
        ORDER BY id_zesp DESC FETCH FIRST 1 ROWS ONLY;
        
        INSERT INTO zespoly(id_zesp, nazwa, adres)
        VALUES(vLastId + 10, pNazwa, pAdres);
    END dodajZespol;
    
    PROCEDURE usunZespol(pNazwa VARCHAR) IS
    BEGIN
        DELETE zespoly 
        WHERE nazwa = pNazwa;
    END usunZespol;
    
    PROCEDURE usunZespol(pId NUMBER) IS
    BEGIN
        DELETE zespoly 
        WHERE id_zesp = pId;
    END usunZespol;
    
    PROCEDURE zmienNazwe(pId NUMBER, pNazwa VARCHAR) IS
    BEGIN
        UPDATE zespoly 
        SET nazwa = pNazwa
        WHERE id_zesp = pId;
    END;
    
    PROCEDURE zmienAdres(pId NUMBER, pAdres VARCHAR) IS
    BEGIN
        UPDATE zespoly 
        SET adres = pAdres
        WHERE id_zesp = pId;
    END;
    
    FUNCTION wezIdentyfikator(pNazwa VARCHAR)
        RETURN NUMBER IS
        vRet NUMBER := -1;
    BEGIN
        SELECT id_zesp INTO vRet FROM zespoly
            WHERE nazwa = pNazwa;
        RETURN vRet;
    END;
        
    FUNCTION wezNazwe(pID NUMBER)
        RETURN VARCHAR IS
        vRet VARCHAR(255) := -1;
    BEGIN
        SELECT nazwa INTO vRet FROM zespoly
            WHERE id_zesp = pID;
        RETURN vRet;
    END;
    
    FUNCTION wezAdres(pID NUMBER)
        RETURN VARCHAR IS
        vRet VARCHAR(255) := -1;
    BEGIN
        SELECT adres INTO vRet FROM zespoly
            WHERE id_zesp = pID;
        RETURN vRet;
    END;
        
END IntZespoly;

--testy zad 8.
BEGIN
    IntZespoly.dodajZespol('test', 'gdzies');
    IntZespoly.dodajZespol('test', 'gdzies');
    IntZespoly.usunZespol(60);
    IntZespoly.usunZespol('test');
    IntZespoly.dodajZespol('test', 'gdzies');
    IntZespoly.zmienNazwe(60, 'test2');
    IntZespoly.zmienAdres(60, 'gdzies dalej');
    DBMS_OUTPUT.PUT_LINE('ID: ' || IntZespoly.wezIdentyfikator('test2'));
    DBMS_OUTPUT.PUT_LINE('Nazwa: ' || IntZespoly.wezNazwe(60));
    DBMS_OUTPUT.PUT_LINE('Adres: ' || IntZespoly.wezAdres(60));
END;

