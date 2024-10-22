library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;

entity mod_1kk_counter is
    Generic (
             slowdown : natural := 1000
            );
    Port (
          clk : in std_logic;
          key : in std_logic_vector(1 downto 0);
          ledr: out std_logic_vector(0 downto 0);
          q_out5, q_out4, q_out3, q_out2, q_out1, q_out0 : out std_logic_vector(3 downto 0)
         );
end entity mod_1kk_counter;

architecture behav of mod_1kk_counter is
    type q_out is array (5 downto 0) of std_logic_vector(3 downto 0);

    signal tc 				: std_logic_vector(6 downto 0);
    signal q_out_signals    : q_out;
    signal enable           : std_logic := '0';
    signal slowed_clock     : std_logic := '0';
    signal not_key          : std_logic := '0';
    signal ceo          	: std_logic_vector(6 downto 0);
begin
	process(clk)
        variable count : natural := 0;
    begin
        if (rising_edge(clk)) then
            count := count + 1;
            if (count = Slowdown) then
                slowed_clock <= '1';
            elsif (count = 2*Slowdown) then
                slowed_clock <= '0';
                count := 0;
            end if;
        end if;
        
    end process;

    process(clk)
        variable count : natural := 0;
    begin
        if (rising_edge(clk)) then
            if (key(1) = '0') then
                if (count = 0) then
                    enable <= not enable;
                end if;
                count := count + 2;
                if (count > 1000) then
                    count := 1000;
                end if;
            else
                if (count /= 0) then
                    count := count - 1;
                end if;
            end if;
        end if;
    end process;

    gen_counters: for i in 5 downto 0 generate
        counter_inst : entity work.d_cntr4ceo
            port map (
                      slowed_clock,
                      not_key,
                      ceo(i),
                      tc(i),
                      ceo(i + 1),
                      q_out_signals(i)
                     );
    end generate gen_counters;

    ceo(0) <= enable;
    not_key <= not key(0);
    q_out0 <= q_out_signals(0);
    q_out1 <= q_out_signals(1);
    q_out2 <= q_out_signals(2);
    q_out3 <= q_out_signals(3);
    q_out4 <= q_out_signals(4);
    q_out5 <= q_out_signals(5);
    ledr <= (ceo(6), others => '0');
end architecture behav;
