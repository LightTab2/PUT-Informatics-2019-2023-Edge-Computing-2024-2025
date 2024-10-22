library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity dec7seg is
	Port ( 	bcd : in  std_logic_vector(3 downto 0);
	 		seg : out std_logic_vector(6 downto 0));
end entity dec7seg;

architecture with_delay of dec7seg is
begin
	with bcd select
		seg <=   b"111_1001" when x"1",
		 		 b"010_0100" when x"2",
		    	 b"011_0000" when x"3",
		    	 b"001_1001" when x"4",
		    	 b"001_0010" when x"5",
		    	 b"000_0010" when x"6",
		    	 b"111_1000" when x"7",
		    	 b"000_0000" when x"8",
		    	 b"001_0000" when x"9",
		    	 b"100_0000" when x"0",
				 b"001_0010" when x"A",  -- S
                 b"100_1111" when x"B",  -- i
                 b"100_0111" when x"C",  -- R
                 b"100_0111" when x"D",  -- v
				 b"010_0000" when x"E",  -- E
		    	 b"111_1111" when others;
end architecture with_delay;
