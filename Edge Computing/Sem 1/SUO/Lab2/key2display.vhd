library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity key2display is
	Generic (
             slowdown : natural := 1000
            );
	Port ( 	
		   max10_clk1_50 	      				: in std_logic;
		   key 			      					: in std_logic_vector(1 downto 0);
		   ledr 			      				: out std_logic_vector(0 downto 0);
		   hex5, hex4, hex3, hex2, hex1, hex0 	: out std_logic_vector(6 downto 0)
		 );
end entity key2display;

architecture display of key2display is
	type q_out 		is array (5 downto 0) of std_logic_vector(3 downto 0);
	type hex_out 	is array (5 downto 0) of std_logic_vector(6 downto 0);

	signal q_out_signals 	: q_out;
	signal hex_out_signals 	: hex_out;
begin
	mod1kkBCD: entity work.mod_1kk_counter	
		generic map (
					 slowdown
					)
		port map (
			 	  max10_clk1_50,
				  key, 
				  ledr, 
				  q_out_signals(5), q_out_signals(4), q_out_signals(3), q_out_signals(2), q_out_signals(1), q_out_signals(0)
				 );

	gen_displays: for i in 5 downto 0 generate
		counter_inst : entity work.dec7seg
			port map (
					   q_out_signals(i),
					   hex_out_signals(i)				
					 );
	end generate gen_displays;

	hex0 <= hex_out_signals(0);
	hex1 <= hex_out_signals(1);
	hex2 <= hex_out_signals(2);
	hex3 <= hex_out_signals(3);
	hex4 <= hex_out_signals(4);
	hex5 <= hex_out_signals(5);

end architecture display;
