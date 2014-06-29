----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    17:42:35 07/12/2010 
-- Design Name: 
-- Module Name:    BMS_small - Behavioral 
-- Project Name: 
-- Target Devices: 
-- Tool versions: 
-- Description: 
--
-- Dependencies: 
--
-- Revision: 
-- Revision 0.01 - File Created
-- Additional Comments: 
--
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

---- Uncomment the following library declaration if instantiating
---- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity BMS_small is
    Port ( OverVoltage : in  STD_LOGIC_VECTOR (7 downto 0);
           UnderVoltage : in  STD_LOGIC_VECTOR (7 downto 0);
           CommMcu : in  STD_LOGIC_VECTOR (7 downto 0);
           SelectComm : in  STD_LOGIC_VECTOR (7 downto 0);
           LedOut : in  STD_LOGIC_VECTOR (7 downto 0);
           IntOut : out  STD_LOGIC_VECTOR (1 downto 0);
           OverOut : out  STD_LOGIC;
           UnderOut : out  STD_LOGIC;
           CommOut : out  STD_LOGIC;
           Takt : in  STD_LOGIC;
           CommIn : in  STD_LOGIC);
end BMS_small;

architecture Behavioral of BMS_small is
   SIGNAL CountTIC : Std_Logic_Vector(19 downto 0) := "00000000000000000000";

begin
    PROCESS(Takt)
    BEGIN 
		  CASE SelectComm(4 downto 0) IS
			   WHEN "00000" => CommOut <= OverVoltage(0);
				WHEN "00001" => CommOut <= UnderVoltage(0);
				WHEN "00010" => CommOut <= CommMcu(0);
				WHEN "00011" => CommOut <= LedOut(0);
				WHEN "00100" => CommOut <= OverVoltage(1);
				WHEN "00101" => CommOut <= UnderVoltage(1);
				WHEN "00110" => CommOut <= CommMcu(1);
				WHEN "00111" => CommOut <= LedOut(1);
				
			   WHEN "01000" => CommOut <= OverVoltage(2);
				WHEN "01001" => CommOut <= UnderVoltage(2);
				WHEN "01010" => CommOut <= CommMcu(2);
				WHEN "01011" => CommOut <= LedOut(2);
				WHEN "01100" => CommOut <= OverVoltage(3);
				WHEN "01101" => CommOut <= UnderVoltage(3);
				WHEN "01110" => CommOut <= CommMcu(3);
				WHEN "01111" => CommOut <= LedOut(3);
				
			   WHEN "10000" => CommOut <= OverVoltage(4);
				WHEN "10001" => CommOut <= UnderVoltage(4);
				WHEN "10010" => CommOut <= CommMcu(4);
				WHEN "10011" => CommOut <= LedOut(4);
				WHEN "10100" => CommOut <= OverVoltage(5);
				WHEN "10101" => CommOut <= UnderVoltage(5);
				WHEN "10110" => CommOut <= CommMcu(5);
				WHEN "10111" => CommOut <= LedOut(5);
				
				WHEN "11000" => CommOut <= OverVoltage(6);
				WHEN "11001" => CommOut <= UnderVoltage(6);
				WHEN "11010" => CommOut <= CommMcu(6);
				WHEN "11011" => CommOut <= LedOut(6);
				WHEN "11100" => CommOut <= OverVoltage(7);
				WHEN "11101" => CommOut <= UnderVoltage(7);
				WHEN "11110" => CommOut <= CommMcu(7);
				WHEN "11111" => CommOut <= LedOut(7);
				
				
				
				
				WHEN OTHERS => CommOut <= '0';
		  END CASE;
    END PROCESS;

end Behavioral;



------------------------------------------------------------------------------------
---- Company: 
---- Engineer: 
---- 
---- Create Date:    17:42:35 07/12/2010 
---- Design Name: 
---- Module Name:    BMS_small - Behavioral 
---- Project Name: 
---- Target Devices: 
---- Tool versions: 
---- Description: 
----
---- Dependencies: 
----
---- Revision: 
---- Revision 0.01 - File Created
---- Additional Comments: 
----
------------------------------------------------------------------------------------
--library IEEE;
--use IEEE.STD_LOGIC_1164.ALL;
--use IEEE.STD_LOGIC_ARITH.ALL;
--use IEEE.STD_LOGIC_UNSIGNED.ALL;
--
------ Uncomment the following library declaration if instantiating
------ any Xilinx primitives in this code.
----library UNISIM;
----use UNISIM.VComponents.all;
--
--entity BMS_small is
--    Port ( OverVoltage : in  STD_LOGIC_VECTOR (7 downto 0);
--           UnderVoltage : in  STD_LOGIC_VECTOR (7 downto 0);
--           CommMcu : in  STD_LOGIC_VECTOR (7 downto 0);
--           SelectComm : in  STD_LOGIC_VECTOR (7 downto 0);
--           LedOut : out  STD_LOGIC_VECTOR (7 downto 0);
--           IntOut : out  STD_LOGIC_VECTOR (1 downto 0);
--           OverOut : out  STD_LOGIC;
--           UnderOut : out  STD_LOGIC;
--           CommOut : out  STD_LOGIC;
--           Takt : in  STD_LOGIC;
--           CommIn : in  STD_LOGIC);
--end BMS_small;
--
--architecture Behavioral of BMS_small is
--   SIGNAL CountTIC : Std_Logic_Vector(19 downto 0) := "00000000000000000000";
--
--begin
--    PROCESS(Takt)
--    BEGIN
--        IF (Takt'EVENT and Takt = '1') THEN 
--				CountTIC <= CountTIC + 1;
--				OverOut <= OverVoltage(0) OR OverVoltage(1) OR OverVoltage(2) OR OverVoltage(3) OR OverVoltage(4) OR OverVoltage(5) OR OverVoltage(6) OR OverVoltage(7);
--				UnderOut <= UnderVoltage(0) OR UnderVoltage(1) OR UnderVoltage(2) OR UnderVoltage(3) OR UnderVoltage(4) OR UnderVoltage(5) OR UnderVoltage(6) OR UnderVoltage(7);
--				CASE CountTIC(19 downto 17) IS
--					WHEN "000" =>
--						LedOut(7 downto 0) <= "00000000";
--					WHEN "001" =>
--						LedOut(7 downto 0) <= UnderVoltage(7 downto 0);
--					WHEN "010" =>
--						LedOut(7 downto 0) <= "00000000";
--					WHEN "011" =>
--						LedOut(7 downto 0) <= UnderVoltage(7 downto 0);
--					WHEN "100" =>
--						LedOut(7 downto 0) <= OverVoltage(7 downto 0) XOR UnderVoltage(7 downto 0);
--					WHEN "101" =>
--						LedOut(7 downto 0) <= OverVoltage(7 downto 0) OR UnderVoltage(7 downto 0);
--					WHEN "110" =>
--						LedOut(7 downto 0) <= OverVoltage(7 downto 0);
--					WHEN "111" =>
--						LedOut(7 downto 0) <= OverVoltage(7 downto 0) OR UnderVoltage(7 downto 0);
--					WHEN OTHERS =>	 
--						LedOut(7 downto 0) <= "00000000";
--            END CASE;				
--        END IF;    
--		  IF SelectComm(3) = '0' THEN
--		  CASE SelectComm(2 downto 0) IS
--			   WHEN "000" => CommOut <= CommMcu(0);
--				WHEN "001" => CommOut <= CommMcu(1);
--				WHEN "010" => CommOut <= CommMcu(2);
--				WHEN "011" => CommOut <= CommMcu(3);
--				WHEN "100" => CommOut <= CommMcu(4);
--				WHEN "101" => CommOut <= CommMcu(5);
--				WHEN "110" => CommOut <= CommMcu(6);
--				WHEN "111" => CommOut <= CommMcu(7);
--				WHEN OTHERS => CommOut <= '0';
--		  END CASE;
--		  END IF;
--    END PROCESS;
--
--end Behavioral;
--
