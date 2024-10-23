--zad 1
CREATE OR REPLACE TRIGGER PoDML
AFTER INSERT OR DELETE OR UPDATE ON Zespoly
DECLARE
    vKomunikat VARCHAR(255);
    vCount NUMBER;
BEGIN
    vKomunikat := '[' || CURRENT_DATE || '] Operacja ';
    CASE
        WHEN INSERTING THEN
        vKomunikat := vKomunikat || 'INSERT ';
        WHEN DELETING THEN
        vKomunikat := vKomunikat || 'DELETE ';
        WHEN UPDATING THEN
        vKomunikat := vKomunikat || 'INSERT ';
    END CASE;
    SELECT COUNT(*) INTO vCount FROM Zespoly;
    vKomunikat := vKomunikat || 'na tabeli Zespoly. Liczba rekordow po operacji: ' || vCount;
    DBMS_OUTPUT.PUT_LINE(vKomunikat);
END;

--zad 1 testy
INSERT INTO ZESPOLY VALUES (110,'ADMINISTRACJA',      'PIOTROWO 3A');
INSERT INTO ZESPOLY VALUES (120,'SYSTEMY ROZPROSZONE','PIOTROWO 3A');
INSERT INTO ZESPOLY VALUES (130,'SYSTEMY EKSPERCKIE', 'STRZELECKA 14');
INSERT INTO ZESPOLY VALUES (140,'ALGORYTMY',          'WLODKOWICA 16');
INSERT INTO ZESPOLY VALUES (150,'BADANIA OPERACYJNE', 'MIELZYNSKIEGO 30');

DELETE FROM ZESPOLY 
WHERE id_zesp = 150;

DELETE FROM ZESPOLY 
WHERE id_zesp > 90;

--zad 2
CREATE OR REPLACE TRIGGER PokazPlace
    BEFORE UPDATE OF placa_pod ON Pracownicy
    FOR EACH ROW
    WHEN ((OLD.placa_pod <> NEW.placa_pod) OR (OLD.placa_pod IS NULL AND NEW.placa_pod IS NOT NULL) OR (OLD.placa_pod IS NOT NULL AND NEW.placa_pod IS NULL))
DECLARE
    vKomunikat VARCHAR(255);
BEGIN
    DBMS_OUTPUT.PUT_LINE('Pracownik ' || :OLD.nazwisko);
    vKomunikat := 'Placa przed modyfikacja: ';
    IF :OLD.placa_pod IS NULL THEN
        vKomunikat := vKomunikat || 'brak';
    ELSE
        vKomunikat := vKomunikat || :OLD.placa_pod;
    END IF;
    DBMS_OUTPUT.PUT_LINE(vKomunikat);

    vKomunikat := 'Placa po modyfikacji: ';
    IF :NEW.placa_pod IS NULL THEN
        vKomunikat := vKomunikat || 'brak';
    ELSE
        vKomunikat := vKomunikat || :NEW.placa_pod;
    END IF;
    DBMS_OUTPUT.PUT_LINE(vKomunikat);
END;

--zad 2 testy
UPDATE Pracownicy SET 
    placa_pod = placa_pod * 1.1
WHERE id_zesp = 20;

UPDATE Pracownicy SET 
    placa_pod = NULL
WHERE id_zesp = 20;

UPDATE Pracownicy SET 
    placa_pod = 600
WHERE id_zesp = 20;

--zad 3
CREATE OR REPLACE TRIGGER UzupelnijPlace
BEFORE INSERT OR UPDATE OF placa_pod ON Pracownicy
FOR EACH ROW
DECLARE
    vPlacaMin Etaty.placa_min%TYPE;
    vPlacaMax Etaty.placa_max%TYPE;
BEGIN
    IF :NEW.etat IS NOT NULL THEN
        SELECT placa_min, placa_max
        INTO vPlacaMin, vPlacaMax
        FROM Etaty WHERE nazwa = :NEW.etat;
        IF :NEW.placa_pod < vPlacaMin THEN
            :NEW.placa_pod := vPlacaMin;
        ELSIF :NEW.placa_pod > vPlacaMax THEN
            :NEW.placa_pod := vPlacaMax;
        END IF;
        :NEW.placa_pod := NVL(:NEW.placa_pod, 0);
    END IF;
        :NEW.placa_dod := NVL(:NEW.placa_dod, 0);
END;

--zad 3 testy
INSERT INTO Pracownicy(id_prac, nazwisko, etat, placa_pod)
VALUES(1000, 'Papuga', 'PROFESOR', 6666);

INSERT INTO Pracownicy(id_prac, nazwisko, etat, placa_pod)
VALUES(1001, 'Kura', 'PROFESOR', 0);

INSERT INTO Pracownicy(id_prac, nazwisko, etat, placa_pod)
VALUES(1002, 'Flaming', 'ASYSTENT', 6666);

INSERT INTO Pracownicy(id_prac, nazwisko, etat, placa_pod)
VALUES(1003, 'Sowa', 'ASYSTENT', 0);

INSERT INTO Pracownicy(id_prac, nazwisko, etat, placa_pod)
VALUES(1004, 'Ptak niegdak', NULL, 6666);

SELECT * FROM Pracownicy
WHERE id_prac > 999;

DELETE FROM Pracownicy
WHERE id_prac > 999;

--zad 4
SELECT MAX(id_zesp) FROM ZESPOLY;

CREATE SEQUENCE SEQ_Zespoly
START WITH 51
  INCREMENT BY 1;
  
CREATE OR REPLACE TRIGGER UzupelnijID
BEFORE INSERT ON Zespoly
FOR EACH ROW
BEGIN
    IF :NEW.id_zesp IS NULL THEN
        :NEW.id_zesp := SEQ_Zespoly.NEXTVAL;
    END IF;
END;

--zad 5
CREATE OR REPLACE VIEW Szefowie(szef, pracownicy)
AS SELECT s.nazwisko, COUNT(p.id_prac) FROM 
Pracownicy s JOIN Pracownicy p ON p.id_szefa = s.id_prac
GROUP BY s.nazwisko;

CREATE OR REPLACE TRIGGER Usuwanie
INSTEAD OF DELETE ON Szefowie
FOR EACH ROW
DECLARE
    CURSOR cPracownicy(szef VARCHAR) IS 
    SELECT p.nazwisko, p.id_prac FROM
    Pracownicy s JOIN Pracownicy p ON p.id_szefa = s.id_prac
    WHERE s.nazwisko = szef
    FOR UPDATE;
    
    vCount NUMBER;
BEGIN 
    FOR vDane IN cPracownicy(:OLD.szef)
    LOOP
        SELECT COUNT(*) INTO vCount FROM Szefowie
        WHERE szef = vDane.nazwisko;
        IF (vCount > 0) THEN
            RAISE_APPLICATION_ERROR(-20001, 'Jeden z podwladnych usuwanego 
            pracownika jest szefem innych pracownikow. Usuwanie anulowane');
        END IF;
    END LOOP;
    
    FOR vDane IN cPracownicy(:OLD.szef)
    LOOP
        DELETE Pracownicy
        WHERE CURRENT OF cPracownicy;
    END LOOP;
    
    DELETE Pracownicy
    WHERE nazwisko = :OLD.szef;
END;

--zad 5 testy
SELECT * FROM Szefowie;

DELETE FROM Szefowie WHERE szef='WEGLARZ';

SELECT * FROM Szefowie;

DELETE FROM Szefowie WHERE szef='MORZY';

SELECT * FROM Szefowie;

--zad 6
CREATE OR REPLACE TRIGGER dbajOLiczbePracWZespolach
BEFORE DELETE OR INSERT OR UPDATE ON Pracownicy
FOR EACH ROW
BEGIN
    CASE
        WHEN INSERTING THEN
            UPDATE Zespoly SET
            liczba_pracownikow = liczba_pracownikow + 1
            WHERE id_zesp = :NEW.id_zesp;
        WHEN DELETING THEN
            UPDATE Zespoly SET
            liczba_pracownikow = liczba_pracownikow - 1
            WHERE id_zesp = :OLD.id_zesp;
        WHEN UPDATING THEN
            UPDATE Zespoly SET
            liczba_pracownikow = liczba_pracownikow - 1
            WHERE id_zesp = :OLD.id_zesp;
            
            UPDATE Zespoly SET
            liczba_pracownikow = liczba_pracownikow + 1
            WHERE id_zesp = :NEW.id_zesp;
    END CASE;
END;

--zad 6 testy
UPDATE ZESPOLY z SET
liczba_pracownikow = (SELECT COUNT(*) FROM pracownicy p WHERE p.id_zesp = z.id_zesp);

SELECT * FROM Zespoly;

INSERT INTO Pracownicy(id_prac, nazwisko, id_zesp, id_szefa)
    VALUES(300,'NOWY PRACOWNIK',40,120);
    
SELECT * FROM Zespoly;

UPDATE Pracownicy SET id_zesp = 10 WHERE id_zesp = 30;

SELECT * FROM Zespoly;

DELETE FROM Pracownicy;

SELECT * FROM Zespoly;

--zad 7
ALTER TABLE Pracownicy DROP CONSTRAINT FK_ID_SZEFA;

ALTER TABLE Pracownicy
ADD CONSTRAINT FK_ID_SZEFA FOREIGN KEY(id_szefa) REFERENCES Pracownicy(id_prac) ON DELETE CASCADE;

CREATE OR REPLACE TRIGGER Usun_Prac
AFTER DELETE ON Pracownicy
FOR EACH ROW
BEGIN 
    DBMS_OUTPUT.PUT_LINE( :OLD.nazwisko); 
END;

DELETE FROM Pracownicy WHERE nazwisko = 'MORZY';
ROLLBACK;

CREATE OR REPLACE TRIGGER Usun_Prac
BEFORE DELETE ON Pracownicy
FOR EACH ROW
BEGIN 
    DBMS_OUTPUT.PUT_LINE( :OLD.nazwisko);
END;

DELETE FROM Pracownicy WHERE nazwisko = 'MORZY';
ROLLBACK;

--zad 8

ALTER TABLE Pracownicy DISABLE ALL TRIGGERS;

SELECT trigger_name, status
FROM User_Triggers
WHERE table_name = 'PRACOWNICY';

DELETE FROM Pracownicy WHERE nazwisko = 'MORZY';
ROLLBACK;

--zad 9
SELECT trigger_name
FROM User_Triggers
WHERE table_name IN ('PRACOWNICY', 'ZESPOLY');

SELECT 'DROP TRIGGER ' || trigger_name || ';' FROM User_Triggers
WHERE table_name IN ('PRACOWNICY', 'ZESPOLY'); --komenda do wpisania do usuniecia

--zad 9 automatycznie
DECLARE
    CURSOR cTrigger IS
    SELECT trigger_name FROM User_Triggers
    WHERE table_name IN ('PRACOWNICY', 'ZESPOLY');
BEGIN  
    FOR vTrigger IN cTrigger
    LOOP  
        DBMS_OUTPUT.PUT_LINE (vTrigger.trigger_name);
        EXECUTE IMMEDIATE 'DROP TRIGGER ' || vTrigger.trigger_name;
    END LOOP;  
END;
