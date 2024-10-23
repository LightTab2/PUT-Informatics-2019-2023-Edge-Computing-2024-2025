library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
entity rejestr is
    generic(
        length: NATURAL := 6
    );
    port(
        wlTransmisji,
        weDanych,
        clk,
        reset 			: IN  STD_LOGIC;
        wyjscie      : OUT STD_LOGIC
    );
end entity rejestr;

LIBRARY ieee;
USE ieee.std_logic_1164.all;
ENTITY flipflopD IS
	PORT(
		D, 
		clk, 
		reset : IN STD_LOGIC;
		Q 		: OUT STD_LOGIC
	);
end flipflopD;

ARCHITECTURE logic OF flipflopD IS
begin
	process(clk) begin
		if RISING_EDGE(clk) then
			if reset = '1' then
				Q <= '0';
			else
				Q <= D;
			end if;
		end if;
	end process;
end logic;

architecture logic of rejestr is
	COMPONENT flipflopD
		PORT (D, clk, reset : IN STD_LOGIC; Q : OUT STD_LOGIC);
	END COMPONENT;
	
	signal zawartosc : STD_LOGIC_VECTOR (length downto 0);
begin
	--generuje [length] przerzutników D
	flip6: for i in 0 to length-1 generate
		Fi: flipflopD PORT MAP (zawartosc(i), clk, reset, zawartosc(i+1));
	end generate;
	
	zawartosc(0) <= weDanych;
	wyjscie <= zawartosc(length);
end architecture logic;