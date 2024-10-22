library ieee;
use ieee.std_logic_1164.all;

entity encoder is
  port (s: in  std_logic_vector(5 downto 0); 
		y: out std_logic_vector(2 downto 0) );
end entity encoder;

architecture beh of encoder is
begin
	with s select
		y <= 	b"000" when b"00_0000",
				b"001" when b"00_0001",
				b"010" when b"00_0010",
				b"011" when b"00_0100",
				b"100" when b"00_1000",
				b"101" when b"01_0000",
				b"110" when b"10_0000",
		 		b"000" when others;		

end architecture beh;
