--------------------------------------------------------------------------------
-- lab VHDL
-- decimal counter, rst asynchro
--------------------------------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.numeric_std.all;
use IEEE.STD_LOGIC_UNSIGNED.all;

entity d_cntr4Aceo is
    Port    (clk : in std_logic;
             rst : in std_logic;
             ce : in std_logic;
             tc : out std_logic;
             ceo : out std_logic;
             q : out std_logic_vector(3 downto 0));
end entity d_cntr4Aceo;

architecture behav of d_cntr4Aceo is
    signal q_tmp : std_logic_vector(q'range) := x"0";
    signal tci : std_logic;
begin
process(clk,rst) begin
    if rst='1' then
        q_tmp <= x"0";
	elsif rising_edge(clk) then
        if ce='1' then
            if tci='1' then
                q_tmp <= x"0";
            else
	            q_tmp <= q_tmp + 1;
            end if;
        end if;
	end if;	
end process;
-- outputs
tci <= '1' when (q_tmp=x"9") else '0';    
ceo <= (tci and ce);
tc <= tci;    
q <= q_tmp;
end architecture behav;
--------------------------------------------------------------------------------