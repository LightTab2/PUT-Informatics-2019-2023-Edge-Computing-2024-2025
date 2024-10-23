Library IEEE;
use ieee.std_logic_1164.all;
entity rr is
	PORT 
	(
		Cin : IN STD_LOGIC;
		A, B: IN STD_LOGIC_VECTOR(7 downto 0);
		
		Cout : OUT STD_LOGIC;
		Sum : OUT STD_LOGIC_VECTOR(7 downto 0)
	);
end rr;

architecture logic of rr is
	component fadder
		PORT 
		(
			A,
			B,
			Cin : IN STD_LOGIC;
			
			S,
			Cout : OUT STD_LOGIC
		);
	end component;

	SIGNAL carry : STD_LOGIC_VECTOR (8 downto 0);
begin
		carry(0) <= Cin;
		adder8: for i in 0 to 7 generate
			FAi : fadder PORT MAP (A => A(i), B => B(i), Cin => Carry(i), S => Sum(i), Cout => Carry(i+1));
		end generate;
	Cout <= carry(8);
end logic;
	
Library IEEE;
use ieee.std_logic_1164.all;
entity fadder is
	PORT(
		A,
		B,
		Cin : IN STD_LOGIC;
		
		s,
		cout : OUT STD_LOGIC
	);
end fadder;

architecture struct of fadder is
begin
		s <= a xor b xor cin;
		cout <= (a and b) or (a and cin) or (b and cin);
end struct;