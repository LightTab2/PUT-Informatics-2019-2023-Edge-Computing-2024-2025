library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity sw2display is
	port (	sw                                 : in  std_logic_vector(9 downto 0);
			hex5, hex4, hex3, hex2, hex1, hex0 : out std_logic_vector(6 downto 0));
end entity sw2display;

architecture display of sw2display is
	signal selectSignal  : std_logic_vector(5 downto 0);
	signal displaySignal : std_logic_vector(3 downto 0);
	signal encodedSignal : std_logic_vector(2 downto 0);

	signal dec0      : std_logic_vector(3 downto 0);
	signal dec1      : std_logic_vector(3 downto 0);
	signal dec2      : std_logic_vector(3 downto 0);
	signal dec3      : std_logic_vector(3 downto 0);
	signal dec4      : std_logic_vector(3 downto 0);
	signal dec5      : std_logic_vector(3 downto 0);
	signal unused_0  : std_logic_vector(3 downto 0);
	signal unused_1  : std_logic_vector(3 downto 0);

	begin
		selectSignal  <= sw(5 downto 0);
		displaySignal <= sw(9 downto 6);	
		encoder: 	entity work.encoder   	port map (selectSignal,  encodedSignal);
		demux_1x8: 	entity work.demux_1x8 	port map (displaySignal, encodedSignal, unused_0, dec0, dec1, dec2, dec3, dec4, dec5, unused_1);
		dec7seg_0: 	entity work.dec7seg  	port map (dec0, hex0);
		dec7seg_1: 	entity work.dec7seg  	port map (dec1, hex1);
		dec7seg_2: 	entity work.dec7seg  	port map (dec2, hex2);
		dec7seg_3: 	entity work.dec7seg  	port map (dec3, hex3);
		dec7seg_4: 	entity work.dec7seg  	port map (dec4, hex4);
		dec7seg_5: 	entity work.dec7seg  	port map (dec5, hex5);

end architecture display;