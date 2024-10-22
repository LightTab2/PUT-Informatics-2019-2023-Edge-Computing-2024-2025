library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity dec7seg is
	Port ( 	bcd : in  std_logic_vector(3 downto 0);
	 		seg : out std_logic_vector(6 downto 0));
end entity dec7seg;

architecture prawda of dec7seg is
begin
	with bcd select
		seg <= 	b"000_0110" when x"1",
		 		b"101_1011" when x"2",
		    	b"100_1111" when x"3",
		    	b"110_0110" when x"4",
		    	b"110_1101" when x"5",
		    	b"111_1101" when x"6",
		    	b"000_0111" when x"7",
		    	b"111_1111" when x"8",
		    	b"110_1111" when x"9",
		    	b"011_1111" when x"0",
				b"111_0111" when x"A",
				b"111_1100" when x"B",
				b"011_1001" when x"C",
				b"101_1110" when x"D",
				b"111_1001" when x"E",
				b"111_0001" when x"F",
		    	b"000_0000" when others;
end architecture prawda;
