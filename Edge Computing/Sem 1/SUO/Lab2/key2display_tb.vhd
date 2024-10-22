library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_UNSIGNED.all;
use IEEE.STD_LOGIC_ARITH.all;

entity key2display_tb is
    generic(
            BASE_FREQ : natural := 1_500_000
           );

end entity key2display_tb;

architecture beh of key2display_tb is
    signal max10_clk1_50                      : std_logic;
	signal key 			                      : std_logic_vector(1 downto 0);
	signal ledr 			                  : std_logic_vector(0 downto 0);
	signal hex5, hex4, hex3, hex2, hex1, hex0 : std_logic_vector(6 downto 0);
    constant T : time := (1_000_000_000/BASE_FREQ/2)*ns;

begin
    UUT: entity work.key2display
        generic map (
                     1
                    )
        port map (
                  max10_clk1_50, 
                  key,
                  ledr,
                  hex5, hex4, hex3, hex2, hex1, hex0
                 );

    generate_signal: process
        variable count      : natural   := 0;
        variable first      : std_logic := '0';
    begin
        max10_clk1_50 <= '1';
        count := count + 1;

        if (count = 200_00 and first = '0') then
            key(1) <= '0';
            first := '1';
        end if;

        if (count = 1_500_000) then
            key(1) <= '0';
        end if;
        if (count = 2_000_000) then
            key(1) <= '0';
        end if;
        if (count = 2_500_000) then
            key(0) <= '0';
            count := 0;
        end if;
        wait for T;

        key(0) <= '1';
        key(1) <= '1';
        max10_clk1_50 <= '0';
        wait for T;
    end process;
    
    process
        variable start_time: time;
        variable end_time: time;
        variable duration: time;
    begin
        wait until ledr = "1";
        start_time := now;
        wait until ledr = "0";
        end_time := now;
        duration := end_time - start_time;

        report  " ceo duration:" & to_string(duration)        & 
                " hex0 out: "    & to_bstring(hex0)           & 
                " hex1 out: "    & to_bstring(hex1)           & 
                " hex2 out: "    & to_bstring(hex2)           & 
                " hex3 out: "    & to_bstring(hex3)           & 
                " hex4 out: "    & to_bstring(hex4)           & 
                " hex5 out: "    & to_bstring(hex5)
            severity Note;
    end process;
    
end architecture beh;