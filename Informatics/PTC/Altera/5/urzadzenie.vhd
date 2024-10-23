library ieee;
use ieee.std_logic_1164.all;

entity urzadzenie is
	port (
		we: in std_logic_vector(3 downto 0);
		wy: out std_logic_vector(14 downto 0));  --zmienna wy steruje diodami. Posiadamy zakres od 0 do (2^4-1) na wejściu, czyli możemy zapalić maksymalnie 15 diod. 
end urzadzenie;

architecture zmienna of urzadzenie is
	use ieee.numeric_std.all;
	signal diody : std_logic_vector(14 downto 0);
begin
--Coś jest nie tak z ponizszym rozwiazaniem, wiec zrobie to w bardziej prymitywny sposob
	--process (we)
	--variable liczba : integer range 0 to 15;
	--begin
		--zerowanie
		--diody <= (others => '0');
		--liczba := to_integer(unsigned(we));
		--for i in 14 to 14 loop
			--przesunięcie bitowe o jeden w lewo, wypelniamy 1 lub 0 
			--if i < liczba then
				--diody <= diody(13 downto 0) & '1';
			--else
				--diody <= diody(13 downto 0) & '0';
			--end if;
		--end loop;
		--wy <= diody;
	--end process;
	with we select wy <=
		"000000000000001" when "0001",
		"000000000000011" when "0010",
		"000000000000111" when "0011",
		"000000000001111" when "0100",
		"000000000011111" when "0101",
		"000000000111111" when "0110",
		"000000001111111" when "0111",
		"000000011111111" when "1000",
		"000000111111111" when "1001",
		"000001111111111" when "1010",
		"000011111111111" when "1011",
		"000111111111111" when "1100",
		"001111111111111" when "1101",
		"011111111111111" when "1110",
		"111111111111111" when "1111",
		"000000000000000" when others;
end;