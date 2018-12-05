#extract used data and recode pktNr back to rwHwPbfl
folders=(~/18/yasso/simona/data/thomas/spinnup ~/18/yasso/simona/data/thomas/Sz_45_Referenz ~/18/yasso/simona/data/thomas/Sz_85_Referenz /dat/tmp/BOKU_Adapt_Auto_KSz_85_BA_Wechsel /dat/tmp/BOKU_Adapt_Auto_KSz_85_Katastrophe)
szenNames=(cvSpinnup cv45Ref cv85Ref cv85BaWechsel cv85Katastrophe)
for ((i = 0; i < ${#folders[*]}; i++)); do
    echo "${szenNames[$i]}"
    echo "#rwHwPbfl jahr BaumNr fraktion BaumArt BHD HT KA Nrepjeha tot BaumVol JAb" >/tmp2/${szenNames[$i]}.txt
    find ${folders[$i]}/{Verb,EN,VN,NatAbgang,Zufallsnutzung} -maxdepth 1 -name '*.cds' -exec awk -F , 'FNR == 1 {n=split(FILENAME,a,"/");jahr=substr(a[n],0,4);fraktion=a[n-1]} {if($12>0){print $1,jahr,$5,fraktion,$6,$9,$10,$11,$12,substr($14,3,1),$15,$18}}' {} + |awk 'FNR==NR{a[$1]=$2; next} {if ($1 in a) {printf a[$1]} else {printf "0"}; $1=""; print $0}' ~/18/yasso/simona/data/rosetta.txt - |tr -s ' ' |sort >>/tmp2/${szenNames[$i]}.txt
done

for ((i = 0; i < ${#szenNames[*]}; i++)); do
    echo "${szenNames[$i]}"
    ./2calcBm /tmp2/${szenNames[$i]}.txt |./3calcInfall |./4infall2chemComp >/tmp2/${szenNames[$i]}IYas.txt
done

