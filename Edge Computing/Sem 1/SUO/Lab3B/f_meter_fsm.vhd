--------------------------------------------------------------------------------
-- lab VHDL
-- simple frequency meter fsm, reset asynchro
--------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity f_meter_fsm is
    port ( clk, reset, time_base_pulse : in std_logic;
           memory_en : out std_logic;
           time_base_rst : out std_logic;
           time_base_on : out std_logic);
end entity;

architecture behav of f_meter_fsm is
    type state_type is (IDLE, BASE_ON, BASE_OFF, MEM_WRITE, CLEAR);
    signal c_state, n_state: state_type;
begin
    proc_fsm: process(c_state, time_base_pulse) 
    begin
            time_base_rst <= '0';
            time_base_on <= '0';
            memory_en <= '0';

            case c_state is
                when IDLE =>  
                    n_state <= BASE_ON;
                when BASE_ON => 
                    if time_base_pulse='1' then
                        n_state <= BASE_OFF;
                    else
                        n_state <= BASE_ON;
                    end if;
                    time_base_on <= '1';
                when BASE_OFF => 
                    n_state <= MEM_WRITE;
                when MEM_WRITE => 
                    n_state <= CLEAR;
                    memory_en <= '1';
                when CLEAR =>
                    n_state <= BASE_ON;
                    time_base_rst <= '1';
            end case;
    end process;

    proc_memory: process (clk,reset)
    begin
        if (reset ='1') then 
            c_state <= IDLE;
        elsif rising_edge(clk) then
            c_state <= n_state;
        end if;
    end process;

end behav;
