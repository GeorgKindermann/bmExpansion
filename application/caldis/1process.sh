#extract used data and recode pktNr back to rwHwPbfl
folders=(~/18/yasso/simona/data/thomas/spinnup ~/18/yasso/simona/data/thomas/Sz_45_Referenz ~/18/yasso/simona/data/thomas/Sz_85_Referenz /dat/tmp/BOKU_Adapt_Auto_KSz_85_BA_Wechsel /dat/tmp/BOKU_Adapt_Auto_KSz_85_Katastrophe /dat/tmp/BOKU_Adapt_Auto_KSz_85_UZV_final /dat/tmp/BOKU_Adapt_Auto_KSz_85_Vorratsaufbau_final /dat/tmp/holzketteReferenz)
szenNames=(cvSpinnup cv45Ref cv85Ref cv85BaWechsel cv85Katastrophe cv85Umtriebszeitverk cv85Vorratsaufbau cvHolzketteRef)
for ((i = 0; i < ${#folders[*]}; i++)); do
    echo "${szenNames[$i]}"
    echo "#rwHwPbfl jahr BaumNr fraktion BaumArt BHD HT KA Nrepjeha tot BaumVol JAb" >/tmp2/${szenNames[$i]}.txt
    find ${folders[$i]}/{Verb,EN,VN,NatAbgang,Zufallsnutzung} -maxdepth 1 -name '*.cds' -exec awk -F , 'FNR == 1 {n=split(FILENAME,a,"/");jahr=substr(a[n],0,4);fraktion=a[n-1]} {if($12>0){print $1,jahr,$5,fraktion,$6,$9,$10,$11,$12,substr($14,3,1),$15,$18}}' {} + |awk 'FNR==NR{a[$1]=$2; next} {if ($1 in a) {printf a[$1]} else {printf "0"}; $1=""; print $0}' ~/18/yasso/simona/data/rosetta.txt - |tr -s ' ' |sort >>/tmp2/${szenNames[$i]}.txt
done

#With sdiMax=1400n/ha, smoothing=15Years and maxinflow of 10tC of loving
#0.8 * C -> max = 12.5
szenNames=(cv45Ref cv85Ref cv85BaWechsel cv85Katastrophe cv85Umtriebszeitverk cv85Vorratsaufbau cvHolzketteRef)
for ((i = 0; i < ${#szenNames[*]}; i++)); do
    echo "${szenNames[$i]}"
    cat /tmp2/${szenNames[$i]}.txt | ./1_5sdiMax 700 | ./2calcBm |./3calcInfall |./4infall2chemComp |./5threshold 12.5 15 2010 2150 | sort -g>/tmp2/${szenNames[$i]}IYas.txt
done
#~0.5 * C -> max = 20.
cat /tmp2/cvSpinnup.txt | ./1_5sdiMax 700 | ./2calcBm |./3calcInfall |./4infall2chemComp |./5threshold 20. 15 2010 | sort -g >/tmp2/cvSpinnupIYas.txt


for ((i = 0; i < ${#szenNames[*]}; i++)); do
    echo "${szenNames[$i]}"
    ./t1calcStock /tmp2/${szenNames[$i]}.txt >/tmp2/${szenNames[$i]}GV.txt
done
./t1calcStock /tmp2/cvSpinnup.txt >/tmp2/cvSpinnupGV.txt
