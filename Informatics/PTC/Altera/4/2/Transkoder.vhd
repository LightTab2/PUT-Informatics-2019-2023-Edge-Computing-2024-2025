library ieee;
use ieee.std_logic_1164.all;
-- IN k.binarny OUT 0,4,3,2,1,5,6

entity Transkoder is
port (
A, B, C : in std_logic;
X, Y, Z : buffer std_logic;
Wyswietlacz : out std_logic_vector(0 to 6));
end Transkoder;

architecture logic of Transkoder is
begin
	-- Obliczane wg. funckji logicznej, którą otrzymałem przy pomocy tablic Karnaugha
	X <= (A and B) or ((not B) and C);
	Y <= B;
	Z <= ((not A) and B and (not C)) or (A and (not B)) or (A and C);
	
	--Kod od wyświetlacza
	process(A, B, C)
	variable wyswietlana : std_logic_vector(2 downto 0);
	begin
		wyswietlana := X & Y & Z;
		case wyswietlana is
		when "001" =>
			Wyswietlacz <= "0110000";	--1
		when "010" =>
			Wyswietlacz <= "1101101";	--2
		when "011" =>
			Wyswietlacz <= "1111001";	--3
		when "100" =>
			Wyswietlacz <= "0110011";	--4
		when "101" =>
			Wyswietlacz <= "1011011";  --5
		when "110" =>
			Wyswietlacz <= "1011111";	--6
		when "111" =>
			Wyswietlacz <= "1110000";	--7
		when others =>
			Wyswietlacz <= "1111110";	--0 i inne przypadki też zostaną ustawione na 0
		end case;
	end process;
end logic;
