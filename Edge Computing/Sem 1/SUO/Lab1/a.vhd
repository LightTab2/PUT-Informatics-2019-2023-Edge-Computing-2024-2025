--------------------------------------------------------------------------------
-- lab VHDL
-- x stage decimal counter, async reset, generate for
--------------------------------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity jk_ff is
    port 
    (
        j   : in std_logic;
        k   : in std_logic;
        clk : in std_logic;
        q   : out std_logic
    );
end entity jk_ff;

architecture struct of jk_ff is
    signal q_last : std_logic := '0';
begin
    process(clk, q_last, j, k)
begin
    if rising_edge(clk) then
        q <= q_last;
        if (k = '1' and q_last = '1') then
            q <= '0';
        elsif (j = '1' and q_last = '0') then
            q <= '1';
        end if;
        q_last <= q;
    end if;
end process;
end architecture struct;