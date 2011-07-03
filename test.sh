#!/bin/sh

a[1]='SS'
b[1]='PSH'

a[2]='SNS'
b[2]='DUP'

a[3]='STS'
b[3]='CPY'

a[4]='SNT'
b[4]='SWP'

a[5]='SNN'
b[5]='DSC'

a[6]='STN'
b[6]='SLD'

a[7]='TSSS'
b[7]='ADD'

a[8]='TSST'
b[8]='SUB'

a[9]='TSSN'
b[9]='MUL'

a[10]='TSTS'
b[10]='DIV'

a[11]='TSTT'
b[11]='MOD'

a[12]='TTS'
b[12]='PUT'

a[13]='TTT'
b[13]='GET'

a[13]='NSS'
b[13]='LBL'

a[14]='NST'
b[14]='JAL'

a[15]='NSN'
b[15]='JMP'

a[16]='NTS'
b[16]='JSZ'

a[17]='NTT'
b[17]='JSN'

a[18]='NTN'
b[18]='RET'

a[19]='NNN'
b[19]='END'

for i in {1..1000}; do
   R=$RANDOM
   c[$i]=${a[$(( $R % 18 + 1 ))]}
   d[$i]=${b[$(( $R % 18 + 1 ))]}
done
for i in {1..1000}; do
   echo -n ${c[$i]}
done
echo 'NNN' # end of the program
echo -n '' > expect
for i in {1..1000}; do
   echo CMD_${d[$i]} >> expect
done
echo CMD_END >> expect

