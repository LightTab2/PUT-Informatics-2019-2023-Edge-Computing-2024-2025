--------------------------------------------------------------------------------
-- lab VHDL
-- testbench : simple frequency meter
--------------------------------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
USE work.pkg_symuli.all;

entity simple_f_meter_tb is
end entity simple_f_meter_tb;

architecture behav of simple_f_meter_tb is
    constant F1_Hz: natural := 12345;
    constant F2_Hz: natural := 574; 
    -- Time definitions
    constant OFFSET    : time := 0 ns;
    constant PERIOD    : time := 100 ns;
    constant SPACE     : time := 2 sec;
    constant F1_PERIOD : time := (1 sec)/F1_Hz;
    constant F2_PERIOD : time := (1 sec)/F2_Hz;

    signal clk_10M, rst : std_logic := '0';
    signal f_in, f1, f2 : std_logic;
    signal hex5, hex4, hex3 : std_logic_vector(7 downto 0);
    signal hex2, hex1, hex0 : std_logic_vector(6 downto 0);
    procedure report_hexes(signal hex5, hex4, hex3, hex2 : std_logic_vector(6 downto 0)) is
        type hexes_t is array(3 downto 0) of std_logic_vector(6 downto 0);
        type hexes_n_t is array(3 downto 0) of natural;
        variable hexes   : hexes_t;
        variable hexes_n : hexes_n_t;
    begin
        hexes(3) := hex5;
        hexes(2) := hex4;
        hexes(1) := hex3;
        hexes(0) := hex2;
        for i in 3 downto 0 loop
            case hexes(i) is
                when "1111001" => hexes_n(i) := 1;
                when "0100100" => hexes_n(i) := 2;
                when "0110000" => hexes_n(i) := 3;
                when "0011001" => hexes_n(i) := 4;
                when "0010010" => hexes_n(i) := 5;
                when "0000010" => hexes_n(i) := 6;
                when "1111000" => hexes_n(i) := 7;
                when "0000000" => hexes_n(i) := 8;
                when "0010000" => hexes_n(i) := 9;
                when "1000000" => hexes_n(i) := 0;
                when others => hexes_n(i) := 14;
            end case;       
         end loop;

        print("Detected= " & to_string(hexes_n(3))  & to_string(hexes_n(2))  & to_string(hexes_n(1))  & to_string(hexes_n(0)));
    end procedure;
begin
    UUT: entity work.simple_f_meter
    port map(clk_10M, rst, f_in, hex5, hex4, hex3, hex2, hex1, hex0);

    stop_after(SPACE * 1.1);
    --pulse(rst, 2*PERIOD, PERIOD);
 
    clk_gen(clk_10M, OFFSET, PERIOD/2, PERIOD);
    clk_gen(f1, 0 ns, F1_PERIOD/2, F1_PERIOD);
    clk_gen(f2, 0 ns, F2_PERIOD/2, F2_PERIOD);
    f_in <= f1 when (now <= SPACE/2 + 300ns) else f2;

    process
    begin
        print("F1= " & to_string(F1_Hz));
        print("F2= " & to_string(F2_Hz));
        wait for SPACE / 2 + 500ns;
        print("Switching to F2...");
        report_hexes(hex5(6 downto 0), hex4(6 downto 0), hex3(6 downto 0), hex2);

        wait for SPACE / 2 + 1000ns;
        print("Simulation complete.");
        report_hexes(hex5(6 downto 0), hex4(6 downto 0), hex3(6 downto 0), hex2);
    end process;

end architecture behav;