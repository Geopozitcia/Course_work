FINITE AUTOMAT (T, P)
State <- 0
for l <- 1 to n
State <- δ(State, ti)
If State == m then
Match Found
end
end
