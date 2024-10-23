@Echo off
FOR /L %%p IN (0,1,4) DO (
FOR /L %%x IN (0,1,14) DO ( Program.exe -file -test < "Testy\ran_%%p_%%x" > "Wyniki\ran_%%p_%%x")
FOR /L %%x IN (0,1,14) DO ( Program.exe -file -test < "Testy\ros_%%p_%%x" > "Wyniki\ros_%%p_%%x")
FOR /L %%x IN (0,1,14) DO ( Program.exe -file -test < "Testy\mal_%%p_%%x" > "Wyniki\mal_%%p_%%x")
FOR /L %%x IN (0,1,14) DO ( Program.exe -file -test < "Testy\A_%%p_%%x" > "Wyniki\A_%%p_%%x")
FOR /L %%x IN (0,1,14) DO ( Program.exe -file -test < "Testy\V_%%p_%%x" > "Wyniki\V_%%p_%%x")
)
pause