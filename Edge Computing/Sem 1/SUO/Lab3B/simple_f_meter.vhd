--------------------------------------------------------------------------------
-- lab VHDL
-- simple frequency meter
--------------------------------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.numeric_std.all;
use IEEE.STD_LOGIC_UNSIGNED.all;

entity simple_f_meter is
    port 
    (
        clk_10M : in std_logic;
        rst : in std_logic;
        -- mierzona czestotliwosc
        f_in : in std_logic;
        -- wyswietlacze z kropka
        hex5, hex4, hex3    : out std_logic_vector(7 downto 0); 
        -- hex1, hex0 odpowiada za jednostke, a hex2 to ostatnia cyfra i nie ma sensu po niej dawac kropki (0084. Hz???)
        hex2, hex1, hex0 	: out std_logic_vector(6 downto 0)
    );
end entity simple_f_meter;

architecture struct of simple_f_meter is
    -- licznik czestotliwosci wypluwa wyniki do [q_cntr]. [q] to rejestr, ktory przetrzyma wartosc [q_cntr] po uplywie sekundy (zeby nam sie nie zmieniala podczas obliczen)
    signal q, q_cntr : std_logic_vector(35 downto 0);
    -- Zczytywanie z [q] jaka jest czestotliwosc jest niewygodne, dlatego latwiej bedzie jak sobie podzielimy to na 9 cyfr. Licznik jest dziesietny, wiec co 4 kolejne bity [q] odpowiadaja za 1 cyfre
    type digits is array (8 downto 0) of std_logic_vector(3 downto 0);
    signal digits_signals : digits;
    -- Sygnal, ktory mowi kiedy zapisac do rejestru (do [q] zapisac [q_cntr])
    signal memory_en : std_logic;
    -- [time_base_on] jest ustawione na stan wysoki, co aktywuje zliczanie w licznikach. Po sekundzie [time_base_on] ustawia sie na stan niski, a [time_base_pulse] na wysoki
    -- Wysoki [time_base_pulse] oznacza, ze minela sekunda, wiec niedlugo po otrzymaniu tego sygnalu, [memory_en] powinno byc wystawione na stan wysoki przez komponent [f_meter_fsm] (maszyna stanow)
    -- Po zakonczonym zapisie nastepuje [time_base_rst] reset licznikow. Nastepnie [time_base_on] jest ustawione na stan wysoki i tak w kolko
    signal time_base_on, time_base_pulse, time_base_rst : std_logic;
 
    -- sygnaly wejscia/wyjscia z [dec7seg] (dec7seg.vhd)
    type freq_out is array (5 downto 0) of std_logic_vector(3 downto 0);
	type hex_out  is array (5 downto 0) of std_logic_vector(6 downto 0);
	signal freq_out_signals : freq_out;
	signal hex_out_signals 	: hex_out;

    -- kropka dziesietna
    signal dots : std_logic_vector(2 downto 0);

    -- prosta maszyna stanow
    type state_type is (CLEAR, CALC, IDLE);
    signal c_state, n_state: state_type;
begin

    -- licznik czasu (sekundy)
    time_base_counter: entity work.cntr_xN
    generic map(7)
    port map
    (
        clk => clk_10M,
        rst => time_base_rst,
        ce  => time_base_on,
        ceo => time_base_pulse,
        q   => open
    );

    -- licznik czestotliwosci
    freq_counter: entity work.cntr_xN
    generic map(9)
    port map
    (
        clk => f_in,
        rst => time_base_rst,
        ce  => time_base_on,
        ceo => open,
        q   => q_cntr
    );

    -- maszyna stanow nadzorujaca prace licznikow (czasu i czestotliwosci) i zapisu do rejestru
    simple_fsm: entity work.f_meter_fsm
    port map
    (
        clk             => clk_10M,
        reset           => rst, 
        time_base_on    => time_base_on,
        time_base_pulse => time_base_pulse,
        time_base_rst   => time_base_rst,
        memory_en       => memory_en
    );

    -- wyswietlacze
    gen_displays: for i in 5 downto 0 generate
        counter_inst : entity work.dec7seg
        port map 
        (
            bcd => freq_out_signals(i),
            seg => hex_out_signals(i)				
        );
    end generate gen_displays;
    
    proc_fsm: process(c_state, memory_en)
         -- zapisuje, ktora cyfra (indeksowanie od 0, wiec i = od 3 do 8), jest rozna od 0
         -- nie ma sensu sprawdzac ponizej 4 cyfry (3 indeks), bo nie wazne czy dostaniemy na wejscie 0000004212, czy 0000000212, czy 000000012, czy 000000002
         -- i tak kropka bedzie w tym samym miejscu i ta sama jednostka: 4.212 KHz, 0.212 KHz, 0.012 KHz, 0.002 KHz,
        variable i : std_logic_vector(3 downto 0) := x"3";
    begin
        -- wartosci domyslne
        freq_out_signals(0) <= x"A"; -- zawsze mozna wpisac "H" w jednostce, czy to kHz, czy MHz
        for j in 1 to 5 loop
            freq_out_signals(j) <= (others => '0'); -- cyfry i pierwszy czlon jednostki (k lub M w kHz i MHz) domyslnie na 0
        end loop;
        dots <= (others => '1'); -- stan wysoki oznacza brak wyswietlania
        
        case c_state is
            when CLEAR => 
            n_state <= IDLE;
            when CALC => 
            n_state <= IDLE;

                i := x"3"; -- ustawiamy na 3 (bo dalej nie ma sensu sprawdzac)
                for j in 8 downto 3 loop
                    if (digits_signals(j) /= x"0") then -- szukamy cyfry roznej od zera
                        i := std_logic_vector(to_unsigned(j, 4)); -- zapisujemy indeks tej cyfry
                        exit;
                    end if;
                end loop; -- jezeli nie znalazl cyfry roznej od 0, to ma 3 przez [i := x"3"]
            when IDLE =>
            n_state <= IDLE;

                if (i = 8 or i = 7 or i = 6) then -- MHz
                    freq_out_signals(1) <= x"C"; -- pierwszy czlon jednostki na 'M'
					dots(8 - to_integer(unsigned(i))) <= '0';
                elsif (i = 5 or i = 4 or i = 3) then -- kHz
                    freq_out_signals(1) <= x"B"; -- pierwszy czlon jednostki na 'k'
                    dots(5 - to_integer(unsigned(i))) <= '0';
                end if;
                -- przypisanie odpowiednich cyfr do sygnalow przekazanych na wejscie enkodera dla wyswietlaczy
                for j in 0 to 3 loop
                    freq_out_signals(2 + j) <= digits_signals(to_integer(unsigned(i)) + j - 3);
                end loop;
                    
                if memory_en = '1' then
                    n_state <= CALC;
                end if;
        end case;  
    end process;

    memory: process(clk_10M, rst)
    begin
        if (rst = '1') then -- reset
            c_state <= CLEAR;
        elsif rising_edge(clk_10M) then
            if memory_en = '1' then -- zapis do rejestru
                q <= q_cntr;
            end if;
            c_state <= n_state; -- przejscie do nastepnego stanu
        end if;
    end process;
    
    -- wydzielenie cyfr z [q] i zapisanie do [digits_signals]
    digitz: for i in 8 downto 0 generate
        digits_signals(i) <= q(((i + 1) * 4) - 1 downto (i * 4));
    end generate digitz;

    -- jednostka
    hex0 <= hex_out_signals(0);
	hex1 <= hex_out_signals(1);
    -- cyfry
	hex2 <= hex_out_signals(2);
	hex3 <= dots(0) & hex_out_signals(3); -- konkatenacja kropki do sygnalu wyswietlania
	hex4 <= dots(1) & hex_out_signals(4);
	hex5 <= dots(2) & hex_out_signals(5);
end architecture struct;