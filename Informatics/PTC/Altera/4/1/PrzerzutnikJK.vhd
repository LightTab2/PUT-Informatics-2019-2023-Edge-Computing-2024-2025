library ieee;
use ieee.std_logic_1164.all;

entity PrzerzutnikJK is
port (
J, K, Clk : in std_logic;
Q : buffer std_logic);
end PrzerzutnikJK;

architecture logic of PrzerzutnikJK is
begin
	process(J, K, Clk)
	begin
	-- Wejście zegarowe musi być narastającym sygnałem
		if (rising_edge(Clk)) then
			if (j = '1') then
				if (k = '1') then
					Q <= not Q;
				else
					-- Czytanie Q wymaga buforowania Q, dlatego Q posiada typ buffer, a nie out. Ewentualnie można było użyć typu inout
					Q <= '1';
				end if;
			elsif (k = '1') then
				Q <= '0';
			end if;
		end if;
	end process;
end logic;
