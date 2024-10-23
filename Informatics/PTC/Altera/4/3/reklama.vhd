LIBRARY ieee;
USE ieee.std_logic_1164.all;

-- Implementacja układu
ENTITY reklama IS
	PORT ( 	
				SW : IN STD_LOGIC_VECTOR(17 DOWNTO 0);

				HEX0, HEX1,
				HEX2, HEX3, 
				HEX4, HEX5, 
				HEX6, HEX7: OUT STD_LOGIC_VECTOR(0 TO 6)
			);
END reklama;

-- Implementacja multipleksera
LIBRARY ieee;
USE ieee.std_logic_1164.all;
ENTITY mux3bit_8to1 IS 
	PORT (
			S, 
			U0, U1,
			U2, U3, 
			U4, U5,
			U6, U7: IN STD_LOGIC_VECTOR(2 DOWNTO 0);

			M0 : OUT STD_LOGIC_VECTOR(2 DOWNTO 0)
	);
END mux3bit_8to1;

-- Implementacja transkodera
LIBRARY ieee;
USE ieee.std_logic_1164.all;
ENTITY char7seg IS
	PORT (
		KodZnaku : IN STD_LOGIC_VECTOR(2 DOWNTO 0);
		
		KodWyswietlania : OUT STD_LOGIC_VECTOR(0 TO 6)
	);
END char7seg;

-- Logika multipleksera
ARCHITECTURE logic OF mux3bit_8to1 IS
Begin 
	With S select M0 <= 
			U0 when "000",
			U1 when "001",
			U2 when "010",
			U3 when "011",
			U4 when "100",
			U5 when "101",
			U6 when "110",
			U7 when "111";
END logic;

-- Logika transkodera
ARCHITECTURE logic OF char7seg IS
Begin
	With KodZnaku select KodWyswietlania <=
		"0100001" when "001", 	-- G
		"1001000" when "010", 	-- H
		"1001111" when "011", 	-- I
		"1000011" when "100", 	-- J
		"1111111" when others; 	-- Spacja
END logic;

-- Logika układu
ARCHITECTURE logic OF reklama IS
	-- Kod spacji
	CONSTANT SPACJA: STD_LOGIC_VECTOR(2 DOWNTO 0) := "000";

	-- Multipekser
	COMPONENT mux3bit_8to1
		PORT ( 
					S,
					U0, U1, 
					U2, U3, 
					U4, U5, 
					U6, U7: IN STD_LOGIC_VECTOR(2 DOWNTO 0);
					
					M0 : OUT STD_LOGIC_VECTOR(2 DOWNTO 0)
				);
	END COMPONENT;

	-- Transkoder
	COMPONENT char7seg
	PORT ( 
				KodZnaku : IN STD_LOGIC_VECTOR(2 DOWNTO 0);
				
				KodWyswietlania : OUT STD_LOGIC_VECTOR(0 TO 6)
			);
	END COMPONENT;

	-- Wyjścia z multiplekserów
	SIGNAL 	M0, M1,
				M2, M3,
				M4, M5,
				M6, M7: STD_LOGIC_VECTOR(2 DOWNTO 0);
			
BEGIN
	-- Tworzy 8 multiplekserów i mapuje porty (tak jakby podłączał). 
	-- Pierwszy port odpowiada za S, wszystkie pomiędzy pierwszym a ostatnim za kolejne U, zaś ostatnie to jest wyjście (M0)
	MUX0: mux3bit_8to1 PORT MAP (SW(17 DOWNTO 15), SW(14 DOWNTO 12), SW(11 DOWNTO 9), SW(8 DOWNTO 6), SW(5 DOWNTO 3), SW(2 DOWNTO 0), SPACJA, SPACJA, SPACJA, M0); 
	MUX1: mux3bit_8to1 PORT MAP (SW(17 DOWNTO 15), SW(11 DOWNTO 9), SW(8 DOWNTO 6), SW(5 DOWNTO 3), SW(2 DOWNTO 0), SPACJA, SPACJA, SPACJA, SW(14 DOWNTO 12), M1);  
	MUX2: mux3bit_8to1 PORT MAP (SW(17 DOWNTO 15), SW(8 DOWNTO 6), SW(5 DOWNTO 3), SW(2 DOWNTO 0), SPACJA, SPACJA, SPACJA, SW(14 DOWNTO 12), SW(11 DOWNTO 9), M2);  
	MUX3: mux3bit_8to1 PORT MAP (SW(17 DOWNTO 15), SW(5 DOWNTO 3), SW(2 DOWNTO 0), SPACJA, SPACJA, SPACJA, SW(14 DOWNTO 12), SW(11 DOWNTO 9), SW(8 DOWNTO 6), M3);  
	MUX4: mux3bit_8to1 PORT MAP (SW(17 DOWNTO 15), SW(2 DOWNTO 0), SPACJA, SPACJA, SPACJA, SW(14 DOWNTO 12), SW(11 DOWNTO 9), SW(8 DOWNTO 6), SW(5 DOWNTO 3), M4);  
	MUX5: mux3bit_8to1 PORT MAP (SW(17 DOWNTO 15), SPACJA, SPACJA, SPACJA, SW(14 DOWNTO 12), SW(11 DOWNTO 9), SW(8 DOWNTO 6), SW(5 DOWNTO 3), SW(2 DOWNTO 0), M5);  
	MUX6: mux3bit_8to1 PORT MAP (SW(17 DOWNTO 15), SPACJA, SPACJA, SW(14 DOWNTO 12), SW(11 DOWNTO 9), SW(8 DOWNTO 6), SW(5 DOWNTO 3), SW(2 DOWNTO 0), SPACJA, M6);   
	MUX7: mux3bit_8to1 PORT MAP (SW(17 DOWNTO 15), SPACJA, SW(14 DOWNTO 12), SW(11 DOWNTO 9), SW(8 DOWNTO 6), SW(5 DOWNTO 3), SW(2 DOWNTO 0), SPACJA, SPACJA, M7);  
	
	-- Tworzy 8 transkoderów i mapuje porty
	H0: char7seg PORT MAP (M0, HEX0);
	H1: char7seg PORT MAP (M1, HEX1);
	H2: char7seg PORT MAP (M2, HEX2);
	H3: char7seg PORT MAP (M3, HEX3);
	H4: char7seg PORT MAP (M4, HEX4);
	H5: char7seg PORT MAP (M5, HEX5);
	H6: char7seg PORT MAP (M6, HEX6);
	H7: char7seg PORT MAP (M7, HEX7);
END logic;