var T1
var 40 T2
var t1
f_main [0]


var t2
var t3
var t4
var t5
var t6
var t7
var t8
var t9
var t10
var t11
var t12
var t13
var t14
var t15
var t16
var t17
var t18
var t19
var T3
var T4

t2 = call f_getint
T1 = t2
t4 = 10
t3 = T1 > t4
if t3 == 0 goto l4
t5 = 1
t1 = t5
goto l1
l4:
t6 = 0
T3 = t6
T4 = T3
l5:
t7 = T3 < T1
if t7 == 0 goto l7
t8 = T3 * 1
t9 = t8 + 0
t10 = t9 * 4
t11 = call f_getint
T2[t10] = t11
t13 = T3 * 1
t14 = t13 + 0
t15 = t14 * 4
t16 = T2[t15]
t12 = T4 + t16
T4 = t12
t18 = 1
t17 = T3 + t18
T3 = t17
goto l5
l7:
param T4
call f_putint
t19 = 0
t1 = t19
goto l1
l1:
return t1

end f_main
