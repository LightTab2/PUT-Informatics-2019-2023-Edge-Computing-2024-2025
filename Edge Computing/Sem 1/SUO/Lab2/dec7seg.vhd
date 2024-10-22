library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity dec7seg is
	generic (delay: time := 3 ns);
	Port ( 	bcd : in  std_logic_vector(3 downto 0);
	 		seg : out std_logic_vector(6 downto 0));
end entity dec7seg;

architecture with_delay of dec7seg is
	signal seg_s: std_logic_vector(seg'range);	
begin
		seg <= not seg_s after delay;
	with bcd select
		seg_s <= b"111_1001" when x"1",
		 		 b"010_0100" when x"2",
		    	 b"011_0000" when x"3",
		    	 b"001_1001" when x"4",
		    	 b"001_0010" when x"5",
		    	 b"000_0010" when x"6",
		    	 b"111_1000" when x"7",
		    	 b"000_0000" when x"8",
		    	 b"001_0000" when x"9",
		    	 b"100_0000" when x"0",
				 b"000_1000" when x"A",
				 b"000_0011" when x"B",
				 b"100_0110" when x"C",
				 b"010_0001" when x"D",
				 b"000_0110" when x"E",
				 b"000_1110" when x"F",
		    	 b"111_1111" when others;
end architecture with_delay;
