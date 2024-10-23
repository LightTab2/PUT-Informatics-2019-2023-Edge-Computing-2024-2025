--zad 1
DECLARE
    CURSOR cAsystenci IS
    SELECT nazwisko, zatrudniony
    FROM Pracownicy
    WHERE etat = 'ASYSTENT'
    ORDER BY nazwisko;
    vNazwisko Pracownicy.nazwisko%type;
    vZatrudniony Pracownicy.zatrudniony%type;
BEGIN
    OPEN cAsystenci;
    LOOP
        FETCH cAsystenci INTO vNazwisko, vZatrudniony;
        EXIT WHEN cAsystenci%NOTFOUND;
        DBMS_OUTPUT.PUT_LINE(vNazwisko || ' pracuje od ' || vZatrudniony);
    END LOOP;
    CLOSE cAsystenci;
END;

--zad 2
DECLARE
    CURSOR cPracownicy IS
    SELECT nazwisko
    FROM (SELECT nazwisko, placa_pod+NVL(placa_dod, 0) placa 
    FROM pracownicy ORDER BY placa DESC);
    vNazwisko Pracownicy.nazwisko%type;
BEGIN
    OPEN cPracownicy;
    LOOP
        FETCH cPracownicy INTO vNazwisko;
        EXIT WHEN cPracownicy%NOTFOUND OR cPracownicy%ROWCOUNT > 3;
        DBMS_OUTPUT.PUT_LINE(cPracownicy%ROWCOUNT || ': ' || vNazwisko);
    END LOOP;
    CLOSE cPracownicy;
END;

--zad 3
DECLARE
    CURSOR cPracownicy IS
    SELECT placa_pod
    FROM Pracownicy
    WHERE TO_CHAR(Pracownicy.zatrudniony, 'D') = 2
    FOR UPDATE;
BEGIN
    FOR vDane IN cPracownicy
    LOOP
        UPDATE Pracownicy
        SET placa_pod = placa_pod * 1.2
        WHERE CURRENT OF cPracownicy;
    END LOOP;
END;

--zad 4
DECLARE
    CURSOR cZespoly IS
    SELECT nazwa
    FROM Zespoly;

    CURSOR cPracownicy(zespol VARCHAR) IS
    SELECT nazwisko, etat, id_zesp
    FROM Pracownicy
    WHERE id_zesp = (SELECT id_zesp FROM Zespoly WHERE nazwa = zespol)
    FOR UPDATE OF nazwisko;
BEGIN
    FOR vZespol IN cZespoly
    LOOP
        FOR vDane IN cPracownicy(vZespol.nazwa)
        LOOP
            IF vZespol.nazwa = 'ALGORYTMY' THEN
                UPDATE Pracownicy
                SET placa_dod = placa_dod + 100
                WHERE CURRENT OF cPracownicy;
            ELSIF vZespol.nazwa = 'ADMINISTRACJA' THEN
                UPDATE Pracownicy
                SET placa_dod = placa_dod + 150
                WHERE CURRENT OF cPracownicy;
            ELSIF vDane.etat = 'STAZYSTA' THEN
                DELETE Pracownicy
                WHERE CURRENT OF cPracownicy;
            END IF;
        END LOOP;
    END LOOP;
END;

--zad 5
CREATE OR REPLACE PROCEDURE PokazPracownikowEtatu(pEtat VARCHAR) IS
    CURSOR cPracownicy(pEtat VARCHAR) IS
    SELECT nazwisko, etat
    FROM Pracownicy
    WHERE etat = pEtat;
BEGIN
    FOR vDane IN cPracownicy(pEtat)
    LOOP
        DBMS_OUTPUT.PUT_LINE(vDane.nazwisko);
    END LOOP;
END PokazPracownikowEtatu;

--zad 6
--zad 6
CREATE OR REPLACE PROCEDURE RaportKadrowy IS
    CURSOR cEtaty IS
    SELECT nazwa
    FROM Etaty;
    
    CURSOR cPracownicy(pEtat VARCHAR) IS
    SELECT nazwisko, etat, placa_pod + nvl(placa_dod,0) pensja
    FROM Pracownicy
    WHERE etat = pEtat;
    
    vCount NUMBER;
    vPensjaSum NUMBER;
BEGIN
    FOR et IN cEtaty
    LOOP
        vCount := 0;
        vPensjaSum := 0;
        DBMS_OUTPUT.PUT_LINE('Etat: ' || et.nazwa);
        DBMS_OUTPUT.PUT_LINE('------------------------------');
        FOR vDane IN cPracownicy(et.nazwa)
        LOOP
            DBMS_OUTPUT.PUT_LINE(cPracownicy%ROWCOUNT || '. ' || vDane.nazwisko || ', pensja: ' || vDane.pensja );
            vCount := vCount + 1;
            vPensjaSum := vPensjaSum + vDane.pensja;
        END LOOP;
        DBMS_OUTPUT.PUT_LINE('Liczba pracownikow: ' || vCount);
        IF vCount = 0 THEN
            DBMS_OUTPUT.PUT_LINE('Srednia placa na etacie: brak');
        ELSE
            DBMS_OUTPUT.PUT_LINE('Srednia placa na etacie: ' || vPensjaSum/vCount);
        END IF;
    END LOOP;
END RaportKadrowy;

--zad 7
CREATE OR REPLACE PACKAGE BODY IntZespoly IS
    PROCEDURE dodajZespol(pNazwa VARCHAR, pAdres VARCHAR) IS
        vLastId NUMBER;
    BEGIN
        SELECT id_zesp INTO vLastId FROM zespoly 
        ORDER BY id_zesp DESC FETCH FIRST 1 ROWS ONLY;
        
        INSERT INTO zespoly(id_zesp, nazwa, adres)
        VALUES(vLastId + 10, pNazwa, pAdres);
        IF NOT SQL%FOUND THEN
            DBMS_OUTPUT.PUT_LINE ('Nie udalo wstawic nowego zespolu');
        END IF;
    END dodajZespol;
    
    PROCEDURE usunZespol(pNazwa VARCHAR) IS
    BEGIN
        DELETE zespoly 
        WHERE nazwa = pNazwa;
        IF NOT SQL%FOUND THEN
            DBMS_OUTPUT.PUT_LINE ('Nie udalo sie usunac zespolu');
        END IF;
    END usunZespol;
    
    PROCEDURE usunZespol(pId NUMBER) IS
    BEGIN
        DELETE zespoly 
        WHERE id_zesp = pId;
        IF NOT SQL%FOUND THEN
            DBMS_OUTPUT.PUT_LINE ('Nie udalo sie usunac zespolu');
        END IF;
    END usunZespol;
    
    PROCEDURE zmienNazwe(pId NUMBER, pNazwa VARCHAR) IS
    BEGIN
        UPDATE zespoly 
        SET nazwa = pNazwa
        WHERE id_zesp = pId;
        IF NOT SQL%FOUND THEN
            DBMS_OUTPUT.PUT_LINE ('Nie udalo sie zmienic nazwy');
        END IF;
    END;
    
    PROCEDURE zmienAdres(pId NUMBER, pAdres VARCHAR) IS
    BEGIN
        UPDATE zespoly 
        SET adres = pAdres
        WHERE id_zesp = pId;
        IF NOT SQL%FOUND THEN
            DBMS_OUTPUT.PUT_LINE ('Nie udalo sie zmienic adresu');
        END IF;
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

--zad 8
CREATE OR REPLACE PACKAGE BODY IntZespoly IS
    exNiemaZespol EXCEPTION;
    exJestjuzZespol EXCEPTION;
    PRAGMA EXCEPTION_INIT(exNiemaZespol, -20404);
    PRAGMA EXCEPTION_INIT(exJestjuzZespol, -20400);
    PROCEDURE dodajZespol(pNazwa VARCHAR, pAdres VARCHAR) IS
        vLastId NUMBER;
        vCount NUMBER;
    BEGIN
        SELECT COUNT(*) INTO vCount FROM Zespoly WHERE nazwa = pNazwa;
        IF vCount > 0 THEN
            RAISE_APPLICATION_ERROR(-20400, 'Juz istnieje zespol o nazwie: ' || pNazwa);
        END IF;
        SELECT id_zesp INTO vLastId FROM zespoly 
        ORDER BY id_zesp DESC FETCH FIRST 1 ROWS ONLY;
        
        INSERT INTO zespoly(id_zesp, nazwa, adres)
        VALUES(vLastId + 10, pNazwa, pAdres);
        IF NOT SQL%FOUND THEN
            DBMS_OUTPUT.PUT_LINE ('Nie udalo wstawic nowego zespolu');
        END IF;
    END dodajZespol;
    
    PROCEDURE usunZespol(pNazwa VARCHAR) IS
    vCount NUMBER;
    BEGIN
        SELECT COUNT(*) INTO vCount FROM Zespoly WHERE nazwa = pNazwa;
        IF vCount = 0 THEN
            RAISE_APPLICATION_ERROR(-20404, 'Nie znaleziono zespolu o nazwie: ' || pNazwa);
        END IF;
        DELETE zespoly 
        WHERE nazwa = pNazwa;
        IF NOT SQL%FOUND THEN
            DBMS_OUTPUT.PUT_LINE ('Nie udalo sie usunac zespolu');
        END IF;
    END usunZespol;
    
    PROCEDURE usunZespol(pId NUMBER) IS
    vCount NUMBER;
    BEGIN
        SELECT COUNT(*) INTO vCount FROM Zespoly WHERE id_zesp = pId;
        IF vCount = 0 THEN
            RAISE_APPLICATION_ERROR(-20404, 'Nie znaleziono zespolu o id: ' || pId);
        END IF;
        DELETE zespoly 
        WHERE id_zesp = pId;
        IF NOT SQL%FOUND THEN
            DBMS_OUTPUT.PUT_LINE ('Nie udalo sie usunac zespolu');
        END IF;
    END usunZespol;
    
    PROCEDURE zmienNazwe(pId NUMBER, pNazwa VARCHAR) IS
    vCount NUMBER;
    BEGIN
    SELECT COUNT(*) INTO vCount FROM Zespoly WHERE id_zesp = pId;
        IF vCount = 0 THEN
            RAISE_APPLICATION_ERROR(-20404, 'Nie znaleziono zespolu o id: ' || pId);
        END IF;
        UPDATE zespoly 
        SET nazwa = pNazwa
        WHERE id_zesp = pId;
        IF NOT SQL%FOUND THEN
            DBMS_OUTPUT.PUT_LINE ('Nie udalo sie zmienic nazwy');
        END IF;
    END;
    
    PROCEDURE zmienAdres(pId NUMBER, pAdres VARCHAR) IS
    vCount NUMBER;
    BEGIN
        SELECT COUNT(*) INTO vCount FROM Zespoly WHERE id_zesp = pId;
        IF vCount = 0 THEN
            RAISE_APPLICATION_ERROR(-20404, 'Nie znaleziono zespolu o id: ' || pId);
        END IF;
        UPDATE zespoly 
        SET adres = pAdres
        WHERE id_zesp = pId;
        IF NOT SQL%FOUND THEN
            DBMS_OUTPUT.PUT_LINE ('Nie udalo sie zmienic adresu');
        END IF;
    END;
    
    FUNCTION wezIdentyfikator(pNazwa VARCHAR)
        RETURN NUMBER IS
        vRet NUMBER := -1;
        vCount NUMBER;
    BEGIN
        SELECT COUNT(*) INTO vCount FROM Zespoly WHERE nazwa = pNazwa;
        IF vCount = 0 THEN
            RAISE_APPLICATION_ERROR(-20404, 'Nie znaleziono zespolu o nazwie: ' || pNazwa);
        END IF;
        SELECT id_zesp INTO vRet FROM zespoly
            WHERE nazwa = pNazwa;
        RETURN vRet;
    END;
        
    FUNCTION wezNazwe(pID NUMBER)
        RETURN VARCHAR IS
        vRet VARCHAR(255) := -1;
        vCount NUMBER;
    BEGIN
        SELECT COUNT(*) INTO vCount FROM Zespoly WHERE id_zesp = pId;
        IF vCount = 0 THEN
            RAISE_APPLICATION_ERROR(-20404, 'Nie znaleziono zespolu o id: ' || pId);
        END IF;
        SELECT nazwa INTO vRet FROM zespoly
            WHERE id_zesp = pID;
        RETURN vRet;
    END;
    
    FUNCTION wezAdres(pID NUMBER)
        RETURN VARCHAR IS
        vRet VARCHAR(255) := -1;
        vCount NUMBER;
    BEGIN
        SELECT COUNT(*) INTO vCount FROM Zespoly WHERE id_zesp = pId;
        IF vCount = 0 THEN
            RAISE_APPLICATION_ERROR(-20404, 'Nie znaleziono zespolu o id: ' || pId);
        END IF;
        SELECT adres INTO vRet FROM zespoly
            WHERE id_zesp = pID;
        RETURN vRet;
    END;
END IntZespoly;