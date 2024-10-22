library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity demux_1x8 is
    port (	input_signal : in  std_logic_vector(3 downto 0);
        	sel          : in  std_logic_vector(2 downto 0);
        	output_0     : out std_logic_vector(3 downto 0);
        	output_1     : out std_logic_vector(3 downto 0);
        	output_2     : out std_logic_vector(3 downto 0);
        	output_3     : out std_logic_vector(3 downto 0);
        	output_4     : out std_logic_vector(3 downto 0);
        	output_5     : out std_logic_vector(3 downto 0);
        	output_6     : out std_logic_vector(3 downto 0);
        	output_7     : out std_logic_vector(3 downto 0));
end demux_1x8;

architecture beh of demux_1x8 is
begin
    output_0 <= input_signal when sel = b"000" else b"0000";
    output_1 <= input_signal when sel = b"001" else b"0000";
    output_2 <= input_signal when sel = b"010" else b"0000";
    output_3 <= input_signal when sel = b"011" else b"0000";
    output_4 <= input_signal when sel = b"100" else b"0000";
    output_5 <= input_signal when sel = b"101" else b"0000";
    output_6 <= input_signal when sel = b"110" else b"0000";
    output_7 <= input_signal when sel = b"111" else b"0000";
end beh;