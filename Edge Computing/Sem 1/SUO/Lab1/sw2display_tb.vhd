library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_UNSIGNED.all;
use IEEE.STD_LOGIC_ARITH.all;

entity sw2display_tb is
    generic(T: time:= 2 ns);
end entity sw2display_tb;

architecture beh of sw2display_tb is
    signal sw                                   : std_logic_vector(9 downto 0);
    signal selectSignal                         : std_logic_vector(5 downto 0);
	signal displaySignal                        : std_logic_vector(3 downto 0);
    signal hex5, hex4, hex3, hex2, hex1, hex0   : std_logic_vector(6 downto 0);
begin
    UUT: entity work.sw2display port map(sw, hex5, hex4, hex3, hex2, hex1, hex0);

    generate_signal: process
        variable display : natural := 15;
        variable sel     : natural := 0;
    begin
        selectSignal  <= conv_std_logic_vector(sel,     selectSignal'length);
		displaySignal <= conv_std_logic_vector(display, displaySignal'length);
        --sw <= displaySignal & selectSignal; --this will not work, bcs of how signals are handled
        sw <= conv_std_logic_vector(display, displaySignal'length) & conv_std_logic_vector(sel, selectSignal'length);
        if display = 0 then
            sel := sel * 2;
            sel := 1 when sel = 0 else sel;
        end if;
        sel := sel when sel < 64 else 0;
        display := display - 1 when display > 0 else 15;
        report  "display in: "  & to_hstring(displaySignal)  & 
                " select in: "  & to_bstring(selectSignal)   & 
                " hex0 out: "   & to_bstring(hex0)           & 
                " hex1 out: "   & to_bstring(hex1)           & 
                " hex2 out: "   & to_bstring(hex2)           & 
                " hex3 out: "   & to_bstring(hex3)           & 
                " hex4 out: "   & to_bstring(hex4)           & 
                " hex5 out: "   & to_bstring(hex5)
            severity Note;
        wait for 8*T;
    end process;
    
end architecture beh;